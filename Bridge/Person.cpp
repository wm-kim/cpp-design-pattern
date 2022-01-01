#include "Person.h"

// pimple idiom is because it is also a manifestation of the bridge design pattern
// hiding the implementation details of a particular class by sticking it into its implementation

// its implementation is deferred or relayed or bridged into a separate class.

// implementation not goes into header file goes into cpp file, so not exposing the details of implementations
// cpp files -> don't have to ship it to the cilent who consumes the API

// improving compilation speed : allows you to change the internal implementations without changing the surface API of the signatures.
// if not consumer, whoever is actually using the header file, they would have to recompile that program
// you can recompile and ship new binaries to the client

class Person::PersonImpl
{
public:
	void greet(Person* p);
};

void Person::PersonImpl::greet(Person* p) // this 를 넘겨받는다
{
	printf("hello %s", p->name.c_str());
}

Person::Person()
	: impl(new PersonImpl)
{
}

Person::~Person()
{
	delete impl;
}

void Person::greet()
{
	impl->greet(this); // 대신 호출
}


