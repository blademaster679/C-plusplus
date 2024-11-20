#include <iostream>
#include <cmath>
#include <string>
#include <iomanip>

class Shape {
public:
    virtual double calculateArea() = 0; // Pure virtual method for area
    virtual double calculatePerimeter() = 0; // Pure virtual method for perimeter
    virtual ~Shape() {} // Virtual destructor
};

class Circle : public Shape {
private:
    double radius;

public:
    Circle(double r) : radius(r) {}
    double calculateArea() override {
        return M_PI * radius * radius; // Area of circle: πr²
    }
    double calculatePerimeter() override {
        return 2 * M_PI * radius; // Perimeter of circle: 2πr
    }
};

class Rectangle : public Shape {
private:
    double length;
    double width;

public:
    Rectangle(double l, double w) : length(l), width(w) {}
    double calculateArea() override {
        return length * width; // Area of rectangle: length * width
    }
    double calculatePerimeter() override {
        return 2 * (length + width); // Perimeter of rectangle: 2*(length + width)
    }
};

class Triangle : public Shape {
private:
    double side1;
    double side2;
    double side3;

public:
    Triangle(double s1, double s2, double s3) : side1(s1), side2(s2), side3(s3) {}
    double calculateArea() override {
        double s = (side1 + side2 + side3) / 2; // Semi-perimeter
        return std::sqrt(s * (s - side1) * (s - side2) * (s - side3)); // Heron's formula
    }
    double calculatePerimeter() override {
        return side1 + side2 + side3; // Perimeter of triangle: side1 + side2 + side3
    }
};

void printShapeInfo(Shape* shape) {
    if (dynamic_cast<Circle*>(shape)) {
        std::cout << "Shape Type: Circle" << std::endl;
    } else if (dynamic_cast<Rectangle*>(shape)) {
        std::cout << "Shape Type: Rectangle" << std::endl;
    } else if (dynamic_cast<Triangle*>(shape)) {
        std::cout << "Shape Type: Triangle" << std::endl;
    }

    std::cout << "Area: " << std::fixed << std::setprecision(2) << shape->calculateArea() << std::endl;
    std::cout << "Perimeter: " << std::fixed << std::setprecision(2) << shape->calculatePerimeter() << std::endl;
}

int main() {
    std::string line;

    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        std::string shapeType;
        iss >> shapeType;

        Shape* shape = nullptr;

        if (shapeType == "circle") {
            double radius;
            iss >> radius;
            shape = new Circle(radius);
        } else if (shapeType == "rectangle") {
            double length, width;
            iss >> length >> width;
            shape = new Rectangle(length, width);
        } else if (shapeType == "triangle") {
            double side1, side2, side3;
            iss >> side1 >> side2 >> side3;
            shape = new Triangle(side1, side2, side3);
        }

        if (shape) {
            printShapeInfo(shape);
            delete shape;
        }
    }

    return 0;
}
