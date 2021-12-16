#pragma once
#include "DFA_Builder.h"

class My_Regex
{
public:
	My_Regex(std::string regex);
	My_Regex(DFA_Builder* dfa_builder);

	int check_str(std::string str);
	int check_str(std::string str, std::map<int, std::string> &capture);

	std::string restore_regex();
	std::string restore_regex(std::string file_name);

	My_Regex* inversion();
private:
	DFA_Builder* dfa_builder;
	std::list<DFA_Node*> dfa_graph;
	DFA_Node* dfa_graph_start;
	std::list<DFA_Node*> dfa_recieves;
	
	Capture_Groups* CG;

	DFA_Node* get_node(int id);
	std::string _R(int k, int i, int j);
	std::string _null_R(int i, int j);
	void _flip_stree(ST_Node* node);
};
