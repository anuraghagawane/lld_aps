#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

class Cloneable {
public:
  virtual Cloneable *clone() const = 0;
  virtual ~Cloneable() = default;
};

class Circle : public Cloneable {
private:
  string color;
  double radius;

public:
  Circle(string color, double radius) : color(move(color)), radius(radius) {}

  // TODO: Implement clone()
  Circle *clone() const override { return new Circle(color, radius); }

  void setRadius(double r) { radius = r; }

  void printInfo() const {
    cout << std::fixed << std::setprecision(1);
    cout << "Circle [Color: " << color << ", Radius: " << radius << "]" << endl;
  }
};

class Rectangle : public Cloneable {
private:
  string color;
  double width, height;

public:
  Rectangle(string color, double w, double h)
      : color(move(color)), width(w), height(h) {}

  // TODO: Implement clone()
  Rectangle *clone() const override {
    return new Rectangle(color, width, height);
  }

  void setWidth(double w) { width = w; }

  void printInfo() const {
    cout << std::fixed << std::setprecision(1);
    cout << "Rectangle [Color: " << color << ", Width: " << width
         << ", Height: " << height << "]" << endl;
  }
};

int main() {
  // After implementing, usage should look like:
  Circle original("Red", 5.0);
  Circle *cloned = static_cast<Circle *>(original.clone());
  cloned->setRadius(10.0);
  original.printInfo(); // Circle [Color: Red, Radius: 5.0]
  cloned->printInfo();  // Circle [Color: Red, Radius: 10.0]
  delete cloned;

  Rectangle rect("Blue", 4.0, 6.0);
  Rectangle *clonedRect = static_cast<Rectangle *>(rect.clone());
  clonedRect->setWidth(8.0);
  rect.printInfo();        // Rectangle [Color: Blue, Width: 4.0, Height: 6.0]
  clonedRect->printInfo(); // Rectangle [Color: Blue, Width: 8.0, Height: 6.0]
  delete clonedRect;
  return 0;
}
