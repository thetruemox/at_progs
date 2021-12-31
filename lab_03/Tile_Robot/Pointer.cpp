#include "Pointer.h"

Pointer::Pointer(std::string var_name)
{
	this->name = var_name;
	this->type = vt_Pointer;
	this->is_mutable = true;
}

var_type Pointer::get_type()
{
	return this->type;
}

void Pointer::set_value(Variable* value)
{
	if (is_mutable)
	{
		this->value = value;
	}
	else throw ("Assignment error, " + this->name + " is a const!");
}

Variable* Pointer::get_value()
{
	return this->value;
}

std::string Pointer::get_name()
{
	return this->name;
}
