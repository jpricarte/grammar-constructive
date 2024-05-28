#pragma once
#include <map>
#include <random>
#include "Problem.h"

namespace selection {
	template <typename T>
	class Selector
	{
	protected:
		std::map<T, double> probabilities;
		std::default_random_engine generator;
	public:
		virtual void updateProbabilitiesInternal(problem::Instance& instance, problem::SolutionPtr solution) = 0;
		virtual void updateProbabilitiesIteration(problem::Instance& instance, problem::SolutionPtr solution) = 0;
		virtual T selectElement(problem::Instance& instance, problem::SolutionPtr solution) = 0;
	};

	template <typename T>
	using SelectorPtr = std::shared_ptr<Selector<T>>;
};

