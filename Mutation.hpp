#pragma once

#include <iostream>
#include "Entity.hpp"

// тип мутации
enum class MutationType {
	Random, // изменение случайного бита
	Swap, // перестановка пары случайных битов
	Reverse // реверс части битов
};

// случайная мутация
void RandomMutation(Entity &entity) {
	unsigned index = GetRandom(M); // получаем бит
	entity.InverseBit(index); // инвертируем заданный бит
}

// мутация перестановкой битов
void SwapMutation(Entity &entity) {
	unsigned index1 = GetRandom(M); // получаем индекс первого бита
	unsigned index2 = GetRandom(M); // получаем индекс второго бита

	bool bit1 = entity.GetBit(index1); // получаем первый бит
	bool bit2 = entity.GetBit(index2); // получаем второй бит

	// переставляем биты
	entity.SetBit(bit2, index1);
	entity.SetBit(bit1, index2);
}

// мутация реверсом части битов
void ReverseMutation(Entity &entity) {
	unsigned i = GetRandom(M); // получаем бит
	unsigned j = M - 1; // правая граница - последний бит

	// пока не схлопнутся границы
	while (i < j) {
		// получаем биты
		bool biti = entity.GetBit(i);
		bool bitj = entity.GetBit(j);

		// проставляем биты в нужные места
		entity.SetBit(j, biti);
		entity.SetBit(i, bitj);

		// сдвигаем границы
		i++;
		j--;
	}
}

// мутация в зависимости от режима
void Mutate(Entity &entity, MutationType type) {
	if (type == MutationType::Random) {
		RandomMutation(entity);
	}
	else if (type == MutationType::Swap) {
		SwapMutation(entity);
	}
	else if (type == MutationType::Reverse) {
		ReverseMutation(entity);
	}
	else
		throw std::runtime_error("unhandled MutationType");
}