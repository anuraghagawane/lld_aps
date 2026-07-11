#include <algorithm>
#include <atomic>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class Order;

class User {
private:
  string id;
  string name;
  string email;
  vector<shared_ptr<Order>> orderHistory;

public:
  User(const string &id, const string &name, const string &email)
      : id(id), name(name), email(email) {}

  string getId() const { return id; }
  string getName() const { return name; }
  string getEmail() const { return email; }

  vector<shared_ptr<Order>> &getOrderHistory() { return orderHistory; }
  void addOrderToHistory(shared_ptr<Order> order) {
    orderHistory.push_back(order);
  }
};

class Product {
private:
  string id;
  string name;
  int price; // in paisa

public:
  Product(const string &id, const string &name, const int price)
      : id(id), name(name), price(price) {}

  string getId() const { return id; }
  string getName() const { return name; }

  int getPrice() const { return price; }
};

class CartItem {
private:
  shared_ptr<Product> product;
  int quantity;

public:
  CartItem(shared_ptr<Product> product, const int quantity)
      : product(product), quantity(quantity) {}

  shared_ptr<Product> getProduct() { return product; }
  const int getQuantity() { return quantity; }
};

class Cart {
private:
  vector<shared_ptr<CartItem>> items;

public:
  void addItem(shared_ptr<CartItem> item) { items.push_back(item); }
  void removeItem(shared_ptr<CartItem> item) {
    items.erase(remove(items.begin(), items.end(), item), items.end());
  }

  vector<shared_ptr<CartItem>> getItems() { return items; }
};

class Inventory {
private:
  unordered_map<string, int> quantities;

public:
  int getQuantity(const string &id) { return quantities[id]; }
  void addQuantity(const string &id, const int quantity) {
    quantities[id] += quantity;
  }

  void reduceQuantity(const string &id, const int quantity) {
    if (quantities[id] < quantity) {
      throw runtime_error("Not enough quantity available");
    }
    quantities[id] -= quantity;
  }
};

class OrderItem {
private:
  shared_ptr<Product> product;
  int quantity;

public:
  OrderItem(shared_ptr<Product> product, const int quantity)
      : product(product), quantity(quantity) {}

  shared_ptr<Product> getProduct() { return product; }
  const int getQuantity() { return quantity; }
};

enum class OrderStatus {
  PAYMENT_PENDING,
  PAYMENT_DONE,
  SHIPPED,
  DELIVERED,
  FAILED,
};

class Order {
private:
  string orderId;
  string userId;
  vector<shared_ptr<OrderItem>> items;
  int totalPrice;
  string address;
  OrderStatus status;

public:
  Order(const string &orderId, vector<shared_ptr<OrderItem>> items,
        const int price, const string &address, const string &userId)
      : orderId(orderId), items(items), totalPrice(price), address(address),
        status(OrderStatus::PAYMENT_PENDING), userId(userId) {}

  string getOrderId() const { return orderId; }

  int getTotalPrice() const { return totalPrice; }

  string getAddress() const { return address; }

  OrderStatus getStatus() const { return status; }

  void paymentDone() { status = OrderStatus::PAYMENT_DONE; }

  void shipped() { status = OrderStatus::SHIPPED; }

  void delivered() { status = OrderStatus::DELIVERED; }

  void failed() { status = OrderStatus::FAILED; }
};

enum class PaymentStatus { PENDING, DONE, FAILED };

class Payment {
private:
  string paymentId;
  string orderId;
  PaymentStatus status;
  int amount;

public:
  Payment(const string &paymentId, const string &orderId, const int amount)
      : paymentId(paymentId), orderId(orderId), status(PaymentStatus::PENDING),
        amount(amount) {}

  PaymentStatus getStatus() const { return status; }

  string getOrderId() const { return orderId; }

  int getAmount() const { return amount; }

  void markPaid() { status = PaymentStatus::DONE; }
  void markFailed() { status = PaymentStatus::FAILED; }
};

class UserService {
private:
  unordered_map<string, shared_ptr<User>> users;

public:
  string createUser(const string &id, const string &name, const string &email) {
    auto user = make_shared<User>(id, name, email);
    users[user->getId()] = user;
    return user->getId();
  }

  void displayOrderHistory(const string &userId) {
    if (!isUserPresent(userId)) {
      throw runtime_error("User not present");
    }
    auto user = users[userId];
    auto orders = user->getOrderHistory();
    cout << "Order History of " << user->getName() << ":" << endl;
    for (auto &order : orders) {
      cout << " - Order id" << order->getOrderId()
           << " Price: " << order->getTotalPrice() << endl;
      ;
    }
  }

private:
  bool isUserPresent(const string &userId) { return users.count(userId); }
};

class CatalogService {
private:
  unordered_map<string, shared_ptr<Product>> products;

public:
  string addProduct(const string &id, const string &name, const int price) {
    auto product = make_shared<Product>(id, name, price);
    products[product->getId()] = product;
    return product->getId();
  }

  shared_ptr<Product> getProductById(const string &id) {
    if (!products.count(id)) {
      throw runtime_error("Product not present");
    }

    return products[id];
  }

  void displayAllProducts() {
    cout << "Products: " << endl;
    for (auto &product : products) {
      cout << " - " << product.second->getName() << " Rs."
           << product.second->getPrice() / 100 << endl;
    }
  }
};

class CartService {
private:
  unordered_map<string, shared_ptr<Cart>> carts;

public:
  void addCart(const string &userid) { carts[userid] = make_shared<Cart>(); }

  void addToCart(const string &userid, shared_ptr<Product> product,
                 const int quantity) {
    auto cartItem = make_shared<CartItem>(product, quantity);
    carts[userid]->addItem(cartItem);
  }

