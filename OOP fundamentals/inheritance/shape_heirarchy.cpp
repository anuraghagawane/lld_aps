#include <iostream>
#include <string>
#include <cmath>
#include <cstdio>
#include <iomanip>

using namespace std;

class Shape {
protected:
    string name;

public:
    Shape(const string& name) : name(name) {}
    virtual ~Shape() {}

    virtual double area() {
        // TODO: return 0 by default
        return 0;
    }

    virtual double perimeter() {
        // TODO: return 0 by default
        return 0;
    }

    void describe() {
        // TODO: print "Shape: name, Area: area, Perimeter: perimeter"
        // Hint: use printf("%.2f", value) for formatting
        cout<<"Shape: "<<name<<", Area: "<<fixed<<setprecision(2)<<area()<<", Perimeter: "<<fixed<<setprecision(2)<<perimeter()<<endl;
    }
};

class Circle : public Shape {
private:
    double radius;

public:
    Circle(double radius) : Shape("Circle") {
        // TODO: initialize this->radius
        this -> radius = radius;
    }

    double area() override {
        // TODO: return M_PI * radius * radius
        return M_PI * radius * radius;
    }

    double perimeter() override {
        // TODO: return 2 * M_PI * radius
        return 2 * M_PI * radius;
    }
};

class Rectangle : public Shape {
private:
    double width;
    double height;

public:
    Rectangle(double width, double height) : Shape("Rectangle") {
        // TODO: initialize this->width and this->height
        this -> width = width;
        this -> height = height;
    }

    double area() override {
        // TODO: return width * height
        return height * width;
    }

    double perimeter() override {
        // TODO: return 2 * (width + height)
        return 2 * (width + height);
    }
};

int main() {
    Circle circle(5.0);
    circle.describe();

    Rectangle rect(4.0, 6.0);
    rect.describe();

    return 0;
}
