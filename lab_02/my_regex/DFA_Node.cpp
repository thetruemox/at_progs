#include "DFA_Node.h"

DFA_Node::DFA_Node(int id)
{
	this->checked = 0; //технический долг, потом убери
	this->id = id;
	this->type = dfa_normal;
}

DFA_Node* DFA_Node::transition(std::string cond)
{
	for (auto it = this->links.begin(); it != this->links.end(); it++)
	{
		if ((*it).second == cond) return (*it).first;
	}
	return nullptr;
}

void DFA_Node::make_link(DFA_Node* to, std::string cond)
{
	this->links.push_back({ to, cond });
}
