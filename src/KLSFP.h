#pragma once

#include "structs/graph.hpp"
#include "structs/union-find.hpp"
#include "Problem.h"
#include <vector>
#include <map>

class KLSFElement : public problem::Element
{
public:
	unsigned int color;
	std::shared_ptr<std::vector<Edge>> edgeList;
	inline KLSFElement(unsigned int color, std::shared_ptr<std::vector<Edge>> edgeList) : color(color), edgeList(edgeList) {};

	bool operator<(const KLSFElement& other) {
		// The element with the most edges is the one that will be choosed
		return edgeList->size() > other.edgeList->size();
	};
};
using KLSFElementPtr = std::shared_ptr<KLSFElement>;

class KLSFSolution : public problem::Solution
{
	unsigned int numColors; // The number of used colors (Stopping condition)
	UnionFind components; // Union-Find structure to keep track of the components
	std::map<problem::ElementPtr, double> elementQuality; // The quality of each element

public:
	KLSFSolution(problem::Instance& instance);
	KLSFSolution(KLSFSolution& other);

	void addElementToSolution(problem::ElementPtr element) override;
	double getElementQuality(problem::ElementPtr element) override;
	double getObjectiveValue() override;
	problem::SolutionPtr clone() override;
	std::vector<problem::ElementPtr> getSolution() override;

	int getVisistedSize() override;
	inline unsigned int getNumColors() { return numColors; };
	inline UnionFind getComponents() { return components; };
	void printGraph();
};

class KLSFInstance : public problem::Instance
{
	Graph &graph;
	std::vector<problem::ElementPtr> colors;
public:
	KLSFInstance(Graph &graph);
	problem::SolutionPtr initializeSolution() override;
	std::vector<problem::ElementPtr> getCandidatesElements(problem::SolutionPtr solution) override;
	bool isValid(problem::SolutionPtr solution, problem::ElementPtr element) override;
	bool isComplete(problem::SolutionPtr solution) override;
	Graph& getGraph();
	std::vector<problem::ElementPtr> getColors();

};

class KLSFProblem : public problem::Problem
{
public:
	double objectiveValue(problem::SolutionPtr solution) override;
	double objectiveValue(problem::SolutionPtr solution, problem::ElementPtr element);
	bool isValid(problem::Instance& instance, problem::SolutionPtr solution, problem::ElementPtr element) override;
	bool isComplete(problem::Instance& instance, problem::SolutionPtr solution) override;
};

