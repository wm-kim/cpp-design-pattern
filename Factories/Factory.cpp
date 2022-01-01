#include <cmath>

enum class PointType
{
    cartesian,
    polar
};

class Point
{
     // insufficent for describing the situation
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
public:
    float x, y;

    friend class PointFactory; // OCP, friend class mechanism is very hostile to OCP
    // making everything public will not break OCP (modifying the codes)
};

// concrete factory
class PointFactory
{
    static Point NewCartesian(float x, float y)
    {
        return Point{ x,y };
    }

    static Point NewPolar(float r, float theta)
    {
        return Point{ r * cos(theta), r * sin(theta) };
    }
};
