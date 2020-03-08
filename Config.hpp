#pragma once

#include <iostream>
#include <string>

enum class SelectionType {
	Random, // случайная селекция
	Tournament, // турнир
	Roullete, // метод релетки
	Cut // метод усечения
};

enum class CrossbreedingType {
	OnePoint, // одноточечное
	TwoPoint, // двухточечное
	Uniform // однородное
};

// тип мутации
enum class MutationType {
	Random, // изменение случайного бита
	Swap, // перестановка пары случайных битов
	Reverse // реверс части битов
};

struct Config {
	std::string mode; // режим работы (min/max)
	int scale; // множитель для сравнений
	double leftBorder; // левая граница пространства поиска
	double rightBorder; // правая граница пространства поиска

	size_t populationSize; // размер популяции

	size_t maxEpochs; // число эпох
	size_t maxValuelessEpochs; // число эпох без улучшения
	double qualityEpsilon; // точность улучшения

	double preservedPart; // доля или число сохраняемых особей
	size_t preservedPositions; // число сохраняемых особей

	SelectionType selectionType; // тип отбора
	double selectionPart; // доля отбираемых (только для случайной селекции, турнира и усечения)

	CrossbreedingType crossbreedingType; // тип скрещивания

	MutationType mutationType; // тип мутации
	double mutationProbability; // вероятность мутации

	bool debugPopulation; // отладочный режим для популяции
	bool debugSelection; // отладочный режим для селекции
};

void EvalConfig(Config &config) {
	config.scale = config.mode == "max" ? 1 : -1;
	config.preservedPositions = (config.preservedPart >= 1 ? config.preservedPart : config.preservedPart * config.populationSize);
}

Config GetDefaultConfig() {
	Config config;

	config.mode = "max";
	config.leftBorder = 0;
	config.rightBorder = 10;

	config.populationSize = 40;

	config.maxEpochs = 100;
	config.maxValuelessEpochs = 15;
	config.qualityEpsilon = 1e-5;

	config.preservedPart = 5;

	config.selectionType = SelectionType::Roullete;
	config.selectionPart = 0.4;

	config.crossbreedingType = CrossbreedingType::TwoPoint;

	config.mutationType = MutationType::Random;
	config.mutationProbability = 0.2; // 20% мутируют

	config.debugPopulation = false;
	config.debugSelection = false;
	
	EvalConfig(config);
	return config;
}

void PrintSelectionType(SelectionType type) {
	switch (type) {
		case SelectionType::Random:
			std::cout << "random";
			break;

		case SelectionType::Tournament:
			std::cout << "tournament";
			break;

		case SelectionType::Roullete:
			std::cout << "roullete";
			break;

		case SelectionType::Cut:
			std::cout << "cut";
			break;
	}
}

void PrintCrossbreedingType(CrossbreedingType type) {
	switch (type) {
		case CrossbreedingType::OnePoint:
			std::cout << "one point";
			break;

		case CrossbreedingType::TwoPoint:
			std::cout << "two point";
			break;

		case CrossbreedingType::Uniform:
			std::cout << "uniform";
			break;
	}
}

void PrintMutationType(MutationType type) {
	switch (type) {
		case MutationType::Random:
			std::cout << "random";
			break;

		case MutationType::Swap:
			std::cout << "swap";
			break;

		case MutationType::Reverse:
			std::cout << "reverse";
			break;
	}
}

void PrintConfig(Config config) {
	std::cout << "mode: " << config.mode << std::endl;
	std::cout << "search space: [" << config.leftBorder << ", " << config.rightBorder << "]" << std::endl;
	std::cout << std::endl;
	std::cout << "population size: " << config.populationSize << std::endl;
	std::cout << std::endl;
	std::cout << "max epochs: " << config.maxEpochs << std::endl;
	std::cout << "max valueless epochs: " << config.maxValuelessEpochs << std::endl;
	std::cout << "quality epsilon: " << config.qualityEpsilon << std::endl;
	std::cout << "preserved positions: " << config.preservedPositions << std::endl;
	std::cout << std::endl;
	std::cout << "selection: ";
	PrintSelectionType(config.selectionType);

	if (config.selectionType == SelectionType::Tournament || config.selectionType == SelectionType::Cut)
		std::cout << " (" << config.selectionPart << ")";
	std::cout << std::endl;

	std::cout << "crossbreeding: ";
	PrintCrossbreedingType(config.crossbreedingType);
	std::cout << std::endl;

	std::cout << "mutation: ";
	PrintMutationType(config.mutationType);
	std::cout << " (" << config.mutationProbability << ")" << std::endl;
}