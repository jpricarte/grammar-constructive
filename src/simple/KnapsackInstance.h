#pragma once

#include "SimpleInstance.h"

#include <vector>


struct KnapsackItem {
	int weight;
	double cost;
	bool selected;
	bool visited;

	bool operator< (const KnapsackItem& other) const;
	inline bool operator==(const KnapsackItem& other) const { return this->weight == other.weight && this->cost == other.cost; };
};

class KnapsackInstance : public simple::Instance<KnapsackItem> {
private:
	// instance
	std::vector<KnapsackItem> items;
	int maxWeight;
	// result
	int totalWeight;
	double totalCost;

public:
	KnapsackInstance();

	void readInstanceFromInput();

	inline int getTotalWeight() const { return totalWeight; };
	inline double getTotalCost() const { return totalCost; };

	virtual double objectiveValue();
	virtual bool shouldStop();
	virtual bool validateChoice(KnapsackItem& choice);
	virtual void addToSolution(KnapsackItem& choice);
	virtual void updateAfterChoice(const KnapsackItem& choice);
	virtual int orderByComparator();
	virtual std::vector<KnapsackItem>& getElements();
	virtual void resetInstance();
	
};