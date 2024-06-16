#pragma once

#include <functional>
#include <random>
#include "ConstructiveAlgorithm.h"
#include "Problem.h"
#include "Selector.h"
#include "../submodule/json/single_include/nlohmann/json.hpp"

struct BeamSearchParams {
	int beamWidth;
	int expansionWidth;
};
using BeamSearchParamsPtr = std::shared_ptr<BeamSearchParams>;

class AlgorithmConfiguration {
	
	nlohmann::json configuration;
	
	std::function<problem::SolutionPtr(problem::Problem&, problem::Instance&, selection::SelectorPtr elementSelector)> algorithm;
	std::function<problem::SolutionPtr(problem::Problem&, problem::Instance&, selection::SelectorPtr elementSelector)> baseAlgorithm;
	selection::SelectorPtr elementSelector;

	// Params struct, may be empty
	BeamSearchParamsPtr beamParams = nullptr;
	ConstructiveAlgorithm::StopCriteriaPtr stopCriteria = nullptr;
	
	int numSolutions;

	void parseAlgorithmType(nlohmann::json& algorithmConfig);

	void parseGreedy(nlohmann::json& algorithmConfig);
	void parseBeamsearch(nlohmann::json& algorithmConfig);
	void parseInterated(nlohmann::json& algorithmConfig);

	void parsePriority(nlohmann::json& algorithmConfig);
	void parseGreedySelection(nlohmann::json& algorithmConfig);
	void parseRandomSelection(nlohmann::json& algorithmConfig);
	void parseWeightedSelection(nlohmann::json& algorithmConfig);
	void parsePheromoneSelection(nlohmann::json& algorithmConfig);
	void parsePilotSelection(nlohmann::json& algorithmConfig);

	void parseStopCriteria(nlohmann::json& algorithmConfig);

	public:
		AlgorithmConfiguration() = default;
		void readConfiguration(std::string filename);
		problem::SolutionPtr run(problem::Problem& problem, problem::Instance& instance);
};