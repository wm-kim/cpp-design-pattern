// Dependency Inversion Principle : best way to form dependency between objects
// not about dependency injection

#include <iostream>
#include <string>
#include <vector>
#include <tuple>

using namespace std;

// A. High-level modules should not depend on low-level modules.
//    Both should depend on abstractions.
// B. Abstractions should not depend on details. 
//    Details should depend on abstractions.

// abstractions : interfaces & base classes

enum class Relationship
{
    parent,
    child,
    sibling
};

struct Person
{
    string name;
};

// depedency on interface rather than implementation
struct RelationshipBrowser
{
    virtual vector<Person> find_all_children_of(const string& name) = 0;
};

struct Relationships : RelationshipBrowser // low-level module, data stores & utitlity functions
{
    vector<tuple<Person, Relationship, Person>> relations; 
    // want to make it private then high-level code will break

    void add_parent_and_child(const Person& parent, const Person& child)
    {
        relations.push_back({ parent, Relationship::parent, child });
        relations.push_back({ child, Relationship::child, parent });
    }

    vector<Person> find_all_children_of(const string& name) override
    {
        vector<Person> result;
        for (auto&& [first, rel, second] : relations) // traversing the tuples
        {
            if (first.name == name && rel == Relationship::parent)
            {
                result.push_back(second);
            }
        }
        return result;
    }
};

struct Research // high-level 
{
    Research(RelationshipBrowser& browser)
    {
        for (auto& child : browser.find_all_children_of("John"))
        {
            cout << "John has a child called " << child.name << endl;
        }
    }

    // breaking DIP

    //  Research(const Relationships& relationships)
    //  {
    //    auto& relations = relationships.relations;
    // // -> moving down to low module and introduce abstraction, 그러니까 찾는 기능을 relationship SRP에 포함된다고 생각하고 이동
    //    for (auto&& [first, rel, second] : relations) 
    //    {
    //      if (first.name == "John" && rel == Relationship::parent)
    //      {
    //        cout << "John has a child called " << second.name << endl;
    //      }
    //    }
    //  }
};

int main()
{
    Person parent{ "John" };
    Person child1{ "Chris" };
    Person child2{ "Matt" };

    Relationships relationships;
    relationships.add_parent_and_child(parent, child1);
    relationships.add_parent_and_child(parent, child2);

    Research _(relationships);

    return 0;
}

// dependency injection allows to specify certain default
// whenever somebody wants a RelationshipsBrowser than they should be providing reference to the Relationships class
// dependency injection plays on top of the DIP 