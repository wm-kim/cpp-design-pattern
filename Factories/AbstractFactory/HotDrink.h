#pragma once // 바로 #pragma once라는 전처리기를 내가 만들고자 하는 헤더파일 안에 넣어주면, 
// 누군가 내 헤더파일을 중복 include 시키려고 해도,
// 결국 컴파일 시에는 한번만 포함이 되는 것이다.
#include <memory>
#include <map>
#include <string>
using namespace std;

struct HotDrink
{
  virtual ~HotDrink() = default;

  virtual void prepare(int volume) = 0;
};

struct Tea : HotDrink
{

  void prepare(int volume) override
  {
    cout << "Take tea bag, boil water, pour " << volume << "ml, add some lemon" << endl;
  }
};

struct Coffee : HotDrink
{
  void prepare(int volume) override
  {
    cout << "Grind some beans, boil water, pour " << volume << "ml, add cream, enjoy!" << endl;
  }
};