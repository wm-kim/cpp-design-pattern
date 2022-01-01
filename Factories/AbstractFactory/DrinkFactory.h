#pragma once
#include <string>
#include "HotDrink.h"
#include "TeaFactory.h"
#include "CoffeeFactory.h"
#include <map>
#include <functional>

struct HotDrink;

class DrinkFactory // facilitator for using all of the factories that we made
{
  map<string, unique_ptr<HotDrinkFactory>> hot_factories;
public:
  // initialize  
  DrinkFactory()
  {
    hot_factories["tea"] = make_unique<TeaFactory>();
    hot_factories["coffee"] = make_unique<CoffeeFactory>();
  }

  unique_ptr<HotDrink> make_drink(const string& name)
  {
    auto drink = hot_factories[name]->make(); // look out in the map
    drink->prepare(200); // oops! fixed amount
    return drink;
  }
};

///////////////////////////////////////////////////////////
// functional factory, use lambda to create the factory
// rather than factory depending on inheritance of abstract factory (HotDrinkFactory)

class DrinkWithVolumeFactory
{
  map<string, function<unique_ptr<HotDrink>()>> factories;
public:

  DrinkWithVolumeFactory()
  {
    factories["tea"] = [] {
      auto tea = make_unique<Tea>();
      tea->prepare(200);
      return tea;
    };
  }

  unique_ptr<HotDrink> make_drink(const string& name);
};

inline unique_ptr<HotDrink> DrinkWithVolumeFactory::make_drink(const string& name)
{
  return factories[name]();
}
