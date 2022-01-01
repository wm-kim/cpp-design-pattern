#include "Headers.hpp"

// intersection of the composite and proxy design patterns

//class Creature
//{
//  int strength, agility, intelligence;
//  // new property
//  int charisma;
//public:
//  int get_strength() const
//  {
//    return strength;
//  }
//
//  void set_strength(int strength)
//  {
//    Creature::strength=strength;
//  }
//
//  int get_agility() const
//  {
//    return agility;
//  }
//
//  void set_agility(int agility)
//  {
//    Creature::agility=agility;
//  }
//
//  int get_intelligence() const
//  {
//    return intelligence;
//  }
//
//  void set_intelligence(int intelligence)
//  {
//    Creature::intelligence=intelligence;
//  }
//
//  // sum, average, max.
//
//  double sum() const {
//    return strength + agility + intelligence;
//  }
//
//  double average() const {
//    return sum() / 3.0;
//  }
//
//  int max() const {
//    return ::max(::max(strength, agility), intelligence);
//  }
//};

// the implementation of aggregate calculations becomes really painful
// having aggregate storage / composite storage for the actual fields
// and having to calculate the aggregate values

class Creature
{
    enum Abilities { str, agl, intl, count }; // not using enum class (for using integer conversion)
    array<int, count> abilities; // count is length of the array
public:
    int get_strength() const { return abilities[str]; }
    void set_strength(int value) { abilities[str] = value; }

    int get_agility() const { return abilities[agl]; }
    void set_agility(int value) { abilities[agl] = value; }

    int get_intelligence() const { return abilities[intl]; }
    void set_intelligence(int value) { abilities[intl] = value; }

    int sum() const {
        return accumulate(abilities.begin(), abilities.end(), 0);
    }

    double average() const {
        return sum() / (double)count;
    }

    int max() const {
        return *max_element(abilities.begin(), abilities.end());
    }
};

//  wonderful operations from the standard library are suddenly available to you.

int main(int ac, char* av[])
{
    Creature orc;
    orc.set_strength(16);
    orc.set_agility(11);
    orc.set_intelligence(9);

    cout << "The orc has an average stat of "
        << orc.average()
        << " and a maximum stat of "
        << orc.max()
        << "\n";

    return 0;
}

// 덕 타이핑을 지원하기 때문에 가능한것 (동적 타이핑의 한 종류) 실행이 되었을 때 해당 Method들을 확인하여 타입을 정한다.