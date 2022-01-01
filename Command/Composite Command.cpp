#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
using namespace std;

struct BankAccount
{
    int balance = 0;
    int overdraft_limit = -500;

    void deposit(int amount)
    {
        balance += amount;
        cout << "deposited " << amount << ", balance now " <<
            balance << "\n";
    }

    bool withdraw(int amount)
    {
        if (balance - amount >= overdraft_limit)
        {
            balance -= amount;
            cout << "withdrew " << amount << ", balance now " <<
                balance << "\n";
            return true;
        }
        return false;
    }
};

struct Command
{
    bool succeeded;
    virtual void call() = 0;
    virtual void undo() = 0;
};

// should really be BankAccountCommand
struct BankAccountCommand : Command
{
    BankAccount& account;
    enum Action { deposit, withdraw } action;
    int amount;

    BankAccountCommand(BankAccount& account, const Action action,
        const int amount)
        : account(account),
        action(action), amount(amount)
    {
        succeeded = false;
    }

    void call() override
    {
        switch (action)
        {
        case deposit:
            account.deposit(amount);
            succeeded = true;
            break;
        case withdraw:
            succeeded = account.withdraw(amount);
            break;
        }
    }

    void undo() override
    {
        if (!succeeded) return;

        switch (action)
        {
        case withdraw:
            if (succeeded)
                account.deposit(amount);
            break;
        case deposit:
            account.withdraw(amount);
            break;
        }
    }
};

// vector doesn't have virtual dtor, but who cares?
struct CompositeBankAccountCommand : vector<BankAccountCommand>, Command
{
    CompositeBankAccountCommand(const initializer_list<value_type>& _Ilist)
        : vector<BankAccountCommand>(_Ilist)
    {
    }

    void call() override
    {
        for (auto& cmd : *this)
            cmd.call();
    }

    void undo() override
    {
        for (auto it = rbegin(); it != rend(); ++it)
            it->undo();
    }
};

struct DependentCompositeCommand : CompositeBankAccountCommand
{
    explicit DependentCompositeCommand(
        const initializer_list<value_type>& _Ilist)
        : CompositeBankAccountCommand{ _Ilist } {}

    void call() override
    {
        bool ok = true;
        for (auto& cmd : *this)
        {
            if (ok)
            {
                cmd.call();
                ok = cmd.succeeded;
            }
            else // 이후에 오는 것은 다 false 처리
            {
                cmd.succeeded = false;
            }
        }
    }
};

struct MoneyTransferCommand : DependentCompositeCommand
{
    MoneyTransferCommand(BankAccount& from,
        BankAccount& to, int amount) :
        DependentCompositeCommand
    {
      BankAccountCommand{from, BankAccountCommand::withdraw, amount},
      BankAccountCommand{to, BankAccountCommand::deposit, amount}
    } {}
};


int main()
{
    BankAccount ba;
    /*vector<BankAccountCommand> commands{*/
    CompositeBankAccountCommand commands{
      BankAccountCommand{ ba, BankAccountCommand::deposit, 100 },
      BankAccountCommand{ ba, BankAccountCommand::withdraw, 200 }
    };

    cout << ba.balance << endl;


    BankAccount ba1, ba2;
    ba1.deposit(100);

    MoneyTransferCommand cmd{ ba1, ba2, 50 }; // 5000 will fail beacuse neither of the bank account commands knows about the fact that the other command has failed.
    // make sure all the commads are tied together
    cmd.call();

    cout << ba1 << endl << ba2 << endl;
    cmd.undo();
    cout << ba1 << endl << ba2 << endl;


    // apply all the commands
    /*for (auto& cmd : commands)
    {
    cmd.call();
    }*/
    commands.call();

    cout << ba.balance << endl;

    /*for_each(commands.rbegin(), commands.rend(),
    [](const BankAccountCommand& cmd) { cmd.undo(); });*/
    commands.undo();

    cout << ba.balance << endl;

    getchar();
    return 0;
}

// you can encounter as you have commands depending on other commands.(roll back the whole thing.)
//ㅅ hese kinds of mechanics, they get added. the complexity of this is really up to you