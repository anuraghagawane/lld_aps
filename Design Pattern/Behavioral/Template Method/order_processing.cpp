#include <iostream>
#include <string>
using namespace std;

class Order {
  string id;
  double subtotal;

public:
  Order(string id, double subtotal) : id(id), subtotal(subtotal) {}
  string getId() { return id; }
  double getSubtotal() { return subtotal; }
};

class OrderProcessor {
public:
  virtual ~OrderProcessor() = default;

  void processOrder(Order *order) {
    validateOrder(order);
    calculateTotal(order);
    applyDiscount(order);
    processPayment(order);
    sendConfirmation(order);
    cout << "Order processing complete: " << order->getId() << endl;
  }

protected:
  virtual void validateOrder(Order *order) = 0;
  virtual void calculateTotal(Order *order) = 0;
  virtual void processPayment(Order *order) = 0;

  virtual void applyDiscount(Order *order) {}

  virtual void sendConfirmation(Order *order) {
    cout << "Sending email confirmation for order " << order->getId() << endl;
  }
};

class StandardOrderProcessor : public OrderProcessor {
protected:
  void validateOrder(Order *order) override {
    cout << "Validating standard order: checking stock and address..." << endl;
  }

  void calculateTotal(Order *order) override {
    double shipping = 5.99;
    double total = order->getSubtotal() + shipping;
    cout << "Standard total: $" << total << " (including $" << shipping
         << " shipping)" << endl;
  }

  void processPayment(Order *order) override {
    cout << "Processing payment via standard gateway..." << endl;
  }
};

class PrimeOrderProcessor : public OrderProcessor {
protected:
  void validateOrder(Order *order) override {
    cout << "Validating Prime order: checking membership and stock..." << endl;
  }

  void calculateTotal(Order *order) override {
    cout << "Prime total: $" << order->getSubtotal() << " (free shipping)"
         << endl;
  }

  void processPayment(Order *order) override {
    cout << "Processing payment via Prime billing..." << endl;
  }

  void applyDiscount(Order *order) override {
    cout << "Applying 10% Prime member discount..." << endl;
  }
};

class InternationalOrderProcessor : public OrderProcessor {
protected:
  void validateOrder(Order *order) override {
    cout << "Validating international order: customs, restricted items, "
            "address format..."
         << endl;
  }

  void calculateTotal(Order *order) override {
    double shipping = 24.99;
    double customs = order->getSubtotal() * 0.15;
    double total = order->getSubtotal() + shipping + customs;
    cout << "International total: $" << total << " (shipping: $" << shipping
         << ", customs: $" << customs << ")" << endl;
  }

  void processPayment(Order *order) override {
    cout << "Processing payment with currency conversion..." << endl;
  }

  void sendConfirmation(Order *order) override {
    cout << "Sending multi-language confirmation with tracking for order "
         << order->getId() << endl;
  }
};

int main() {
  Order order1("ORD-001", 49.99);
  Order order2("ORD-002", 149.99);
  Order order3("ORD-003", 89.99);

  StandardOrderProcessor standard;
  standard.processOrder(&order1);

  cout << endl;

  PrimeOrderProcessor prime;
  prime.processOrder(&order2);

  cout << endl;

  InternationalOrderProcessor international;
  international.processOrder(&order3);

  return 0;
}
