#include "parser.h"
#include "StaticSelector.h"
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
	return algorithm(problem, instance);
}

void AlgorithmConfiguration::parseGreedy(json& algorithmConfig)
{
    assert(algorithmConfig.contains("priority"));
	parsePriority(algorithmConfig);

	this->baseAlgorithm = [&](problem::Problem& p, problem::Instance& i) {
		return ConstructiveAlgorithm::greedyAlgorithm(p, i, this->elementSelector);
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

	this->baseAlgorithm = [&](problem::Problem& p, problem::Instance& i) {
		return ConstructiveAlgorithm::beamsearchAlgorithm(p, i, this->elementSelector, beamParams->beamWidth, beamParams->expansionWidth);
	};
}

void AlgorithmConfiguration::parseInterated(json& algorithmConfig)
{
	assert(algorithmConfig.contains("internal-algorithm"));
	assert(algorithmConfig.contains("stop"));

	parseAlgorithmType(algorithmConfig["internal-algorithm"]);
	parseStopCriteria(algorithmConfig["stop"]);
	
	this->algorithm = [&](problem::Problem& p, problem::Instance& i) {
		return ConstructiveAlgorithm::multistartAlgorithmMaxIterations(p, i, this->baseAlgorithm, this->stopCriteria);
	};
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
