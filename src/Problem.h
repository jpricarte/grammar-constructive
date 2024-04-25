#pragma once

#include <vector>
#include <memory>

/**
* @brief Namespace for the problem implementation
* @details This namespace contains all the abstract classes needed to define a problem and use it in a heuristic algorithm
*/
namespace problem {

	struct Element {
		virtual ~Element() = default;
	};
	using ElementPtr = std::shared_ptr<Element>;


	/**
	* @brief This class represents a partial or complete solution for a problem (elements and its values)
	* @tparam E Element type
	* @details It is an abstract class that must be implemented by the user
	*/

	class Solution {
	public:
		Solution() = default;
		virtual void addElementToSolution(ElementPtr element) = 0;
		virtual void addElementToVisited(ElementPtr element) = 0;
		virtual double getObjectiveValue() = 0;
		virtual bool wasVisited(ElementPtr element) = 0;
		virtual std::vector<ElementPtr> getSolution() = 0;
		virtual std::shared_ptr<Solution> clone() = 0;
	};
	using SolutionPtr = std::shared_ptr<Solution>;


	/**
	* @brief This class represents a defined instance (elements and contraints and its values)
	* @tparam E Element type
	* @details It is an abstract class that must be implemented by the user
	*/
	class Instance {
	public:
		virtual SolutionPtr initializeSolution() = 0;
		virtual std::vector<ElementPtr> getCandidatesElements(SolutionPtr solution) = 0;
	};

	/**
	* @brief This class represents a problem nature (objective function, solution validity and completeness)
	* @tparam E Element type
	* @details It is an abstract class that must be implemented by the user
	*/
	class Problem {
	public:
		virtual double objectiveValue(SolutionPtr solution) = 0;
		virtual bool isValid(Instance& instance, SolutionPtr solution, ElementPtr element) = 0;
		virtual bool isComplete(Instance& instance, SolutionPtr solution) = 0;
		virtual bool elementCompleteSolution(Instance& instance, SolutionPtr solution, ElementPtr element) = 0;
	};
}