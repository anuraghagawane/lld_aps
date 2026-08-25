#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <string>
#include <unordered_map>

using namespace std;

enum class Symbol { BEL };
enum class OrderType { BUY, SELL };
enum class OrderStatus { OPEN, PARTIALLY_FILLED, FILLED, CANCELLED };

class User {
private:
  string id;
  string name;
  int availableBalance;
  int reservedBalance;

  int stockBalance;
  int reservedStockBalance;

  mutable mutex mtx;

public:
  User(const string &id, const string &name, int availableBalance,
       int stockBalance)
      : id(id), name(name), availableBalance(availableBalance),
        stockBalance(stockBalance), reservedBalance(0),
        reservedStockBalance(0) {}
  bool reserve(int amount) {
    lock_guard<mutex> lock(mtx);
    if (availableBalance < amount)
      return false;
    reservedBalance += amount;
    availableBalance -= amount;
    return true;
  }

  void release(int amount) {
    lock_guard<mutex> lock(mtx);
    availableBalance += amount;
    reservedBalance -= amount;
  }

  void deductReserved(int amount) {
    lock_guard<mutex> lock(mtx);
    reservedBalance -= amount;
  }

  bool reserveStock(int quantity) {
    lock_guard<mutex> lock(mtx);
    if (stockBalance < quantity)
      return false;
    reservedStockBalance += quantity;
    stockBalance -= quantity;
    return true;
  }

  void releaseStock(int quantity) {
    lock_guard<mutex> lock(mtx);
    stockBalance += quantity;
    reservedStockBalance -= quantity;
  }

  void deductReservedStock(int quantity) {
    lock_guard<mutex> lock(mtx);
    reservedStockBalance -= quantity;
  }

  void addAmount(int amount) {
    lock_guard<mutex> lock(mtx);
    availableBalance += amount;
  }

  void addStock(int quantity) {
    lock_guard<mutex> lock(mtx);
    stockBalance += quantity;
  }
};

int getTime() {
  static int time = 0;
  return ++time;
}

class Order {
private:
  string id;
  shared_ptr<User> user;
  Symbol symbol;
  OrderType side;
  int price;
  int quantity;
  int remainingQuantity;
  OrderStatus status;
  int time;
  int spentAmount = 0;

public:
  Order(const string &id, shared_ptr<User> user, Symbol symbol, OrderType side,
        int price, int quantity)
      : id(id), user(user), symbol(symbol), side(side), price(price),
        quantity(quantity), remainingQuantity(quantity),
        status(OrderStatus::OPEN), time(getTime()) {}

  string getId() const { return id; }
  int getPrice() const { return price; }

  int getRemainingQuantity() const { return remainingQuantity; }
  shared_ptr<User> getUser() const { return user; }
  int getTime() const { return time; }
  OrderType getSide() const { return side; }

  void reduceQuantity(int quantity, int price) {
    remainingQuantity -= quantity;
    spentAmount += quantity * price;
    if (remainingQuantity == 0) {
      status = OrderStatus::FILLED;
      return;
    }
    status = OrderStatus::PARTIALLY_FILLED;
  }

  bool isFilled() { return status == OrderStatus::FILLED; }

  int getRemainingAmount() { return price * quantity - spentAmount; }
};

struct CompareBuyOrder {
  bool operator()(shared_ptr<Order> &a, shared_ptr<Order> &b) const {
    if (a->getPrice() != b->getPrice())
      return a->getPrice() < b->getPrice();

    return a->getTime() > b->getTime();
  }
};
struct CompareSellOrder {
  bool operator()(const shared_ptr<Order> &a,
                  const shared_ptr<Order> &b) const {
    if (a->getPrice() != b->getPrice())
      return a->getPrice() > b->getPrice();

    return a->getTime() > b->getTime();
  }
};

class MatchingEngine;
class OrderBook {
private:
  priority_queue<shared_ptr<Order>, vector<shared_ptr<Order>>, CompareBuyOrder>
      buy;
  priority_queue<shared_ptr<Order>, vector<shared_ptr<Order>>, CompareSellOrder>
      sell;

  mutex mtx;

  friend class MatchingEngine;

public:
  void addOrder(shared_ptr<Order> order) {
    lock_guard<mutex> lock(mtx);
    if (order->getSide() == OrderType::BUY) {
      buy.push(order);
    } else {
      sell.push(order);
    }
  }

  bool buyEmpty() { return buy.empty(); }
  bool sellEmpty() { return sell.empty(); }
  shared_ptr<Order> getTopBuy() const { return buy.top(); }
  shared_ptr<Order> getTopSell() const { return sell.top(); }
  void consumeTopBuy() { buy.pop(); }
  void consumeTopSell() { sell.pop(); }
};

