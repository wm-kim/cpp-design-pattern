#include <map>
#include <memory>
#include <iostream>
using namespace std;


// multiton does is it sets up the kind of key value store where you can optionally restrict the
// number of elements which are actually included in that storeand given out to the client.
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
        // 이미 만들어져 있다면 그것을 return 한다.
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
//    static int id; // static field를 상속할 수 없다.
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

// Monostate design pattern is that the client isn't really informed it is singleton
// its a viable solution to singleton problem
// But it is a solution which brings a lot of drawbacks. not recommand of using it