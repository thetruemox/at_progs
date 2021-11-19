#pragma once
#include <list>
#include <string>

enum DFA_node_type
{
	dfa_start, dfa_receiving, dfa_normal
};

class DFA_Node
{
public:
	DFA_Node(int id);
	DFA_node_type type;

	int checked;
	int id;

	DFA_Node* transition(std::string cond);

	void make_link(DFA_Node* to, std::string cond);
	std::list<std::pair<DFA_Node*, std::string>> links;
	//переход куда и по какому условию
private:
};
