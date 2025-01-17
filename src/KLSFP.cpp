#include "KLSFP.h"
#include <random>
#include "Problem.h"

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
    this->elementQuality = other.elementQuality;
}

void KLSFSolution::addElementToSolution(problem::ElementPtr element)
{
    Solution::addElementToSolution(element);
    // Increase operations global counter
    gOperationCounter++;
    // Update the number of colors
    this->numColors++;
    // Update the connected components
    auto klsfElement = static_pointer_cast<KLSFElement>(element);
    auto edges = *(klsfElement->edgeList);
    for (auto &[u, v] : edges)
    {
        this->components.unite(u, v);
    }
    // Sort the elements again
    elementQuality.clear();
    std::sort(this->candidates.begin(), this->firstVisited, [this](problem::ElementPtr a, problem::ElementPtr b) {
        return this->getElementQuality(a) < this->getElementQuality(b);
    });
}

double KLSFSolution::getElementQuality(problem::ElementPtr element)
{
    if (elementQuality.contains(element))
    {
        return elementQuality[element];
    }
    else
    {
        // Increase operations global counter
        gOperationCounter++;

        // Return the number of edges or connected componets
        auto klsfElement = static_pointer_cast<KLSFElement>(element);
        auto auxUF = UnionFind(this->components);
        auto edges = *(klsfElement->edgeList);
        for (Edge edge : edges)
        {
            auxUF.unite(edge.first, edge.second);
        }
        elementQuality[element] = (double) auxUF.numComponents;

        return (double) auxUF.numComponents;
    }
}

double KLSFSolution::getObjectiveValue()
{
    // Returns the number of connected components in the solution
    return this->components.numComponents;
}

problem::SolutionPtr KLSFSolution::clone()
{
    return make_shared<KLSFSolution>(*this);
}

std::vector<problem::ElementPtr> KLSFSolution::getSolution()
{
    // Return the colors in the solution
    return this->solution;
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
    auto result = solution->getCandidatesElements();
    return result;
}

bool KLSFInstance::isValid(problem::SolutionPtr solution, problem::ElementPtr element)
{
    auto klsfSolution = static_pointer_cast<KLSFSolution>(solution);
    // Returns true if the element can be added to the solution
    return this->getGraph().nMaxColors > klsfSolution->getNumColors();
}

bool KLSFInstance::isComplete(problem::SolutionPtr solution)
{
    // Returns true if the solution contains min(k, numColors) colors
    auto klsfSolution = static_pointer_cast<KLSFSolution>(solution);
    // Returns true if an element can't be added to the solution
    return this->getGraph().nMaxColors <= klsfSolution->getNumColors();
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
    // Increase operations global counter
    gOperationCounter++;

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
