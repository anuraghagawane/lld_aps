#include <iostream>
#include <string>

using namespace std;

class PaymentStrategy {
public:
  virtual ~PaymentStrategy() = default;
  virtual bool pay(double amount) = 0;
};

class CreditCardPayment : public PaymentStrategy {
private:
  string cardNumber;
  string expiryDate;

public:
  CreditCardPayment(const string &card, const string &expiry)
      : cardNumber(card), expiryDate(expiry) {}

  bool pay(double amount) override {
    cout << "Charging $" << amount << " to credit card ending in "
         << cardNumber.substr(cardNumber.length() - 4) << endl;

    return true;
  }
};

class PayPalPayment : public PaymentStrategy {
private:
  string email;

public:
  PayPalPayment(const string &e) : email(e) {}

  bool pay(double amount) override {
    cout << "Sending $" << amount << " via PayPal to " << email << endl;
    return true;
  }
};

class CryptoPayment : public PaymentStrategy {
private:
  string walletAddress;

public:
  CryptoPayment(const string &wallet) : walletAddress(wallet) {}

  bool pay(double amount) override {
    cout << "Transferring $" << amount << " in crypto to " << walletAddress
         << endl;
    return true;
  }
};

class CheckoutService {
private:
  PaymentStrategy *paymentStrategy;

public:
  CheckoutService(PaymentStrategy *strategy) : paymentStrategy(strategy) {}

  void setPaymentStrategy(PaymentStrategy *strategy) {
    paymentStrategy = strategy;
  }

  bool checkout(double amount) { return paymentStrategy->pay(amount); }
};

int main() {
  CreditCardPayment card("4111111111111111", "12/26");
  CheckoutService checkout(&card);
  checkout.checkout(99.99);

  PayPalPayment paypal("user@example.com");
  checkout.setPaymentStrategy(&paypal);
  checkout.checkout(49.99);

  CryptoPayment crypto("0xABC123...");
  checkout.setPaymentStrategy(&crypto);
  checkout.checkout(149.99);

  return 0;
}
