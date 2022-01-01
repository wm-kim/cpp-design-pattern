#include "Person.hpp"
#include "ChatRoom.hpp"
#include <iostream>

Person::Person(const string &name) : name(name) {}

void Person::say(const string &message) const
{
  room->broadcast(name, message); // using room's method
}

void Person::pm(const string &who, const string &message) const // from who
{
  room->message(name, who, message); // using room's method
}

void Person::receive(const string &origin, const string &message)
{
  string s{origin + ": \"" + message + "\""};
  std::cout << "[" << name << "'s chat session]" << s << "\n";
  chat_log.emplace_back(s);
}

bool Person::operator==(const Person &rhs) const {
  return name == rhs.name;
}

bool Person::operator!=(const Person &rhs) const {
  return !(rhs == *this);
}
