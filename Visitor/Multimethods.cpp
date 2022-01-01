instead of calling the left accept and right, accpet individually, you would actually take the
left and the right and you would call some sort of combine accept on it. [multiple dispatch, multimethods]

depending on which object collides with each other, object, you have different effects.

#include <iostream>
#include <string>
#include <typeindex>
#include <map>
#include <functional>
using namespace std;

struct GameObject;
void collide(GameObject& first, GameObject& second); // global function

struct GameObject
{
	virtual ~GameObject() = default;
	virtual type_index type() const = 0; // 복사 및 대입이 가능한 type_info

	virtual void collide(GameObject& other) { ::collide(*this, other); }
};

// CRTP (Curiously recurring template pattern)
template <typename T> struct GameObjectImpl : GameObject
{
	type_index type() const override
	{
		return typeid(T);
	}
};

struct Planet : GameObjectImpl<Planet> {};
struct Asteroid : GameObjectImpl<Asteroid> {};
struct Spaceship : GameObjectImpl<Spaceship> {};
struct ArmedSpaceship : Spaceship
{
	type_index type() const override {
		return typeid(ArmedSpaceship); // required for collision to function
	}
}; // model limitation

void spaceship_planet() { cout << "spaceship lands on planet\n"; }
void asteroid_planet() { cout << "asteroid burns up in atmosphere\n"; }
void asteroid_spaceship() { cout << "asteroid hits and destroys spaceship\n"; }
void asteroid_armed_spaceship() { cout << "spaceship shoots asteroid\n"; }

map<pair<type_index, type_index>, void(*)(void)> outcomes{
	{{typeid(Spaceship), typeid(Planet)}, spaceship_planet},
	{{typeid(Asteroid),typeid(Planet)}, asteroid_planet},
	{{typeid(Asteroid),typeid(Spaceship)}, asteroid_spaceship},
	{{typeid(Asteroid), typeid(ArmedSpaceship)}, asteroid_armed_spaceship}
};

// global function
void collide(GameObject& first, GameObject& second)
{
	auto it = outcomes.find({ first.type(), second.type() }); // find in pair
	if (it == outcomes.end()) // don't get anything
	{
		it = outcomes.find({ second.type(), first.type() }); // also looks for it in reverse order
		if (it == outcomes.end())
		{
			cout << "objects pass each other harmlessly\n";
			return;
		}
	}
	it->second(); // function ptr
}

int main__(int argc, char* argv[])
{
	ArmedSpaceship spaceship;
	Asteroid asteroid;
	Planet planet;

	collide(planet, spaceship);
	collide(planet, asteroid);
	collide(spaceship, asteroid);
	collide(planet, planet);

	cout << "Member collision:\n";
	planet.collide(asteroid);

	// but this won't work
	spaceship.collide(planet);

	getchar();
	return 0;
}
