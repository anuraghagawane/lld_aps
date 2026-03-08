#include <iostream>
#include <string>
using namespace std;

class Address {
    string city;
public:
    Address(string city) : city(city) {}

    string getCity() const { return city; }
};

class Customer {
    Address address;
public:
    Customer(Address address) : address(address) {}

    Address getAddress() const { return address; }
};

class CreditCard {
    string last4Digits;
public:
    CreditCard(string last4Digits) : last4Digits(last4Digits) {}

    string getLast4Digits() const { return last4Digits; }
};

class Payment {
    CreditCard creditCard;
public:
    Payment(CreditCard creditCard) : creditCard(creditCard) {}

    CreditCard getCreditCard() const { return creditCard; }
};

class Order {
    Customer customer;
    Payment payment;
public:
    Order(Customer customer, Payment payment)
        : customer(customer), payment(payment) {}

    // TODO: Add delegation methods

    Customer getCustomer() const { return customer; }
    Payment getPayment() const { return payment; }

    string getCustomerCity() const {
        return customer.getAddress().getCity();
    }

    string getPaymentLast4Digits() const {
        return payment.getCreditCard().getLast4Digits();
    }
};

class OrderSummaryPrinter {
public:
    void print(const Order& order) {
        // CURRENT (violates LoD):
        // string city = order.getCustomer().getAddress().getCity();
        // string last4 = order.getPayment().getCreditCard().getLast4Digits();

        // TODO: Refactor to use delegation methods on Order
        string city = order.getCustomerCity(); // replace with order.getCustomerCity()
        string last4 = order.getPaymentLast4Digits(); // replace with order.getPaymentLast4Digits()

        cout << "Ship to: " << city << endl;
        cout << "Paid with card ending in: " << last4 << endl;
    }
};

int main() {
    Address address("San Francisco");
    Customer customer(address);
    CreditCard card("4242");
    Payment payment(card);
    Order order(customer, payment);

    OrderSummaryPrinter printer;
    printer.print(order);
    return 0;
}
