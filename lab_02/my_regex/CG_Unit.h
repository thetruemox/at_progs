#pragma once
#include <vector>
#include <string>

class CG_Unit
{
public:
	CG_Unit(int CG_num, int stree_min_id, int stree_max_id);

	int saw_it(int nfa_node_id);
	int contains(int arr_id, int node_id);

	int CG_num;
	std::string cap_str;
	
	int stree_min_id;
	int stree_max_id;
	int stree_node_id;

	std::vector<int> NFA_arr;
	std::vector<int> DFA_arr;
	std::vector<int> Min_DFA_arr;
};

