#pragma once

#include "structs/Graph.hpp"
#include "structs/union-find.hpp"
#include "Problem.h"
#include <vector>

class KLSFElement : public problem::Element
{
	std::shared_ptr<std::vector<Edge>> edgeList;
};

class KLSFSolution : public problem::Solution
{
	std::vector<problem::ElementPtr> usedColors;
	std::vector<problem::ElementPtr> allColors;
	unsigned int usedColors; // The number of used colors (Stopping condition)
	UnionFind components; // Union-Find structure to keep track of the components
public:
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
};

class KLSFInstance : public problem::Instance
{
	Graph &graph;
public:
	problem::SolutionPtr initializeSolution() override;
	std::vector<problem::ElementPtr> getCandidatesElements(problem::SolutionPtr solution) override;
};

class KLSFProblem : public problem::Problem
{
public:
	double objectiveValue(problem::SolutionPtr solution) override;
	double objectiveValue(problem::SolutionPtr solution, problem::ElementPtr element) override;
	bool isValid(problem::Instance& instance, problem::SolutionPtr solution, problem::ElementPtr element) override;
	bool isComplete(problem::Instance& instance, problem::SolutionPtr solution) override;
};

