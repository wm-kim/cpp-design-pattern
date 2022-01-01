#pragma once

#include <string>
#include <vector>

template <typename> struct Observer; // formal declaration for the observer

template <typename T>
struct Observable
{
  std::vector<Observer<T>*> observers; // not a thread-safe 
public:
  void notify(T& source, const std::string& field_name) //  inform all the observers that some change has been made
  {
    for (auto observer : observers)
      observer->field_changed(source, field_name); // fire the notifications
  }

  void subscribe(Observer<T>& observer)
  {
    observers.push_back(&observer);
  }

  void unsubscribe(Observer<T>& observer)
  {
    observers.erase(
      remove(observers.begin(), observers.end(), &observer),
      observers.end() // classic erace remove idiom
    );
  }
};


// ================================================

template <typename T> struct Observable2
{
    signal<void(T&, const string&)> field_changed;
};

class Person2 : public Observable2
{
    int age;
public:
    int get_age() const { return age; }
    void set_age(int age)
    {
        if (this->age == age) return;
        this->age = age;
        field_changed(*this, "age");
    }
};

// how do you subscribe (slots) to the signals, made an observable, but I didn't need a new observer

int main2(int ac, char* av[])
{
    Person2 p2;
    auto conn = p2.field_changed.connect([](Person2& p, const string& field_name)
        { cout << field_name << "has changed \n" });

    p2.set_age(20);
    conn.disconnect();
}