#include "ST_Node.h"

ST_Node::ST_Node(char value, int index, stn_type type)
{
	this->value = value;
	this->index = index;
	this->type = type;
	this->parent = nullptr;
	this->left_ptr = nullptr;
	this->right_ptr = nullptr;
}