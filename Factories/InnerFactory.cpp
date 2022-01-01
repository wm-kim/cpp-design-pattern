#include <cmath>

// do not need this for factory
enum class PointType
{
    cartesian,
    polar
};

class Point
{
    // everything is private 

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

    // use a factory method
    Point(float x, float y) : x(x), y(y) {}

    class PointFactory // make it private and make it singleton
    {
        PointFactory() {}
    public:
        static Point NewCartesian(float x, float y)
        {
            return { x,y };
        }
        static Point NewPolar(float r, float theta)
        {
            return{ r * cos(theta), r * sin(theta) };
        }
    };


public:
    float x, y;
    static PointFactory Factory; // singleton
    // or use getFactory()
};

int main_2()
{
    // will not work
    // Point p{ 1,2 };

    // nope!
    // Point::PointFactory pf; 

    // if factory is public, then
    //auto p = Point::PointFactory::NewCartesian(3, 4);

    // at any rate, use this
    auto pp = Point::Factory.NewCartesian(2, 3);

    return 0;
}

// upside of inner class : access to private members, can have control of life time
// downside of inner class : can't access private members, can't have control of life time
