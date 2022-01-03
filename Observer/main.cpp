#include "Headers.hpp"
#include "Observer.hpp"
#include "SaferObservable.hpp"

class Person : public Observable<Person> 
{
  int age{0}; // want to know when age got changes
public:
  Person() {}
  Person(int age) : age(age) {}

  int get_age() const
  {
    return age;
  }

  // wants to track when age changes
  void set_age(int age)
  {
    // Person::age=age;
    if (this->age == age) return;

    auto old_can_vote = get_can_vote(); // caching

    this->age = age;
    notify(*this, "age"); 

    if (old_can_vote != get_can_vote()) // compare with the old value
      notify(*this, "can_vote");
  }

  //  problem of dependent properties. properties exist in C# not in C++ 
  bool get_can_vote() const {
    return age >= 16;
  // problem if you want to monitor the changes of this property. 
  // Because there is no way to call notify on it (no setter only getter)

  // rewrite set_age to incorporate the idea of sending notifications on changes of voting status

  // dependencies on the different kind of observables, they become really tricky to handle
  // - build additional pieces of infrastructure to track the changes of the properties and generate appropriate notifications
  }
};

// observer(ConsolePersonObserver) & observable (person)
// in order to use this observer, person class has to support this paradigm. Obseravable interface
struct ConsolePersonObserver : public Observer<Person>
{
private:
  void field_changed(Person &source, const std::string &field_name) override
  {
    cout << "Person's " << field_name << " has changed to ";
    if (field_name == "age") cout << source.get_age();

    // this approach does not really scale
    if (field_name == "can_vote")
      cout << boolalpha << source.get_can_vote();
    cout << ".\n";
  }
};

int main1(int ac, char* av[])
{
  Person person{10};
  ConsolePersonObserver cpo;
  person.subscribe(cpo);

  person.set_age(11);
  person.set_age(12);

  person.unsubscribe(cpo);
  person.set_age(13);

  return 0;
}

// ================================================
// using boost signals

template <typename T>
struct Observable2
{
  signal<void(T &, const string &)> field_changed;
};

class Person2 : public Observable2
{
  int age;

public:
  int get_age() const { return age; }
  void set_age(int age)
  {
    if (this->age == age)
      return;
    this->age = age;
    field_changed(*this, "age"); // invoke the signal, all the slots(subscribers) will be notified
  }
};

int main2(int ac, char *av[])
{
  Person2 p2;
  // subscribe to the signal with a lambda function
  auto conn = p2.field_changed.connect([](Person2 &p, const string &field_name)
                                       { cout << field_name << "has changed \n" });

  p2.set_age(20);
  conn.disconnect();
}

