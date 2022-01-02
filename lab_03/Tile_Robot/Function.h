#pragma once
#include "Variable.h"
#include <vector>
#include <map>

class Function
{
public:
	
private:
	std::string name;
	std::map<std::string, Variable*> scope;
	var_type return_type;
	int start_i;
};
