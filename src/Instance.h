#pragma once

#include <vector>

template<class T> class Instance {
public:
	virtual double objectiveValue() = 0;
	virtual bool shouldStop() = 0;
	virtual bool validateChoice(const T& choice) = 0;
	
	virtual void addToSolution(T& choice) = 0;
	virtual void updateCandidates(const T& choice) = 0;
	virtual int orderByComparator() = 0;
	virtual std::vector<T>& getElements() = 0;
};
