#pragma once
#include <list>
#include "NFA_Node.h"

class DFA_Agent
{
public:
	DFA_Agent(int id);
	int id;
	int checked;
	int saw_it(NFA_Node* node);
	std::list<NFA_Node*> set_of_states;
private:
};
