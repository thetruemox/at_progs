#pragma once
#include <list>
#include <map>
#include <fstream>
#include "Syntax_Tree.h"
#include "NFA_Graph.h"

class NFA_Builder
{
public:
	NFA_Builder(std::string regex);
	
	void draw_nfa_graph(std::string file_name);
	void draw_syntax_tree(std::string file_name);

	NFA_Node* get_start();
	NFA_Node* get_recieve();

	std::map<std::string, int> get_abc();
private:
	Syntax_Tree* stree;
	int nfa_nodes_id;
	NFA_Node* start;
	NFA_Node* recieve;	
	std::list<NFA_Graph*> graphs;
	std::map<std::string, int> abc;

	//Макнотон-Ямада-Томпсон 
	void build(ST_Node* node); 
	void create_graph(ST_Node* node);

	void _recursive_drawing(NFA_Node* node, std::ofstream* out);
	NFA_Graph* find_graph(int index);
};

