#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <regex>
#include <stack>

#include "Function.h"
#include "Integer.h"
#include "String.h"
#include "Pointer.h"

class Interpreter
{
public:
	/*
	* Работа интерпретатора состоит в исполнении двух функций: 
	* Collect (собирает информацию о функциях)
	* и
	* Execute (выполняет код внутри функций)
	*/

	Interpreter(std::string file_name);

private:
	std::stack<int> call_stack; //В стеке хранится позиция начала вызываемой функции
	std::vector<std::string> code;
	std::map<std::string, Function*> functions;

	void _collect();
	void _execute();
};