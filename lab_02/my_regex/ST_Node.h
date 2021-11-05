#pragma once
#include <list>

enum stn_type 
{
	a_node, plus_node, cat_node, or_node, stn_error
};

class ST_Node
{
public:
	ST_Node(char value, int index, stn_type type);

	char value;
	int index;
	ST_Node* parent;
	ST_Node* left_ptr;
	ST_Node* right_ptr;
	stn_type type;

private:
};
