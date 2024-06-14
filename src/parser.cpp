#include "parser.h"
#include "StaticSelector.h"
#include "DependentSelector.h"
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
	else if (algorithmConfig["type"] == "iterated")
	{
		parseInterated(algorithmConfig);
	}
	else assert(false);
}

problem::SolutionPtr AlgorithmConfiguration::run(problem::Problem& problem, problem::Instance& instance)
{
	return algorithm(problem, instance, elementSelector);
}

void AlgorithmConfiguration::parseGreedy(json& algorithmConfig)
{
    assert(algorithmConfig.contains("priority"));
	parsePriority(algorithmConfig);

	this->baseAlgorithm = [&](problem::Problem& p, problem::Instance& i, selection::SelectorPtr s) {
		return ConstructiveAlgorithm::greedyAlgorithm(p, i, s);
	};
}

void AlgorithmConfiguration::parseBeamsearch(json& algorithmConfig)
{
	assert(algorithmConfig.contains("priority"));
	parsePriority(algorithmConfig);

	beamParams = shared_ptr<BeamSearchParams>(new BeamSearchParams());
	if (algorithmConfig.contains("beam-width"))
	{
		int width = algorithmConfig["beam-width"];
		beamParams->beamWidth = width;
	}
	else
	{
		beamParams->beamWidth = 1;
	}

	if (algorithmConfig.contains("expansion-width"))
	{
		int width = algorithmConfig["expansion-width"];
		beamParams->expansionWidth = width;
	}
	else
	{
		beamParams->expansionWidth = 1;
	}

	this->baseAlgorithm = [&](problem::Problem& p, problem::Instance& i, selection::SelectorPtr s) {
		return ConstructiveAlgorithm::beamsearchAlgorithm(p, i, s, beamParams->beamWidth, beamParams->expansionWidth);
	};
}

void AlgorithmConfiguration::parseInterated(json& algorithmConfig)
{
	assert(algorithmConfig.contains("internal-algorithm"));
	assert(algorithmConfig.contains("stop"));

	parseAlgorithmType(algorithmConfig["internal-algorithm"]);
	parseStopCriteria(algorithmConfig["stop"]);

	if (algorithmConfig.contains("num-solutions") and (algorithmConfig["num-solutions"] > 1))
	{
		this->numSolutions = algorithmConfig["num-solutions"];
		this->algorithm = [&](problem::Problem& p, problem::Instance& i, selection::SelectorPtr s) {
			return ConstructiveAlgorithm::multistartAlgorithm(p, i, this->baseAlgorithm, s, this->stopCriteria, this->numSolutions);
		};
	}
	else
	{
		this->algorithm = [&](problem::Problem& p, problem::Instance& i, selection::SelectorPtr s) {
			return ConstructiveAlgorithm::multistartAlgorithm(p, i, this->baseAlgorithm, s, this->stopCriteria);
		};
	}
}

void AlgorithmConfiguration::parsePriority(json& algorithmConfig)
{
	auto priorityConfig = algorithmConfig["priority"];

	if (priorityConfig.contains("type"))
	{
		if (priorityConfig["type"] == "greedy")
		{
			parseGreedySelection(algorithmConfig);
		}
		else if (priorityConfig["type"] == "random")
		{
			parseRandomSelection(algorithmConfig);
		}
		else if (priorityConfig["type"] == "weighted")
		{
			parseWeightedSelection(algorithmConfig);
		}
		else if (priorityConfig["type"] == "pheromone")
		{
			parsePheromoneSelection(algorithmConfig);
		}
		else assert(false);
	}
	else assert(false);
}

void AlgorithmConfiguration::parseStopCriteria(json& algorithmConfig)
{
	stopCriteria = shared_ptr<ConstructiveAlgorithm::StopCriteria>(new ConstructiveAlgorithm::StopCriteria());
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
		stopCriteria->maxNoImprov = algorithmConfig["max-no-improvement-iterations"];
	}
	else
	{
		stopCriteria->maxNoImprov = 0;
	}

	if (stopCriteria->maxIterations == 0 and stopCriteria->maxNoImprov == 0)
	{
		assert(false);
	}
}

void AlgorithmConfiguration::parseGreedySelection(nlohmann::json& algorithmConfig)
{
	elementSelector = static_pointer_cast<selection::Selector>(make_shared<selection::GreedySelector>());
}

void AlgorithmConfiguration::parseRandomSelection(nlohmann::json& algorithmConfig)
{
	auto config = algorithmConfig["priority"];
	double alpha = config["alpha-value"];
	int kValue = config["k-value"];

	elementSelector = static_pointer_cast<selection::Selector>(make_shared<selection::RandomSelector>(alpha, kValue));
}

void AlgorithmConfiguration::parseWeightedSelection(nlohmann::json& algorithmConfig)
{
	auto config = algorithmConfig["priority"];

	elementSelector = static_pointer_cast<selection::Selector>(make_shared<selection::WeightedRandomSelector>());
}

void AlgorithmConfiguration::parsePheromoneSelection(nlohmann::json& algorithmConfig)
{
	auto config = algorithmConfig["priority"];
	double alpha = config["alpha-value"];
	double beta = config["beta-value"];
	double phi = config["phi-value"];

	elementSelector = static_pointer_cast<selection::Selector>(make_shared<selection::PheromoneSelector>(alpha, beta, phi));
}
