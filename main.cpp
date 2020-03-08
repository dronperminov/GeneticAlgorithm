#include <iostream>
#include <cmath>

#include "Config.hpp"
#include "ConfigParser.hpp"
#include "Entity.hpp"
#include "Population.hpp"
#include "Crossbreeding.hpp"
#include "Mutation.hpp"

using namespace std;

// оптимизируемая функция
double f(double x) {
	return x * sin(x + 5) * cos(x - 6) * sin(x + 7) * cos(x - 8) * sin(x / 3);
}

int main() {
	ConfigParser parser;
	Config config = parser.Parse("config.txt");
	PrintConfig(config); // выводим конфигурацию

	int nochangingIterations;
	double prevBest;

	Population population(config.populationSize); // создаём популяцию

	for (size_t epoch = 0; epoch < config.maxEpochs; epoch++) {
		population.Eval(config, f); // оцениваем популяцию
		Entity bestEntity = population.GetBestEntity(config); // получаем лучшую особь

		double x = bestEntity.Eval(config.leftBorder, config.rightBorder);
		cout << "Epoch " << epoch << "\tbest: f(x) = " << f(x) << ", where x = " << x << endl;

		if (config.debug) {
			cout << population; // выводим текущую популяцию
			cout << endl;
		}

		double currBest = bestEntity.GetScore(); // получаем оценку приспособленности

		// если не нулевая эпоха и значение изменилось недостаточно сильно
		if (epoch > 0 && (fabs(prevBest - currBest) < config.qualityEpsilon)) {
			nochangingIterations++; // увеличиваем счётчик эпох без изменений

			// если превышен лимит без изменений
			if (nochangingIterations >= config.maxValuelessEpochs) {
				cout << "Score has not improved over " << nochangingIterations << " epoches" << endl; // сообщаем об этом
				break; // и выходим
			}

			cout << "Score has not improved, (iteration: " << nochangingIterations << ")" << endl;
		}
		else {
			nochangingIterations = 0;
		}

		prevBest = currBest; // обновляем предыдущее лучшее значение

		population.Selection(config); // запускаем селекцию
		population.Crossbreeding(config); // запускаем скрещивание
		population.Mutation(config); // запускаем мутацию
	}
}