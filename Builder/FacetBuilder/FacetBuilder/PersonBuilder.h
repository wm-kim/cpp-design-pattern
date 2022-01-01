#pragma once
#include "Person.h"

class PersonAddressBuilder;
class PersonJobBuilder;

class PersonBuilderBase
{
protected:
  Person& person;
  // base class which has reference and use derived class which has actual object. don't wan't to be copied multiple times
  explicit PersonBuilderBase(Person& person)
    : person{ person }
  {
  }
public:
  // Actual API
  operator Person() const 
  {
    return std::move(person);
  } 

  // builder facets
  PersonAddressBuilder lives() const;
  PersonJobBuilder works() const;
};

// want to construct just once
class PersonBuilder : public PersonBuilderBase
{
  Person p;
public:
  PersonBuilder(): PersonBuilderBase{p} 
  {
  }
};