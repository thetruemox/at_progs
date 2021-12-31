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

	My_Regex* addition();
	My_Regex* inversion();

	std::list<std::string> findall(std::string line, std::string regex);

private:
	DFA_Builder* dfa_builder;
	std::list<DFA_Node*> dfa_graph;
	DFA_Node* dfa_graph_start;
	std::list<DFA_Node*> dfa_recieves;
	
	Capture_Groups* CG;

	void draw_dfa_graph(std::list<DFA_Node*> dfa_graph, std::string file_name);

	DFA_Node* get_node(int id);
	DFA_Node* get_node(int id, std::list<DFA_Node*> node_list);

	std::string _R(int k, int i, int j);
	std::string _null_R(int i, int j);
	void _flip_stree(ST_Node* node);
	std::list<DFA_Node*> _multiply(std::list<DFA_Node*> graph_A, std::list<DFA_Node*> graph_B, std::map<std::string, int> abc);
	DFA_Node* _find_mul_node(std::list<DFA_Node*> graph, int id1, int id2);
	void _delete_terminal_state(std::list<DFA_Node*> graph);
};
