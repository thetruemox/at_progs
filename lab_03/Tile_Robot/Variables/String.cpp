#include "String.h"

String::String(std::string var_name)
{
    this->name = var_name;
    this->type = vt_String;
    this->is_mutable = true;
}

var_type String::get_type()
{
    return this->type;
}

void String::set_value(std::string value)
{
    if (is_mutable)
    {
        this->value = value;
    }
    else throw ("Assignment error, " + this->name + " is a const!");
}

void String::set_value(char value, int index)
{
    if (is_mutable)
    {
        this->value[index] = value;
    }
    else throw ("Assignment error, " + this->name + " is a const!");
}

std::string String::get_value()
{
    return this->value;
}

char String::get_value(int index)
{
    return this->value[index];
}

std::string String::get_name()
{
    return this->name;
}
