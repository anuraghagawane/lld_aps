#include <atomic>
#include <iostream>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <unordered_map>
#include <vector>

using namespace std;

class User {
private:
  string id;
  string name;

public:
  User(const string &id, const string &name) : id(id), name(name) {}

  string getId() const { return id; }
  string getName() const { return name; }
};

class DeliveryAgent : public User {
private:
  bool available;

public:
  DeliveryAgent(const string &id, const string &name)
      : User(id, name), available(true) {}

  bool isAvailable() { return available; }

  void setAvailability(bool status) { available = status; }
};

class MenuItem {
private:
  string id;
  string name;
  int price;
  bool available;

public:
  MenuItem(const string &id, const string &name, const int price)
      : id(id), name(name), price(price), available(true) {}

  string getId() const { return id; }
  string getName() const { return name; }
  int getPrice() const { return price; }
  bool isAvailable() const { return available; }

  void setAvailability(bool available) { this->available = available; }
  void setPrice(int newPrice) { price = newPrice; }
};

class Menu {
private:
  unordered_map<string, shared_ptr<MenuItem>> menuItems;

public:
  vector<shared_ptr<MenuItem>> getAllItems() {
    vector<shared_ptr<MenuItem>> items;
    for (auto &[id, item] : menuItems) {
      items.push_back(item);
    }

    return items;
  }

  void display() {
    vector<MenuItem> items;
    for (auto &[id, item] : menuItems) {
      cout << "ID: " << item->getId() << ", Item: " << item->getName()
           << ", Price: " << item->getPrice() << endl;
    }
  }
  void addItem(const string &id, const string &name, const int price) {
    menuItems.emplace(id, make_shared<MenuItem>(id, name, price));
  }

  shared_ptr<MenuItem> getItem(const string &itemId) {
    if (!menuItems.count(itemId)) {
      throw runtime_error("Item not in menu");
    }
    return menuItems[itemId];
  }

  void changePrice(const string &itemId, int newPrice) {
    if (!menuItems.count(itemId)) {
      throw runtime_error("Item not in menu");
    }
    menuItems[itemId]->setPrice(newPrice);
  }

  bool isAvailable(const string &itemId) {
    if (!menuItems.count(itemId)) {
      throw runtime_error("Item not in menu");
    }
    return menuItems[itemId]->isAvailable();
  }

  void setAvailability(const string &itemId, bool available) {
    if (!menuItems.count(itemId)) {
      throw runtime_error("Item not in menu");
    }
    menuItems[itemId]->setAvailability(available);
  }
};

class Restaurant {
private:
  string id;
  string name;
  Menu menu;

  atomic<int> count = 1;

public:
  Restaurant(const string &id, const string &name) : id(id), name(name) {}

  string getId() const { return id; }
  string getName() const { return name; }

  string addMenu(const string &name, int price) {
    const string id = "ITEM_" + to_string(count.fetch_add(1));
    menu.addItem(id, name, price);
    return id;
  }

  void changePrice(const string &itemId, int newPrice) {
    menu.changePrice(itemId, newPrice);
  }

  void setAvailability(const string &itemId, bool available) {
    menu.setAvailability(itemId, available);
  }

  shared_ptr<MenuItem> getItem(const string &itemId) {
    return menu.getItem(itemId);
  }

  bool isAvailable(vector<string> &itemIds) {
    for (auto &id : itemIds) {
      if (!menu.isAvailable(id))
        return false;
    }

    return true;
  }

  void displayMenu() { menu.display(); }
};

class OrderItem {
private:
  string menuItemId;
  string name;
  int unitPrice;
  int quantity;

public:
  OrderItem(const string &menuItemId, const string &name, const int unitPrice,
            const int quantity)
      : menuItemId(menuItemId), name(name), unitPrice(unitPrice),
        quantity(quantity) {}

  string getItemId() const { return menuItemId; }
  string getName() const { return name; }

  int getUnitPrice() const { return unitPrice; }
  int getQuantity() const { return quantity; }
};

enum class OrderStatus {
  CREATED,
  PLACED,
  CANCELLED,
  RESTAURANT_ACCEPTED,
  PREPARING,
  WAITING_TO_PICKED,
  DELIVERY_ON_WAY,
  DELIVERED,
  FAILED,
};

class Order {
private:
  string id;
  string customerId;
  string restaurantId;
  vector<OrderItem> orderItems;
  int totalAmount;
  OrderStatus orderStatus;
  string paymentId;
  mutex mtx;

public:
  Order(const string &id, const string &customerId, const string &restaurantId,
        vector<OrderItem> &orderItems)
      : id(id), customerId(customerId), restaurantId(restaurantId),
        totalAmount(0), orderStatus(OrderStatus::CREATED),
        orderItems(orderItems) {
    for (auto &item : orderItems) {
      totalAmount += item.getQuantity() * item.getUnitPrice();
    }
  }

