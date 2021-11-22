#pragma once
#include "DFA_Builder.h"

class My_Regex
{
public:
	My_Regex(std::string regex);
	int check_str(std::string str);
private:
	DFA_Node* dfa_graph_start;
};
