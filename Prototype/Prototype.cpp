#include <string>
#include <iostream>
#include <memory>
#include <functional>
#include <sstream>
#include "Person.h"
using namespace std;
#include <boost/serialization/serialization.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

// 포인터나 reference를 복사하면 나중에 수정할 때 같이 바뀜 - define copy constructor
// 혹은 Address 자체를 copy constructor로 만든다.
// Consumer에게 prototype 제공 - prototype factory를 통해서 that use static method that has static object
// object copy 구현을 직접 만들어야한다. ex copy constuctor / copy assignment operator, no magic bullet
// using serialization for save and load and clone - getting cloning fuctionality
// copy constuctor / copy assignment operator는 자동적으로 생성되지만 serialization은 구현하지 않으면 컴파일이 안된다.

struct Address
{
    string street;
    string city;
    int suite;


    Address(const string& street, const string& city, const int suite)
        : street{ street },
        city{ city },
        suite{ suite }
    {
    }

    // more robust
    /*Address(const Address& other)
      : street{other.street},
        city{other.city},
        suite{other.suite}
    {
    }*/

    friend ostream& operator<<(ostream& os, const Address& obj)
    {
        return os
            << "street: " << obj.street
            << " city: " << obj.city
            << " suite: " << obj.suite;
    }
};


struct Contact
{
    string name;
    Address* address;

    Contact& operator=(const Contact& other)
    {
        if (this == &other)
            return *this;
        name = other.name;
        address = other.address;
        return *this;
    }

    Contact() : name(nullptr), address(nullptr)
    {} // required for serialization

    Contact(string name, Address* address)
        : name{ name }, address{ address }
    {
        //this->address = new Address{ *address };
    }

    // define copy constructor
    Contact(const Contact& other)
        : name{ other.name }
        //, address{ new Address{*other.address} }
    {
        address = new Address(
            other.address->street,
            other.address->city,
            other.address->suite
        );
    }


private:

    // traverse object and serialize correctly : C++ not possible
    // approach : serialization & deserialization, using boost library this time
    // automatic implementation of prototype pattern

    friend class boost::serialization::access;

    template <class archive>
    void save(archive& ar, const unsigned version) const
    {
        ar << name;
        ar << address; // don't have to dereference a pointer (boost가 알아서 해줌)
    }

    template <class archive>
    void load(archive& ar, const unsigned version)
    {
        ar >> name;
        ar >> address;
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()

public:
    ~Contact()
    {
        delete address;
    }


    friend ostream& operator<<(ostream& os, const Contact& obj)
    {
        return os
            << "name: " << obj.name
            << " works at " << *(obj.address); // note the star here
    }
};


struct EmployeeFactory // prototype을 여기에 집어넣어서 사용하게 함
{
    static Contact main;
    static Contact aux;

    static unique_ptr<Contact> NewMainOfficeEmployee(string name, int suite)
    {
        //static Contact p{ "", new Address{ "123 East Dr", "London", 0 } };
        return NewEmployee(name, suite, main); // (name, suite, p);
    }

    static unique_ptr<Contact> NewAuxOfficeEmployee(string name, int suite)
    {
        return NewEmployee(name, suite, aux);
    }

private:
    static unique_ptr<Contact> NewEmployee(string name, int suite, Contact& proto)
    {
        auto result = make_unique<Contact>(proto);
        // customize
        result->name = name;
        result->address->suite = suite;
        return result;
    }
};


// prototype으로 쓰라고 Contact의 global variable를 제공할 수 있다. - not explicit enough
// 이걸 이용해서 copy constructor & copy assignment를 사용하여 만드는것이 가능

// 아니면 explicit clone function을 만들 수 도 있음
// contsruct의 생성자를 private으로 만들어서 직접 생성을 못하게 하고 factory를 사용하게끔 할 수 도 있다.


//Contact EmployeeFactory::main{ "", new Address{ "123 East Dr", "London", 0 } };
//Contact EmployeeFactory::aux{ "", new Address{ "123B East Dr", "London", 0 } };

int main_3423()
{
    // this is tedious
    // Contact john{ "John Doe", new Address{"123 East Dr", "London"} };
    // Contact jane{ "Jane Doe", new Address{"123 East Dr", "London"} };

    auto addr = new Address{ "123 East Dr", "London", 0 /* ? */ };

    //Contact john{ "John Doe", addr };
    //john.address->suite = 123;
    //Contact jane{ "Jane Doe", addr };
    //jane.address->suite = 124;

    //Contact jane2{ jane }; // shallow copy
    //jane2.address->suite = 555;



    //
    //std::cout << jane2 << std::endl;

    // whenever an address is needed, make a copy
    /*Contact john{ "John Doe", new Address{*addr} };
    john.address->suite = 123;

    Contact jane{ "Jane Doe", new Address{*addr} };
    jane.address->suite = 125;

    cout << john << "\n" << jane << endl;*/

    // much better. let's list employees
    //Contact employee{ "Unknown", new Address{"628 Happy St", "Joy", 0} };

    //// we can use this prototype to create john and jane
    //Contact john{ employee };
    //john.name = "John Doe";
    //john.address->suite = 123;

    //Contact jane{ employee };
    //jane.name = "Jane Doe";
    //jane.address->suite = 125;

    //cout << john << "\n" << jane << "\n";

    //delete addr;



    // 4. Boost Serialization

    // too much work in getting the copying working

    //auto john = EmployeeFactory::NewAuxOfficeEmployee("John Doe", 123);
    //auto jane = EmployeeFactory::NewMainOfficeEmployee("Jane Doe", 125);

    //cout << *john << "\n" << *jane << "\n"; // note the stars here

    delete addr;

    getchar();
    return 0;
}

// 자세한 serialize 수행을 보려면 영상을 참고해라