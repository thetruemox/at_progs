#pragma once
#include "Variable.h"
#include <vector>

class Function
{
public:
	
private:
	std::string name;
	std::vector<Variable*> scope;
	var_type return_type;
	int start_i;
};
