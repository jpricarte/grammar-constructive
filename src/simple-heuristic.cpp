// simple-heuristic.cpp: define o ponto de entrada para o aplicativo.
//

#include "simple-heuristic.h"

using namespace std;

template<class T> void greedyAlgorithm(Instance<T>& instance, function<T&(Instance<T>&)> selectionAlgorithm);
template<class T> void orderByPriority(Instance<T>& instance);
template<class T> T& getNextElement(Instance<T>& instance);
template<class T> T& getNextElementRandom(Instance<T>& instance, int k);
template<class T> T& getNextElementRandom(Instance<T>& instance, double alpha);


int main()
{
    auto instance = KnapsackInstance();
    instance.readInstanceFromInput();

    function<KnapsackItem& (Instance<KnapsackItem>&)> selectionFuncion = [] (auto& item) -> KnapsackItem& {return getNextElementRandom(item, 0.8); };

    greedyAlgorithm<KnapsackItem>(instance, selectionFuncion);

    cout << "Final result: weight=" << instance.getTotalWeight() << " value=" << instance.getTotalCost() << endl;
    for (auto e : instance.getItems()) {
        cout << e.weight << " " << e.cost << endl;
    }
    return 0;
}

template<class T> void greedyAlgorithm(Instance<T>& instance, function<T& (Instance<T>&)> selectionAlgorithm)
{
    // Stopping criterion
    while (not instance.shouldStop()) {
        // Choose next element
        T& choosedElement = selectionAlgorithm(instance);

        // Verify if is a valid choice 
        if (instance.validateChoice(choosedElement))
        {
            // Update state
            instance.addToSolution(choosedElement);
        }

        // Update candidates
        instance.updateCandidates(choosedElement);
    }
}

template<class T>
void orderByPriority(Instance<T>& instance)
{
    sort(instance.getElements().begin(), instance.getElements().end(), instance.compareElements);
}

template<class T>
T& getNextElement(Instance<T>& instance)
{
    // Sort elements based on an defined order
    instance.orderByComparator();
    
    // Return the first element on list
    auto element = instance.getElements().front();

    // Remove element from list
    instance.getElements().erase(instance.getElements().begin());
    return element;
}

template<class T>
T& getNextElementRandom(Instance<T>& instance, int k)
{
    // Sort elements based on an defined order
    instance.orderByComparator();

    int windowSize = min(k, (int)instance.getElements().size());
    int index = rand() % windowSize;

    // Save the element on index
    auto element = instance.getElements().at(index);

    return element;
}

template<class T>
T& getNextElementRandom(Instance<T>& instance, double alpha)
{
    // Sort elements based on an defined order
    instance.orderByComparator();

    auto& element = instance.getElements().front();
    // Choose a random element from list with probability 1-alpha
    if (rand() % 100 < (1 - alpha) * 100) {
		int index = rand() % instance.getElements().size();
        element = instance.getElements().at(index);
    }

    return element;
}


