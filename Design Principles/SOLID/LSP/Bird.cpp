#include <iostream>

// Before: Penguin extends Bird but can't fly
// class Bird {
// public:
//   virtual void eat() { std::cout << "Bird is eating" << std::endl; }
//
//   virtual void fly() { std::cout << "Bird is flying" << std::endl; }
//
//   virtual ~Bird() = default;
// };
//
// class Sparrow : public Bird {
// public:
//   void eat() override { std::cout << "Sparrow is eating" << std::endl; }
//   void fly() override { std::cout << "Sparrow is flying" << std::endl; }
// };
//
// class Penguin : public Bird {
// public:
//   void eat() override { std::cout << "Penguin is eating" << std::endl; }
//   void fly() override { throw std::runtime_error("Penguins can't fly!"); }
// };
//
// void makeBirdFly(Bird &bird) {
//   bird.fly(); // Crashes for Penguin!
// }
//
// int main() {
//   Sparrow sparrow;
//   Penguin penguin;
//   makeBirdFly(sparrow); // Works fine
//   makeBirdFly(penguin); // runtime_error!
//   return 0;
// }

// TODO: Split Bird into a Bird interface (eat) and a FlyingBird interface
// (fly).
// TODO: Sparrow implements FlyingBird, Penguin implements only Bird.

class Bird {
public:
  virtual void eat() = 0;
  virtual ~Bird() = default;
};

class FlyingBird : public Bird {
public:
  virtual void fly() = 0;
};

class Sparrow : public FlyingBird {
public:
  void eat() override { std::cout << "Sparrow is eating" << std::endl; }
  void fly() override { std::cout << "Sparrow is flying" << std::endl; }
};

class Penguin : public Bird {
public:
  void eat() override { std::cout << "Penguin is eating" << std::endl; }
};

void makeBirdFly(FlyingBird &bird) {
  bird.fly(); // Crashes for Penguin!
};

int main() {
  Sparrow sp;
  Penguin penguin;
  sp.eat();
  sp.fly();
  penguin.eat();
  // penguin.fly();
  return 0;
}
