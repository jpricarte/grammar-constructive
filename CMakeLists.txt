﻿# CMakeList.txt: projeto de CMake para simple-heuristic, incluir origem e definir
# lógica específica a um projeto aqui.
#
cmake_minimum_required (VERSION 3.30)

project ("simple-heuristic")

# find_package(Boost 1.83.0 COMPONENTS program_options)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

# Adicione a origem ao executável deste projeto.
add_executable (simple-heuristic "src/simple-heuristic.cpp" "src/simple-heuristic.h" "src/ConstructiveAlgorithm.h" "src/Problem.h" "src/KnapsackProblem.h" "src/KnapsackProblem.cpp" "src/ConstructiveAlgorithm.cpp" "src/parser.h" "src/parser.cpp" "src/Selector.h" "src/StaticSelector.h" "src/StaticSelector.cpp"  "src/KLSFP.h" "src/KLSFP.cpp" "src/structs/graph.hpp" "src/structs/graph.cpp" "src/structs/union-find.hpp" "src/Problem.cpp" "src/DependentSelector.h" "src/DependentSelector.cpp" "src/FSSP.h" "src/FSSP.cpp"
"submodule/fssp/instance.hpp" "submodule/fssp/instance.cpp")

target_link_libraries(simple-heuristic ${Boost_LIBRARIES})
set_property(TARGET simple-heuristic PROPERTY CXX_STANDARD 20)

# TODO: Adicione testes e instale destinos, se necessário.
