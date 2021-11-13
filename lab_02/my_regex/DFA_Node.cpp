#include "DFA_Node.h"

DFA_Node::DFA_Node(int id)
{
	this->checked = 0; //технический долг, потом убери
	this->id = id;
	this->type = dfa_normal;
}

void DFA_Node::make_link(DFA_Node* to, std::string cond)
{
	this->links.push_back({ to, cond });
}
