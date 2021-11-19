#pragma once
#include "NFA_Builder.h"
#include "DFA_Agent.h"
#include "DFA_Node.h"
#include <map>
#include <fstream>

class DFA_Builder
{
public:
	DFA_Builder(std::string regex);

	void draw_min_dfa_graph(std::string file_name);
	void draw_dfa_graph(std::string file_name);
	void draw_nfa_graph(std::string file_name);
	void draw_syntax_tree(std::string file_name);

private:
	NFA_Builder* nfa_builder;

	DFA_Node* get_node(int id);
	DFA_Node* get_front();

	int is_unique(DFA_Agent* new_agent, DFA_Agent* agent, std::string cond);
	void z_eps(DFA_Agent* agent, NFA_Node* node);
	void _recursive_drawing(DFA_Node* node, std::ofstream* out);

	void minimize();
	int _find_group(int id, std::list<std::pair<int, std::list<int>*>>* splits);
	DFA_Node* get_mg_node(int g_id);

	std::map<std::string, int> abc;
	std::list<DFA_Agent*> agents;
	std::list<DFA_Node*> graph;
	std::list<DFA_Node*> min_graph;
};

