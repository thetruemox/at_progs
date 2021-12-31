#pragma once
#include "Variable.h"
#include "Integer.h"
#include "Pointer.h"
#include "String.h"
#include <vector>

class Array : virtual public Variable
{
public:
	Array(std::string arr_name, var_type contained_type);
	var_type get_contained_type();
	var_type get_type() override;
	std::string get_name() override;

	void set_value(Variable* value, int index);
	void set_value(std::vector<Variable*> value);

	Variable* get_value(int index);
	std::vector<Variable*> get_value();

private:
	std::string name;

	var_type type;
	var_type contained_type;

	std::vector<Variable*> value;
	bool is_mutable;
};