#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "Config.hpp"

class ConfigParser {
	std::string GetText(const std::string& line, size_t &start); // получение текста
	void ProcessLine(const std::string& line, Config &config); // обработка строки файла
public:
	Config Parse(const std::string& path);
};

// получение текста
std::string ConfigParser::GetText(const std::string& line, size_t &start) {
	// ищем непробельный символ
	while (start < line.length() && (line[start] == ' ' || line[start] == '='))
		start++;

	std::string text = "";

	// наращиваем строку до конца, до пробела или равенства
	while (start < line.length() && line[start] != ' ' && line[start] != '=') {
		text += line[start];
		start++;
	}

	return text;
}

// обработка строки файла
void ConfigParser::ProcessLine(const std::string& line, Config &config) {
	size_t i = 0;
	std::string key = GetText(line, i);
	std::string value = GetText(line, i);

	if (key == "mode") {
		if (value != "min" && value != "max")
			throw std::runtime_error("unknown mode argument '" + value + "'");

		config.mode = value;
	}
	else if (key == "left_border" || key == "left" || key == "a") {
		config.leftBorder = std::stod(value);
	}
	else if (key == "right_border" || key == "right" || key == "b") {
		config.rightBorder = std::stod(value);
	}
	else if (key == "population_size") {
		config.populationSize = std::stoi(value);

		if (config.populationSize < 3)
			throw std::runtime_error("invalid population size '" + value + "'");
	}
	else if (key == "max_epochs" || key == "total_epochs" || key == "epochs" || key == "iterations") {
		config.maxEpochs = std::stoi(value);

		if (config.maxEpochs < 1)
			throw std::runtime_error("invalid max epochs parameter '" + value + "'");
	}
	else if (key == "max_valueless_epochs" || key == "valueless_epochs" || key == "valueless_iterations") {
		config.maxValuelessEpochs = std::stoi(value);

		if (config.maxValuelessEpochs < 1)
			throw std::runtime_error("invalid max valueless epochs parameter '" + value + "'");
	}
	else if (key == "quality_epsilon" || key == "epsilon") {
		config.qualityEpsilon = std::stod(value);

		if (config.qualityEpsilon <= 0 || config.qualityEpsilon > 1)
			throw std::runtime_error("invalid quality epsilon parameter '" + value + "'");
	}
	else if (key == "preserved_part") {
		config.preservedPart = std::stod(value);

		if (config.preservedPart <= 0)
			throw std::runtime_error("invalid preserved part value '" + value + "'");
	}
	else if (key == "selection_type" || key == "selection_mode" || key == "selection") {
		if (value == "random")
			config.selectionType = SelectionType::Random;
		else if (value == "tournament")
			config.selectionType = SelectionType::Tournament;
		else if (value == "roulette")
			config.selectionType = SelectionType::Roullete;
		else if (value == "cut")
			config.selectionType = SelectionType::Cut;
		else
			throw std::runtime_error("unknown selection type '" + value + "'");
	}
	else if (key == "selection_part") {
		config.selectionPart = std::stod(value);

		if (config.selectionPart <= 0)
			throw std::runtime_error("invalid selection part value '" + value + "'");
	}
	else if (key == "crossbreeding_type" || key == "crossbreeding_mode" || key == "crossbreeding") {
		if (value == "one_point")
			config.crossbreedingType = CrossbreedingType::OnePoint;
		else if (value == "two_point")
			config.crossbreedingType = CrossbreedingType::TwoPoint;
		else if (value == "uniform")
			config.crossbreedingType = CrossbreedingType::Uniform;
		else
			throw std::runtime_error("unknown crossbreeding type '" + value + "'");
	}
	else if (key == "mutation_type" || key == "mutation_mode" || key == "mutation") {
		if (value == "random")
			config.mutationType = MutationType::Random;
		else if (value == "two_point")
			config.mutationType = MutationType::Swap;
		else if (value == "reverse")
			config.mutationType = MutationType::Reverse;
		else
			throw std::runtime_error("unknown mutation type '" + value + "'");
	}
	else if (key == "mutation_probability") {
		config.mutationProbability = std::stod(value);

		if (config.mutationProbability <= 0 || config.mutationProbability > 1)
			throw std::runtime_error("invalid mutation probability value '" + value + "'");
	}
	else if (key == "debug") {
		if (value == "true" || value == "") {
			config.debug = true;
		}
		else if (value == "false") {
			config.debug = false;
		}
		else {
			throw std::runtime_error("invalid debug value '" + value + "'");
		}
	}
	else {
		throw std::runtime_error("unknown parameter '" + key + "'");
	}
}

Config ConfigParser::Parse(const std::string& path) {
	std::ifstream f(path);
	Config config = GetDefaultConfig();

	if (!f) {
		std::cout << "Warning: config file '" << path << "' not found, using default configuration" << std::endl; 
		return config;
	}

	std::string line; // строка для считывания строк файла

	// пока не считаем строки всего файла
	while (std::getline(f, line)) {
		if (line == "" || line[0] == '#') // игнорируем комментарии
			continue;

		ProcessLine(line, config); // обрабатываем очередную строку файла
	}

	f.close(); // закрываем файл
	EvalConfig(config); // обрабатываем конфиг

	return config; // возвращаем конфигурационный файл
}