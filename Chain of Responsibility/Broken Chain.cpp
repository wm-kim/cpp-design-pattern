#include <iostream>
#include <string>
using namespace std;
#include <boost/signals2.hpp>
using namespace boost::signals2; // a library for implementing events, sending notifications of different events to classes.

// event broker = cor + observer (sginals2) + cqs (command query seperation) separate idea of splitting the system into commandsand queries.

struct Query
{
    string creature_name;
    enum Argument { attack, defense } argument; // because its already inside the structure not going to use enum-class
    int result;


    Query(const string& creature_name, const Argument argument, const int result)
        : creature_name(creature_name),
        argument(argument),
        result(result)
    {
    }
};

struct Game // also covering mediator design pattern
{
    signal<void(Query&)> queries; // signal is defining the event that people can subscribe to 
    // in order to figure out a creature's attack or defense value,
    // you query it and every single other object in the game has a chance to modify the result of that query
    // invoke it and the query goes through every single subscriber on that
};

class Creature
{
    Game& game;
    int attack, defense;
public:
    string name;
    Creature(Game& game, const string& name, const int attack, const int defense)
        : game(game),
        attack(attack),
        defense(defense),
        name(name)
    {
    }

    // no need for this to be virtual
    int GetAttack() const
    {
        // perform the query
        Query q{ name, Query::Argument::attack, attack }; // initial value
        game.queries(q); // take the signal that we have in our mediator // 동시에 game에 집어넣음
        return q.result; // and invoke the signals
    }

    friend ostream& operator<<(ostream& os, const Creature& obj)
    {
        return os
            << "name: " << obj.name
            << " attack: " << obj.GetAttack() // note here
            << " defense: " << obj.defense;
    }
};



class CreatureModifier // subscribes to the query
{
    Game& game; // have reference of our mediator (which is our events)
    Creature& creature;
public:
    virtual ~CreatureModifier() = default;

    // there is no handle() function

    CreatureModifier(Game& game, Creature& creature)
        : game(game),
        creature(creature)
    {
    }
};

class DoubleAttackModifier : public CreatureModifier
{
    connection conn;
public:
    DoubleAttackModifier(Game& game, Creature& creature)
        : CreatureModifier(game, creature)
    {
        // whenever someone wants this creature's attack, we return DOUBLE the value
        // subscribes to the event
        conn = game.queries.connect([&](Query& q) // lambda function which takes query, notice not a const query (going to modify its result)
            {
                // requirements
                if (q.creature_name == creature.name &&
                    q.argument == Query::Argument::attack)
                    q.result *= 2;
            });
    }

    ~DoubleAttackModifier()
    {
        conn.disconnect(); // unsubscribe in the destructor.
    }
};

// similar idea, but Query instead of Command
int main(int ac, char* av)
{
    Game game;
    Creature goblin{ game, "Strong Goblin", 2, 2 };

    cout << goblin << endl;

    {
        DoubleAttackModifier dam{ game, goblin };

        cout << goblin << endl;
    }

    cout << goblin << endl;

    getchar();
    return 0;
}
