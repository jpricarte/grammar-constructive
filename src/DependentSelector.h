#pragma once

#include "Selector.h"
#include "StaticSelector.h"
#include "Problem.h"
#include <map>
#include <vector>

namespace selection {
	class PheromoneSelector : public Selector
	{
		std::map<problem::ElementPtr, double> pheromones;
		std::vector<double> probabilities;
		double alpha; // Weight of pheromones
		double beta; // Weight of the quality of the element
		double evaporationRate; // Rate of evaporation of the pheromones


	public:
		inline PheromoneSelector(double alpha, double beta, double evaporationRate) : alpha(alpha), beta(beta), evaporationRate(evaporationRate) {};

		void initialize(problem::Instance& instance, problem::SolutionPtr solution) override;
		void updateProbabilitiesInternal(problem::Instance& instance, problem::SolutionPtr solution) override;
		void updateProbabilitiesIteration(problem::Instance& instance, problem::SolutionPtr solution) override;

		problem::ElementPtr selectElement(problem::Instance& instance, problem::SolutionPtr solution) override;
	};

	class PilotSelector : public Selector
	{
		int kValue;
		selection::SelectorPtr baseSelector;
	public:
		PilotSelector(int kValue);

		inline void initialize(problem::Instance& instance, problem::SolutionPtr solution) override {};
		inline void updateProbabilitiesInternal(problem::Instance& instance, problem::SolutionPtr solution) override {};
		inline void updateProbabilitiesIteration(problem::Instance& instance, problem::SolutionPtr solution) override {};

		problem::ElementPtr selectElement(problem::Instance& instance, problem::SolutionPtr solution) override;
	};
};


