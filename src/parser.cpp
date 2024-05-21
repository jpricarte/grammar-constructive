#include "parser.h"
#include <fstream>
#include <cassert>
#include <random>

using namespace std;
using namespace nlohmann;

void AlgorithmConfiguration::readConfiguration(std::string filename)
{
	ifstream file(filename);
	this->configuration = json::parse(file);

	parseAlgorithmType(this->configuration);
}

void AlgorithmConfiguration::parseAlgorithmType(json& algorithmConfig)
{
	assert(algorithmConfig.contains("type"));

	if (algorithmConfig["type"] == "greedy")
	{
		parseGreedy(algorithmConfig);
		this->algorithm = this->baseAlgorithm;
	}
	else if (algorithmConfig["type"] == "beamsearch")
	{
		parseBeamsearch(algorithmConfig);
		this->algorithm = this->baseAlgorithm;
	}
	else if (algorithmConfig["type"] == "independent")
	{
		parseIndependent(algorithmConfig);
	}
	else assert(false);
}

problem::SolutionPtr AlgorithmConfiguration::run(problem::Problem& problem, problem::Instance& instance)
{
	return algorithm(problem, instance);
}

void AlgorithmConfiguration::parseGreedy(json& algorithmConfig)
{
    assert(algorithmConfig.contains("priority"));
	parsePriority(algorithmConfig);

	this->baseAlgorithm = [&](problem::Problem& p, problem::Instance& i) {
		return ConstructiveAlgorithm::greedyAlgorithm(p, i, this->elementselection);
	};
}

void AlgorithmConfiguration::parseBeamsearch(json& algorithmConfig)
{
	assert(algorithmConfig.contains("priority"));
	parsePriority(algorithmConfig);

	beamParams = shared_ptr<BeamSearchParams>(new BeamSearchParams());
	if (algorithmConfig.contains("beam-width"))
	{
		beamParams->beamWidth = this->configuration["beam-width"];
	}
	else
	{
		beamParams->beamWidth = 1;
	}

	if (this->configuration.contains("expansion-width"))
	{
		beamParams->expansionWidth = this->configuration["expansion-width"];
	}
	else
	{
		beamParams->expansionWidth = 1;
	}

	this->baseAlgorithm = [&](problem::Problem& p, problem::Instance& i) {
		return ConstructiveAlgorithm::beamsearchAlgorithm(p, i, this->elementselection, beamParams->beamWidth, beamParams->expansionWidth);
	};
}

void AlgorithmConfiguration::parseIndependent(json& algorithmConfig)
{
	assert(this->configuration.contains("internal-algorithm"));
	assert(this->configuration.contains("stop"));

	parseAlgorithmType(this->configuration["internal-algorithm"]);
	parseStopCriteria(this->configuration["stop"]);
	
	this->algorithm = [&](problem::Problem& p, problem::Instance& i) {
		return ConstructiveAlgorithm::multistartAlgorithmMaxIterations(p, i, this->baseAlgorithm, this->stopCriteria->maxIterations);
	};
}

void AlgorithmConfiguration::parsePriority(json& algorithmConfig)
{
	auto priorityConfig = algorithmConfig["priority"];

	double alpha;
	if (priorityConfig.contains("alpha-value"))
	{
		alpha = priorityConfig["alpha-value"];
	}
	else
	{
		alpha = 1; // Always select the best element
	}

	int kValue;
	if (priorityConfig.contains("k-value"))
	{
		kValue = priorityConfig["k-value"];
	}
	else
	{
		kValue = 0; // Select between all elements
	}

	string distribution = "";
	if (priorityConfig.contains("distribution"))
	{
		distribution = priorityConfig["distribution"];
	}

	if (distribution == "uniform" or distribution.empty())
	{
		this->random_distribuiton = [&](int max) -> int {
			uniform_int_distribution<int> distribution(0, max);
			return distribution(generator);
		};
	}
	else assert(false);

	this->elementselection = [&](problem::Instance& i, problem::SolutionPtr s) -> problem::ElementPtr {
		return ConstructiveAlgorithm::getElementRandomSelection(i, s, alpha, kValue, this->random_distribuiton);
	};
}

void AlgorithmConfiguration::parseStopCriteria(json& algorithmConfig)
{
	stopCriteria = shared_ptr<StopCriteria>(new StopCriteria());
	if (algorithmConfig.contains("max-iterations"))
	{
		stopCriteria->maxIterations = algorithmConfig["max-iterations"];
	}
	else
	{
		stopCriteria->maxIterations = 0;
	}

	if (algorithmConfig.contains("max-no-improvement-iterations"))
	{
		stopCriteria->maxNoImprovementIterations = algorithmConfig["max-no-improvement-iterations"];
	}
	else
	{
		stopCriteria->maxNoImprovementIterations = 0;
	}

	if (stopCriteria->maxIterations == 0 and stopCriteria->maxNoImprovementIterations == 0)
	{
		assert(false);
	}
}