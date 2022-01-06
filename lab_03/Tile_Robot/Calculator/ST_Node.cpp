#include "ST_Node.h"

ST_Node::ST_Node(char sign, int index, stn_type type)
{
	this->is_checked = 0;
	this->sign = sign;
	this->index = index;
	this->type = type;
	this->parent = nullptr;
	this->left_ptr = nullptr;
	this->right_ptr = nullptr;
}