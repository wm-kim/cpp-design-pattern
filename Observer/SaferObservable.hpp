#pragma once

#include <string>
#include <vector>
#include <mutex>
   
// stick a lock into every single one of these functions so that only one of them can be executing at any given moment

// problem of re-entrance : what if you call unsubscribe inside notify - TrafficAdministration
// think about the sequence of invocations

template <typename> struct Observer;

template <typename T>
struct SaferObservable
{
  std::vector<Observer<T>*> observers;
  typedef std::recursive_mutex mutex_t;
  mutex_t mtx;
public:
  void notify(T& source, const std::string& field_name)
  {
    // we took this lock here with a mutex and then we are trying to get another scoped lock with the same mutex.

    std::scoped_lock<mutex_t> lock{mtx};
    for (auto observer : observers)
      if (observer)
        observer->field_changed(source, field_name);
  }

  void subscribe(Observer<T>& observer)
  {
    std::scoped_lock<mutex_t> lock{mtx};
    observers.push_back(&observer);
  }

  void unsubscribe(Observer<T>& observer)
  {
    // avoid taking the lock here actively. avoid changing the collection
    // set the observer to nullptr, which is only possible if you have a vector
    auto it = std::find(begin(observers), end(observers), &observer);
    if (it != end(observers))
      *it = nullptr;
//    std::scoped_lock<mutex_t> lock{mtx};
//    observers.erase(
//      remove(observers.begin(), observers.end(), &observer),
//      observers.end()
//    );
  }
};


// other alternative, is to go from an ordinary mutex to a recursive mutex.
// most hated structure if you go to a site like Stack Overflow, not a popular choice 