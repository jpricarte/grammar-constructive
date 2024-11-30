#include "FSSP.h"
#include "Problem.h"
#include <boost/smart_ptr/shared_ptr.hpp>
#include <memory>

/* Solution */

const uint16_t FSSSolution::findJiFromJobNumber(const int& jobNumber)
{
    for (auto i=0; i<bsSolution.pi.size(); i++) {
        if (bsSolution.pi[i]==jobNumber)
        {
            return i;
        }
    }
    return -1;
}

FSSSolution::FSSSolution(FSSSolution& other) : bsSolution(other.bsSolution), parentInstance(other.parentInstance), objectiveValue(other.objectiveValue)
{
    this->candidates = other.candidates;
    this->solution = other.solution;
    this->firstVisited = this->candidates.end() - other.getVisistedSize();
}

FSSSolution::FSSSolution(problem::Instance& parentInstance) : problem::Solution(), parentInstance(parentInstance), bsSolution(((FSSInstance&)parentInstance).getBaseInstance()) {
    this->objectiveValue = -1;
    auto fssInstance = (FSSInstance&)parentInstance;
    this->solution = std::vector<problem::ElementPtr>();
    this->candidates = std::vector<problem::ElementPtr>();
    this->candidates.reserve(fssInstance.getNumJobs());
    for (auto element : fssInstance.getJobs())
    {
        candidates.push_back(element);
    }
    this->firstVisited = this->candidates.end();
}

std::vector<problem::ElementPtr> FSSSolution::getSolution() {
    return this->solution;
}

void FSSSolution::addElementToSolution(problem::ElementPtr element) {
    this->solution.push_back(element);
    this->elementsQuality.clear();
    auto fssElement = static_pointer_cast<FSSElement>(element);
    std::sort(this->candidates.begin(), this->firstVisited, [this](problem::ElementPtr a, problem::ElementPtr b) {
        return this->getElementQuality(a) < this->getElementQuality(b);
    });

    auto fssInstance = (FSSInstance&) this->parentInstance;
    //TODO: mapear jobNumber com ji do bsSolution
    auto ji = findJiFromJobNumber(fssElement->jobNumber);
    gOperationCounter++;
    this->bsSolution.addi(ji, fssInstance.getBaseInstance());
}

double FSSSolution::getElementQuality(problem::ElementPtr element) {
    if (this->elementsQuality.count(element))
        return this->elementsQuality[element];

    gOperationCounter++;
    auto fssElement = std::static_pointer_cast<FSSElement>(element);
    auto ji = findJiFromJobNumber(fssElement->jobNumber);
    auto fssInstance = (FSSInstance&)this->parentInstance;
    auto bsInstance = fssInstance.getBaseInstance();
    double quality;
    if (this->solution.size()==0)
        quality = this->bsSolution.Ivalue(ji, bsInstance);
    else
        quality = this->bsSolution.Bvalue(ji, bsInstance);
    this->elementsQuality[element] = quality;
    return quality;
}

double FSSSolution::getObjectiveValue() {
    if (this->objectiveValue < 0) {
        gOperationCounter++;
        auto bsInstance = ((FSSInstance&)parentInstance).getBaseInstance();
        auto ns = PFSSolution(bsInstance);
        ns.pi = this->bsSolution.pi;
        ns.computeFlowtime(bsInstance);
        this->objectiveValue = ns.ms;
    }
    return this->objectiveValue;
}

std::shared_ptr<problem::Solution> FSSSolution::clone() {
    return std::make_shared<FSSSolution>(*this);
}

/* Instance */

FSSInstance::FSSInstance(InstanceFSSP bsInstance) : bsInstance(bsInstance) {
    this->jobs = {};
    this->numJobs = bsInstance.n;
    this->numMachines = bsInstance.m;
    for (int i=0; i<this->numJobs; i++) {
        jobs.push_back(make_shared<FSSElement>(i+1));
    }
}

problem::SolutionPtr FSSInstance::initializeSolution() {
    return make_shared<FSSSolution>(*this);
}

std::vector<problem::ElementPtr> FSSInstance::getCandidatesElements(problem::SolutionPtr solution) {
    return solution->getCandidatesElements();
}

bool FSSInstance::isValid(problem::SolutionPtr solution, problem::ElementPtr element) {
    return true;
}

bool FSSInstance::isComplete(problem::SolutionPtr solution) {
    auto solutionSize = solution->getSolutionSize();
    return solutionSize == numJobs;
}

/* Problem */
double FSSProblem::objectiveValue(problem::SolutionPtr solution) {
    auto fssSolution = std::static_pointer_cast<FSSSolution>(solution);
    return fssSolution->getObjectiveValue();
}

double FSSProblem::objectiveValue(problem::SolutionPtr solution, problem::ElementPtr element) {
    auto fssSolution = static_pointer_cast<FSSSolution>(solution);
    auto fssElement = static_pointer_cast<FSSElement>(element);

    return fssSolution->getElementQuality(fssElement);
}

bool FSSProblem::isValid(problem::Instance& instance, problem::SolutionPtr solution, problem::ElementPtr element){
    return instance.isValid(solution, element);
}

bool FSSProblem::isComplete(problem::Instance& instance, problem::SolutionPtr solution){
    return instance.isComplete(solution);
}
