#pragma once

#include <iostream>
#include <ctime>
#include <random>

const unsigned M = 30; // число бит генома
const unsigned TwoM = 1 << M; // максимальное значение генома

std::mt19937 generator(time(0));
std::uniform_int_distribution<unsigned> distribution(0, TwoM);
std::uniform_real_distribution<double> realDistribution(0.0, 1.0);

class Entity {
	unsigned bits; // битовое представление особи
	double score; // приспособленность
public:
	Entity(); // создание особи

	double Eval(double a, double b) const; // получение численного значения особи
	void Score(double (*f)(double), double a, double b); // оценка приспособленности
	double GetScore() const; // получение оценки приспособленности

	void InverseBit(int index); // инверсия бита по индексу
	bool GetBit(int index) const; // получение бита по индексу
	void SetBit(int index, bool bit); // установка бита по индексу

	friend std::ostream& operator<<(std::ostream& os, const Entity& entity); // вывод особи
};

double GetRandom() {
	return realDistribution(generator);
}

unsigned GetRandom(unsigned maxValue) {
	return distribution(generator) % maxValue;
}

unsigned GetRandom(unsigned minValue, unsigned maxValue) {
	return minValue + distribution(generator) % (maxValue - minValue);
}

// создание особи
Entity::Entity() {
	bits = GetRandom(TwoM); // генерируем случайные биты
	score = 0;
}

// получение численного значения особи
double Entity::Eval(double a, double b) const {
	return a + bits * (b - a) / TwoM;
}

// оценка приспособленности
void Entity::Score(double (*f)(double), double a, double b) {
	double x = Eval(a, b); // получаем вещественное представление особи
	score = f(x); // вычисляем значение функции
}

// получение оценки приспособленности
double Entity::GetScore() const {
	return score;
}

// инверсия бита по индексу
void Entity::InverseBit(int index) {
	bits ^= 1 << index;
}

// получение бита по индексу
bool Entity::GetBit(int index) const {
	return (bits >> index) & 1;
}

// установка бита по индексу
void Entity::SetBit(int index, bool bit) {
	if (bit) {
		bits |= 1 << index;
	}
	else {
		bits &= ~(1 << index);
	}
}

// вывод особи
std::ostream& operator<<(std::ostream& os, const Entity& entity) {
	os << "entity: " << entity.score << ", bits: [";

	// выводим биты слева направо
	for (int i = M - 1; i >= 0; i--)
		os << ((entity.bits >> i) & 1) << " ";

	return os << "] (int value: " << entity.bits << ")";
}