  shared_ptr<Cart> getCart(const string &userid) { return carts[userid]; }
};

class OrderService {
private:
  unordered_map<string, shared_ptr<Order>> orders;

  atomic<int> orderCounter;

public:
  string createOrder(shared_ptr<Cart> cart, const string &userId,
                     const string &address) {
    string orderId = "ORD_" + to_string(orderCounter.fetch_add(1));
    int totalPrice = 0;
    auto items = cart->getItems();
    vector<shared_ptr<OrderItem>> orderItems;
    for (auto &item : items) {
      orderItems.emplace_back(
          make_shared<OrderItem>(item->getProduct(), item->getQuantity()));
      totalPrice += item->getProduct()->getPrice() * item->getQuantity();
    }
    auto order =
        make_shared<Order>(orderId, orderItems, totalPrice, address, userId);

    orders[orderId] = order;
    return orderId;
  }

  int getPrice(const string &orderId) {
    return orders[orderId]->getTotalPrice();
  }

  void markPaid(const string &orderId) { orders[orderId]->paymentDone(); }
};

class InventoryService {
private:
  unordered_map<string, int> quantities;

public:
  void addItem(const string &productId, const int quantity) {
    quantities[productId] += quantity;
  }

  void removeItem(const string &productId, const int quantity) {
    if (!quantities.count(productId)) {
      throw runtime_error("Product not found in inventory");
    }

    if (quantities[productId] < quantity) {
      throw runtime_error("Stock not available");
    }

    quantities[productId] -= quantity;
  }

  bool isInStock(const string &productId, const int quantity) {
    if (!quantities.count(productId)) {
      throw runtime_error("Product not found in inventory");
    }

    return quantities[productId] >= quantity;
  }
};

class PaymentService {
  unordered_map<string, shared_ptr<Payment>> payments;

  atomic<int> paymentCounter;

public:
  string createPayment(const string &orderId, const int amount) {
    string paymentId = "PAY_" + to_string(paymentCounter.fetch_add(1));
    auto payment = make_shared<Payment>(paymentId, orderId, amount);
    payments[paymentId] = payment;
    return paymentId;
  }

  shared_ptr<Payment> getPayment(const string &paymentId) {
    checkIfExists(paymentId);
    return payments[paymentId];
  }

  void pay(const string &paymentId) {
    checkIfExists(paymentId);
    payments[paymentId]->markPaid();
    cout << "Payment done" << endl;
  }

private:
  void checkIfExists(const string &paymentId) {
    if (!payments.count(paymentId)) {
      throw runtime_error("Payment not available with id: " + paymentId);
    }
  }
};

class ShoppingService {
private:
  unique_ptr<UserService> userService;
  unique_ptr<CatalogService> catalogService;
  unique_ptr<CartService> cartService;
  unique_ptr<OrderService> orderService;
  unique_ptr<InventoryService> inventoryService;
  unique_ptr<PaymentService> paymentService;

public:
  ShoppingService()
      : userService(make_unique<UserService>()),
        catalogService(make_unique<CatalogService>()),
        cartService(make_unique<CartService>()),
        orderService(make_unique<OrderService>()),
        inventoryService(make_unique<InventoryService>()),
        paymentService(make_unique<PaymentService>()) {}

  string createUser(const string &userId, const string &name,
                    const string &email) {

    userService->createUser(userId, name, email);
    cartService->addCart(userId);
    return userId;
  }

  void printOrderHistory(const string &userId) {
    userService->displayOrderHistory(userId);
  }

  void displayAllProducts() { catalogService->displayAllProducts(); }

  string addProduct(const string &id, const string &name, const int price,
                    const int quantity) {
    auto productId = catalogService->addProduct(id, name, price);
    inventoryService->addItem(productId, quantity);
    return productId;
  }

  void addToCart(const string &userId, const string &productId,
                 const int quantity) {
    cartService->addToCart(userId, catalogService->getProductById(productId),
                           quantity);
    cout << "Added to cart: " << productId << endl;
  }

  pair<string, string> createOrder(const string &userId,
                                   const string &address) {
    auto cart = cartService->getCart(userId);
    // check for Stock
    for (auto product : cart->getItems()) {
      if (!inventoryService->isInStock(product->getProduct()->getId(),
                                       product->getQuantity())) {
        throw runtime_error("Product not in stock:" +
                            product->getProduct()->getName());
      }
    }

    auto orderId = orderService->createOrder(cart, userId, address);

    for (auto product : cart->getItems()) {
      inventoryService->removeItem(product->getProduct()->getId(),
                                   product->getQuantity());
    }
    auto paymentId =
        paymentService->createPayment(orderId, orderService->getPrice(orderId));

    cout << "Order created, orderId: " << orderId << " PaymentId: " << paymentId
         << endl;

    return {orderId, paymentId};
  }

  void completePayment(const string &paymentId) {
    paymentService->pay(paymentId);
    auto payment = paymentService->getPayment(paymentId);
    orderService->markPaid(payment->getOrderId());

    cout << "Payment Completed: " << paymentId << endl;
  }
};

int main() {
  ShoppingService shoppingService = ShoppingService();
  auto u1 = shoppingService.createUser("U1", "anurag", "ah@gmail.com");

  auto p1 = shoppingService.addProduct("P1", "Iphone", 20, 10);
  auto p2 = shoppingService.addProduct("P2", "Fridge", 200, 10);

  shoppingService.displayAllProducts();

  shoppingService.addToCart("U1", "P1", 1);
  shoppingService.addToCart("U1", "P2", 1);

  auto [o1, pay1] = shoppingService.createOrder("U1", "ABC road");

  shoppingService.completePayment(pay1);
  return 0;
}
