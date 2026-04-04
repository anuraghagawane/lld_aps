#include <cstdio>
#include <string>
using namespace std;

class Pizza {
public:
  virtual double getCost() = 0;
  virtual string getDescription() = 0;
  virtual ~Pizza() {}
};

class PlainPizza : public Pizza {
public:
  double getCost() override { return 5.00; }
  string getDescription() override { return "Plain pizza"; }
};

class PizzaDecorator : public Pizza {
protected:
  // TODO: Add a Pizza* field to store the wrapped pizza
  Pizza *inner;

public:
  PizzaDecorator(Pizza *pizza) {
    // TODO: Store the wrapped pizza pointer
    this->inner = pizza;
  }

  double getCost() override {
    // TODO: Delegate to the wrapped pizza's getCost()
    return inner->getCost();
  }

  string getDescription() override {
    // TODO: Delegate to the wrapped pizza's getDescription()
    return inner->getDescription();
  }
};

class CheeseDecorator : public PizzaDecorator {
public:
  CheeseDecorator(Pizza *pizza) : PizzaDecorator(pizza) {}

  double getCost() override {
    // TODO: Return the wrapped pizza's cost + 1.50
    return inner->getCost() + 1.50;
  }

  string getDescription() override {
    // TODO: Return the wrapped pizza's description + ", cheese"
    return inner->getDescription() + ", cheese";
  }
};

class OliveDecorator : public PizzaDecorator {
public:
  OliveDecorator(Pizza *pizza) : PizzaDecorator(pizza) {}

  double getCost() override {
    // TODO: Return the wrapped pizza's cost + 2.00
    return inner->getCost() + 2.00;
  }

  string getDescription() override {
    // TODO: Return the wrapped pizza's description + ", olives"
    return inner->getDescription() + ", olives";
  }
};

class MushroomDecorator : public PizzaDecorator {
public:
  MushroomDecorator(Pizza *pizza) : PizzaDecorator(pizza) {}

  double getCost() override {
    // TODO: Return the wrapped pizza's cost + 1.00
    return inner->getCost() + 1.00;
  }

  string getDescription() override {
    // TODO: Return the wrapped pizza's description + ", mushrooms"
    return inner->getDescription() + ", mushrooms";
  }
};

int main() {
  PlainPizza plain;
  printf("%s | $%.2f\n", plain.getDescription().c_str(), plain.getCost());

  PlainPizza p2;
  CheeseDecorator cheese(&p2);
  OliveDecorator cheeseOlive(&cheese);
  printf("%s | $%.2f\n", cheeseOlive.getDescription().c_str(),
         cheeseOlive.getCost());

  PlainPizza p3;
  CheeseDecorator c3(&p3);
  OliveDecorator o3(&c3);
  MushroomDecorator loaded(&o3);
  printf("%s | $%.2f\n", loaded.getDescription().c_str(), loaded.getCost());
  return 0;
}
