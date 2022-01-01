
//  wrap the Pymble idiom inside some sort of class that you can subsequently use
// pointer or reference to the implementation

// pimpl.h

#include <memory>

// library class typically lowercase
template <typename T>
class pimpl
{
private:
  std::unique_ptr<T> impl;

public:
  pimpl();
  ~pimpl();

  // fowarding constructor
  template <typename... Args>
  pimpl(Args &&...args);

  // treat the impl in a similar way to the way we treat the underlying object.
  T *operator->();
  T &operator*();
};
