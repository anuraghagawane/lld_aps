#include <atomic>
#include <iostream>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

enum class Item { TOMATO, POTATO, BREAD, EGG };

string getItemName(Item item) {
  switch (item) {
  case Item::TOMATO:
    return "tomato";
  case Item::POTATO:
    return "potato";
  case Item::BREAD:
    return "bread";
  case Item::EGG:
    return "egg";
  default:
    return "";
  };

  return "";
}

class Recipe {
private:
  unordered_map<Item, int> quantity;

public:
  void update(Item item, int quant) { quantity[item] = quant; }

  vector<pair<Item, int>> getRecipeItem() {
    vector<pair<Item, int>> items;
    for (auto [item, quant] : quantity) {
      items.push_back({item, quant});
    }

    return items;
  }
};

class Ingredients {
private:
  unordered_map<Item, int> quantity;
  mutex mu;

public:
  bool checkAndReserve(vector<Recipe> &recipes) {
    lock_guard<mutex> lock(mu);
    unordered_map<Item, int> required;
    for (auto r : recipes) {
      for (auto [item, q] : r.getRecipeItem()) {
        required[item] += q;
      }
    }

    if (!check(required)) {
      return false;
    }

    for (auto [item, quant] : required) {
      quantity[item] -= quant;
    }

    return true;
  }

  void refill(vector<pair<Item, int>> items) {
    lock_guard<mutex> lock(mu);
    for (auto [item, quant] : items) {
      quantity[item] += quant;
    }
  }

  void releaseReserved(vector<Recipe> &recipes) {
    lock_guard<mutex> lock(mu);
    unordered_map<Item, int> required;
    for (auto r : recipes) {
      for (auto [item, q] : r.getRecipeItem()) {
        required[item] += q;
      }
    }

    for (auto [item, quant] : required) {
      quantity[item] += quant;
    }
  }

  void display() {
    lock_guard<mutex> lock(mu);
    cout << "Stock: " << endl;
    for (auto [item, q] : quantity) {
      cout << getItemName(item) << ": " << q << endl;
    }
  }

private:
  // internal fucntion doesn't need to acquire lock
  bool check(unordered_map<Item, int> &required) {
    for (auto [item, quant] : required) {
      if (quantity[item] < quant) {
        return false;
      }
    }
    return true;
  }
};

class Dish {
private:
  string name;
  Recipe recipe;
  int price;

public:
  Dish(const string &name, Recipe recipe, int price)
      : name(name), recipe(recipe), price(price) {}

  string getName() const { return name; }

  Recipe &getRecipe() { return recipe; }

  int getPrice() const { return price; }
};

enum class Role {
  CHEF,
  WAITER,
};

class Staff {
private:
  string id;
  string name;
  Role role;

public:
  Staff(const string &id, const string &name, Role role)
      : id(id), name(name), role(role) {}

  string getId() const { return id; }
  string getName() const { return name; }
  Role getRole() const { return role; }
};

enum class OrderStatus {
  PLACED,
  PREPARING,
  COMPLETED,
  FAILED,
};

class Order {
private:
  string id;
  int amount;
  vector<Dish> dishes;
  OrderStatus status;
  shared_ptr<Staff> waiter;
  shared_ptr<Staff> cook;

public:
  Order(const string &id, vector<Dish> dishes, shared_ptr<Staff> waiter,
        shared_ptr<Staff> cook)
      : id(id), dishes(dishes), waiter(waiter), cook(cook) {
    int totalAmount = 0;
    for (auto dish : dishes)
      totalAmount += dish.getPrice();
    this->amount = totalAmount;
    this->status = OrderStatus::PLACED;
  }

  string getId() const { return id; }

  int getTotalAmount() const { return amount; }

  vector<Dish> getDishes() { return dishes; }

  shared_ptr<Staff> getWaiter() { return waiter; }
  shared_ptr<Staff> getCook() { return cook; }

  void prepare() { this->status = OrderStatus::PREPARING; }

  void complete() { this->status = OrderStatus::COMPLETED; }

  void fail() { this->status = OrderStatus::FAILED; }
};

class PaymentProvider {
public:
  virtual bool pay(int amount) = 0;
};

class UPIPayment : public PaymentProvider {
private:
  string upiId;

public:
  UPIPayment(const string &upiId) : upiId(upiId) {}
  bool pay(int amount) override {
    cout << "Payed " << amount << " using UPI" << endl;
    return true;
  }
};

class CardPayment : public PaymentProvider {
private:
  string cardId;

public:
  CardPayment(const string &cardId) : cardId(cardId) {}
  bool pay(int amount) override {
    cout << "Payed " << amount << " using CARD" << endl;
    return true;
  }
};

enum PaymentStatus {
  INITIALIZED,
  PAYED,
  FAILED,
};

class Payment {
private:
  string id;
  string orderId;
  int amount;
  PaymentProvider &paymentProvider;
  PaymentStatus status;

public:
  Payment(const string &id, const string &orderId, int amount,
          PaymentProvider &paymentProvider)
      : id(id), orderId(orderId), amount(amount),
        paymentProvider(paymentProvider), status(PaymentStatus::INITIALIZED) {}

  bool pay() {
    if (paymentProvider.pay(amount)) {
      status = PaymentStatus::PAYED;
      return true;
    }
    status = PaymentStatus::FAILED;
    return false;
  };

  string getId() const { return id; }
  string getOrderId() const { return orderId; }
  int getAmount() const { return amount; }

  bool isDone() { return status == PaymentStatus::PAYED; }
};

