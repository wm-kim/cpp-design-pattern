#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <functional>
using namespace std;
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

// dynamic decorator

struct Shape
{
    virtual string str() const = 0;
};

struct Circle : Shape
{
    float radius;

    Circle() {}

    explicit Circle(const float radius)
        : radius{ radius }
    {
    }

    void resize(float factor)
    {
        radius *= factor;
    }

    string str() const override
    {
        ostringstream oss;
        oss << "A circle of radius " << radius;
        return oss.str();
    }
};

struct Square : Shape
{
    float side;

    Square() {}

    explicit Square(const float side)
        : side{ side }
    {
    }

    string str() const override
    {
        ostringstream oss;
        oss << "A square of with side " << side;
        return oss.str();
    }
};

// we are not changing the base class of existing objects

// cannot make, e.g., ColoredSquare, ColoredCircle, etc.

struct ColoredShape : Shape
{
    Shape& shape;
    string color;

    ColoredShape(Shape& shape, const string& color)
        : shape{ shape },
        color{ color }
    {
    }
    // dynamic decorator. can create decorator at runtime. simply instantiate colorshape providing the argument
    // shape is getting decorated. decrotor inhereted from shape
    string str() const override
    {
        ostringstream oss;
        oss << shape.str() << " has the color " << color;
        return oss.str();
    }
};

struct TransparentShape : Shape
{
    Shape& shape;
    uint8_t transparency;


    TransparentShape(Shape& shape, const uint8_t transparency)
        : shape{ shape },
        transparency{ transparency }
    {
    }

    string str() const override
    {
        ostringstream oss;
        oss << shape.str() << " has "
            << static_cast<float>(transparency) / 255.f * 100.f
            << "% transparency";
        return oss.str();
    }
};

int main()
{
    vector<Shape*> shapes;
    shapes.push_back(new Circle{ 5 });
    shapes.push_back(new Square{ 10 });
    shapes.push_back(new ColoredShape{ *shapes[0], "red" });
    shapes.push_back(new TransparentShape{ *shapes[1], 128 });

    for (auto shape : shapes)
    {
        cout << shape->str() << endl;
    }

    for (auto shape : shapes)
    {
        delete shape;
    }
    ColorShape{ColorShape{ColorShape{Square}}}; // this is also allowed

    // problem : if Shape is circle. ColorShape cannot call resize method
    // dynamic decorator allows to build decorations at runtime but downside is you don't have access to the underlying API
    // restricted by the intreface of shape interface

    return 0;
}
//===========================================================================================================================

// static implementation. ues c++ trick to bake in the information about a particular decorator, inside the type.

// mixin inheritance. && perfect fowarding.
// you can inherit from a template argument, which is something that's not available in other programming languages

// note: class, not typename 
// ColorShape2<Circle> redCircle; // tpye is statically defined ( + advantage : can call underlying methods)
// contraint type T into Shape - use concept : c++20

template <typename T>
concept IsAShape = std::is_base_of<Shape, T>::value; 
// which is just going to check that the type T inherits from shape
// if not program will not compile

template <typename T> struct ColoredShape2 : T // template <IsAShape T> struct ColoredShape2 : T 
{
    // constraint T to be Shape Type
    static_assert(is_base_of<Shape, T>::value,
        "Template argument must be a Shape");

    string color;

    // need this (or not!)
    ColoredShape2() {}

    // constructor fowarding
    template <typename...Args>
    ColoredShape2(const string& color, Args ...args)
        : T(std::forward<Args>(args)...), color{ color }
        // you cannot call base class constructor here because you have no idea what it is
        // fowarding all the arugments and hopes that the base class constructor can handle them
    {
    }

    string str() const override
    {
        ostringstream oss;
        oss << T::str() << " has the color " << color;
        return oss.str();
    }
};

template <typename T> struct TransparentShape2 : T
{
    uint8_t transparency;

    template<typename...Args>
    TransparentShape2(const uint8_t transparency, Args ...args)
        : T(std::forward<Args>(args)...), transparency{ transparency }
    {
    }

    string str() const override
    {
        ostringstream oss;
        oss << T::str() << " has "
            << static_cast<float>(transparency) / 255.f * 100.f
            << "% transparency";
        return oss.str();
    }
};

void main2()
{
    ColoredShape2<Circle> green_circle{ "green", 5 };
    cout << green_circle.str() << endl;

    // won't work without a default constructor
    ColoredShape2<Circle> green_circle2{ "green" };
    green_circle2.radius = 123;
    cout << green_circle2.str() << endl;

    // // making lots of these new types, size of the executable increases
    TransparentShape2<ColoredShape2<Square>> blue_invisible_square{ 0 }; // {0, "blue", 321}
    blue_invisible_square.color = "blue";
    blue_invisible_square.side = 321;
    cout << blue_invisible_square.str() << endl;
}

//===========================================================================================================================

// can also decorate function rather than class
struct Logger
{
    std::function<void()> func; // void returning doesn't take any arguments
    string name;

    Logger(const function<void()>& func, const string& name)
        : func{ func },
        name{ name }
    {
    }

    void operator()() const
    {
        cout << "Entering " << name << endl;
        func();
        cout << "Exiting " << name << endl;
    }
};

// instead of keeping std function can also have function as a template argument 
// in C++, you can't infer the template arguments in a construtor
template <typename Func>
struct Logger2
{
    Func func;
    string name;

    // doenst use std::function directly but still feed function directly and execute (and its fine)
    // but not going to infer the argument 'Func' from invocation
    Logger2(const Func& func, const string& name)
        : func{ func },
        name{ name }
    {
    }

    void operator()() const
    {
        cout << "Entering " << name << endl;
        func();
        cout << "Exiting " << name << endl;
    }
};

// helper function, global factory method. this approach let you infer type of 'Func' from the argument
template <typename Func> auto make_logger2(Func func,
    const string& name)
{
    return Logger2<Func>{ func, name }; 
}

int main3()
{
    // constuctor call
    Logger([]() { cout << "Hello" << endl; }, "HelloFunction")();

    // discarding the return value
    auto log = make_logger2([]() { cout << "Hello" << endl; }, "HelloFunction");
    log();
}

//===========================================================================================================================

// what if you have a function which takes arguent and does return values?
double add(double a, double b)
{
    cout << a << "+" << b << "=" << (a + b) << endl;
    return a + b;
}

// need partial specialization for this to work
template <typename> struct Logger3;

// return type and argument list
template <typename R, typename... Args>
struct Logger3<R(Args...)> // return value R, and other Args...
{
    Logger3(function<R(Args...)> func, const string& name)
        : func{ func },
        name{ name }
    {
    }

    // function call operator
    R operator() (Args ...args)
    {
        cout << "Entering " << name << endl;
        R result = func(args...);
        cout << "Exiting " << name << endl;
        return result;
    }

    function<R(Args ...)> func;
    string name;
};

// helper global function for figuring out the arguments of this particular template class. don't need to specify arguments
template <typename R, typename... Args>
auto make_logger3(R(*func)(Args...), const string& name) // used function pointer instead of std::function
{
    return Logger3<R(Args...)>(
        std::function<R(Args...)>(func), // create std::function from function pointer 
        name);
}

void main4()
{
    auto logged_add = make_logger3(add, "Add");
    auto result = logged_add(2, 3);
}

