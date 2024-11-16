#include <vector>

#include "../submodule/fssp/beam_search.hpp"
#include "../submodule/fssp/instance.hpp"
#include "Problem.h"


struct FSSElement : public problem::Element {
    int jobNumber;

    inline FSSElement(int jobNumber) :jobNumber(jobNumber) {};
};

class FSSSolution : public problem::Solution {
private:
    const uint16_t findJiFromJobNumber(const int& jobNumber);
public:
    beamnode bsSolution;
    problem::Instance& parentInstance;
    int objectiveValue;

    FSSSolution(FSSSolution& other);
    FSSSolution(problem::Instance& parentInstance);

    std::vector<problem::ElementPtr> getSolution() override;
    void addElementToSolution(problem::ElementPtr element) override;
    double getElementQuality(problem::ElementPtr element) override;
    double getObjectiveValue() override;
    std::shared_ptr<problem::Solution> clone() override;
};

class FSSInstance : public problem::Instance {
private:
    InstanceFSSP bsInstance;
    int numJobs;
    int numMachines;
    std::vector<problem::ElementPtr> jobs;
public:
    FSSInstance(InstanceFSSP bsInstance);
    problem::SolutionPtr initializeSolution() override;
    std::vector<problem::ElementPtr> getCandidatesElements(problem::SolutionPtr solution) override;
    bool isValid(problem::SolutionPtr solution, problem::ElementPtr element) override;
    bool isComplete(problem::SolutionPtr solution) override;

    inline InstanceFSSP getBaseInstance() { return bsInstance; };
    inline int getNumJobs() { return numJobs; };
    inline std::vector<problem::ElementPtr> getJobs() { return jobs; };
};

class FSSProblem : public problem::Problem {
    double objectiveValue(problem::SolutionPtr solution) override;
    bool isValid(problem::Instance& instance, problem::SolutionPtr solution, problem::ElementPtr element) override;
    bool isComplete(problem::Instance& instance, problem::SolutionPtr solution) override;
};
