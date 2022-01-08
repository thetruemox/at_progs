#include "Integer.h"

Integer::Integer(std::string var_name)
{
    this->name = var_name;
    this->type = vt_Integer;
    this->is_mutable = true;
}

Integer::Integer(std::string var_name, int value)
{
    this->name = var_name;
    this->type = vt_Integer;
    this->is_mutable = false;
    this->value = value;
}

var_type Integer::get_type()
{
    return this->type;
}

void Integer::set_value(int value)
{
    if (is_mutable)
    {
        this->value = value;
    }
    else throw (std::string)("Assignment error, " + this->name + " is a const!");
}

int Integer::get_value()
{
    return this->value;
}

std::string Integer::get_name()
{
    return this->name;
}
