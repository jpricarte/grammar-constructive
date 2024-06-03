#include "KLSFP.h"

using namespace std;


void KLSFSolution::addElementToSolution(problem::ElementPtr element)
{
	// Add the edges of the color to the solution
}

void KLSFSolution::addElementToVisited(problem::ElementPtr element)
{
	// Send the colors to the visited set
}

void KLSFSolution::addElementToIterationOptions(problem::ElementPtr element)
{
	// Add the color to the iteration options vector
}

double KLSFSolution::getElementQuality(problem::ElementPtr element)
{
	// Return the number of edges or connected componets (TBD)
}

bool KLSFSolution::wasVisited(problem::ElementPtr element)
{
	// Return true if the color was visited (added to solution or doesn't reduce the number of connected components)
	return false;
}

void KLSFSolution::cleanIterationOptions()
{
	// Clear the iteration options vector
}

double KLSFSolution::getObjectiveValue()
{
	// Returns the number of connected components in the solution
	return 0.0;
}

problem::SolutionPtr KLSFSolution::clone()
{
	// TBD what to do
	return problem::SolutionPtr();
}

std::vector<problem::ElementPtr> KLSFSolution::getCandidatesElements()
{
	// Return the colors that are not selected in the solution, sorted by the number of connected components that the graph will have if the color is added
	return std::vector<problem::ElementPtr>();
}

std::vector<problem::ElementPtr> KLSFSolution::getSolution()
{
	// Return the colors in the solution
	return std::vector<problem::ElementPtr>();
}

std::set<problem::ElementPtr> KLSFSolution::getIterationOptions()
{
	// Returns the options saved to be added in the option
	return std::set<problem::ElementPtr>();
}


problem::SolutionPtr KLSFInstance::initializeSolution()
{
	return std::make_shared<KLSFSolution>();
}

std::vector<problem::ElementPtr> KLSFInstance::getCandidatesElements(problem::SolutionPtr solution)
{
	// Return the colors that are not selected in the solution, sorted by the number of connected components that the graph will have if the color is added
	return std::vector<problem::ElementPtr>();
}

double KLSFProblem::objectiveValue(problem::SolutionPtr solution)
{
	// Return the number of connected components in the solution
	return 0.0;
}

double KLSFProblem::objectiveValue(problem::SolutionPtr solution, problem::ElementPtr element)
{
	// Return the number of connected components in the solution if the element is added
	return 0.0;
}

bool KLSFProblem::isValid(problem::Instance& instance, problem::SolutionPtr solution, problem::ElementPtr element)
{
	// Returns true if the element can be added to the solution
	return false;
}

bool KLSFProblem::isComplete(problem::Instance& instance, problem::SolutionPtr solution)
{
	// Returns true if the solution contains min(k, numColors) colors
	return false;
}