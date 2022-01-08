#pragma once
#include "Variable.h"

class Integer : virtual public Variable
{
public:
	//При этом объявлении создается переменная
	Integer(std::string var_name);
	//При этом объявлении создается константа
	Integer(std::string var_name, int value);
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