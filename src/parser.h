#pragma once

#include <functional>
#include "ConstructiveAlgorithm.h"
#include "Problem.h"
#include "../submodule/json/single_include/nlohmann/json.hpp"

class AlgorithmConfiguration {
	
	nlohmann::json configuration;
	
	std::function<problem::SolutionPtr(problem::Problem&, problem::Instance&)> algorithm;
	std::function<problem::ElementPtr(problem::Instance&, problem::SolutionPtr)> elementselection;
	std::function<int(int)> random_distribuiton;

	public:
		AlgorithmConfiguration() = default;

		void loadConfiguration(std::string path);
};