#include "CG_Unit.h"

CG_Unit::CG_Unit(int CG_num, int stree_min_id, int stree_max_id)
{	
	this->CG_num = CG_num;
	this->stree_min_id = stree_min_id;
	this->stree_max_id = stree_max_id;	
}

int CG_Unit::saw_it(int nfa_node_id)
{
	for (int i = 0; i < this->NFA_arr.size(); i++)
	{
		if (this->NFA_arr[i] == nfa_node_id) return 1;
	}
	return 0;
}

int CG_Unit::contains(int arr_id, int node_id)
{
	switch (arr_id)
	{
	case 0:
		for (int i = 0; i < NFA_arr.size(); i++)
		{
			if (NFA_arr[i] == node_id) return 1;
		}
		break;
	case 1:
		for (int i = 0; i < DFA_arr.size(); i++)
		{
			if (DFA_arr[i] == node_id) return 1;
		}
		break;
	case 2:
		for (int i = 0; i < Min_DFA_arr.size(); i++)
		{
			if (Min_DFA_arr[i] == node_id) return 1;
		}
		break;
	default:
		return -1;
		break;
	}

	return 0;
}
