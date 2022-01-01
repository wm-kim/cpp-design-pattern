// classic state design pattern implementations that you see out there are just not very realistic

#include <iostream>
#include <string>
using namespace std;

class LightSwitch;

struct State
{
    // What if we are already in the ON state and we call ON once again, what do we do?
    virtual void on(LightSwitch* ls) // override 되지 않음
    {
        cout << "Light is already on\n";
    }
    virtual void off(LightSwitch* ls) // override 되지 않음
    {
        cout << "Light is already off\n";
    }
};

struct OnState : State
{
    OnState()
    {
        cout << "Light turned on\n";
    }

    void off(LightSwitch* ls) override;
};

struct OffState : State
{
    OffState()
    {
        cout << "Light turned off\n";
    }

    void on(LightSwitch* ls) override;
};

class LightSwitch
{
    State* state;
public:
    LightSwitch()
    {
        state = new OffState();
    }
    void set_state(State* state)
    {
        this->state = state;
    }
    void on() { state->on(this); }
    void off() { state->off(this); }
};

void OnState::off(LightSwitch* ls)
{
    cout << "Switching light off...\n";
    ls->set_state(new OffState());
    delete this; // its bizzare but valid
}

void OffState::on(LightSwitch* ls)
{
    cout << "Switching light on...\n";
    ls->set_state(new OnState());
    delete this;
}

void main_3()
{
    LightSwitch ls;
    ls.on();
    ls.off();
    ls.off();
    getchar();
}

// idea that you have every single individual state allowing functions to be called on it to switch to a different state 
// is really weird.