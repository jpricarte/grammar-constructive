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

	/**
	* @brief This class represents a partial or complete solution for a problem (elements and its values)
	* @tparam E Element type
	* @details It is an abstract class that must be implemented by the user
	*/

	class Solution {
	public:
		Solution() = default;
		virtual void addElementToSolution(std::shared_ptr<Element> element) = 0;
		virtual void addElementToVisited(std::shared_ptr<Element> element) = 0;
		virtual double getObjectiveValue() = 0;
		virtual bool wasVisited(std::shared_ptr<Element> element) = 0;
		virtual std::vector<std::shared_ptr<Element>> getSolution() = 0;
		virtual std::shared_ptr<Solution> clone() = 0;
	};

	/**
	* @brief This class represents a defined instance (elements and contraints and its values)
	* @tparam E Element type
	* @details It is an abstract class that must be implemented by the user
	*/
	class Instance {
	public:
		virtual std::shared_ptr<Solution> initializeSolution() = 0;
		virtual std::vector<std::shared_ptr<Element>> getCandidatesElements(std::shared_ptr<Solution> solution) = 0;
	};

	/**
	* @brief This class represents a problem nature (objective function, solution validity and completeness)
	* @tparam E Element type
	* @details It is an abstract class that must be implemented by the user
	*/
	class Problem {
	public:
		virtual double objectiveValue(std::shared_ptr<Solution> solution) = 0;
		virtual bool isValid(Instance& instance, std::shared_ptr<Solution> solution, std::shared_ptr<Element> element) = 0;
		virtual bool isComplete(Instance& instance, std::shared_ptr<Solution> solution) = 0;
		virtual bool elementCompleteSolution(Instance& instance, std::shared_ptr<Solution> solution, std::shared_ptr<Element> element) = 0;
	};
}