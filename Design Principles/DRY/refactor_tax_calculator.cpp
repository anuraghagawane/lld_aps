#include <iostream>
#include <memory>
#include <iomanip>

using namespace std;

// Before: Each processor duplicates tax calculation
// class USOrderProcessor {
// public:
//     void processOrder(double amount) {
//         double tax = amount * 0.10; // Duplicated tax logic
//         double total = amount + tax;
//         cout << "US Order - Subtotal: $" << amount
//              << ", Tax: $" << tax << ", Total: $" << total << endl;
//     }
// };

// class EUOrderProcessor {
// public:
//     void processOrder(double amount) {
//         double tax = amount * 0.20; // Duplicated tax logic
//         double total = amount + tax;
//         cout << "EU Order - Subtotal: $" << amount
//              << ", Tax: $" << tax << ", Total: $" << total << endl;
//     }
// };

// class UKOrderProcessor {
// public:
//     void processOrder(double amount) {
//         double tax = amount * 0.15; // Duplicated tax logic
//         double total = amount + tax;
//         cout << "UK Order - Subtotal: $" << amount
//              << ", Tax: $" << tax << ", Total: $" << total << endl;
//     }
// };

// TODO: Extract a TaxCalculator interface and region implementations.
// TODO: Refactor OrderProcessor to accept a TaxCalculator.

class TaxCalculator {
public:
    virtual ~TaxCalculator() = default;
    virtual double calculateTax(double amount) const = 0;
    virtual string getRegion() const = 0;
};

class USTaxCalculator : public TaxCalculator {
public:
    double calculateTax(double amount) const override {
        return amount * 0.10;
    }
    
    string getRegion() const override {
        return "US";
    }
};

class EUTaxCalculator : public TaxCalculator {
public:
    double calculateTax(double amount) const override {
        return amount * 0.20;
    }
    
    string getRegion() const override {
        return "EU";
    }
};

class UKTaxCalculator : public TaxCalculator {
public:
    double calculateTax(double amount) const override {
        return amount * 0.15;
    }
    
    string getRegion() const override {
        return "UK";
    }
};

class OrderProcessor {
    unique_ptr<TaxCalculator> taxCalculator;
public:
    OrderProcessor(unique_ptr<TaxCalculator> calculator)
        : taxCalculator(move(calculator)) {}

    void processOrder(double amount)  {
        double tax = taxCalculator->calculateTax(amount);
        double total = amount + tax;
        cout << taxCalculator->getRegion() << " Order - Subtotal: $"
             << fixed << setprecision(2) << amount << ", Tax: $" << tax
             << ", Total: $" << total << endl;
    }
};

int main() {
    // After refactoring, usage should look like:
    OrderProcessor usProcessor(make_unique<USTaxCalculator>());
    usProcessor.processOrder(100.0);
    OrderProcessor euProcessor(make_unique<EUTaxCalculator>());
    euProcessor.processOrder(100.0);
    OrderProcessor ukProcessor(make_unique<UKTaxCalculator>());
    ukProcessor.processOrder(100.0);
    return 0;
}
