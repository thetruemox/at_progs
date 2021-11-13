#include "DFA_Agent.h"

DFA_Agent::DFA_Agent(int id)
{
	this->id = id;
	this->checked = 0;
}

int DFA_Agent::saw_it(NFA_Node* node)
{
	for (auto it = this->set_of_states.begin(); it != this->set_of_states.end(); it++)
	{
		if ((*it)->id == node->id) return 1;
	}
	return 0;
}
