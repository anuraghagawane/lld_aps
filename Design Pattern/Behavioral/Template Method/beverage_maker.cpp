#include <iostream>
using namespace std;

class BeverageMaker {
public:
  virtual ~BeverageMaker() = default;

  void prepareBeverage() {
    boilWater();
    brew();
    pourInCup();
    addCondiments();
  }

protected:
  virtual void brew() = 0;
  virtual void addCondiments() = 0;

private:
  void boilWater() { cout << "Boiling water..." << endl; }
  void pourInCup() { cout << "Pouring into cup..." << endl; }
};

class TeaMaker : public BeverageMaker {
protected:
  void brew() override {
    // TODO: Print "Steeping the tea bag..."
    cout << "Steeping the tea bag..." << endl;
  }
  void addCondiments() override {
    // TODO: Print "Adding lemon..."
    cout << "Adding lemon..." << endl;
  }
};

class CoffeeMaker : public BeverageMaker {
protected:
  void brew() override {
    // TODO: Print "Dripping coffee through filter..."
    cout << "Dripping coffee through filter..." << endl;
  }
  void addCondiments() override {
    // TODO: Print "Adding sugar and milk..."
    cout << "Adding sugar and milk..." << endl;
  }
};

int main() {
  TeaMaker tea;
  cout << "--- Making Tea ---" << endl;
  tea.prepareBeverage();

  cout << endl;

  CoffeeMaker coffee;
  cout << "--- Making Coffee ---" << endl;
  coffee.prepareBeverage();

  return 0;
}
