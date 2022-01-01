// visitor examples for design patterns c++ book
#include <sstream>
#include <string>
#include <iostream>
using namespace std;

// double dispatch approach exists is because it is impossible to perform a dispatch at runtime, at least in C++.
// runtime checking (trick the system into doing so.)

struct SubtractionExpression;
struct DoubleExpression;
struct AdditionExpression;

struct ExpressionVisitor // component which is able to visit every element of the expression hierarchy.
{
    virtual void visit(DoubleExpression* de) = 0;
    virtual void visit(AdditionExpression* ae) = 0;
    virtual void visit(SubtractionExpression* se) = 0;
    // 타입에 따라 불려지는 method가 결정된다.

}; // if you want printing, evaluation ... , you would have to implement this "interface"

struct ExpressionPrinter : ExpressionVisitor
{
    ostringstream oss;
    string str() const { return oss.str(); }
    void visit(DoubleExpression* de) override;  
    void visit(AdditionExpression* ae) override;
    void visit(SubtractionExpression* se) override;
};

struct ExpressionEvaluator : ExpressionVisitor
{
    double result;
    void visit(DoubleExpression* de) override;
    void visit(AdditionExpression* ae) override;
    void visit(SubtractionExpression* se) override;
};

struct Expression
{
    virtual void accept(ExpressionVisitor* visitor) = 0; // called accept, almost by convention
};

// could have one implementation of accept via the curiously recurring template pattern (더 복잡)

struct DoubleExpression : Expression
{
    double value;
    explicit DoubleExpression(const double value)
        : value{ value } {}

    void accept(ExpressionVisitor* visitor) override
    {
        visitor->visit(this); // double hop, need the type of the this pointer
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

    void accept(ExpressionVisitor* visitor) override
    {
        visitor->visit(this); // double hop
    }
};

struct SubtractionExpression : Expression
{
    Expression* left, * right;

    SubtractionExpression(Expression* const left, Expression* const right)
        : left{ left }, right{ right } {}

    ~SubtractionExpression()
    {
        delete left;
        delete right;
    }

    void accept(ExpressionVisitor* visitor) override
    {
        visitor->visit(this);
    }
};

// ==============================================================


void ExpressionPrinter::visit(DoubleExpression* de)
{
    oss << de->value;
}

void ExpressionPrinter::visit(AdditionExpression* e)
{
    bool need_braces = dynamic_cast<SubtractionExpression*>(e->right); // only add Brace's if we actually need to.
    e->left->accept(this); // accept calls visit
    oss << "-";
    if (need_braces) oss << "(";
    e->right->accept(this);
    if (need_braces) oss << ")";
}

void ExpressionPrinter::visit(SubtractionExpression* se)
{
    // only had Brace's if the right hand side of whatever expression we're in, at least in the subtraction part
    bool need_braces = dynamic_cast<SubtractionExpression*>(se->right); 
    // take a single argument, it's very easy to do the double dispatch 
    // because it's always obvious what overload you should be calling.
    if (need_braces) oss << "(";
    se->left->accept(this);
    oss << "-";
    se->right->accept(this);
    if (need_braces) oss << ")";
}

// extensibility of OCP 

void ExpressionEvaluator::visit(DoubleExpression* de)
{
    result = de->value;
}

void ExpressionEvaluator::visit(AdditionExpression* ae)
{
    ae->left->accept(this);
    auto temp = result;
    ae->right->accept(this);
    result += temp;
}

void ExpressionEvaluator::visit(SubtractionExpression* se)
{
    se->left->accept(this);
    auto temp = result;
    se->right->accept(this);
    result -= temp;
}

// ==============================================================

void main()
{
    auto e = new AdditionExpression{
      new DoubleExpression{ 1 },
      new SubtractionExpression {
        new DoubleExpression{ 2 },
        new DoubleExpression{ 3 }
      }
    };
    ostringstream oss;
    ExpressionPrinter printer;
    ExpressionEvaluator evaluator;
    printer.visit(e);
    evaluator.visit(e);
    cout << printer.str() << " = " << evaluator.result << endl;
    getchar();
}
