#include <iostream>
#include <string>
#include <cstdio>
#include <algorithm>
#include <iomanip>
using namespace std;

class Discount {
protected:
    string label;

public:
    Discount(const string& label) : label(label) {}
    virtual ~Discount() {}

    virtual double apply(double price) = 0;

    void describe(double originalPrice) {
        // TODO: call apply(originalPrice) and print:
        //   "label: $originalPrice -> $discountedPrice"
        // Hint: use printf("%.2f", value) for formatting
        cout<<label<<": $"<<originalPrice<<" -> $"<<fixed<<setprecision(2)<<apply(originalPrice)<<endl;
    }
};

class PercentageDiscount : public Discount {
    double percentage;

public:
    PercentageDiscount(double percentage)
        : Discount(string()) {
        char buf[32];
        snprintf(buf, sizeof(buf), "%.1f", percentage);
        this->label = string(buf) + "% off";
        // TODO: initialize this->percentage
        this -> percentage = percentage;
    }

    double apply(double price) override {
        // TODO: return price * (1 - percentage / 100)
        return price * (1 - percentage / 100);
    }
};

class FlatDiscount : public Discount {
    double amount;

public:
    FlatDiscount(double amount)
        : Discount(string()) {
        char buf[32];
        snprintf(buf, sizeof(buf), "%.1f", amount);
        this->label = "$" + string(buf) + " off";
        // TODO: initialize this->amount
        this -> amount = amount;
    }

    double apply(double price) override {
        // TODO: return max(price - amount, 0.0)
        return max(price - amount, 0.0);
    }
};

class BuyOneGetOneFree : public Discount {
public:
    BuyOneGetOneFree() : Discount("Buy 1 Get 1 Free") {}

    double apply(double price) override {
        // TODO: return price / 2
        return price / 2;
    }
};

class OrderProcessor {
public:
    void processOrder(const string& itemName, double price, Discount& discount) {
        // TODO: print "Item: itemName" then call discount.describe(price)
        cout<<"Item: "<<itemName<<endl;
        discount.describe(price);
    }
};

int main() {
    OrderProcessor processor;

    PercentageDiscount pct(20);
    FlatDiscount flat(15);
    BuyOneGetOneFree bogo;

    processor.processOrder("Laptop", 999.99, pct);
    processor.processOrder("Headphones", 49.99, flat);
    processor.processOrder("Keyboard", 79.98, bogo);
    return 0;
}
