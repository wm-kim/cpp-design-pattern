#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <sstream>
#include <memory>
using namespace std;
#include <boost/lexical_cast.hpp>

// lexing ================================================= turns into sequnce of token

struct Token
{
    enum Type { integer, plus, minus, lparen, rparen } type;
    string text;

    explicit Token(Type type, const string& text) :
        type{ type }, text{ text } {}

    friend ostream& operator<<(ostream& os, const Token& obj)
    {
        return os << "`" << obj.text << "`";
    }
};

vector<Token> lex(const string& input)
{
    vector<Token> result;

    for (int i = 0; i < input.size(); ++i)
    {
        switch (input[i])
        {
        case '+':
            result.push_back(Token{ Token::plus, "+" });
            break;
        case '-':
            result.push_back(Token{ Token::minus, "-" });
            break;
        case '(':
            result.push_back(Token{ Token::lparen, "(" });
            break;
        case ')':
            result.push_back(Token{ Token::rparen, ")" });
            break;
        default: 
            // idenfying integers
            ostringstream buffer;
            buffer << input[i];
            for (int j = i + 1; j < input.size(); ++j)
            {
                if (isdigit(input[j]))
                {
                    buffer << input[j];
                    ++i;
                }
                else
                {
                    result.push_back(Token{ Token::integer, buffer.str() });
                    break;
                }
            }
            // not much nessesary
            if (!buffer.str().empty()) // 20201210
                result.push_back(Token{ Token::integer, buffer.str() });
        }
    }

    return result;
}

// parsing ===================================================== parse token into object-oriented structure

struct Element
{
    virtual ~Element() = default;
    virtual int eval() const = 0; // also be using the visitor design pattern
};

struct Integer : Element
{
    int value;
    explicit Integer(const int value)
        : value(value)
    {
    }
    int eval() const override { return value; }
};

struct BinaryOperation : Element
{
    enum Type { addition, subtraction } type;
    shared_ptr<Element> lhs, rhs;

    int eval() const override
    {
        if (type == addition)
            return lhs->eval() + rhs->eval();
        return lhs->eval() - rhs->eval();
    }
};

// parse and put into lhs or either rhs
unique_ptr<Element> parse(const vector<Token>& tokens)
{
    auto result = make_unique<BinaryOperation>();
    bool have_lhs = false;
    for (size_t i = 0; i < tokens.size(); i++)
    {
        auto token = tokens[i];

        switch (token.type)
        {
        case Token::integer:
        {
            int value = boost::lexical_cast<int>(token.text);
            auto integer = make_shared<Integer>(value);
            if (!have_lhs) {
                result->lhs = integer;
                have_lhs = true;
            }
            else result->rhs = integer;
        }
        break;
        case Token::plus:
            result->type = BinaryOperation::addition;
            break;
        case Token::minus:
            result->type = BinaryOperation::subtraction;
            break;

        // ignore the ) because when we hit the ( we're going to be looking for the )

        case Token::lparen:
        {
            int j = i;
            for (; j < tokens.size(); ++j)
                if (tokens[j].type == Token::rparen)
                    break; // found it!

            vector<Token> subexpression(&tokens[i + 1], &tokens[j]);
            auto element = parse(subexpression); 
            // feed back into parse function recursively and get result 
            // then assign element either the lhs or rhs
            if (!have_lhs)
            {
                result->lhs = move(element);
                have_lhs = true;
            }
            else result->rhs = move(element);
            i = j; // advance
        }
        break;
        }
    }
    return result;
}



int main()
{
    string input{ "(14-4)+11" }; // see if you can make nested braces work
    auto tokens = lex(input);

    // let's see the tokens
    for (auto& t : tokens)
        cout << t << "   ";
    cout << endl;

    try {
        auto parsed = parse(tokens);
        cout << input << " = " << parsed->eval() << endl;
    }
    catch (const exception& e)
    {
        cout << e.what() << endl;
    }

    getchar();
    return 0;
}
