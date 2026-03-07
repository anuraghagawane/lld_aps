#include <iostream>
#include <string>

class Operation {
public:
    virtual double calculate(double a, double b) const = 0;
    virtual ~Operation() = default;
};

class Addition : public Operation {
public:
    double calculate(double a, double b) const override {
        return a + b;
    }
};

class Subtraction : public Operation {
public:
    double calculate(double a, double b) const override {
        return a - b;
    }
};

class Multiplication : public Operation {
public:
    double calculate(double a, double b) const override {
        return a * b;
    }
};

class Division : public Operation {
public:
    double calculate(double a, double b) const override {
        if (b == 0) throw std::invalid_argument("Division by zero");
        return a / b;
    }
};

// here things are not simple as we are inheriting from other class to just perform a arithmetic operation
class Calculator {
public:
    double execute(const Operation& op, double a, double b) const {
        return op.calculate(a, b);
    }
};

// below class keeps things simple
class Calculator2 {
public:
    double calculate(const std::string& op, double a, double b) const {
        if (op == "+") return a + b;
        else if (op == "-") return a - b;
        else if (op == "*") return a * b;
        else if (op == "/") {
            if (b == 0) throw std::invalid_argument("Division by zero");
            return a / b;
        } else {
            throw std::invalid_argument("Unknown operator: " + op);
        }
    }
};

int main() {
  Calculator calc1;
  Addition add;
  Division div;

  std::cout << "5 + 3 = " << calc1.execute(add, 5, 3) << "\n";
  std::cout << "10 / 2 = " << calc1.execute(div, 10, 2) << "\n";

  Calculator2 calc2;
  std::cout << "7 * 4 = " << calc2.calculate("*", 7, 4) << "\n";
  std::cout << "9 - 5 = " << calc2.calculate("-", 9, 5) << "\n";

  return 0;
}
