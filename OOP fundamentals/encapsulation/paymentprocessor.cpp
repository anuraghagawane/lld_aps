#include <iostream>
#include <string>

using namespace std;

class PaymentProcessor {
private:
  string cardNumber;
  double amount;

  string maskCardNumber(const string& cardNumber) {
    return "****_****_****_" + cardNumber.substr(cardNumber.length() - 4);
  }

public:
  PaymentProcessor(const string& cardNumber, double amount) :
    cardNumber(maskCardNumber(cardNumber)), amount(amount) {}

  void processPayment() {
    cout<<"Processing payment of $"<<amount<<" for card "<<cardNumber<<endl;
  }
};

int main() {
  PaymentProcessor payment("1234567812345678", 250.00);
  payment.processPayment();

  return 0;
}
