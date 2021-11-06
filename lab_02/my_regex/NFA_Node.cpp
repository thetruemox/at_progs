#include "NFA_Node.h"

NFA_Node::NFA_Node(NFA_node_type type, int id)
{
	this->checked = 0; //технический долг, потом убери
	this->type = type;
	this->id = id;
}

void NFA_Node::make_link(NFA_Node* to, std::string cond)
{
	this->links.push_back({ to, cond });
}
