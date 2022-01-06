#pragma once
#include <list>
#include <string>
#include "../Variable.h"

enum stn_type 
{
	a_node, mult_node, div_node, mod_node, plus_node, sub_node, even_node, more_node, less_node, stn_error
};

class ST_Node
{
public:
	ST_Node(char sign, int index, stn_type type);

	int is_checked;

	char sign;
	Variable* value;

	int index;
	ST_Node* parent;
	ST_Node* left_ptr;
	ST_Node* right_ptr;
	stn_type type;

private:
};
