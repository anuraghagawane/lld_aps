#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>

using namespace std;

class Shape {
protected:
    string name;

public:
    Shape(const string& name) : name(name) {}
    virtual ~Shape() {}

    virtual double area() = 0;
    virtual double perimeter() = 0;

    void describe() {
        // Print: "Shape: [name], Area: [area], Perimeter: [perimeter]"
        // Use fixed << setprecision(2) for formatting

        cout<<"Shape: "<<name<<", Area: "<<fixed<<setprecision(2)<<area()<<", Perimeter: "<<fixed<<setprecision(2)<<perimeter()<<endl;
    }
};

class Circle : public Shape {
private:
    double radius;

public:
    Circle(double radius) : Shape("Circle"), radius(radius) {}

    double area() override {
        // Area = pi * r^2
        return M_PI * radius * radius;
    }

    double perimeter() override {
        // Perimeter = 2 * pi * r
        return 2 * M_PI * radius;
    }
};

class Rectangle : public Shape {
private:
    double width;
    double height;

public:
    Rectangle(double width, double height)
        : Shape("Rectangle"), width(width), height(height) {}

    double area() override {
        // Area = width * height
        return width * height;
    }

    double perimeter() override {
        // Perimeter = 2 * (width + height)
        return 2 * (width + height);
    }
};

int main() {
    Circle circle(5.0);
    circle.describe();

    Rectangle rectangle(4.0, 6.0);
    rectangle.describe();

    return 0;
}
