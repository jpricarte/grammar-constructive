#pragma once

#include "Instance.h"
#include <vector>


struct KnapsackItem {
	int weight;
	double cost;

	inline bool operator< (const KnapsackItem& other) { return -this->cost < -other.cost; };
	inline bool operator==(const KnapsackItem& other) { return this->weight == other.weight && this->cost == other.cost; };
};

class KnapsackInstance : public Instance<KnapsackItem> {
private:
	// instance
	std::vector<KnapsackItem> items;
	int maxWeight;
	// result
	std::vector<KnapsackItem> selectedItems;
	int totalWeight;
	double totalCost;
public:
	KnapsackInstance();

	void readInstanceFromInput();

	inline int getTotalWeight() { return totalWeight; };
	inline double getTotalCost() { return totalCost; };
	inline const std::vector<KnapsackItem>& getItems() { return selectedItems; };

	virtual double objectiveValue();
	virtual bool shouldStop();
	virtual bool validateChoice(const KnapsackItem& choice);
	virtual void addToSolution(const KnapsackItem& choice);
	virtual void updateCandidates(const KnapsackItem& choice);
	virtual void orderByComparator();
	virtual std::vector<KnapsackItem>& getElements();
	
};