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
	NFA_Builder(Syntax_Tree* stree);

	void draw_nfa_graph(std::string file_name);
	void draw_syntax_tree(std::string file_name);

	Syntax_Tree* get_stree();
	NFA_Node* get_start();
	NFA_Node* get_recieve();
	Capture_Groups* get_CG();

	std::map<std::string, int> get_abc();
private:
	Syntax_Tree* stree;
	int nfa_nodes_id;
	NFA_Node* start;
	NFA_Node* recieve;	
	std::list<NFA_Graph*> graphs;
	std::map<std::string, int> abc;

	Capture_Groups* CG;


	//��������-�����-������� 
	void build(ST_Node* node); 
	void create_graph(ST_Node* node);

	void _recursive_drawing(NFA_Node* node, std::ofstream* out);
	NFA_Graph* find_graph(int index);
	void cg_around_graph(NFA_Node* A, NFA_Node* B, CG_Unit* unit);
};