  string getId() const { return id; }
  string getCustomerId() const { return customerId; }
  string getRestaurantId() const { return restaurantId; }
  int getTotalAmount() const { return totalAmount; }
  string getPaymentId() const { return paymentId; }
  vector<string> getItemsIds() const {
    vector<string> itemIds;
    for (auto orderItem : orderItems) {
      itemIds.push_back(orderItem.getItemId());
    }

    return itemIds;
  }

  OrderStatus getStatus() {
    lock_guard<mutex> lock(mtx);
    return orderStatus;
  }

  void setPaymentId(string paymentId) { this->paymentId = paymentId; }

  void markPlaced() {
    lock_guard<mutex> lock(mtx);
    if (orderStatus != OrderStatus::CREATED) {
      throw runtime_error("Cannot mark placed");
    }
    orderStatus = OrderStatus::PLACED;
    cout << "ORDER: " << this->id << " Placed" << endl;
  }

  void markAccepted() {
    lock_guard<mutex> lock(mtx);
    if (orderStatus != OrderStatus::PLACED) {
      throw runtime_error("Cannot mark restaurant accepted");
    }
    orderStatus = OrderStatus::RESTAURANT_ACCEPTED;
    cout << "ORDER: " << this->id << " Accepted" << endl;
  }

  void markCancelled() {
    lock_guard<mutex> lock(mtx);
    if (orderStatus != OrderStatus::CREATED &&
        orderStatus != OrderStatus::PLACED) {
      throw runtime_error("Cannot mark cancelled");
    }
    orderStatus = OrderStatus::CANCELLED;
    cout << "ORDER: " << this->id << " Cancelled" << endl;
  }

  void markPreparing() {
    lock_guard<mutex> lock(mtx);
    if (orderStatus != OrderStatus::RESTAURANT_ACCEPTED) {
      throw runtime_error("Cannot mark preparing");
    }
    orderStatus = OrderStatus::PREPARING;
    cout << "ORDER: " << this->id << " Preparing" << endl;
  }

  void markPrepared() {
    lock_guard<mutex> lock(mtx);
    if (orderStatus != OrderStatus::PREPARING) {
      throw runtime_error("Cannot mark prepared");
    }

    orderStatus = OrderStatus::WAITING_TO_PICKED;
    cout << "ORDER: " << this->id << " waiting to be picked" << endl;
  }

  void markPicked() {
    lock_guard<mutex> lock(mtx);
    if (orderStatus != OrderStatus::WAITING_TO_PICKED) {
      throw runtime_error("Cannot mark picked");
    }

    orderStatus = OrderStatus::DELIVERY_ON_WAY;
    cout << "ORDER: " << this->id << " ON WAY" << endl;
  }

  void markDelivered() {
    lock_guard<mutex> lock(mtx);
    if (orderStatus != OrderStatus::DELIVERY_ON_WAY) {
      throw runtime_error("Cannot mark delivered");
    }

    orderStatus = OrderStatus::DELIVERED;
    cout << "ORDER: " << this->id << " Delivered" << endl;
  }

  void markFailed() {
    lock_guard<mutex> lock(mtx);
    if (orderStatus != OrderStatus::CREATED &&
        orderStatus != OrderStatus::PLACED) {
      throw runtime_error("Cannot fail the order");
    }

    orderStatus = OrderStatus::FAILED;
  }
};

enum class PaymentStatus {
  PENDING,
  DONE,
  FAILED,
  REFUNDED,
};

class Payment {
private:
  string id;
  string orderId;
  int amount;
  PaymentStatus status;

public:
  Payment(const string &id, const string &orderId, int amount)
      : id(id), orderId(orderId), amount(amount),
        status(PaymentStatus::PENDING) {}

  void pay() { status = PaymentStatus::DONE; }
  void refund() { status = PaymentStatus::REFUNDED; }
};

enum class DeliveryStatus {
  YET_TO_PICK,
  ON_WAY,
  DELIVERED,
};

class DeliveryTask {
private:
  string id;
  string orderId;
  string agentId;
  DeliveryStatus status;
  mutex mtx;

public:
  DeliveryTask(const string &id, const string &orderId)
      : id(id), orderId(orderId), status(DeliveryStatus::YET_TO_PICK) {}

  string getId() const { return id; }
  string getOrderId() const { return orderId; }
  string getAgentId() const { return agentId; }

  DeliveryStatus getStatus() {
    lock_guard<mutex> lock(mtx);
    return status;
  }

