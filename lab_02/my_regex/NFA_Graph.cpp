#include "NFA_Graph.h"

NFA_Graph::NFA_Graph(int index)
{
	this->enter_node = nullptr;
	this->exit_node = nullptr;
	this->index = index;
}

void NFA_Graph::add_node(NFA_Node* node)
{
	this->nodes.push_back(node);
}

void NFA_Graph::delete_node(NFA_Node* node)
{
	for (auto it = this->nodes.begin(); it != this->nodes.end(); it++)
	{
		if ((*it) == node)
		{
			this->nodes.erase(it);
			break;
		}
	}
}
