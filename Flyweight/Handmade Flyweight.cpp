#include <iostream>
#include <string>
#include <cstdint>
using namespace std;

#include <boost/bimap.hpp>
#include <boost/flyweight.hpp>
#include <boost/flyweight/key_value.hpp>
using namespace boost;
using namespace flyweights;

// coloring in the console by-letter vs using ranges

// boost.flyweight  

// naive
typedef uint32_t key;

// mmorpg
struct User
{
    User(const string& first_name, const string& last_name)
        : first_name{ add(first_name) }, last_name{ add(last_name) } 
    {
    }

    // getters . assuming these are immutable for now.
    const string& get_first_name() const
    {
        return names.left.find(last_name)->second;
    }

    // getters 
    const string& get_last_name() const
    {
        return names.left.find(last_name)->second;
    }

    static void info()
    {
        for (auto entry : names.left) 
        {
            cout << "Key: " << entry.first << ", Value: " << entry.second << endl;
        }
    }

    friend ostream& operator<<(ostream& os, const User& obj)
    {
        // verifying index 
        return os
            << "first_name: " << obj.first_name << " " << obj.get_first_name()
            << " last_name: " << obj.last_name << " " << obj.get_last_name();
    }

protected:
    // using a map to store the indices.
    static bimap<key, string> names;
    static key seed;

    static key add(const string& s)
    {
        // check if it is already in the bimap
        auto it = names.right.find(s);
        if (it == names.right.end()) // don't find the key
        {
            // add it
            key id = ++seed;
            names.insert(bimap<key, string>::value_type(seed, s));
            return id;
        }
        return it->second;
    }

    key first_name, last_name; // store first and last name as key
};

// initialization of static members
int User::seed = 0;
bimap<key, string> User::names{}; // bimap allows us to search both side (key from value & value from key)


void Handmade_flyweight()
{
    User user1{ "John", "Smith" };
    User user2{ "John", "Smith" };

    cout << user1 << endl << user2 << endl;
}

void naive_flyweight()
{
    User john_doe{ "John", "Doe" };
    User jane_doe{ "Jane", "Doe" };

    cout << "John " << john_doe << endl;
    cout << "Jane " << jane_doe << endl;

    User::info();
}

/////////////////////// can also use library solutions - using boost

struct User2
{
    // users share names! e.g., John Smith
    flyweight<string> first_name, last_name;
    //string first_name, last_name;
    // ...

    User2(const string& first_name, const string& last_name) // can assign a fly weight of string from string 
    { // can print it to the command line without any particular issues
    }
};

void boost_flyweight()
{
    User2 john_doe{ "John", "Doe" };
    User2 jane_doe{ "Jane", "Doe" };

    // 포인터로 비교해본다
    cout << boolalpha << (&jane_doe.last_name.get() == &john_doe.last_name.get());
    //cout << (&jane_doe.last_name == &john_doe.last_name);
}

int main_()
{
    naive_flyweight();
    boost_flyweight();

    getchar();
    return 0;
}