  void markOnWay(string agentId) {
    lock_guard<mutex> lock(mtx);
    if (status != DeliveryStatus::YET_TO_PICK) {
      throw runtime_error("Cannot mark on the way");
    }

    this->agentId = agentId;
    status = DeliveryStatus::ON_WAY;
  }

  void markDelivered() {
    lock_guard<mutex> lock(mtx);
    if (status != DeliveryStatus::ON_WAY) {
      throw runtime_error("Cannot mark delivered");
    }
    status = DeliveryStatus::DELIVERED;
  }
};

class RestaurantService {
private:
  unordered_map<string, unique_ptr<Restaurant>> restaurants;

  atomic<int> count = 1;

public:
  string addRestaurant(const string &name) {
    const string id = "RES_" + to_string(count.fetch_add(1));
    auto restaurant = make_unique<Restaurant>(id, name);
    restaurants[id] = std::move(restaurant);
    return id;
  }

  string addMenuItem(const string &restaurantId, const string &itemName,
                     const int price) {
    if (!restaurants.count(restaurantId)) {
      throw runtime_error("restaurant not found");
    }

    return restaurants[restaurantId]->addMenu(itemName, price);
  }

  void displayMenu(const string &restaurantId) {
    if (!restaurants.count(restaurantId)) {
      throw runtime_error("restaurant not found");
    }

    restaurants[restaurantId]->displayMenu();
  }

  void updateMenuPrice(const string &restaurantId, const string &itemId,
                       const int price) {
    if (!restaurants.count(restaurantId)) {
      throw runtime_error("restaurant not found");
    }
    restaurants[restaurantId]->changePrice(itemId, price);
  }

  void updateAvailability(const string &restaurantId, const string &itemId,
                          const bool available) {
    if (!restaurants.count(restaurantId)) {
      throw runtime_error("restaurant not found");
    }
    restaurants[restaurantId]->setAvailability(itemId, available);
  }

  shared_ptr<MenuItem> getMenuItem(const string &restaurantId,
                                   const string &menuItemId) {
    if (!restaurants.count(restaurantId)) {
      throw runtime_error("Restaurant not present");
    }
    return restaurants[restaurantId]->getItem(menuItemId);
  }

  bool canAcceptOrder(const string &restaurantId, vector<string> &itemIds) {
    if (!restaurants.count(restaurantId)) {
      throw runtime_error("Restaurant not present");
    }
    return restaurants[restaurantId]->isAvailable(itemIds);
  }
};

class PaymentService {
private:
  unordered_map<string, unique_ptr<Payment>> payments;

public:
  string createPayment(const string &orderId, int amount) {
    const string id = "PAY_" + orderId;
    auto payment = make_unique<Payment>(id, orderId, amount);
    payments[id] = std::move(payment);
    return id;
  }

  bool pay(const string &paymentId) {
    if (!payments.count(paymentId)) {
      throw runtime_error("Payment not found");
    }
    payments[paymentId]->pay();
    return true;
  }

  bool refund(const string &paymentId) {
    if (!payments.count(paymentId)) {
      throw runtime_error("Payment not found");
    }
    payments[paymentId]->refund();

    return true;
  }
};

class DeliveryService {
private:
  unordered_map<string, unique_ptr<DeliveryTask>> tasks;
  unordered_map<string, unique_ptr<DeliveryAgent>> agents;

  atomic<int> agentCounter = 1;

public:
  string registerAgent(const string &name) {
    const string id = "AGENT_" + to_string(agentCounter.fetch_add(1));
    auto agent = make_unique<DeliveryAgent>(id, name);
    agents[id] = std::move(agent);
    return id;
  }

  string createDeliveryTask(const string &orderId) {
    const string id = "DEL_" + orderId;
    auto task = make_unique<DeliveryTask>(id, orderId);
    tasks[id] = std::move(task);
    return id;
  }

  void acceptTask(const string &agentId, const string &taskId) {
    if (!agents.count(agentId)) {
      throw runtime_error("Agent not found");
    }

    auto &agent = agents[agentId];

    if (!tasks.count(taskId)) {
      throw runtime_error("task not found");
    }

    auto &task = tasks[taskId];

    if (!agent->isAvailable()) {
      throw runtime_error("Agent not available");
    }

    task->markOnWay(agentId);
    agent->setAvailability(false);
    cout << "Task: " << taskId << " Out for Delivery" << endl;
  }

  void markDelivered(const string &agentId, const string &taskId) {
    if (!agents.count(agentId)) {
      throw runtime_error("Agent not found");
    }

    auto &agent = agents[agentId];

    if (!tasks.count(taskId)) {
      throw runtime_error("task not found");
    }

    auto &task = tasks[taskId];
    task->markDelivered();
    agent->setAvailability(true);
    cout << "Task: " << taskId << " Delivered" << endl;
  }
};

