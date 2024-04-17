// simple-heuristic.cpp: define o ponto de entrada para o aplicativo.
//

#include "simple-heuristic.h"

#include <functional>

using namespace std;

int main(int argc, char* argv[])
{
    auto problem = KnapsackProblem();
    auto instance = KnapsackInstance(argv[1]);

    double value = ConstructiveAlgorithm::greedyAlgorithm<KnapsackInstance, KnapsackSolution, KnapsackElement>(problem, instance);

    cout << "Objective value: " << value << endl;

    return 0;
}
