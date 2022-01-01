#include "Foo.h"

class Foo::impl
{
    // implementation detail
    public:
        void greet(Foo* p);
};

void Foo::greet()
{
    impl->greet(this);
}