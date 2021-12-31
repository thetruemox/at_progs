#pragma once
#include "Variable.h"

class Integer : virtual public Variable
{
public:
	Integer(std::string var_name);
	var_type get_type() override;
	std::string get_name() override;

	void set_value(int value);
	int get_value();

private:
	std::string name;
	var_type type;
	int value;
	bool is_mutable;
};