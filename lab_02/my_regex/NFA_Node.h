#pragma once
#include <list>
#include <string>

enum NFA_node_type 
{
	nfa_start, nfa_receiving, nfa_normal
};


class NFA_Node
{
public:
	NFA_Node(NFA_node_type type, int id);
	NFA_node_type type;

	int checked;
	int id;

	void make_link(NFA_Node* to, std::string cond);
	std::list<std::pair<NFA_Node*, std::string>> links;
	//переход куда и по какому условию
private:
};
