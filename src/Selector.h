#pragma once
#include <map>
#include <random>
#include "Problem.h"

namespace selection {
	class Selector
	{
	protected:
		std::default_random_engine generator;
	public:
		virtual void initialize(problem::Instance& instance, problem::SolutionPtr solution) = 0;
		virtual void updateProbabilitiesInternal(problem::Instance& instance, problem::SolutionPtr solution) = 0;
		virtual void updateProbabilitiesIteration(problem::Instance& instance, problem::SolutionPtr solution) = 0;
		virtual problem::ElementPtr selectElement(problem::Instance& instance, problem::SolutionPtr solution) = 0;
	};

	using SelectorPtr = std::shared_ptr<Selector>;
};

