#include <iostream>
#include <string>
using namespace std;

struct Creature
{
    string name;
    int attack, defense;

    Creature(const string& name, const int attack, const int defense)
        : name(name),
        attack(attack),
        defense(defense)
    {
    }


    friend ostream& operator<<(ostream& os, const Creature& obj)
    {
        return os
            << "name: " << obj.name
            << " attack: " << obj.attack
            << " defense: " << obj.defense;
    }
};

// has a pointer to the next member of the same class

class CreatureModifier
{
    CreatureModifier* next{ nullptr }; // unique_ptr
protected:
    Creature& creature; // pointer or shared_ptr
public:
    explicit CreatureModifier(Creature& creature)
        : creature(creature)
    {
    }
    virtual ~CreatureModifier() = default;

    // list a modifier into a chain
    void add(CreatureModifier* cm)
    {
        if (next) next->add(cm);
        else next = cm;
    }

    // two approaches:

    // 1. Always call base handle(). There could be additional logic here.
    // 2. Only call base handle() when you cannot handle things yourself.

    virtual void handle() // perform the modifications
    {
        if (next) next->handle(); // default behaviour
    }
};



// able to disable the whole chain - simply fail to propogate class call
// 
// 1. Double the creature's attack
// 2. Increase defense by 1 unless power > 2
// 3. No bonuses can be applied to this creature

class NoBonusesModifier : public CreatureModifier
{
public:
    explicit NoBonusesModifier(Creature& creature)
        : CreatureModifier(creature)
    {
    }

    void handle() override
    {
        // nothing, prevents the walking
    }
};

class DoubleAttackModifier : public CreatureModifier
{
public:
    explicit DoubleAttackModifier(Creature& creature)
        : CreatureModifier(creature)
    {
    }

    void handle() override
    {
        creature.attack *= 2;
        CreatureModifier::handle(); // allow us to walk the chain of responsibility
    }
};

class IncreaseDefenseModifier : public CreatureModifier
{
public:
    explicit IncreaseDefenseModifier(Creature& creature)
        : CreatureModifier(creature)
    {
    }


    void handle() override
    {
        if (creature.attack <= 2)
            creature.defense += 1;
        CreatureModifier::handle();
    }
};

int main_()
{
    Creature goblin{ "Goblin", 1, 1 };
    CreatureModifier root{ goblin }; // base modifier doesn't do anything
    DoubleAttackModifier r1{ goblin };
    DoubleAttackModifier r1_2{ goblin };
    IncreaseDefenseModifier r2{ goblin };
    //NoBonusesModifier nb{ goblin }; // effectively Command objects

    //root.add(&nb);
    root.add(&r1);
    root.add(&r1_2); // a=4
    root.add(&r2); // no-op

    root.handle(); // annoying

    cout << goblin << endl;

    //getchar();
    return 0;
}

// that this particular implementation of a chain of responsibility is really quite artificial, because
// essentially what's happening here is you're building a singly linked list.
// why not use list or vector