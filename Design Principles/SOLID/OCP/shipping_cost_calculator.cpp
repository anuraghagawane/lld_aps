#include <iomanip>
#include <iostream>

using namespace std;

// class ShippingCostCalculator {
// public:
//     double calculate(const string& shippingType, double weight) {
//         if (shippingType == "Standard") {
//             return weight * 1.5;
//         } else if (shippingType == "Express") {
//             return weight * 3.0;
//         } else if (shippingType == "Overnight") {
//             return weight * 5.0;
//         } else if (shippingType == "International") {
//             return weight * 10.0;
//         }
//         throw invalid_argument("Unknown shipping type: " + shippingType);
//     }
// };

// int main() {
//     ShippingCostCalculator calculator;
//     cout << "Standard: $" << calculator.calculate("Standard", 2.0) << endl;
//     cout << "Express: $" << calculator.calculate("Express", 2.0) << endl;
//     return 0;
// }

// TODO: Define a ShippingStrategy interface with a calculateCost(weight)
// method.
// TODO: Create concrete implementations for each shipping type.
// TODO: Refactor ShippingCostCalculator to accept a ShippingStrategy.
class ShippingStrategy {
public:
  virtual ~ShippingStrategy() = default;
  virtual int calculateCost(double weight) = 0;
};

class StandardShipping : public ShippingStrategy {
public:
  int calculateCost(double weight) override { return weight * 1.5; }
};

class ExpressShipping : public ShippingStrategy {
public:
  int calculateCost(double weight) override { return weight * 3.0; }
};

class OvernightShipping : public ShippingStrategy {
public:
  int calculateCost(double weight) override { return weight * 5.0; }
};

class InternationalShipping : public ShippingStrategy {
public:
  int calculateCost(double weight) override { return weight * 10.0; }
};

class ShippingCostCalculator {
public:
  double calculate(ShippingStrategy &shipping, double weight) {
    return shipping.calculateCost(weight);
  }
};

int main() {
  ShippingCostCalculator calculator;
  StandardShipping ss;
  cout << fixed << setprecision(1);
  cout << "Standard: $" << calculator.calculate(ss, 2.0) << endl;
  ExpressShipping es;
  cout << "Express: $" << calculator.calculate(es, 2.0) << endl;
  OvernightShipping on;
  cout << "Overnight: $" << calculator.calculate(on, 2.0) << endl;
  InternationalShipping is;
  cout << "International: $" << calculator.calculate(is, 2.0) << endl;
  return 0;
}