class OrderService {
private:
  unordered_map<string, unique_ptr<Order>> orders;
  atomic<int> orderCount = 1;
  RestaurantService &restaurantService;
  PaymentService &paymentService;
  DeliveryService &deliveryService;

public:
  OrderService(RestaurantService &restaurantService,
               PaymentService &paymentService, DeliveryService &deliveryService)
      : restaurantService(restaurantService), paymentService(paymentService),
        deliveryService(deliveryService) {}

  string createOrder(const string &customerId, const string &restaurantId,
                     unordered_map<string, int> items) {
    vector<OrderItem> orderItems;
    for (auto &[itemId, quantity] : items) {
      auto item = restaurantService.getMenuItem(restaurantId, itemId);
      orderItems.emplace_back(itemId, item->getName(), item->getPrice(),
                              quantity);
    }
    const string id = "ORD_" + to_string(orderCount.fetch_add(1));
    auto order = make_unique<Order>(id, customerId, restaurantId, orderItems);
    auto orderIds = order->getItemsIds();
    if (!restaurantService.canAcceptOrder(order->getRestaurantId(), orderIds)) {
      cout << "Cannot accept order" << endl;
      return id;
    }
    auto paymentId = paymentService.createPayment(id, order->getTotalAmount());
    order->setPaymentId(paymentId);
    orders[id] = std::move(order);
    return id;
  }

  bool makePayment(const string &orderId) {
    if (!orders.count(orderId)) {
      throw runtime_error("order not found");
    }

    auto &order = orders[orderId];

    paymentService.pay(order->getPaymentId());
    order->markPlaced();

    return true;
  }

  void processRefund(const string &orderId) {
    if (!orders.count(orderId)) {
      throw runtime_error("order not found");
    }

    auto &order = orders[orderId];

    paymentService.refund(order->getPaymentId());
  }

  string getPaymentId(const string &orderId) {
    if (!orders.count(orderId)) {
      throw runtime_error("order not found");
    }
    return orders[orderId]->getPaymentId();
  }

  OrderStatus getOrderStatus(const string &orderId) {
    if (!orders.count(orderId)) {
      throw runtime_error("order not found");
    }
    return orders[orderId]->getStatus();
  }

  void cancelOrder(const string &orderId) {
    if (!orders.count(orderId)) {
      throw runtime_error("order not found");
    }

    orders[orderId]->markCancelled();
  }

  void acceptOrder(const string &orderId) {
    if (!orders.count(orderId)) {
      throw runtime_error("order not found");
    }
    auto &order = orders[orderId];
    auto orderIds = order->getItemsIds();
    if (!restaurantService.canAcceptOrder(order->getRestaurantId(), orderIds)) {
      cout << "Cannot accept order. processing refund" << endl;
      processRefund(orderId);
      return;
    }
    order->markAccepted();
  }

  void markPreparing(const string &orderId) {
    if (!orders.count(orderId)) {
      throw runtime_error("order not found");
    }
    orders[orderId]->markPreparing();
  }

  string markReady(const string &orderId) {
    if (!orders.count(orderId)) {
      throw runtime_error("order not found");
    }
    orders[orderId]->markPrepared();
    return deliveryService.createDeliveryTask(orderId);
  }

  void markOutForDelivery(const string &orderId) {
    if (!orders.count(orderId)) {
      throw runtime_error("order not found");
    }
    orders[orderId]->markPicked();
  }

  void markDelivered(const string &orderId) {
    if (!orders.count(orderId)) {
      throw runtime_error("order not found");
    }
    orders[orderId]->markDelivered();
  }
};

class NotificationService;
class OrderEventPublisher;

int main() {
  RestaurantService restaurantService;
  PaymentService paymentService;
  DeliveryService deliveryService;
  OrderService orderService(restaurantService, paymentService, deliveryService);

  string a1 = deliveryService.registerAgent("XY");

  string r1 = restaurantService.addRestaurant("Desi dhaba");
  string i1 = restaurantService.addMenuItem(r1, "Veg Kolhapuri", 230);
  string i2 = restaurantService.addMenuItem(r1, "Butter Naan", 30);

  restaurantService.displayMenu(r1);

  auto u1 = User("U1", "AH");
  unordered_map<string, int> order;
  order[i1] = 1;
  order[i2] = 5;
  string o1 = orderService.createOrder(u1.getId(), r1, order);

  orderService.makePayment(o1);

  // restaurantService.updateAvailability(r1, i1, false);
  orderService.acceptOrder(o1);
  orderService.markPreparing(o1);
  string d1 = orderService.markReady(o1);
  deliveryService.acceptTask(a1, d1);
  orderService.markOutForDelivery(o1);

  deliveryService.markDelivered(a1, d1);
  orderService.markDelivered(o1);
  return 0;
}
