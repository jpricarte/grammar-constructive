#pragma once

#include <functional>
#include <memory>
#include "simple/SimpleInstance.h"
#include "Problem.h"

namespace SimpleConstructiveAlgorithm {

	/**
	 * @brief Create a solution using a multistart algorithm with a maximum number of iterations
	 *
	 * @tparam T Type of the elements
	 * @param instance Instance of the problem
	 * @param selectionAlgorithm Function that selects the next element
	 */
	template<class T> double multistartAlgorithmMaxInterations(simple::Instance<T>& instance, std::function<T& (simple::Instance<T>&)> selectionAlgorithm, const int maxIterations)
	{
		double best_val = INFINITY;
	
		for (int i=0; i<maxIterations; i++)
		{
			double result = greedyAlgorithm<T>(instance, selectionAlgorithm);
			if (result < best_val) {
				best_val = result;
			}
			instance.resetInstance();
		}
		return best_val;
	}


	/**
	 * @brief Create a solution using a greedy algorithm
	 *
	 * @tparam T Type of the elements
	 * @param instance Instance of the problem
	 * @param selectionAlgorithm Function that selects the next element
	 */
	template<class T> double greedyAlgorithm(simple::Instance<T>& instance, std::function<T& (simple::Instance<T>&)> selectionAlgorithm)
	{
		// Stopping criterion
		while (not instance.shouldStop()) {
			// Choose next element
			T& choosedElement = selectionAlgorithm(instance);

			// Verify if is a valid choice 
			if (instance.validateChoice(choosedElement))
			{
				// Update state
				instance.addToSolution(choosedElement);
			}

			// Update candidates
			instance.updateAfterChoice(choosedElement);
		}
		return instance.objectiveValue();
	}

	/**
	 * @brief Get the best element of an instance at the moment
	 *
	 * @tparam T Type of the elements
	 * @param instance Instance of the problem
	 * @return T& Best element
	 */
	template<class T> T& getBestElement(simple::Instance<T>& instance)
	{
		// Sort elements based on < operator, been the first element the best
		instance.orderByComparator();

		// Return the first element on list
		return instance.getElements().front();
	}

	/**
	 * @brief Get a random element between the k best elements
	 *
	 * @tparam T Type of the elements
	 * @param instance Instance of the problem
	 * @param k the size of the window
	 * @return T& the selected element
	 */
	template<class T> T& getElementKSelection(simple::Instance<T>& instance, int k)
	{
		// Sort elements based on < operator, been the first element the best
		int validElementsWindowSize = instance.orderByComparator();
		int windowSize = k < validElementsWindowSize ? k : validElementsWindowSize;
		int index = rand() % windowSize;

		// Save the element on index
		auto& element = instance.getElements().at(index);

		return element;
	}

	/**
	 * @brief Get the best element with probability alpha
	 *
	 * @tparam T Type of the elements
	 * @param instance Instance of the problem
	 * @param alpha Probability of choosing the best element
	 * @return T& the selected element
	 */
	template<class T> T& getElementAlphaSelection(simple::Instance<T>& instance, double alpha)
	{
		// Sort elements based on < operator, been the first element the best
		int validElementsWindowSize = instance.orderByComparator();

		auto& element = instance.getElements().front();
		// Choose a random element from list with probability 1-alpha
		if (rand() % 100 < (1 - alpha) * 100) {
			int index = rand() % validElementsWindowSize;
			element = instance.getElements().at(index);
		}

		return element;
	}

}

namespace ConstructiveAlgorithm {

	template <class S, typename E>
	std::shared_ptr<E> getBestElement(problem::Instance<S, E>& instance, std::shared_ptr<problem::Solution<E>> solution)
	{
		auto elements = instance.getElements();
		auto bestElement = elements.front();
		for (auto element : elements)
		{
			if (solution.evaluateElement(element) < solution.evaluateElement(bestElement))
			{
				bestElement = element;
			}
		}
		return bestElement;
	}

	template <class I, class S, typename E>
	double greedyAlgorithm(problem::Problem<I, S, E>& problem, problem::Instance<S, E>& instance)
	{
		std::shared_ptr<problem::Solution<E>> solution = instance.initializeSolution();
		while(not problem.isComplete(instance, *solution))
		{
			auto choosedElement = getBestElement(instance, solution);
			if (solution->wasVisited(choosedElement))
			{
				solution->addElementToSolution(choosedElement);
			}
			solution->addElementToVisited(choosedElement);
		}
		return solution->getObjectiveValue();
	}
}