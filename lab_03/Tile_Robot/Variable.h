#pragma once
#include <string>

enum var_type
{
	vt_Variable, vt_Integer, vt_String, vt_Pointer, vt_Array
};

class Variable
{
public:
	virtual var_type get_type() = 0;
	virtual std::string get_name() = 0;
};

