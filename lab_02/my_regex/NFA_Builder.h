#pragma once
#include <list>
#include <fstream>
#include "Syntax_Tree.h"
#include "NFA_Graph.h"

class NFA_Builder
{
public:
	NFA_Builder();
	void build(Syntax_Tree* stree, ST_Node* node);
	void draw_graph();
	
private:
	void create_graph(ST_Node* node);
	void _recursive_drawing(NFA_Node* node, std::ofstream* out);
	NFA_Graph* find_graph(int index);

	int nfa_nodes_id;

	NFA_Node* start;
	NFA_Node* recieve;
	
	std::list<NFA_Graph*> graphs;
};

