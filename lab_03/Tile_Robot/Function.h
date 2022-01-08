#pragma once
#include "Variables/Variable.h"
#include <vector>
#include <map>

class Function
{
public:
	Function(Variable* return_var, std::string name, int start_i);
	
	void add_var(Variable* variable);
	void add_arg(Variable* variable);
	Variable* get_var(std::string var_name);
	std::string get_fun_name();
	int get_start_i();

private:
	std::string name;
	std::vector<Variable**> args;
	std::map<std::string, Variable*> scope;
	Variable* return_var;

	int start_i;
};
