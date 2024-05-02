#pragma once

#include <vector>
#include <memory>
#include <set>

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
		// Set the element as part of the solution
		virtual void addElementToSolution(ElementPtr element) = 0;
		// Set the element as visited (it should no be evaluated again)
		virtual void addElementToVisited(ElementPtr element) = 0;
		// Add a element to iteration options as a valid candidate, it will be used to evaluate multiple candidates in the same iteration
		virtual void addElementToIterationOptions(ElementPtr element) = 0;
		// Get the objective value of the solution (if is a maximization problem, return -value)
		virtual double getObjectiveValue() = 0;
		// Return if the element was visited
		virtual bool wasVisited(ElementPtr element) = 0;
		// Return the candidates elements to be added to the solution (must be valid candidates in the current state)
		virtual std::vector<ElementPtr> getCandidatesElements() = 0;
		// Return the elements that are part of the solution
		virtual std::vector<ElementPtr> getSolution() = 0;
		// It should work as a temporary memory, to store the possible candidates to be added in the solution, will be cleaned after each iteration
		virtual std::set<ElementPtr> getIterationOptions() = 0;
		// Clone the solution
		virtual std::shared_ptr<Solution> clone() = 0;
		// Clean options for the iteration
		virtual void cleanIterationOptions() = 0;
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
		virtual double objectiveValue(SolutionPtr solution, ElementPtr element) = 0;
		virtual bool isValid(Instance& instance, SolutionPtr solution, ElementPtr element) = 0;
		virtual bool isComplete(Instance& instance, SolutionPtr solution) = 0;
		virtual bool elementCompleteSolution(Instance& instance, SolutionPtr solution, ElementPtr element) = 0;
	};
}