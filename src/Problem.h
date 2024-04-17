#pragma once

#include <vector>
#include <memory>

/**
* @brief Namespace for the problem implementation
* @details This namespace contains all the abstract classes needed to define a problem and use it in a heuristic algorithm
*/
namespace problem {
	/**
	* @brief This class represents a problem nature (objective function, solution validity and completeness)
	* @tparam I Instance type
	* @tparam S Solution type
	* @tparam E Element type
	* @details It is an abstract class that must be implemented by the user
	*/
	template<class I, class S, typename E>
	class Problem {
	public:
		virtual double objectiveValue(S& solution) = 0;
		virtual bool isValid(I& instance, S& solution, std::shared_ptr<E> element) = 0;
		virtual bool isComplete(I& instance, S& solution) = 0;
	};

	/**
	* @brief This class represents a defined instance (elements and contraints and its values)
	* @tparam S Solution type
	* @tparam E Element type
	* @details It is an abstract class that must be implemented by the user
	*/
	template<class S, typename E>
	class Instance {
	public:
		virtual std::shared_ptr<S> initializeSolution() = 0;
	};

	/**
	* @brief This class represents a partial or complete solution for a problem (elements and its values)
	* @tparam E Element type
	* @details It is an abstract class that must be implemented by the user
	*/
	template <typename E>
	class Solution {
	public:
		virtual void addElementToSolution(std::shared_ptr<E> element) = 0;
		virtual void addElementToVisited(std::shared_ptr<E> element) = 0;
		virtual double getObjectiveValue() = 0;
		virtual bool wasVisited(std::shared_ptr<E> element) = 0;
		virtual std::vector<std::shared_ptr<E>> getSolution() = 0;
		virtual std::vector<std::shared_ptr<E>> getVisited() = 0;
	};
}