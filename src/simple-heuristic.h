﻿// simple-heuristic.h: arquivo de inclusão para arquivos de inclusão padrão do sistema,
// ou arquivos de inclusão específicos a um projeto.

#pragma once

#include <iostream>
#include <vector>

#include "KnapsackProblem.h"
#include "KLSFP.h"
#include "ConstructiveAlgorithm.h"
#include "parser.h"
#include "Selector.h"
#include "StaticSelector.h"
#include "structs/graph.hpp"
#include "FSSP.h"

int autoFSSP(int argc, char* argv[]);
int autoKLSFP(int argc, char* argv[]);
int autoKnapsack(int argc, char* argv[]);
