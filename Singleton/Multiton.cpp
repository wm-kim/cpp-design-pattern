#include <map>
#include <memory>
#include <iostream>
using namespace std;

// mono state design pattern is a variation of the Singleton design pattern
// viable solution to singleton problem but has lot of drawbacks
class Printer
{
    static int id; // cannot inherit static field, inflexible, client isn't informed about the fact that is singleton
public:
    int get_id() const
    {
        return id;
    }

    void set_id(int id)
    {
        Printer::id = id;
    }
}

// multiton does is it sets up the kind of key value store where you can optionally restrict the
// number of elements which are actually included in that store 
enum class Importance
{
    primary,
    secondary,
    tertiary
};

// doesn't mean to be instantiated only use static get method
template <typename T, typename Key = std::string>
class Multiton
{
public:
    static shared_ptr<T> get(const Key& key)
    {
        if (const auto it = instances.find(key); // C++17 initialize in if statement
            it != instances.end()) // provided
        {
            return it->second;
        }

        auto instance = make_shared<T>();
        instances[key] = instance;
        return instance;
    }
protected:
    Multiton() = default;
    virtual ~Multiton() = default;
private:
    static map<Key, shared_ptr<T>> instances;
};

template <typename T, typename Key>
map<Key, shared_ptr<T>> Multiton<T, Key>::instances;

class Printer
{
public:
    Printer()
    {
        ++Printer::totalInstanceCount;
        cout << "A total of " <<
            Printer::totalInstanceCount <<
            " instances created so far\n";
    }
private:
    static int totalInstanceCount;
};
int Printer::totalInstanceCount = 0;

//class Printer
//{
//    static int id; 
//public:
//    int get_id() const { return id; }
//    void set_id(int value) { id = value; }
//};

int main()
{
    typedef Multiton<Printer, Importance> mt;

    auto main = mt::get(Importance::primary);
    auto aux = mt::get(Importance::secondary);
    auto aux2 = mt::get(Importance::secondary); // expecting returning same instance
}
