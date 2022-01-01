// Factories : Factory Methods & Abstract Factory

// A component responsible solely for the wholesale (not piecewise) creation of objects 

// object creation logic becomes too convoluted 
// Constructor is not descriptive
//  - name mandated by name of containing types
//  - cannot overload with same arguments with different names
//  - can turn into optional parameter hell

// Object creation (non-piecewise, unlike builder) can be outsourced to 
// can still have paradigm of a constuctor like call
//  - a seperate function : Factory method
//  - That may exist in a seperate class (Factory)
//  - can create hierachy of factories with Abstract factory

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

enum class PointType
{
    cartesian,
    polar
};

class Point
{
    /*Point(float a, float b, PointType type = PointType::cartesian)
    {
    if (type == PointType::cartesian)
    {
    x = a; b = y;
    }
    else
    {
    x = a*cos(b);
    y = a*sin(b);
    }
    }*/


    Point(const float x, const float y)
        : x{ x },
        y{ y }
    {
    }

public:
    float x, y;


    friend std::ostream& operator<<(std::ostream& os, const Point& obj)
    {
        return os
            << "x: " << obj.x
            << " y: " << obj.y;
    }

    static Point NewCartesian(float x, float y)
    {
        return{ x,y }; // return Point{x,y} in modern c++ can omit the name of return type
    }
    static Point NewPolar(float r, float theta)
    {
        return{ r * cos(theta), r * sin(theta) };
    }
};

int main_z()
{
    // will not work
    //Point p{ 1,2 };

    auto p = Point::NewPolar(5, M_PI_4);
    std::cout << p << std::endl;

    getchar();
    return 0;
}