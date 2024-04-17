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
		KnapsackItem e{ weight, cost, false, false };
		this->items.push_back(e);
	}
}

double KnapsackInstance::objectiveValue()
{
	return - this->totalCost;
}

bool KnapsackInstance::shouldStop()
{
	return this->items[0].visited;
}

bool KnapsackInstance::validateChoice(KnapsackItem& choice)
{
	choice.visited = true;
	return choice.weight + this->totalWeight <= this->maxWeight;
}

void KnapsackInstance::addToSolution(KnapsackItem& choice)
{
	choice.selected = true;
	this->totalCost += choice.cost;
	this->totalWeight += choice.weight;
}

void KnapsackInstance::updateAfterChoice(const KnapsackItem& choice)
{
	// Send the choosed item to the end of the list
	auto it = find(this->items.begin(), this->items.end(), choice);
	rotate(it, it + 1, this->items.end());
}

int KnapsackInstance::orderByComparator()
{
	sort(this->items.begin(), this->items.end());
	for (int i=0; i<this->items.size(); i++) {
		if (this->items[i].visited) {
			return i;
		}
	}
	return this->items.size();
}

void KnapsackInstance::resetInstance()
{
	for (auto& item : this->items) {
		item.visited = false;
		item.selected = false;
	}
	this->totalCost = 0;
	this->totalWeight = 0;
}

vector<KnapsackItem>& KnapsackInstance::getElements()
{
	return this->items;
}

bool KnapsackItem::operator<(const KnapsackItem& other) const
{
	if (this->visited && !other.visited)
		return false;
	if (!this->visited && other.visited)
		return true;
	return -(this->cost / this->weight) < -(other.cost / other.weight);
}