class Trade {
private:
  string tradeId;
  string buyOrderId;
  string sellOrderId;
  int price;
  int quantity;

public:
  Trade(const string &tradeId, const string &buyOrderId,
        const string &sellOrderId, int price, int quantity)
      : tradeId(tradeId), buyOrderId(buyOrderId), sellOrderId(sellOrderId),
        price(price), quantity(quantity) {}
};

class TradeRecords {
private:
  vector<unique_ptr<Trade>> trades;
  mutex mtx;

public:
  void addTrade(const string &buyOrderId, const string &sellOrderId, int price,
                int quantity) {
    lock_guard<mutex> lock(mtx);
    string id = "T_" + to_string(trades.size() + 1);
    trades.push_back(
        make_unique<Trade>(id, buyOrderId, sellOrderId, price, quantity));
    cout << "Trade added" << id << endl;
  }
};

class MatchingEngine {
private:
  OrderBook &orderBook;
  TradeRecords &tradeRecords;

public:
  MatchingEngine(OrderBook &orderBook, TradeRecords &tradeRecords)
      : orderBook(orderBook), tradeRecords(tradeRecords) {}

  void match() {
    lock_guard<mutex> lock(orderBook.mtx);
    while (!orderBook.buyEmpty() && !orderBook.sellEmpty() &&
           orderBook.getTopBuy()->getPrice() >=
               orderBook.getTopSell()->getPrice()) {

      cout << "Match" << endl;
      auto buy = orderBook.getTopBuy();
      auto sell = orderBook.getTopSell();

      int quantity =
          min(buy->getRemainingQuantity(), sell->getRemainingQuantity());

      int price = sell->getPrice();
      int amount = quantity * price;

      buy->reduceQuantity(quantity, price);
      sell->reduceQuantity(quantity, price);

      buy->getUser()->deductReserved(amount);
      sell->getUser()->deductReservedStock(quantity);
      buy->getUser()->addStock(quantity);
      sell->getUser()->addAmount(amount);

      if (buy->isFilled()) {
        cout << buy->getId() << " Filled" << endl;
        buy->getUser()->release(buy->getRemainingAmount());
        orderBook.consumeTopBuy();
      }

      if (sell->isFilled()) {
        cout << sell->getId() << " Filled" << endl;
        orderBook.consumeTopSell();
      }

      tradeRecords.addTrade(buy->getId(), sell->getId(), price, quantity);
    }
  }
};

class TradingService {
private:
  vector<shared_ptr<Order>> orders;
  unordered_map<string, shared_ptr<User>> users;
  OrderBook orderBook;
  MatchingEngine *matchEngine;
  TradeRecords tradeRecords;

  atomic<int> ordercount = 1;

public:
  TradingService() {
    this->matchEngine = new MatchingEngine(orderBook, tradeRecords);
  }
  void addUser(const string &id, const string &name, int availableBalance,
               int stockBalance) {
    auto user = make_shared<User>(id, name, availableBalance, stockBalance);
    users[id] = user;
  }

  bool buy(string userId, int quantity, int price) {
    if (!users.count(userId))
      throw runtime_error("User not found");

    auto user = users[userId];

    if (!user->reserve(quantity * price)) {
      throw runtime_error("Insufficient balance");
    }

    string orderId = "ORD_" + to_string(ordercount.fetch_add(1));
    auto order = make_shared<Order>(orderId, user, Symbol::BEL, OrderType::BUY,
                                    price, quantity);

    orders.push_back(order);
    orderBook.addOrder(order);
    return true;
  }

  bool sell(string userId, int quantity, int price) {
    if (!users.count(userId))
      throw runtime_error("User not found");

    auto user = users[userId];

    if (!user->reserveStock(quantity)) {
      throw runtime_error("Insufficient stock");
    }

    string orderId = "ORD_" + to_string(ordercount.fetch_add(1));
    auto order = make_shared<Order>(orderId, user, Symbol::BEL, OrderType::SELL,
                                    price, quantity);

    orders.push_back(order);
    orderBook.addOrder(order);
    return true;
  }

  void match() { matchEngine->match(); }

  void displayOrders() {
    cout << "All orders" << endl;
    for (auto order : orders) {
      cout << order->getId() << " " << order->getRemainingQuantity() << endl;
    }
  }
};

int main() {
  TradingService tradingService;
  tradingService.addUser("U1", "ABC", 1000, 5);
  tradingService.addUser("U2", "XYZ", 2000, 10);
  tradingService.addUser("U3", "MNQ", 1400, 8);
  tradingService.addUser("U4", "DEF", 1800, 20);

  tradingService.buy("U1", 3, 195);
  tradingService.buy("U2", 5, 200);
  tradingService.sell("U3", 6, 190);
  tradingService.sell("U4", 3, 70);

  tradingService.match();

  tradingService.displayOrders();
  return 0;
}
