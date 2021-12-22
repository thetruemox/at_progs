#pragma once
#include <list>
#include <string>

enum DFA_node_type
{
	dfa_start, dfa_receiving, dfa_normal, dfa_start_and_receiving
};

class DFA_Node
{
public:
	DFA_Node(int id);
	DFA_Node(int id, int mul_id_f, int mul_id_s);
	DFA_node_type type;

	int checked;
	int id;

	int mul_id_f, mul_id_s;

	DFA_Node* transition(std::string cond);
	DFA_Node* transition(char cond);

	void make_link(DFA_Node* to, std::string cond);
	std::list<std::pair<DFA_Node*, std::string>> links;
	//переход куда и по какому условию
private:
};
