#include <cstdio>

using namespace std;

class ShapeVisitor;

class Shape {
public:
  virtual void accept(ShapeVisitor *visitor) = 0;
  virtual ~Shape() {}
};

class Circle : public Shape {
private:
  double radius;

public:
  Circle(double r) : radius(r) {}

  double getRadius() const { return radius; }

  void accept(ShapeVisitor *visitor) override;
};

class Rectangle : public Shape {
private:
  double width;
  double height;

public:
  Rectangle(double w, double h) : width(w), height(h) {}

  double getWidth() const { return width; }

  double getHeight() const { return height; }

  void accept(ShapeVisitor *visitor) override;
};

class ShapeVisitor {
public:
  virtual void visitCircle(Circle *circle) = 0;
  virtual void visitRectangle(Rectangle *rectangle) = 0;
  virtual ~ShapeVisitor() {}
};

class AreaCalculatorVisitor : public ShapeVisitor {
public:
  void visitCircle(Circle *circle) override {
    double area = 3.141592653589793 * circle->getRadius() * circle->getRadius();
    printf("Area of Circle: %.2f\n", area);
  }

  void visitRectangle(Rectangle *rectangle) override {
    double area = rectangle->getWidth() * rectangle->getHeight();
    printf("Area of Rectangle: %.2f\n", area);
  }
};

class SvgExporterVisitor : public ShapeVisitor {
public:
  void visitCircle(Circle *circle) override {
    printf("<circle r=\"%.2f\" />\n", circle->getRadius());
  }

  void visitRectangle(Rectangle *rectangle) override {
    printf("<rect width=\"%.2f\" height=\"%.2f\" />\n", rectangle->getWidth(),
           rectangle->getHeight());
  }
};

void Circle::accept(ShapeVisitor *visitor) { visitor->visitCircle(this); }

void Rectangle::accept(ShapeVisitor *visitor) { visitor->visitRectangle(this); }

int main() {
  Shape *shapes[] = {new Circle(5), new Rectangle(10, 4), new Circle(2.5)};

  printf("=== Calculating Areas ===\n");
  AreaCalculatorVisitor areaVisitor;
  for (int i = 0; i < 3; i++) {
    shapes[i]->accept(&areaVisitor);
  }

  printf("\n=== Exporting to SVG ===\n");
  SvgExporterVisitor svgVisitor;
  for (int i = 0; i < 3; i++) {
    shapes[i]->accept(&svgVisitor);
  }

  for (int i = 0; i < 3; i++) {
    delete shapes[i];
  }

  return 0;
}
