#include <iostream>
#include "Entity.hpp"
#include "Crossbreeding.hpp"
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

	cout << endl;
	Entity entity1;
	Entity entity2;
	cout << " parent 1: " << entity1 << endl;
	cout << " parent 2: " << entity2 << endl;
	cout << "one point: " << Crossbreed(entity1, entity2, CrossbreedingType::OnePoint) << endl;
	cout << "two point: " << Crossbreed(entity1, entity2, CrossbreedingType::TwoPoint) << endl;
	cout << "  uniform: " << Crossbreed(entity1, entity2, CrossbreedingType::Uniform) << endl;
}