#pragma once

#include <map>

#include "Selector.h"

namespace selection {

	template <typename T>
	class GreedySelector : public Selector<T>
	{
	public:
		GreedySelector() = default;
		inline void updateProbabilitiesInternal(problem::Instance& instance, problem::SolutionPtr solution) override { /*Do nothing*/ };

		inline void updateProbabilitiesIteration(problem::Instance& instance, problem::SolutionPtr solution) override { /*Do nothing*/ };

		T selectElement(problem::Instance& instance, problem::SolutionPtr solution) override;
	};

	template <typename T>
	class RandomSelector : public Selector<T>
	{
	private:
		int kValue;
		double alphaValue;
		std::uniform_int_distribution<int> distribution;

	public:
		inline RandomSelector(int kValue, double alphaValue) : kValue(kValue), alphaValue(alphaValue) {};

		inline void updateProbabilitiesInternal(problem::Instance& instance, problem::SolutionPtr solution) override { /*Do nothing*/ };

		inline void updateProbabilitiesIteration(problem::Instance& instance, problem::SolutionPtr solution) override { /*Do nothing*/ };

		T selectElement(problem::Instance& instance, problem::SolutionPtr solution) override;
	};
};	