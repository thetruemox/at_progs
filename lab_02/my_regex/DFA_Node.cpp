#include "DFA_Node.h"

DFA_Node::DFA_Node(int id)
{
	this->checked = 0; //??????????? ????, ????? ?????
	this->id = id;
	this->type = dfa_normal;
}

DFA_Node::DFA_Node(int id, int mul_id_f, int mul_id_s)
{
	this->checked = 0; //??????????? ????, ????? ?????
	this->id = id;
	this->mul_id_f = mul_id_f;
	this->mul_id_s = mul_id_s;
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

DFA_Node* DFA_Node::transition(char cond)
{
	for (auto it = this->links.begin(); it != this->links.end(); it++)
	{
		if (((*it).second)[0] == cond) return (*it).first;
	}
	return nullptr;
}

void DFA_Node::make_link(DFA_Node* to, std::string cond)
{
	this->links.push_back({ to, cond });
}
