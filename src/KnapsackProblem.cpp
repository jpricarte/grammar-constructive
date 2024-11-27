#include "KnapsackProblem.h"

#include <fstream>
#include <algorithm>

KnapsackSolution::KnapsackSolution(size_t numElements) : currentValue(0), currentWeight(0)
{
    this->solution = std::vector<problem::ElementPtr>();
    this->solution.reserve(numElements);
    this->candidates = std::vector<problem::ElementPtr>();
    this->candidates.reserve(numElements);
    this->options = std::set<problem::ElementPtr>();
    this->currentValue = 0;
    this->currentWeight = 0;
}

KnapsackSolution::KnapsackSolution(problem::Instance& instance)
{
    auto knapsackInstance = (KnapsackInstance&) instance;
    this->solution = std::vector<problem::ElementPtr>();
    this->solution.reserve(knapsackInstance.getElements().size());
    this->candidates = std::vector<problem::ElementPtr>();
    for (auto element : knapsackInstance.getElements())
    {
        this->candidates.push_back(element);
    }
    this->options = std::set<problem::ElementPtr>();
    this->firstVisited = this->candidates.end();
    this->currentValue = 0;
    this->currentWeight = 0;
}

int KnapsackSolution::getCurrentWeight () const
{
    return this->currentWeight;
}

void KnapsackSolution::addElementToSolution(problem::ElementPtr element)
{
    auto knapsackElement = std::dynamic_pointer_cast<KnapsackElement>(element);
    this->solution.push_back(element);
    this->currentValue += knapsackElement->value;
    this->currentWeight += knapsackElement->weight;
}

double KnapsackSolution::getElementQuality(problem::ElementPtr element)
{
    return 1 / std::static_pointer_cast<KnapsackElement>(element)->weight;
}

double KnapsackSolution::getObjectiveValue()
{
    return -this->currentValue;
}

int KnapsackSolution::getVisistedSize()
{
    // get number of elements from the first visited to the end
    return std::distance(this->firstVisited, this->candidates.end());
}

std::vector<problem::ElementPtr> KnapsackSolution::getSolution()
{
    return this->solution;
}

problem::SolutionPtr KnapsackSolution::clone()
{
    auto newSolution = std::make_shared<KnapsackSolution>(this->solution.size());
    for (auto element : this->solution)
    {
        newSolution->addElementToSolution(element);
    }
    for (auto element : this->candidates)
    {
        newSolution->candidates.push_back(element);
    }
    newSolution->firstVisited = newSolution->candidates.end() - this->getVisistedSize();
    newSolution->setCurrentValue(this->currentValue);
    newSolution->setCurrentWeight(this->currentWeight);
    return newSolution;
}


KnapsackInstance::KnapsackInstance(std::string filepath)
{
    this->capacity = 0;
    std::ifstream file(filepath);
    std::string line;
    if (file.is_open()) {
        // read empty line
        getline(file, line);
        // read number of elements
        int numElements;
        file >> numElements;
        // Set size of elements
        this->elements.reserve(numElements);
        // read capacity
        file >> this->capacity;
        // read empty line
        getline(file, line);
        while (not file.eof()) {
            if (file.peek() == '#') {
                getline(file, line);
                continue;
            }
            double value;
            int weight;
            file >> value >> weight;
            auto element = std::make_shared<KnapsackElement>(value, weight);
            this->elements.push_back(element);
        }
    }
    sortItems();
}

int KnapsackInstance::getCapacity() const
{
    return this->capacity;
}

std::vector<problem::ElementPtr> KnapsackInstance::getElements() const
{
    return this->elements;
}

void KnapsackInstance::sortItems()
{
    std::sort(this->elements.begin(), this->elements.end(), [](problem::ElementPtr a, problem::ElementPtr b) {
        auto elementA = std::dynamic_pointer_cast<KnapsackElement>(a);
        auto elementB = std::dynamic_pointer_cast<KnapsackElement>(b);
        return *elementA < *elementB;
    });
}

problem::SolutionPtr KnapsackInstance::initializeSolution()
{
    return std::make_shared<KnapsackSolution>(*this);
}


std::vector<problem::ElementPtr> KnapsackInstance::getCandidatesElements(problem::SolutionPtr solution)
{
    // Get the candidates from the solution
    auto knapsackSolution = std::dynamic_pointer_cast<KnapsackSolution>(solution);
    return knapsackSolution->getCandidatesElements();
}

bool KnapsackInstance::isValid(problem::SolutionPtr solution, problem::ElementPtr element)
{
    auto knapsackElement = std::static_pointer_cast<KnapsackElement>(element);
    auto knapsackSolution = std::static_pointer_cast<KnapsackSolution>(solution);

    return ((knapsackSolution->getCurrentWeight() + knapsackElement->weight) <= this->getCapacity());
}

bool KnapsackInstance::isComplete(problem::SolutionPtr solution)
{
    auto knapSackSolution = std::dynamic_pointer_cast<KnapsackSolution>(solution);
    return this->getElements().size() <= knapSackSolution->getVisistedSize();
}


double KnapsackProblem::objectiveValue(problem::SolutionPtr solution)
{
    double value = 0;
    for (auto& genericElement : solution->getSolution())
    {
        auto element = std::static_pointer_cast<KnapsackElement>(genericElement);
        value += element->value;
    }

    return -value;
}

double KnapsackProblem::objectiveValue(problem::SolutionPtr solution, problem::ElementPtr element)
{
    auto knapsackElement = std::static_pointer_cast<KnapsackElement>(element);
    return solution->getObjectiveValue() + -(knapsackElement->value);
}

bool KnapsackProblem::isValid(problem::Instance& instance, problem::SolutionPtr solution, problem::ElementPtr element)
{
    auto knapsackElement = std::static_pointer_cast<KnapsackElement>(element);
    auto knapsackSolution = std::static_pointer_cast<KnapsackSolution>(solution);
    auto& knapsackInstance = (KnapsackInstance&) instance;

    return ((knapsackSolution->getCurrentWeight() + knapsackElement->weight) <= knapsackInstance.getCapacity());
}

bool KnapsackProblem::isComplete(problem::Instance& instance, problem::SolutionPtr solution)
{
    auto knapSackSolution = std::dynamic_pointer_cast<KnapsackSolution>(solution);
    auto& knapsackInstance = (KnapsackInstance&) instance;
    return knapsackInstance.getElements().size() <= knapSackSolution->getVisistedSize();
}
