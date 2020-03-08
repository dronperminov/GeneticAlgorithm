#pragma once

#include <iostream>
#include "Entity.hpp"

enum class CrossbreedingType {
	OnePoint, // одноточечное
	TwoPoint, // двухточечное
	Uniform // однородное
};

// одноточечное скрещивание
Entity OnePointCrossbreed(const Entity &entity1, const Entity &entity2) {
	Entity entity; // создаём новую особь
	unsigned point = GetRandom(M); // выбираем точку для скрещивания

	// заполняем биты новой особи
	for (int i = 0; i < M; i++) {
		bool bit;

		if (i < point)
			bit = entity1.GetBit(i); // берём из первого родителя
		else
			bit = entity2.GetBit(i); // берём из второго родителя

		entity.SetBit(i, bit); // проставляем полученный бит
	}

	return entity;
}

// двухточечное скрещивание
Entity TwoPointCrossbreed(const Entity &entity1, const Entity &entity2) {
	Entity entity; // создаём новую особь
	unsigned point1 = GetRandom(M); // выбираем первую точку для скрещивания
	unsigned point2 = GetRandom(M); // выбираем вторую точку для скрещивания

	// переставляем точки, если они не в том порядке
	if (point1 > point2) {
		unsigned tmp = point1;
		point1 = point2;
		point2 = tmp;
	}

	// заполняем биты новой особи
	for (int i = 0; i < M; i++) {
		bool bit;

		if (i < point1 || i > point2)
			bit = entity1.GetBit(i); // берём из первого родителя
		else
			bit = entity2.GetBit(i); // берём из второго родителя

		entity.SetBit(i, bit); // проставляем полученный бит
	}

	return entity;
}

// однородное скрещивание
Entity UniformCrossbreed(const Entity &entity1, const Entity &entity2) {
	Entity entity; // создаём новую особь

	// заполняем биты новой особи
	for (int i = 0; i < M; i++) {
		bool bit;

		if (GetRandom() < 0.5)
			bit = entity1.GetBit(i); // берём из первого родителя
		else
			bit = entity2.GetBit(i); // берём из второго родителя

		entity.SetBit(i, bit); // проставляем полученный бит
	}

	return entity;
}

Entity Crossbreed(const Entity &entity1, const Entity &entity2, CrossbreedingType type) {
	if (type == CrossbreedingType::OnePoint)
		return OnePointCrossbreed(entity1, entity2);
	
	if (type == CrossbreedingType::TwoPoint)
		return TwoPointCrossbreed(entity1, entity2);

	if (type == CrossbreedingType::Uniform)
		return UniformCrossbreed(entity1, entity2);

	throw std::runtime_error("unhandled CrossbreedingType");
}