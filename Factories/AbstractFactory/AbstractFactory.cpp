// abstract factory : families of factories, use inheritence allowing polymorphic call on different factory

#include <iostream>
#include <memory>
#include <map>
#include "HotDrink.h"
#include "DrinkFactory.h"
using namespace std;

// abstract factory : families of factories, use inheritence allowing polymorphic call on different factory
// have family of products, and have corresponding family of factories that shares common interface
// can stick into any kind of data structure

unique_ptr<HotDrink> make_drink(string type)
{
  unique_ptr<HotDrink> drink;
  if (type == "tea")
  {
    drink = make_unique<Tea>();
    drink->prepare(200);
  }
  else 
  {
    drink = make_unique<Coffee>();
    drink->prepare(50);
  }
  return drink;
}

int main53()
{
  auto d = make_drink("tea");

  DrinkFactory df;
  df.make_drink("coffee");

  getchar();
  return 0;
}