
// singleton을 constructor를 숨기고 get method를 쓰는것보다 IOC container, dependency injection framework
// singleton을 대신해서 만들어줌. IOC container : allow yourself configure different components with different lifetime
// singleton is nothing more than specification of a lifetimes, 여기서는 boost의 di를 사용할 것이다.

#include "di.hpp"
#include <cstdio>
#include <iostream>
#include <memory>

using std::make_unique;
using std::make_shared;

struct Engine
{
  float volume = 5;
  int horse_power = 400;

  friend std::ostream& operator<<(std::ostream& os, const Engine& obj)
  {
    return os
      << "volume: " << obj.volume
      << " horse_power: " << obj.horse_power;
  }
};

struct ILogger
{
  virtual ~ILogger()
  {
  }

  virtual void Log(const std::string& s) = 0;
};

struct ConsoleLogger : ILogger
{
  ConsoleLogger()
  {
  }

  void Log(const std::string& s) override
  {
    std::cout << "LOG:" << s.c_str() << std::endl;
  }
};

struct Car
{
  std::unique_ptr<Engine> engine;
  std::shared_ptr<ILogger> logger;

  Car(std::unique_ptr<Engine> engine, const std::shared_ptr<ILogger>& logger)
    : engine{move(engine)},
      logger{logger}
  {
    logger->Log("making a car");
  }

  friend std::ostream& operator<<(std::ostream& os, const Car& obj)
  {
    return os << "car with engine: " << *obj.engine;
  }
};

int main123()
{
  // manual
  auto logger = make_shared<ConsoleLogger>();
  auto c = make_shared<Car>(make_unique<Engine>(),logger);

  using namespace boost;
  auto injector = di::make_injector(
    di::bind<ILogger>().to<ConsoleLogger>()
  );
  auto car = injector.create<std::shared_ptr<Car>>();

  std::cout << *car << std::endl;

  getchar();
  return 0;
}

//////////////////////////////////////////////


struct IFoo // interface
{
    virtual string name() = 0;
};

struct Foo : IFoo
{
    static int id;
    Foo() { ++id; }

    string name() override {
        return "foo" + lexical_cast<string>(id);
    }
};

int Foo::id = 0;

struct Bar
{
    std::shared_ptr<IFoo> foo;
};

int main()
{
  // whenever somebody asks for an foo, map it to Foo / provide an instance of foo by default create another instance
  auto injector = di::make_injector(di::bind<IFoo>().to<Foo>().in(di::singleton)); // how the object is created

  auto bar1 = injector.create<std::shared_ptr<Bar>>();
  auto bar2 = injector.create<std::shared_ptr<Bar>>();

  cout << bar1->foo->name() << endl;
  cout << bar2->foo->name() << endl;

}



