#pragma once

#include <iostream>
#include <vector>

#include "Config.hpp"
#include "Entity.hpp"
#include "Crossbreeding.hpp"
#include "Mutation.hpp"

class Population {
	std::vector<Entity> entities; // вектор особей

	void PreserveBestEntities(const Config& config); // сохранение лучших особей
	std::vector<double> GetFitness(const Config& config); // расчёт фитнес функции в зависимости от оценочной функции

	std::vector<Entity> RandomSelection(const Config& config); // случайный отбор
	std::vector<Entity> TournamentSelection(const Config& config); // отбор турниром
	std::vector<Entity> RoulleteSelection(const Config& config); // отбор рулеткой
	std::vector<Entity> CutSelection(const Config& config); // отбор усечением

public:
	Population(int populationSize); // создание популяции заданного размера

	void Eval(const Config &config, double (*f)(double x)); // оценка приспособленности особей
	void Sort(const Config &config); // упорядочивание по возрастанию приспособленности
	Entity GetBestEntity(const Config& config) const; // получение самой приспособленной особи

	void Selection(const Config& config); // отбор
	void Crossbreeding(const Config &config); // скрещивание
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

// сохранение лучших особей
void Population::PreserveBestEntities(const Config& config) {
	// перемещаем в начало лучшие позиции
	for (size_t i = 0; i < config.preservedPositions; i++) {
		double bestScore = entities[i].GetScore();
		size_t bestIndex = i;

		for (size_t j = i + 1; j < config.populationSize; j++) {
			double score = entities[j].GetScore();

			if ((score - bestScore) * config.scale > 0) {
				bestScore = score;
				bestIndex = j;
			}
		}

		// переставляем очередную лучшую особь в начало
		Entity tmp = entities[i];
		entities[i] = entities[bestIndex];
		entities[bestIndex] = tmp;
	}
}

// расчёт фитнес функции в зависимости от оценочной функции
std::vector<double> Population::GetFitness(const Config& config) {
	std::vector<double> fitness(config.populationSize);

	double minScore = entities[0].GetScore();
	double maxScore = entities[0].GetScore();

	// считаем минимальное и максимальное значение оценочной функции
	for (size_t i = 1; i < config.populationSize; i++) {
		double score = entities[i].GetScore();

		if (score > maxScore)
			maxScore = score;

		if (score < minScore)
			minScore = score;
	}

	// заполняем значение фитнесс функции
	for (size_t i = 0; i < config.populationSize; i++) {
		if (config.mode == "max") {
			fitness[i] = (entities[i].GetScore() - minScore) / (maxScore - minScore);
		}
		else {
			fitness[i] = (maxScore - entities[i].GetScore()) / (maxScore - minScore);
		}
	}

	return fitness;
}

// случайный отбор
std::vector<Entity> Population::RandomSelection(const Config& config) {
	std::vector<Entity> parents;

	for (size_t i = config.preservedPositions; i < config.selectionSize; i++) {
		size_t index = GetRandom(config.populationSize);
		parents.push_back(entities[index]);
	}

	return parents;
}

// отбор турниром
std::vector<Entity> Population::TournamentSelection(const Config& config) {
	std::vector<Entity> parents;

	for (size_t i = config.preservedPositions; i < config.selectionSize; i++) {
		size_t index1 = GetRandom(config.populationSize);
		size_t index2 = GetRandom(config.populationSize);

		double score1 = entities[index1].GetScore();
		double score2 = entities[index2].GetScore();

		if ((score1 - score2) * config.scale > 0) {
			parents.push_back(entities[index1]);
		}
		else {
			parents.push_back(entities[index2]);
		}
	}

	return parents;
}

// отбор рулеткой
std::vector<Entity> Population::RoulleteSelection(const Config& config) {
	std::vector<double> fitness = GetFitness(config);
	double sum = 0;

	// считаем нормализованную сумму оценки приспособленности
	for (size_t i = 0; i < config.populationSize; i++)
		sum += fitness[i];
	
	std::vector<Entity> parents;

	// отбираем подходящие особи
	for (size_t i = config.preservedPositions; i < config.selectionSize; i++) {
		double rnd = GetRandom();
		double pi = 0;
		size_t j = 0;

		while (pi > rnd || rnd > pi + fitness[j] / sum) {
			pi += fitness[j] / sum;
			j++;
		}

		parents.push_back(entities[j]);
	}

	return parents;
}

// отбор усечением
std::vector<Entity> Population::CutSelection(const Config& config) {
	Sort(config);
	std::vector<Entity> parents;

	for (size_t i = config.preservedPositions; i < config.selectionSize; i++)
		parents.push_back(entities[i]);

	return parents;
}

// отбор
void Population::Selection(const Config& config) {
	PreserveBestEntities(config); // защищаем лучшие особи
	std::vector<Entity> parents;

	if (config.selectionType == SelectionType::Random) {
		parents = RandomSelection(config);
	}
	else if (config.selectionType == SelectionType::Tournament) {
		parents = TournamentSelection(config);
	}
	else if (config.selectionType == SelectionType::Roullete) {
		parents = RoulleteSelection(config);
	}
	else if (config.selectionType == SelectionType::Cut) {
		parents = CutSelection(config);
	} else {
		throw std::runtime_error("unhandled SelectionType");
	}

	// копируем выбранные особи после защищённых
	for (size_t i = config.preservedPositions; i < config.selectionSize; i++)
		entities[i] = parents[i - config.preservedPositions];
}

// скрещивание
void Population::Crossbreeding(const Config &config) {
	for (size_t i = config.selectionSize; i < config.populationSize; i++) {
		int parent1 = GetRandom(config.selectionSize); // выбираем первого родителя
		int parent2 = GetRandom(config.selectionSize); // выбираем второго родителя

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