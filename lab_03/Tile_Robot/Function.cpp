#include "Function.h"

Function::Function(Variable* return_var, std::string name, std::vector<Variable**> args, int start_i)
{
	this->return_var = return_var;
	this->name = name;
	this->args = args;
	this->start_i = start_i;
}

void Function::add_var(Variable* variable)
{
	this->scope[variable->get_name()] = variable;
}

Variable* Function::get_var(std::string var_name)
{
	return this->scope[var_name];
}
