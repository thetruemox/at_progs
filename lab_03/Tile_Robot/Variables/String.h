#pragma once
#include "Variable.h"

class String : virtual public Variable
{
public:
	//При этом объявлении создается переменная
	String(std::string var_name);
	//При этом объявлении создается константа
	String(std::string var_name, std::string value);
	var_type get_type() override;
	std::string get_name() override;

	void set_value(std::string value);
	void set_value(char value, int index);

	std::string get_value();
	char get_value(int index);

private:
	std::string name;
	var_type type;
	std::string value;
	bool is_mutable;
};