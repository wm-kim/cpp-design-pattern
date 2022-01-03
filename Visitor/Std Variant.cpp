#include <iostream>
#include <variant>

using namespace std;

// some house has actually have names as opposed to numbers in address
// Want to have this ability of being able to store one value or the other value.

// Old C++
struct House
{
    union {
        string house_name;
        int house_num;
    };
    enum class Type {name, number} type;
};

// =============================================

// modern C++

// need to overload the function call operator () for every single type of the variance.
// checks at compile time whether you implmented all of the cases.
struct AddressPrinter // visitor
{
    void operator()(const string& house_name) const {
        cout << "A house called " << house_name << "\n";
    }

    void operator()(const int house_number) const {
        cout << "House number " << house_number << "\n";
    }
};

int main(int ac, char* av[])
{
    // can specify as template arguments all of the varieties of data you want to store.
    variant<string, int> house;

    //house = "Montefiore Castle";
    house = 221;

    // how can youprint that part of the house which has assigned -> visitor pattern

    AddressPrinter ap;

    // takes the first agrument as callable, and second arugment as thing you want to visit 
    std::visit(ap, house); // call the appropriate overload of the function call operator

    // what if you want to create a visitor in-place? use lambda
    std::visit([](auto& arg) {
        using T = decay_t<decltype(arg)>; // figure out type of the argument

        if constexpr (is_same_v<T, string>) 
        {
            cout << "A house called " << arg.c_str() << "\n";
        }
        else
        {
            cout << "House number " << arg << "\n";
        }
        }, house);

    return 0;
}