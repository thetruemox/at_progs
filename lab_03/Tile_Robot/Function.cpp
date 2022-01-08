#include "Function.h"

Function::Function(Variable* return_var, std::string name, int start_i)
{
	this->return_var = return_var;
	this->name = name;
	this->start_i = start_i;
	this->beneficiary = nullptr;
	this->called_context = nullptr;
}

void Function::add_var(Variable* variable)
{
	this->scope[variable->get_name()] = variable;
}

void Function::add_arg(Variable* variable)
{
	this->scope[variable->get_name()] = variable;
	this->args.push_back(&(this->scope[variable->get_name()]));
}

Variable* Function::get_var(std::string var_name)
{
	return this->scope[var_name];
}

std::string Function::get_fun_name()
{
	return this->name;
}

var_type Function::get_ret_type()
{
	return this->return_var->get_type();
}

Variable* Function::get_return_var()
{
	return this->return_var;
}

int Function::get_start_i()
{
	return this->start_i;
}

int Function::get_args_size()
{
	return this->args.size();
}

void Function::set_arg(int index, int value)
{
	Variable** ptr_tmp = this->args[index];
	Integer* tmp = dynamic_cast<Integer*>(*ptr_tmp);
	if (tmp == nullptr) throw (std::string)("The argument type does not match the parameter");
	tmp->set_value(value);
}

void Function::set_beneficiary(Variable* beneficiary)
{
	this->beneficiary = beneficiary;
}

void Function::make_benefit()
{
	if (this->beneficiary == nullptr) return;

	switch (this->return_var->get_type())
	{
	case vt_Integer:
		dynamic_cast<Integer*>(this->beneficiary)->set_value(dynamic_cast<Integer*>(this->return_var)->get_value());
		break;
	default:
		throw (std::string)("This type of beneficiary is not supported (yet?)");
		break;
	}
}

void Function::set_called_context(Function* context)
{
	this->called_context = context;
}

Function* Function::get_called_context()
{
	return this->called_context;
}
