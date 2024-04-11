#include "KnapsackInstance.h"

// Private includes
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

KnapsackInstance::KnapsackInstance()
{
	// instance
	this->items = vector<KnapsackItem>();
	this->maxWeight = 0;
	// result
	this->selectedItems = vector<KnapsackItem>();
	this->totalWeight = 0;
	this->totalCost = 0;
}

void KnapsackInstance::readInstanceFromInput()
{
	string line;
	// Read empty line
	getline(cin, line);
	// Get number of items
	getline(cin, line);
	// Set size of items
	this->items.reserve(stoi(line));
	this->selectedItems.reserve(stoi(line));
	// Get max weight
	getline(cin, line);
	this->maxWeight = stoi(line);
	// Read empty line
	getline(cin, line);
	while (getline(cin, line)) {
		if (line.empty())
			break;

		istringstream iss(line);
		int weight;
		double cost;
		iss >> cost >> weight;
		KnapsackItem e{ weight, cost };
		this->items.push_back(e);
	}
}

double KnapsackInstance::objectiveValue()
{
	return this->totalCost;
}

bool KnapsackInstance::shouldStop()
{
	return this->items.empty();
}

bool KnapsackInstance::validateChoice(const KnapsackItem& choice)
{
	return choice.weight + this->totalWeight <= this->maxWeight;
}

void KnapsackInstance::addToSolution(KnapsackItem& choice)
{
	this->selectedItems.push_back(choice);
	this->totalCost += choice.cost;
	this->totalWeight += choice.weight;
}

void KnapsackInstance::updateCandidates(const KnapsackItem& choice)
{
	// Erase choice from items list
	auto it = find(this->items.begin(), this->items.end(), choice);
	if (it != this->items.end())
		this->items.erase(it);
}

int KnapsackInstance::orderByComparator()
{
	sort(this->items.begin(), this->items.end());
	return this->items.size();
}

vector<KnapsackItem>& KnapsackInstance::getElements()
{
	return this->items;
}