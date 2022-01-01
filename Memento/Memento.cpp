#include <iostream>
#include <string>
#include <vector>
#include <memory>
using namespace std;

// snapshop state
class Memento
{
    int balance; // doesn't allow to change the internal state
public:
    Memento(int balance)
        : balance(balance)
    {
    }
    friend class BankAccount; // nested class로 만들어도 된다.
    friend class BankAccount2;
};

class BankAccount
{
    int balance = 0;
public:
    explicit BankAccount(const int balance)
        : balance(balance)
    {
    }

    Memento deposit(int amount)
    {
        balance += amount;
        return { balance }; // compiler can infer it from the return type of the method itself.
    }

    void restore(const Memento& m)
    {
        balance = m.balance;
    }

    friend ostream& operator<<(ostream& os, const BankAccount& obj)
    {
        return os << "balance: " << obj.balance;
    }
};


// ================================================================ 
// we can certainly save the entire history, save every single memento


class BankAccount2 // supports undo/redo
{
    int balance = 0;
    vector<shared_ptr<Memento>> changes;
    int current; // index
public:
    explicit BankAccount2(const int balance)
        : balance(balance)
    {
        changes.emplace_back(make_shared<Memento>(balance));
        current = 0;
    }

    shared_ptr<Memento> deposit(int amount) // we want to both store it internally and return it
    {
        balance += amount;
        auto m = make_shared<Memento>(balance);
        changes.push_back(m);
        ++current;
        return m;
    }

    void restore(const shared_ptr<Memento>& m)
    {
        if (m) // check it is valid, there can be default initialized momento         
        {
            balance = m->balance;
            changes.push_back(m);
            current = changes.size() - 1;
        }
    }

    shared_ptr<Memento> undo()
    {
        if (current > 0)
        {
            --current;
            auto m = changes[current];
            balance = m->balance;
            return m;
        }
        return{}; // sometimes you try to undo something and there is no undo functionality. 
    }

    shared_ptr<Memento> redo()
    {
        if (current + 1 < changes.size())
        {
            ++current;
            auto m = changes[current];
            balance = m->balance;
            return m;
        }
        return{};
    }

    friend ostream& operator<<(ostream& os, const BankAccount2& obj)
    {
        return os << "balance: " << obj.balance;
    }
};

void memento()
{
    BankAccount ba{ 100 };
    auto m1 = ba.deposit(50); // 150
    auto m2 = ba.deposit(25); // 175
    cout << ba << "\n";

    // undo to m1
    ba.restore(m1);
    cout << ba << "\n";

    // redo
    ba.restore(m2);
    cout << ba << "\n";
}

void undo_redo()
{
    BankAccount2 ba{ 100 };
    ba.deposit(50);
    ba.deposit(25); // 125
    cout << ba << "\n";

    ba.undo();
    cout << "Undo 1: " << ba << "\n";
    ba.undo();
    cout << "Undo 2: " << ba << "\n";
    ba.redo();
    cout << "Redo 2: " << ba << "\n";

    ba.undo();
}

int main()
{
    // memento()
    undo_redo();

    getchar();
    return 0;
}
