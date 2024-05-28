#pragma once
#include <map>
#include <random>
#include "Problem.h"

namespace selection {
	class Selector
	{
	protected:
		std::map<problem::ElementPtr, double> probabilities;
	public:
		virtual void updateProbabilitiesInternal(problem::Instance& instance, problem::SolutionPtr solution) = 0;
		virtual void updateProbabilitiesIteration(problem::Instance& instance, problem::SolutionPtr solution) = 0;
		virtual problem::ElementPtr selectElement(problem::Instance& instance, problem::SolutionPtr solution) = 0;
	};

	using SelectorPtr = std::shared_ptr<Selector>;
};

