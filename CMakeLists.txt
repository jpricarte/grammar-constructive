# CMakeList.txt: projeto de CMake para simple-heuristic, incluir origem e definir
# lógica específica a um projeto aqui.
#
cmake_minimum_required (VERSION 3.8)

# Habilite a Recarga Dinâmica para compiladores MSVC, se houver suporte.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("simple-heuristic")

# Adicione a origem ao executável deste projeto.
add_executable (simple-heuristic "src/simple-heuristic.cpp" "src/simple-heuristic.h" "src/ConstructiveAlgorithm.h" "src/Problem.h" "src/KnapsackProblem.h" "src/KnapsackProblem.cpp" "src/ConstructiveAlgorithm.cpp" "src/parser.h" "src/parser.cpp" "src/Selector.h" "src/StaticSelector.h" "src/StaticSelector.cpp" "src/DependentSelector.h" "src/KLSFP.h" "src/KLSFP.cpp" "src/structs/graph.hpp" "src/structs/graph.cpp" "src/structs/union-find.hpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET simple-heuristic PROPERTY CXX_STANDARD 20)
endif()

# TODO: Adicione testes e instale destinos, se necessário.
