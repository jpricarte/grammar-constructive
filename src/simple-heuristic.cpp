// simple-heuristic.cpp: define o ponto de entrada para o aplicativo.
//

#include "simple-heuristic.h"

#include <functional>

using namespace std;

int main() {
    auto instance = sat::SatInstance();
    instance.readInstanceFromInput();

    function<sat::Variable& (Instance<sat::Variable>&)> selectionFuncion = [](auto& instance) ->sat::Variable& {
        return ConstructiveAlgorithm::getElementAlphaSelection<sat::Variable>(instance, 0.2); 
    };

    ConstructiveAlgorithm::greedyAlgorithm<sat::Variable>(instance, selectionFuncion);
    
    cout << "Final result: " << instance.objectiveValue() << endl;
    for (auto& var : instance.getClauses()) {
        cout << "Clause " <<  " = ";
        switch (var.value) {
        case sat::Value::TRUE:
            cout << "True";
            break;

        case sat::Value::FALSE:
            cout << "False";
            break;

        case sat::Value::UNDEFINED:
            cout << "Undefined";
            break;
        }
        cout << endl;
    }

    for (auto& var : instance.getVariables()) {
        cout << "Variable " << var.id << " = ";
        switch (var.value) {
        case sat::Value::TRUE:
			cout << "True";
			break;

        case sat::Value::FALSE:
            cout << "False";
            break;

        case sat::Value::UNDEFINED:
            cout << "Undefined";
			break;
        }
        cout << endl;
	}
    return 0;
}

int mainks()
{
    auto instance = KnapsackInstance();
    instance.readInstanceFromInput();

    function<KnapsackItem& (Instance<KnapsackItem>&)> selectionFuncion = [](auto& instance) -> KnapsackItem& {return ConstructiveAlgorithm::getBestElement<KnapsackItem>(instance); };

    ConstructiveAlgorithm::greedyAlgorithm<KnapsackItem>(instance, selectionFuncion);

    cout << "Final result: weight=" << instance.getTotalWeight() << " value=" << instance.getTotalCost() << endl;
    for (auto& e : instance.getItems()) {
        cout << e.weight << " " << e.cost << endl;
    }
    return 0;
}
