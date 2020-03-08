#pragma once

#include <iostream>
#include <vector>

#include "Config.hpp"
#include "Entity.hpp"
#include "Crossbreeding.hpp"
#include "Mutation.hpp"

class Population {
	std::vector<Entity> entities; // вектор особей

	size_t TournamentSelection(const Config& config); // отбор турниром
	size_t RoulleteSelection(const Config& config); // отбор рулеткой
	size_t CutSelection(const Config& config); // отбор усечением

public:
	Population(int populationSize); // создание популяции заданного размера

	void Eval(const Config &config, double (*f)(double x)); // оценка приспособленности особей
	void Sort(const Config &config); // упорядочивание по возрастанию приспособленности
	Entity GetBestEntity(const Config& config) const; // получение самой приспособленной особи

	size_t Selection(const Config& config); // отбор
	void Crossbreeding(const Config &config, size_t parentsCount); // скрещивание
	void Mutation(const Config &config); // мутация

	friend std::ostream& operator<<(std::ostream& os, const Population& population); // вывод популяции
};

// создание популяции заданного размера
Population::Population(int populationSize) {
	for (int i = 0; i < populationSize; i++)
		entities.push_back(Entity()); // добавляем особи
}

// оценка приспособленности особей
void Population::Eval(const Config &config, double (*f)(double x)) {
	for (size_t i = 0; i < config.populationSize; i++)
		entities[i].Score(f, config.leftBorder, config.rightBorder);
}

// упорядочивание по возрастанию приспособленности
void Population::Sort(const Config &config) {
	for (size_t i = 0; i < config.populationSize; i++) {
        for (size_t j = i; j > 0 && (entities[j - 1].GetScore() - entities[j].GetScore()) * config.scale < 0; j--) {
            Entity tmp = entities[j];
            entities[j] = entities[j - 1];
            entities[j - 1] = tmp;
        }
    }
}

// получение самой приспособленной особи
Entity Population::GetBestEntity(const Config& config) const {
	size_t best = 0;

	// ищем среди всех особей самую приспособленную
	for (size_t i = 1; i < config.populationSize; i++)
		if ((entities[i].GetScore() - entities[best].GetScore()) * config.scale > 0)
			best = i;

	return entities[best]; // возвращаем самую приспособленную особь
}

// отбор турниром
size_t Population::TournamentSelection(const Config& config) {
	if (config.debugSelection)
		std::cout << "tournament selection: " << std::endl;

	size_t parentsCount = config.populationSize * config.selectionPart;
	std::vector<Entity> parents;

	for (size_t i = 0; i < parentsCount; i++) {
		size_t index1 = GetRandom(config.populationSize);
		size_t index2 = GetRandom(config.populationSize);

		double score1 = entities[index1].GetScore();
		double score2 = entities[index2].GetScore();

		if (config.debugSelection)
			std::cout << "Tournament " << (i + 1) << ". " << score1 << " vs " << score2 << ", winner: ";
		
		if ((score1 - score2) * config.scale > 0) {
			parents.push_back(entities[index1]);
			
			if (config.debugSelection)
				std::cout << " first" << std::endl;
		}
		else {
			parents.push_back(entities[index2]);

			if (config.debugSelection)
				std::cout << " second" << std::endl;
		}
	}

	// помещаем родителей в начало
	for (size_t i = config.preservedPositions; i < parentsCount; i++)
		entities[i] = parents[i];

	return parentsCount;
}

// отбор рулеткой
size_t Population::RoulleteSelection(const Config& config) {
	if (config.debugSelection)
		std::cout << "Roullete selection: " << std::endl;

	double minValue, maxValue;

	if (config.mode == "max") {
		minValue = entities[config.populationSize - 1].GetScore();
		maxValue = entities[0].GetScore();
	}
	else {
		minValue = entities[0].GetScore();
		maxValue = entities[config.populationSize - 1].GetScore();
	}

	double delta = maxValue - minValue;
	double sum = 0;

	// считаем нормализованную сумму оценки приспособленности
	for (size_t i = 0; i < config.populationSize; i++)
		sum += (entities[i].GetScore() - minValue) / delta;
	
	size_t j = 0;
	double pi = 1;

	// отбираем подходящие особи
	for (size_t i = 0; i < config.populationSize; i++) {
		if (config.debugSelection)
			std::cout << "p(" << (i + 1) << ") = " << pi;

		if (GetRandom() < pi) {
			entities[j++] = entities[i];

			if (config.debugSelection)
				std::cout << " (selected)";
		}

		if (config.debugSelection)
			std::cout << std::endl;

		double score = (entities[i].GetScore() - minValue) / delta;
		pi -= score / sum;
	}

	return j;
}

// отбор усечением
size_t Population::CutSelection(const Config& config) {
	Sort(config);
	return config.populationSize * config.selectionPart;
}

// отбор
size_t Population::Selection(const Config& config) {
	if (config.selectionType == SelectionType::Tournament)
		return TournamentSelection(config);

	if (config.selectionType == SelectionType::Roullete)
		return RoulleteSelection(config);

	if (config.selectionType == SelectionType::Cut)
		return CutSelection(config);

	throw std::runtime_error("unhandled SelectionType");
}

// скрещивание
void Population::Crossbreeding(const Config &config, size_t parentsCount) {
	for (size_t i = parentsCount; i < config.populationSize; i++) {
		int parent1 = GetRandom(parentsCount); // выбираем первого родителя
		int parent2 = GetRandom(parentsCount); // выбираем второго родителя

		entities[i] = Crossbreed(entities[parent1], entities[parent2], config.crossbreedingType); // выполняем скрещивание
	}
}

// мутация
void Population::Mutation(const Config &config) {
	for (size_t i = config.preservedPositions; i < config.populationSize; i++) // мутируют только незащищённые особи
		if (GetRandom() < config.mutationProbability)
			Mutate(entities[i], config.mutationType); // выполняем мутацию
}

// вывод популяции
std::ostream& operator<<(std::ostream& os, const Population& population) {
	for (size_t i = 0; i < population.entities.size(); i++)
		os << (i + 1) << ". " << population.entities[i] << std::endl;

	return os;
}