#pragma once
#include <list>
#include <string>

enum stn_type 
{
	a_node, plus_node, cat_node, or_node, stn_error
};

class ST_Node
{
public:
	ST_Node(char value, int index, stn_type type);

	int is_checked;

	std::string value;
	int index;
	ST_Node* parent;
	ST_Node* left_ptr;
	ST_Node* right_ptr;
	stn_type type;

private:
};
