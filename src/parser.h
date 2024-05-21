#pragma once

#include <functional>
#include <random>
#include "ConstructiveAlgorithm.h"
#include "Problem.h"
#include "../submodule/json/single_include/nlohmann/json.hpp"

struct BeamSearchParams {
	int beamWidth;
	int expansionWidth;
};
using BeamSearchParamsPtr = std::shared_ptr<BeamSearchParams>;

struct StopCriteria {
	int maxIterations;
	int maxNoImprovementIterations;
};
using StopCriteriaPtr = std::shared_ptr<StopCriteria>;

class AlgorithmConfiguration {
	
	nlohmann::json configuration;
	
	std::function<problem::SolutionPtr(problem::Problem&, problem::Instance&)> algorithm;
	std::function<problem::SolutionPtr(problem::Problem&, problem::Instance&)> baseAlgorithm;
	std::function<problem::ElementPtr(problem::Instance&, problem::SolutionPtr)> elementselection;
	std::function<int(int)> random_distribuiton;
	std::default_random_engine generator;

	// Params struct, may be empty
	BeamSearchParamsPtr beamParams = nullptr;
	StopCriteriaPtr stopCriteria = nullptr;

	void parseAlgorithmType(nlohmann::json& algorithmConfig);

	void parseGreedy(nlohmann::json& algorithmConfig);
	void parseBeamsearch(nlohmann::json& algorithmConfig);
	void parseIndependent(nlohmann::json& algorithmConfig);

	void parsePriority(nlohmann::json& algorithmConfig);
	void parseStopCriteria(nlohmann::json& algorithmConfig);

	public:
		AlgorithmConfiguration() = default;
		void readConfiguration(std::string filename);
		problem::SolutionPtr run(problem::Problem& problem, problem::Instance& instance);
};