#pragma once
#include "DFA_Builder.h"

class My_Regex
{
public:
	My_Regex(std::string regex);
	int check_str(std::string str);
	std::string restore_regex(std::string file_name);
private:
	std::list<DFA_Node*> dfa_graph;
	DFA_Node* dfa_graph_start;
	std::list<DFA_Node*> dfa_recieves;
	
	DFA_Node* get_node(int id);
	std::string _R(int k, int i, int j);
	std::string _null_R(int i, int j);
};
