#pragma once
#include <string>
#include <vector>
using namespace std;

// mediator is something that everybody can reference. 
// global static variable or something, or it's just passed in in the constructor of every created objects
struct ChatRoom;

struct Person {
  string name;
  ChatRoom* room{nullptr};
  vector<string> chat_log;

  Person(const string &name);

  void say(const string& message) const;
  void pm(const string& who,
    const string& message) const;

  void receive(const string& origin,
    const string& message);

  bool operator==(const Person &rhs) const; // don't want to call your own receive

  bool operator!=(const Person &rhs) const;
};


