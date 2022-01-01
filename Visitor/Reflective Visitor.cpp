// visitor examples for design patterns c++ book

// serious problem and essentially a limitation of the way that most of the statically typed, object oriented languages actually work


#include <sstream>
#include <string>
#include <iostream>
using namespace std;

struct Expression
{
    virtual ~Expression() = default;
};

struct DoubleExpression : Expression
{
    double value;
    explicit DoubleExpression(const double value)
        : value{ value } {}
};

struct AdditionExpression : Expression
{
    Expression* left, * right;

    AdditionExpression(Expression* const left, Expression* const right)
        : left{ left }, right{ right } {}

    ~AdditionExpression()
    {
        delete left;
        delete right;
    }
};

// seperate component
struct ExpressionPrinter
{
    /*void print(DoubleExpression *de, ostringstream& oss) const
    {
      oss << de->value;
    }
    void print(AdditionExpression *ae, ostringstream& oss) const // unable to write the corresponding print expression for an additional expression.
    {
      oss << "(";
      print(ae->left, oss); // we don't have an overload of print, which actually takes an expression. (double? addition?)
      
      oss << "+";
      print(ae->right, oss);
      oss << ")";
    }*/

    // might work in some dynamically typed language, but in a statically typed, compiled kind of language like C++, it's simply not going to work.


    ostringstream oss;

    void print(Expression* e)
    {
        if (auto de = dynamic_cast<DoubleExpression*>(e))
        {
            oss << de->value;
        }
        else if (auto ae = dynamic_cast<AdditionExpression*>(e))
        {
            oss << "(";
            print(ae->left); // recursive
            oss << "+";
            print(ae->right);
            oss << ")";
        }
    }
    // dynamic cast typically has a runtime cost associated with it.
    //  have to keep adding the ifs for every single element in the hierarchy
    // these kinds of type checks in other programming languages, they are done via a mechanism called reflection

    string str() const { return oss.str(); }
};

void main_3_()
{
    auto e = new AdditionExpression{
      new DoubleExpression{ 1 },
      new AdditionExpression{
        new DoubleExpression{ 2 },
        new DoubleExpression{ 3 }
      }
    };
    ostringstream oss;
    //e->print(oss);
    ExpressionPrinter ep;
    ep.print(e);
    cout << ep.str() << endl;
}