#include "Function.h"

Function::Function(Variable* return_var, std::string name, int start_i)
{
	this->return_var = return_var;
	this->name = name;
	this->start_i = start_i;
}

void Function::add_var(Variable* variable)
{
	this->scope[variable->get_name()] = variable;
}

void Function::add_arg(Variable* variable)
{
	this->scope[variable->get_name()] = variable;
	this->args.push_back(&variable);
}

Variable* Function::get_var(std::string var_name)
{
	return this->scope[var_name];
}

int Function::get_start_i()
{
	return this->start_i;
}
