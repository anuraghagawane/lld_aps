#include <atomic>
#include <iostream>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <unordered_map>

using namespace std;

enum class Ingredient {
  MILK,
  COFFEE,
  SUGAR,
};

string getIngredientName(Ingredient ingredient) {
  if (Ingredient::MILK == ingredient)
    return "Milk";
  if (Ingredient::COFFEE == ingredient)
    return "Coffee";
  if (Ingredient::SUGAR == ingredient)
    return "Sugar";

  return "No name found";
}

class Coffee {
private:
  string name;
  double price;
  unordered_map<Ingredient, int> ingredients;

public:
  Coffee(string name, double price, unordered_map<Ingredient, int> ingredients)
      : name(name), price(price), ingredients(ingredients) {}

  // getter
  string getName() const { return name; }

  double getPrice() const { return price; }

  unordered_map<Ingredient, int> getIngredients() { return ingredients; }
};

class Inventory {
private:
  unordered_map<Ingredient, int> inventory;
  mutable mutex mu;

public:
  void add(Ingredient ingredient, int amount) {
    lock_guard<mutex> lock(mu);
    inventory[ingredient] += amount;
  }

  bool checkStock(shared_ptr<Coffee> coffee) {
    lock_guard<mutex> lock(mu);
    return check(coffee);
  }

  bool reserve(shared_ptr<Coffee> coffee) {
    lock_guard<mutex> lock(mu);
    if (!check(coffee))
      return false;

    for (auto [in, req] : coffee->getIngredients()) {
      inventory[in] -= req;
    }

    return true;
  }

  void showStock() {
    cout << "STOCK: " << endl;
    for (auto [in, stock] : inventory) {
      cout << getIngredientName(in) << " " << stock << endl;
    }
  }

private:
  bool check(shared_ptr<Coffee> coffee) {
    for (auto [in, req] : coffee->getIngredients()) {
      if (inventory[in] < req) {
        return false;
      }
    }

    return true;
  }
};

enum class OrderStatus {
  CREATED,
  PAYMENT_DONE,
  REFUNDED,
  COMPLETED,
};

class Order {
private:
  string id;
  shared_ptr<Coffee> coffee;
  double billCost;
  OrderStatus status;

public:
  Order(const string &id, shared_ptr<Coffee> coffee)
      : id(id), coffee(coffee), status(OrderStatus::CREATED) {
    billCost = coffee->getPrice();
  }

  string getId() { return id; }
  shared_ptr<Coffee> getCoffee() const { return coffee; }
  double getBillCost() const { return billCost; }
  OrderStatus getStatus() const { return status; }

  void setStatus(OrderStatus status) { this->status = status; }

  void setPaymentDone() { status = OrderStatus::PAYMENT_DONE; }

  void setRefund() { status = OrderStatus::REFUNDED; }

  void complete() { status = OrderStatus::COMPLETED; }
};

class OrderService {
private:
  unordered_map<string, shared_ptr<Order>> orders;
  atomic<int> counter{1};

public:
  shared_ptr<Order> createOrder(shared_ptr<Coffee> coffee) {
    string id = "ORD_" + to_string(counter.fetch_add(1));
    auto order = make_shared<Order>(id, coffee);
    orders[order->getId()] = order;
    return order;
  }

  shared_ptr<Order> getOrder(const string &orderId) {
    if (!orders.count(orderId)) {
      throw runtime_error("Order not found!");
    }
    return orders[orderId];
  }
};

enum class PaymentStatus { PENDING, COMPLETED, REFUNDED, FAILED };

class Payment {
private:
  string id;
  shared_ptr<Order> order;
  PaymentStatus status;

public:
  Payment(const string &id, shared_ptr<Order> order)
      : id(id), order(order), status(PaymentStatus::PENDING) {}

  void complete() { status = PaymentStatus::COMPLETED; }
  void refund() { status = PaymentStatus::REFUNDED; }
  string getId() { return id; }
};

