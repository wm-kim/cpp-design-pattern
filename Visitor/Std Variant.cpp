#include "Headers.hpp"

// some house has actually have names as opposed to numbers. Want to have this ability of being able to store one value or the other value.
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

// need to overload the function call operator () for every single type of the variance.
struct AddressPrinter
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

    AddressPrinter ap;
    std::visit(ap, house); // (visitor, the stuff you want to visit)

    // what if you want to create a visitor in-place? use lambda
    std::visit([](auto& arg) {
        using T = decay_t<decltype(arg)>; // figure out type of the argument

        if constexpr (is_same_v<T, string>) // c++17 ������ Ÿ�ӿ� ���ǥ������ boolean(true/false)�� ���Ѵ�.
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