#pragma once
#include "Variable.h"

class Pointer : virtual public Variable
{
public:
	Pointer(std::string var_name);
	var_type get_type() override;
	std::string get_name() override;

	void set_value(Variable* value);
	Variable* get_value();

private:
	std::string name;
	var_type type;
	Variable* value;
	bool is_mutable;
};