class OrderService {
private:
  unordered_map<string, shared_ptr<Order>> orders;
  atomic<int> ordercount = 0;
  mutex mu;

public:
  string createOrder(vector<Dish> dishes, shared_ptr<Staff> waiter,
                     shared_ptr<Staff> cook) {
    string orderId = "ORD_" + to_string(ordercount.fetch_add(1));
    auto order = make_shared<Order>(orderId, dishes, waiter, cook);
    lock_guard<mutex> lock(mu);
    orders[orderId] = order;
    return orderId;
  }

  shared_ptr<Order> getOrder(string orderId) {
    lock_guard<mutex> lock(mu);
    if (!orders.count(orderId)) {
      throw runtime_error("Order not found");
    }
    return orders[orderId];
  }

  int getAmount(string orderId) {
    lock_guard<mutex> lock(mu);
    if (!orders.count(orderId)) {
      throw runtime_error("Order not found");
    }
    return orders[orderId]->getTotalAmount();
  }
};

class PaymentService {
  unordered_map<string, shared_ptr<Payment>> payments;
  atomic<int> paymentcount = 0;
  mutex mu;

public:
  string createPayment(string orderId, int amount,
                       PaymentProvider &paymentProvider) {
    string paymentid = "PAY_" + to_string(paymentcount.fetch_add(1));

    auto payment =
        make_shared<Payment>(paymentid, orderId, amount, paymentProvider);

    lock_guard<mutex> lock(mu);
    payments[paymentid] = payment;

    return paymentid;
  }

  bool makePayment(string paymentid) {
    shared_ptr<Payment> payment;
    {
      lock_guard<mutex> lock(mu);
      if (!payments.count(paymentid)) {
        throw runtime_error("Payment not found!");
      }
      payment = payments[paymentid];
    }
    return payment->pay();
  }
};

class ManagementService {
  vector<shared_ptr<Staff>> waiters;
  vector<shared_ptr<Staff>> cooks;
  Ingredients &ingredients;
  OrderService &orderService;
  PaymentService &paymentService;

  atomic<int> currWaiter = 1;
  atomic<int> currCook = 1;

public:
  ManagementService(Ingredients &ingredients, OrderService &orderService,
                    PaymentService &paymentService)
      : ingredients(ingredients), orderService(orderService),
        paymentService(paymentService) {}

  void addStaff(shared_ptr<Staff> staff) {
    if (staff->getRole() == Role::CHEF) {
      cooks.push_back(staff);
    } else {
      waiters.push_back(staff);
    }
  }

  string createOrder(vector<Dish> dishes) {
    if (waiters.empty() || cooks.empty()) {
      throw runtime_error("No waiter or cooks available");
    }
    vector<Recipe> recipes;
    for (auto d : dishes)
      recipes.push_back(d.getRecipe());
    if (!ingredients.checkAndReserve(recipes)) {
      throw runtime_error("Failed to reserve ingredients");
    }

    int waiterIdx = currWaiter.fetch_add(1) % waiters.size();
    int cookIdx = currCook.fetch_add(1) % cooks.size();

    cout << waiterIdx << " " << cookIdx << endl;
    auto waiter = waiters[waiterIdx];
    auto cook = cooks[cookIdx];

    auto orderId = orderService.createOrder(dishes, waiter, cook);

    return orderId;
  }

  bool makePayment(string orderId, PaymentProvider &paymentProvider) {
    auto payment = paymentService.createPayment(
        orderId, orderService.getAmount(orderId), paymentProvider);

    if (!paymentService.makePayment(payment)) {
      auto order = orderService.getOrder(orderId);
      vector<Recipe> recipes;
      for (auto d : order->getDishes())
        recipes.push_back(d.getRecipe());

      ingredients.releaseReserved(recipes);
      order->fail();

      cout << "FAILED TO CREATE ORDER AND RESTOCKING THE INGREDIENTS" << endl;
      return false;
    }

    auto order = orderService.getOrder(orderId);
    prepare(order);

    cout << "Order completed" << endl;

    return true;
  }

private:
  void prepare(shared_ptr<Order> order) {
    order->prepare();
    cout << "Preparing order ....." << endl;
    order->complete();
  }
};

int main() {
  auto orderService = OrderService();
  auto paymentService = PaymentService();
  auto ingredients = Ingredients();
  ingredients.refill({{Item::BREAD, 10},
                      {Item::EGG, 10},
                      {Item::POTATO, 10},
                      {Item::TOMATO, 10}});
  auto managementSvc =
      ManagementService(ingredients, orderService, paymentService);

  auto staff1 = make_shared<Staff>("S1", "abc", Role::CHEF);
  auto staff2 = make_shared<Staff>("S2", "def", Role::CHEF);
  auto staff3 = make_shared<Staff>("S3", "ght", Role::WAITER);
  auto staff4 = make_shared<Staff>("S4", "xyz", Role::WAITER);
  managementSvc.addStaff(staff1);
  managementSvc.addStaff(staff2);
  managementSvc.addStaff(staff3);
  managementSvc.addStaff(staff4);

  auto pp1 = UPIPayment("ABC");

  auto recipe = Recipe();
  recipe.update(Item::BREAD, 2);
  recipe.update(Item::EGG, 2);
  recipe.update(Item::POTATO, 2);
  recipe.update(Item::TOMATO, 2);

  auto dish1 = Dish("dish1", recipe, 200);

  ingredients.display();
  auto orderId = managementSvc.createOrder({dish1});
  managementSvc.makePayment(orderId, pp1);

  ingredients.display();
  return 0;
}
