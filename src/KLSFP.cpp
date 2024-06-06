#include "KLSFP.h"
#include <random>

using namespace std;

KLSFSolution::KLSFSolution(problem::Instance& instance)
{
	auto klsfInstance = (KLSFInstance&)instance;
	this->numColors = 0;
	this->components = UnionFind(klsfInstance.getGraph().nNodes);
	// Initialize the number of colors
	this->candidates.reserve(klsfInstance.getGraph().nColors);
	this->solution.reserve(klsfInstance.getGraph().nMaxColors);

	// Initialize elements
	for (auto element : klsfInstance.getColors())
	{
		this->candidates.push_back(element);
	}
	this->firstVisited = this->candidates.end();
}

KLSFSolution::KLSFSolution(KLSFSolution& other)
{
	this->numColors = other.numColors;
	this->components = other.components;
	this->candidates = other.candidates;
	this->solution = other.solution;
	this->firstVisited = this->candidates.end() - other.getVisistedSize();
}

void KLSFSolution::addElementToSolution(problem::ElementPtr element)
{
	// Add the color to the solution
	this->solution.push_back(element);
	// Update the number of colors
	this->numColors++;
	// Update the connected components
	auto klsfElement = static_pointer_cast<KLSFElement>(element);
	auto edges = *(klsfElement->edgeList);
	for (auto &[u, v] : edges)
	{
		this->components.unite(u, v);
	}
}

void KLSFSolution::addElementToVisited(problem::ElementPtr element)
{
	// Send the colors to the visited set
	// TODO: create a alternative, receiving the iterator
	auto it = std::find(this->candidates.begin(), this->candidates.end(), element);
	std::rotate(it, it + 1, this->firstVisited);
	this->firstVisited--;
}

void KLSFSolution::addElementToIterationOptions(problem::ElementPtr element)
{
	// Add the color to the iteration options vector
	this->options.insert(element);
}

double KLSFSolution::getElementQuality(problem::ElementPtr element)
{
	// Return the number of edges or connected componets
	auto klsfElement = static_pointer_cast<KLSFElement>(element);
	auto auxUF = UnionFind(this->components);
	auto edges = *(klsfElement->edgeList);
	for (Edge edge : edges)
	{
		auxUF.unite(edge.first, edge.second);
	}
	return (double) auxUF.numComponents;
}

bool KLSFSolution::wasVisited(problem::ElementPtr element)
{
	// Return true if the color was visited (added to solution or doesn't reduce the number of connected components)
	return false;
}

void KLSFSolution::cleanIterationOptions()
{
	// Clear the iteration options vector
	this->options.clear();
}

double KLSFSolution::getObjectiveValue()
{
	// Returns the number of connected components in the solution
	return this->components.numComponents;
}

problem::SolutionPtr KLSFSolution::clone()
{
	// TBD what to do
	return make_shared<KLSFSolution>(*this);
}

std::vector<problem::ElementPtr> KLSFSolution::getCandidatesElements()
{
	// return from the first visited to the end
	return std::vector<problem::ElementPtr>(this->candidates.begin(), this->firstVisited);
}

std::vector<problem::ElementPtr> KLSFSolution::getSolution()
{
	// Return the colors in the solution
	return this->solution;
}

std::set<problem::ElementPtr> KLSFSolution::getIterationOptions()
{
	// Returns the options saved to be added in the option
	return this->options;
}

int KLSFSolution::getVisistedSize()
{
	// get number of elements from the first visited to the end
	return std::distance(this->firstVisited, this->candidates.end());
}

void KLSFSolution::printGraph()
{
	for(auto element : this->solution)
	{
		auto klsfElement = static_pointer_cast<KLSFElement>(element);
		auto edges = *(klsfElement->edgeList);
		for(auto &[u, v] : edges)
		{
			cout << u << " " << v << endl;
		}
	}
}


KLSFInstance::KLSFInstance(Graph& graph) : graph(graph)
{
	// Initialize the number of colors
	this->colors.reserve(graph.nColors);

	// Initialize elements
	for (unsigned i = 0; i < graph.nColors; i++)
	{
		this->colors.push_back(std::make_shared<KLSFElement>(i, std::make_shared<std::vector<Edge>>(graph.ce[i])));
	}
}

problem::SolutionPtr KLSFInstance::initializeSolution()
{
	return std::make_shared<KLSFSolution>(*this);
}

std::vector<problem::ElementPtr> KLSFInstance::getCandidatesElements(problem::SolutionPtr solution)
{
	// Return from the begin to the first visited
	auto klsfSolution = static_pointer_cast<KLSFSolution>(solution);
	return klsfSolution->getCandidatesElements();
}

Graph& KLSFInstance::getGraph()
{
	return this->graph;
}

std::vector<problem::ElementPtr> KLSFInstance::getColors()
{
	return this->colors;
}

double KLSFProblem::objectiveValue(problem::SolutionPtr solution)
{
	// Return the number of connected components in the solution
	return solution->getObjectiveValue();
}

double KLSFProblem::objectiveValue(problem::SolutionPtr solution, problem::ElementPtr element)
{
	auto klsfSolution = static_pointer_cast <KLSFSolution>(solution);
	UnionFind auxUF(klsfSolution->getComponents());

	auto klsfElement = static_pointer_cast<KLSFElement>(element);
	auto edges = *(klsfElement->edgeList);
	for (Edge edge : edges)
	{
		auxUF.unite(edge.first, edge.second);
	}
	return (double) auxUF.numComponents;
}

bool KLSFProblem::isValid(problem::Instance& instance, problem::SolutionPtr solution, problem::ElementPtr element)
{
	auto klsfInstace = (KLSFInstance&)instance;
	auto klsfSolution = static_pointer_cast<KLSFSolution>(solution);
	// Returns true if the element can be added to the solution
	return klsfInstace.getGraph().nMaxColors > klsfSolution->getNumColors();
}

bool KLSFProblem::isComplete(problem::Instance& instance, problem::SolutionPtr solution)
{
	// Returns true if the solution contains min(k, numColors) colors
	auto klsfInstace = (KLSFInstance&)instance;
	auto klsfSolution = static_pointer_cast<KLSFSolution>(solution);
	// Returns true if an element can't be added to the solution
	return klsfInstace.getGraph().nMaxColors <= klsfSolution->getNumColors();
}