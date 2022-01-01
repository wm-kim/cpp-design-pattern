// visitor examples for design patterns c++ book
#include <sstream>
#include <string>
#include <iostream>
using namespace std;

struct Expression
{
    virtual ~Expression() = default;
    virtual void print(ostringstream& oss) = 0; // breaking OCP, SRP. go into existing code and modified
    // would be much nicer to have them in a separate component
};

struct DoubleExpression : Expression
{
    double value;
    explicit DoubleExpression(const double value)
        : value{ value } {}

    void print(ostringstream& oss) override // stream is visitor
    {
        oss << value;
    }
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

    void print(ostringstream& oss) override
    {
        oss << "(";
        left->print(oss); // oss << left (double), recursive, goes through heirarchy
        oss << "+";
        right->print(oss); // oss << right (double)
        oss << ")";
    }
};

void main_2_()
{
    auto e = new AdditionExpression{
      new DoubleExpression{1},
      new AdditionExpression{
        new DoubleExpression{2},
        new DoubleExpression{3}
      }
    };
    ostringstream oss;
    e->print(oss);
    cout << oss.str() << endl;
}