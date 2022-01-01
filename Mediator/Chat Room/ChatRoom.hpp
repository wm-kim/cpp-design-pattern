#pragma once
#include <algorithm>


// allow anyone who has pointed to the chat room or in the reference to broadcast
struct ChatRoom
{
  vector<Person*> people; // usually make this private

  void broadcast(const string& origin,
    const string& message);

  void join(Person* p);

  void message(const string& origin,
    const string& who,
    const string& message)
  {
    auto target = std::find_if(
      begin(people),
      end(people),
      [&](const Person *p) {
        return p->name == who;
      });
    if (target != end(people)) // 찾았으면 받는다. person에 pm method에서 사용
    {
      (*target)->receive(origin, message);
    }
  }
};


