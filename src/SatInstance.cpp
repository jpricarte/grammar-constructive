#include "SatInstance.h"

#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;
using namespace sat;

bool sat::Variable::operator<(const Variable& other) const
{
	// Sort variables by the number of undefined clauses they are in (ascending), given preference to undefined variables
	if (this->value == Value::UNDEFINED && other.value != Value::UNDEFINED)
	{
		return true;
	}
	else if (this->value != Value::UNDEFINED && other.value == Value::UNDEFINED)
	{
		return false;
	}
	else
	{
		int numClausesThis = 0;
		int numClausesOther = 0;
		for (int clauseIndex : this->clauses)
		{
			if (this->value == Value::UNDEFINED)
			{
				numClausesThis++;
			}
		}
		for (int clauseIndex : other.clauses)
		{
			if (other.value == Value::UNDEFINED)
			{
				numClausesOther++;
			}
		}
		return numClausesThis > numClausesOther;
	}
}

sat::SatInstance::SatInstance()
{
	this->variables = vector<Variable>();
	this->clauses = vector<Clause>();
}

void sat::SatInstance::readInstanceFromInput()
{
	int clauseIndex = 0;
	string line;
	while (getline(cin, line))
	{
		if (line.empty() || line[0] == 'c' || line[0] == '%' || line[0] == '0')
		{
			continue;
		}
		else if (line[0] == 'p')
		{
			int numVariables, numClauses;
			istringstream iss(line);
			string temp;
			iss >> temp;
			iss >> temp;
			iss >> numVariables;
			iss >> numClauses;
			for (int i = 0; i < numVariables; i++)
			{
				this->variables.push_back(Variable{ i+1, Value::UNDEFINED, vector<int>() });
			}
			for (int i=0; i < numClauses; i++)
			{
				this->clauses.push_back(Clause{ vector<Literal>(), Value::UNDEFINED });
			};
		}
		else
		{
			int var;
			istringstream iss(line);
			Clause& clause = this->clauses[clauseIndex];
			while (iss >> var)
			{
				if (var == 0)
				{
					break;
				}
				int varIndex = abs(var) - 1;
				Variable& variable = this->variables.at(varIndex);
				Literal lit = { variable, var < 0 };
				clause.literals.push_back(lit);
				variable.clauses.push_back(clauseIndex);
			}
			clauseIndex++;
		}
	}

}

double sat::SatInstance::objectiveValue()
{
	double numClausesTrue = 0;
	for (Clause clause : this->clauses)
	{
		if (clause.value == Value::TRUE)
		{
			numClausesTrue++;
		}
	}
	return numClausesTrue;
}

bool sat::SatInstance::shouldStop()
{
	for (auto& variable : this->variables)
	{
		if (variable.value == Value::UNDEFINED)
		{
			return false;
		}
	}
	return true;
}

bool sat::SatInstance::validateChoice(const Variable& choice)
{
	return choice.value == Value::UNDEFINED;
}

void sat::SatInstance::addToSolution(Variable& choice)
{
	// Count number of undefined clauses where the choice is true and false
	int numClausesTrue = 0;
	int numClausesFalse = 0;
	for (int clauseIndex : choice.clauses)
	{
		Clause& clause = this->clauses[clauseIndex];
		if (clause.value == Value::TRUE)
		{
			continue;
		}
		for (Literal lit : clause.literals)
		{
			if (lit.variable.id == choice.id)
			{
				if (lit.negated)
					numClausesFalse++;
				else
					numClausesTrue++;
			}
		}
	}
	if (numClausesTrue >= numClausesFalse)
	{
		choice.value = Value::TRUE;
	}
	else
	{
		choice.value = Value::FALSE;
	}
}

void sat::SatInstance::updateCandidates(const Variable& choice)
{
	// Set value clauses that choice make satisfiable to true
	for (auto& clause : this->clauses)
	{
		if (clause.value == Value::UNDEFINED)
		{
			bool clauseSatisfied = false;
			bool containsUndefinedVars = false;

			for (Literal& lit : clause.literals)
			{
				if (lit.variable.id == choice.id)
				{
					if (lit.negated && choice.value == Value::FALSE)
					{
						clauseSatisfied = true;
						break;
					}
					else if (!lit.negated && choice.value == Value::TRUE)
					{
						clauseSatisfied = true;
						break;
					}
				}
				else if (lit.variable.value == Value::UNDEFINED)
				{
					containsUndefinedVars = true;
				}
			}

			if (clauseSatisfied)
			{
				clause.value = Value::TRUE;
			} 
			else if (not containsUndefinedVars)
			{
				clause.value = Value::FALSE;
			}
		}
	}
}

int sat::SatInstance::orderByComparator()
{
	// Sort variables by the number of clauses they are in (ascending), given preference to undefined variables
	sort(this->variables.begin(), this->variables.end());
	int index = 0;
	while (index < this->variables.size() && this->variables[index].value == Value::UNDEFINED) 
		index++;
	return index;
}

std::vector<Variable>& sat::SatInstance::getElements()
{
	return this->variables;
}