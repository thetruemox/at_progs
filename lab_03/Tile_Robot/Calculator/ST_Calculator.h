#pragma once
#include <list>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <regex>

#include "ST_Node.h"
#include "../Function.h"
#include "../Variables/Integer.h"
#include "../Variables/String.h"

enum bracket_type
{
	BT_open, BT_close, bt_error
};

class ST_Calculator
{
public:
	ST_Calculator(std::string math_expr);
	ST_Node* get_root();
	void draw_syntax_tree(std::string file_name);

	void calculate(Integer* result, Function* cur_fun, ST_Node* cur_node);
	void calculate(String* result, Function* cur_fun, ST_Node* cur_node);
private:
	std::list<ST_Node*> nodes;
	std::list<std::pair<int, bracket_type>> brackets;
	ST_Node* root;

	void add_brackets(int open, int close);
	int is_this_bracket(int index);
	void create_node(char value, int index, stn_type type);
	void add_children(int parent_i, int child_i); 
	//Поиск самого старшего элемента в поддереве

	bracket_type _get_bracket_type(int index);
	ST_Node* _get_node(int index);
	stn_type _get_type(int index);
	ST_Node* _get_root();

	//Заменяет все не-операторы на @, а их значения и индексы помещаются в node_values
	std::string a_parse(std::string reg);
	std::map<int, std::string> node_values;
};
