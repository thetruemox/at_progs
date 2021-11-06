#pragma once
#include "NFA_Node.h"
#include <list>

class NFA_Graph
{
public:
	NFA_Graph(int index);

	int index;

	NFA_Node* enter_node;
	NFA_Node* exit_node;

	void add_node(NFA_Node* node);
	void delete_node(NFA_Node* node);
	std::list<NFA_Node*> nodes;
private:
};
