#pragma once
#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "ST_Node.h"
#include "Capture_Groups.h"

enum bracket_type
{
	BT_open, BT_close, bt_error
};

class Syntax_Tree
{
public:
	Syntax_Tree(std::string regex);
	ST_Node* get_root();
	void draw_syntax_tree(std::string file_name);

	Capture_Groups* get_CG() {return this->CG;}

private:
	void add_brackets(int open, int close);
	int is_this_bracket(int index);
	void create_node(char value, int index, stn_type type);
	void add_children(int parent_i, int child_i); 
	//Поиск самого старшего элемента в поддереве

	std::string rep_parse(std::string reg);
	std::string cg_parse(std::string reg);

	bracket_type _get_bracket_type(int index);
	ST_Node* _get_node(int index);
	stn_type _get_type(int index);
	ST_Node* _get_root();

	std::list<ST_Node*> nodes;
	std::list<std::pair<int, bracket_type>> brackets;
	ST_Node* root;
	Capture_Groups* CG;
};
