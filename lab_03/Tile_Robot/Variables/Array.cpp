#include "Array.h"

Array::Array(std::string arr_name, var_type contained_type, int size)
{
	this->name = arr_name;
	this->type = vt_Array;
	this->contained_type = contained_type;
	this->is_mutable = true;
	this->size = size;

	this->value = new std::vector<Variable*>[size];

	switch (contained_type)
	{
	case vt_Integer:
		for (int i = 0; i < size; i++)
		{
			this->value->push_back(new Integer(std::to_string(i)));
		}
		break;
	default:
		break;
	}
}

var_type Array::get_contained_type()
{
	return this->contained_type;
}

var_type Array::get_type()
{
	return this->type;
}

std::string Array::get_name()
{
	return this->name;
}

void Array::set_value(Variable* value, int index)
{
	if (is_mutable)
	{
		if (value->get_type() == this->contained_type)
		{
			switch (contained_type)
			{
			case vt_Integer:
				dynamic_cast<Integer*>(this->value->at(index))->set_value(dynamic_cast<Integer*>(value)->get_value());
				break;
			case vt_String:
				dynamic_cast<String*>(this->value->at(index))->set_value(dynamic_cast<String*>(value)->get_value());
				break;
			case vt_Pointer:
				dynamic_cast<Pointer*>(this->value->at(index))->set_value(dynamic_cast<Pointer*>(value)->get_value());
				break;
			default:
				break;
			}
		}
		
	}
	else throw ("Assignment error, " + this->name + " is a const!");
}

void Array::set_value(std::vector<Variable*>* value)
{
	this->value = value;
}

int Array::get_size()
{
	return this->size;
}

Variable* Array::get_value(int index)
{
	return this->value->at(index);
}

std::vector<Variable*>* Array::get_value()
{
	return this->value;
}
