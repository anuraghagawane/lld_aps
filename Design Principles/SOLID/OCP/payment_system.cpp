#include <iostream>

using namespace std;

class PaymentMethod {
public:
  virtual void processPayment(double amount) = 0;
  virtual ~PaymentMethod() = default;
};

class CreditCardPayment : public PaymentMethod {
public:
  void processPayment(double amount) override {
    cout << "Processing credit card payment of $" << amount << endl;
  }
};

class PaypalPayment : public PaymentMethod {
public:
  void processPayment(double amount) override {
    cout << "Processing Paypal payment of $" << amount << endl;
  }
};

class UPIPayment : public PaymentMethod {
public:
  void processPayment(double amount) override {
    cout << "Processing UPI payment of $" << amount << endl;
  }
};

class PaymentProcessor {
public:
  void process(PaymentMethod *paymentMethod, double amount) {
    paymentMethod->processPayment(amount);
  }
};

class CheckoutService {
public:
  void processPayment(PaymentMethod *method, double amount) {
    PaymentProcessor processor;
    processor.process(method, amount);
  }
};

int main() {
  CheckoutService checkout;
  CreditCardPayment credit;
  PaypalPayment paypal;
  UPIPayment upi;

  checkout.processPayment(&credit, 100.0);
  checkout.processPayment(&paypal, 100.0);
  checkout.processPayment(&upi, 100.0);
  return 0;
}
