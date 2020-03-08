#include <iostream>
#include "Entity.hpp"
#include "Mutation.hpp"

using namespace std;

int main() {
	Entity entity;
	cout << entity << endl;
	Mutate(entity, MutationType::Random);
	cout << entity << endl;
	Mutate(entity, MutationType::Swap);
	cout << entity << endl;
	Mutate(entity, MutationType::Reverse);
	cout << entity << endl;
}