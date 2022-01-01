#include <string>
#include <iostream>
#include <vector>
#include <sstream>
using namespace std;
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

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

// we are not changing the base class of existing
// objects

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

// ColorShape { ColorShape { ColorShape { Square } } }; this is also alowed 
// 문제점 : shape가 원이라면 ColorShape는 resize method를 호출할 수 없다.  downside is you don't have access to the underlying API 


// static implementation. bake in the information about a particular decorator right inside the type.

// mixin inheritance. 
// you can inherit from a template argument, which is something that's not available in other programming languagesand is available in C++.


// note: class, not typename 
// ColorShape2<Circle> redCircle; // tpye is statically defined ( + advantage : can call underlying methods)

// concept : c++20
template <typename T>
concept IsAShape = std::is_base_of<Shape, T>::value;
// which is just going to check that the type T inherits from shape

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
        // you cannot call base class ctor here
        // b/c you have no idea what it is
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

void wrapper()
{
    Circle circle{ 5 };
    cout << circle.str() << endl;

    ColoredShape red_circle{ circle, "red" };
    cout << red_circle.str() << endl;

    //red_circle.resize(); // oops

    TransparentShape red_half_visible_circle{ red_circle, 128 };
    cout << red_half_visible_circle.str() << endl;
}

void mixin_inheritance()
{
    // won't work without a default constructor
    ColoredShape2<Circle> green_circle{ "green" };
    green_circle.radius = 123;
    cout << green_circle.str() << endl;

    TransparentShape2<ColoredShape2<Square>> blue_invisible_square{ 0 };
    blue_invisible_square.color = "blue";
    blue_invisible_square.side = 321;
    cout << blue_invisible_square.str() << endl;
}

// making lotsand lots of these new types, which once again has certain implications in terms of the size of the executable
////////////////////////////////////////////////////////////////////////////////


// can also decorate fucntion rather than class

struct Logger
{
    function<void()> func;
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

// in C++, you can't infer the template arguments in a constructive.
template <typename Func>
struct Logger2
{
    Func func;
    string name;

    // doenst use std::function directly but still feed function directly and execute (and its fine)
    // but not going to infer the argument from invocation
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

// helper function
template <typename Func> auto make_logger2(Func func,
    const string& name)
{
    return Logger2<Func>{ func, name }; // feels redundant (global factory method), this approach let you infer type of func from the argument
}



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

// helper global function for figuring out the arguments of this particular template class.
// don't need to specify arguments
template <typename R, typename... Args>
auto make_logger3(R(*func)(Args...), const string& name) // used function pointer
{
    return Logger3<R(Args...)>(
        std::function<R(Args...)>(func), // create std::function from function pointer 
        name);
}

void function_decorator()
{
    //Logger([]() {cout << "Hello" << endl; }, "HelloFunction")();

    // cannot do this
    //make_logger2([]() {cout << "Hello" << endl; }, "HelloFunction")();
    auto call = make_logger2([]() {cout << "Hello!" << endl; }, "HelloFunction");
    call();

    auto logged_add = make_logger3(add, "Add");
    auto result = logged_add(2, 3);
}

void constructor_forwarding()
{
    struct NotAShape
    {
        virtual string str() const { return string{}; }
    };

    // we don't want this to be legal, thus a static_assert above
    //ColoredShape2<NotAShape> legal;

    // no code completion for this case
    // can comment out argument, too! (default constructor)
    TransparentShape2<Square> hidden_square{ 1, 2 };
    cout << hidden_square.str() << endl;

    ColoredShape2<TransparentShape2<Square>> sq = { "red", 51, 5 };
    cout << sq.str() << endl;
}

int main()
{
    function_decorator();
    //wrapper();
    //mixin_inheritance();
    //constructor_forwarding();

    getchar();
    return 0;
}
