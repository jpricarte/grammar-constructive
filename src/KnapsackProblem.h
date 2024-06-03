#pragma once

#include "Problem.h"
#include <vector>
#include <set>
#include <memory>
#include <string>
#include <algorithm>

struct KnapsackElement : public problem::Element {
public:
	double value;
	int weight;

	KnapsackElement(double value, int weight) : value(value), weight(weight) {};

	bool operator<(const KnapsackElement& other) {
		// Inverted because is a maximization problem
		return -(value/weight) < -(other.value/other.weight);
	};
};


class KnapsackSolution : public problem::Solution {
	std::vector<problem::ElementPtr> solution;
	std::vector<problem::ElementPtr> candidates;
	std::vector<problem::ElementPtr>::iterator firstVisited;
	std::set<problem::ElementPtr> options;

	double currentValue;
	int currentWeight;

public:

	KnapsackSolution(size_t numElements);

	KnapsackSolution(problem::Instance& instance);

	void addElementToSolution(problem::ElementPtr element) override;

	void addElementToVisited(problem::ElementPtr element) override;

	void addElementToIterationOptions(problem::ElementPtr element) override;

	double getElementQuality(problem::ElementPtr element) override;

	bool wasVisited(problem::ElementPtr element) override;

	void cleanIterationOptions() override;

	double getObjectiveValue() override;

	problem::SolutionPtr clone() override;

	std::vector<problem::ElementPtr> getCandidatesElements() override;

	std::vector<problem::ElementPtr> getSolution() override;

	std::set<problem::ElementPtr> getIterationOptions() override;

	int getCurrentWeight() const;

	std::vector<problem::ElementPtr> getVisited();

	int getVisistedSize();

	inline void setCurrentValue(double value) { this->currentValue = value;  };

	inline void setCurrentWeight(int weight) { this->currentWeight = weight; };
};

class KnapsackInstance : public problem::Instance {
	std::vector<problem::ElementPtr> elements;
	int capacity;

public:
	KnapsackInstance() : capacity(0) {};
	
	KnapsackInstance(std::string filename);

	int getCapacity() const;

	std::vector<problem::ElementPtr> getElements() const;

	void sortItems();

	problem::SolutionPtr initializeSolution() override;

	std::vector<problem::ElementPtr> getCandidatesElements(problem::SolutionPtr solution) override;
};

class KnapsackProblem : public problem::Problem {
public:
	KnapsackProblem() {};

	double objectiveValue(problem::SolutionPtr solution) override;

	double objectiveValue(problem::SolutionPtr solution, problem::ElementPtr element) override;

	bool isValid(problem::Instance& instance, problem::SolutionPtr solution, problem::ElementPtr element) override;

	bool isComplete(problem::Instance& instance, problem::SolutionPtr solution) override;
};