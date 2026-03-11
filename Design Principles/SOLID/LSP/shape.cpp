#include <iostream>

using namespace std;

// Before: Square extends Rectangle but breaks its contract
// class Rectangle {
// protected:
//   int width = 0;
//   int height = 0;
//
// public:
//   virtual void setWidth(int w) { width = w; }
//   virtual void setHeight(int h) { height = h; }
//   int getArea() const { return width * height; }
//   virtual ~Rectangle() = default;
// };

// class Square : public Rectangle {
// public:
//   void setWidth(int w) override {
//     width = w;
//     height = w;
//   }
//   void setHeight(int h) override {
//     width = h;
//     height = h;
//   }
// };

// Client code that breaks with Square

// int main() {
//   Rectangle rect;
//   Square sq;
//   resize(rect); // Area: 50
//   resize(sq);   // Area: 100 -- LSP violation!
//   return 0;
// }

// TODO: Refactor using a Shape interface with getArea().
// TODO: Rectangle and Square should be independent implementations of Shape.

class Shape {
public:
  virtual ~Shape() = default;
  virtual int getArea() const = 0;
};

class Square : public Shape {
  int side;

public:
  Square(int side) : side(side) {}
  void setSide(int length) { side = length; }
  int getArea() const override { return side * side; }
};

class Rectangle : public Shape {
  int width = 0;
  int height = 0;

public:
  Rectangle(int width, int height) : width(width), height(height) {}
  void setWidth(int w) { width = w; }

  void setHeight(int h) { height = h; }

  int getArea() const override { return width * height; }
};

void resize(Rectangle &rect) {
  rect.setWidth(5);
  rect.setHeight(10);
  cout << "Area: " << rect.getArea() << endl;
}
int main() {
  Square sq(5);
  Rectangle rect(5, 10);

  cout << "Rectangle Area: " << rect.getArea() << endl;
  cout << "Square Area: " << sq.getArea() << endl;
  return 0;
}
