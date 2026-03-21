#include <cmath>
#include <cstdio>
#include <memory>

using namespace std;

class Shape {
public:
  virtual double area() = 0;
  virtual void describe() = 0;
  virtual ~Shape() = default;
};

class Circle : public Shape {
  // TODO: Add a private field for radius (double)
  double radius;

public:
  Circle(double radius) {
    // TODO: Store the radius
    this->radius = radius;
  }

  double area() override {
    // TODO: Return pi * radius * radius (use M_PI)
    return M_PI * radius * radius;
  }

  void describe() override {
    // TODO: Print "Circle with area: " followed by area() formatted to 2
    // decimal places Hint: Use printf("Circle with area: %.2f\n", area())
    printf("Circle with area: %.2f\n", area());
  }
};

class Rectangle : public Shape {
  // TODO: Add private fields for width and height (double)
  double width, height;

public:
  Rectangle(double width, double height) {
    // TODO: Store width and height
    this->width = width;
    this->height = height;
  }

  double area() override {
    // TODO: Return width * height
    return width * height;
  }

  void describe() override {
    // TODO: Print "Rectangle with area: " followed by area() formatted to 2
    // decimal places
    printf("Rectangle with area: %.2f\n", area());
  }
};

class Triangle : public Shape {
  // TODO: Add private fields for base and height (double)
  double base, height;

public:
  Triangle(double base, double height) {
    // TODO: Store base and height
    this->base = base;
    this->height = height;
  }

  double area() override {
    // TODO: Return 0.5 * base * height
    return 0.5 * base * height;
  }

  void describe() override {
    // TODO: Print "Triangle with area: " followed by area() formatted to 2
    // decimal places
    printf("Triangle with area: %.2f\n", area());
  }
};

class ShapeCreator {
public:
  // Factory method - subclasses decide which Shape to create
  virtual unique_ptr<Shape> createShape() = 0;
  virtual ~ShapeCreator() = default;

  void describe() {
    // TODO: Call createShape() to get a Shape instance
    // TODO: Call describe() on the shape
    auto shape = createShape();
    shape->describe();
  }
};

class CircleCreator : public ShapeCreator {
public:
  unique_ptr<Shape> createShape() override {
    // TODO: Return a new Circle with radius 5
    // Hint: Use make_unique<Circle>(5)
    return make_unique<Circle>(5);
  }
};

class RectangleCreator : public ShapeCreator {
public:
  unique_ptr<Shape> createShape() override {
    // TODO: Return a new Rectangle with width 4 and height 6
    return make_unique<Rectangle>(4, 6);
  }
};

class TriangleCreator : public ShapeCreator {
public:
  unique_ptr<Shape> createShape() override {
    // TODO: Return a new Triangle with base 3 and height 8
    return make_unique<Triangle>(3, 8);
  }
};

int main() {
  unique_ptr<ShapeCreator> creator = make_unique<CircleCreator>();
  creator->describe();

  creator = make_unique<RectangleCreator>();
  creator->describe();

  creator = make_unique<TriangleCreator>();
  creator->describe();

  return 0;
}
