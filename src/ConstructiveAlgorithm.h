#pragma once

#include <functional>
#include "Instance.h"

namespace ConstructiveAlgorithm {
	/**
	 * @brief Create a solution using a greedy algorithm
	 *
	 * @tparam T Type of the elements
	 * @param instance Instance of the problem
	 * @param selectionAlgorithm Function that selects the next element
	 */
	template<class T> void greedyAlgorithm(Instance<T>& instance, std::function<T& (Instance<T>&)> selectionAlgorithm)
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
			instance.updateCandidates(choosedElement);
		}
	}

	/**
	 * @brief Get the best element of an instance at the moment
	 *
	 * @tparam T Type of the elements
	 * @param instance Instance of the problem
	 * @return T& Best element
	 */
	template<class T> T& getBestElement(Instance<T>& instance)
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
	template<class T> T& getElementKSelection(Instance<T>& instance, int k)
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
	template<class T> T& getElementAlphaSelection(Instance<T>& instance, double alpha)
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