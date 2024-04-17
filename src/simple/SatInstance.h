#pragma once

#include "SimpleInstance.h"

namespace sat {
	enum Value {
		TRUE = 1,
		FALSE = 0,
		UNDEFINED = -1
	};

	struct Variable {
		int id; // Variable id
		Value value; // 0 = false, 1 = true, -1 = undefined
		std::vector<int> clauses; // Clauses that contain this variable
		bool operator<(const Variable& other) const;
	};

	struct Literal {
		Variable& variable;
		bool negated;
	};

	struct Clause {
		std::vector<Literal> literals; // Variables in this clause
		Value value; // Defined by the variables
	};

	struct SatInstance : public simple::Instance<Variable> {
	private:
		std::vector<Variable> variables;
		std::vector<Clause> clauses;

	public:
		SatInstance();
		void readInstanceFromInput();
		inline std::vector<Variable>& getVariables() { return variables; };
		inline std::vector<Clause>& getClauses() { return clauses; };

		virtual double objectiveValue();
		virtual bool shouldStop();
		virtual bool validateChoice(Variable& choice);

		virtual void addToSolution(Variable& choice);
		virtual void updateAfterChoice(const Variable& choice);
		virtual int orderByComparator();
		virtual std::vector<Variable>& getElements();
		virtual void resetInstance();
	};
} // namespace SAT