class PaymentService {
private:
  unordered_map<string, shared_ptr<Payment>> payments;
  atomic<int> counter{1};

public:
  shared_ptr<Payment> createPayment(shared_ptr<Order> order) {
    string id = "PAY_" + to_string(counter.fetch_add(1));
    auto payment = make_shared<Payment>(id, order);
    payments[payment->getId()] = payment;
    return payment;
  }

  shared_ptr<Payment> getPayment(const string &paymentId) {
    if (!payments.count(paymentId)) {
      throw runtime_error("Payment not found!");
    }
    return payments[paymentId];
  }
};

class CoffeeMaker {
  shared_ptr<Inventory> inventory;

public:
  CoffeeMaker(shared_ptr<Inventory> inventory) : inventory(inventory) {}
  bool prepare(shared_ptr<Order> order) {
    auto coffee = order->getCoffee();
    if (!inventory->checkStock(coffee)) {
      return false;
    }

    if (!inventory->reserve(coffee)) {
      return false;
    }

    cout << "Dispensing...." << endl;
    return true;
  }

  void showStock() { inventory->showStock(); }
};

class VendingMachine {
  unordered_map<string, shared_ptr<Coffee>> coffeeMenu;
  OrderService orderSvc;
  PaymentService paySvc;
  shared_ptr<CoffeeMaker> coffeeMaker;

public:
  VendingMachine(shared_ptr<CoffeeMaker> coffeeMaker)
      : coffeeMaker(coffeeMaker) {
    // string name, double price, unordered_map<Ingredient, int> ingredients
    unordered_map<Ingredient, int> capIng = {{Ingredient::MILK, 10},
                                             {Ingredient::COFFEE, 5},
                                             {Ingredient::SUGAR, 3}};
    auto cap = make_shared<Coffee>("cappucino", 20, capIng);

    coffeeMenu[cap->getName()] = cap;

    unordered_map<Ingredient, int> espIng = {{Ingredient::COFFEE, 5},
                                             {Ingredient::SUGAR, 3}};
    auto esp = make_shared<Coffee>("Espresso", 30, capIng);
    coffeeMenu[esp->getName()] = esp;
  }

  void showMenu() {
    cout << "MENU: " << endl;
    for (auto [name, item] : coffeeMenu) {
      cout << "name: " << name << ", price: " << item->getPrice() << endl;
    }
  }

  void showStock() { coffeeMaker->showStock(); }

  void makeOrder(string coffeeId) {
    if (!coffeeMenu.count(coffeeId)) {
      cout << "WRONG coffeeId entered" << endl;
      return;
    }
    auto coffee = coffeeMenu[coffeeId];
    auto order = orderSvc.createOrder(coffee);

    auto payment = paySvc.createPayment(order);
    payment->complete(); // for now we will just make it pass
    order->setPaymentDone();

    if (!coffeeMaker->prepare(order)) {
      payment->refund();
      order->setRefund();
      cout << "FAILED TO PREPARE, REFUND DONE!" << endl;
      return;
    }

    order->complete();
    cout << "Order: " << order->getId() << ", Payment: " << payment->getId()
         << ", Status: Done" << endl;
  }
};

int main() {
  cout << "Coffee Vending Machine" << endl;
  auto inventory = make_shared<Inventory>();
  inventory->add(Ingredient::COFFEE, 50);
  inventory->add(Ingredient::MILK, 50);
  inventory->add(Ingredient::SUGAR, 50);
  auto coffeeMaker = make_shared<CoffeeMaker>(inventory);
  VendingMachine vendingMachine(coffeeMaker);

  vendingMachine.showMenu();
  vendingMachine.showStock();
  vendingMachine.makeOrder("Espresso");
  vendingMachine.makeOrder("Espresso");
  vendingMachine.makeOrder("Espresso");
  vendingMachine.makeOrder("cappucino");
  vendingMachine.makeOrder("Espresso");
  vendingMachine.makeOrder("Espresso");
  vendingMachine.showStock();
  return 0;
}
