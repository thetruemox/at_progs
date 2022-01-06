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
	/*
	* В этом стеке находится номер позиции в коде, к которой 
	* необходимо вернуться в случае встречи return или начала 
	* программы
	*/
	std::stack<int> call_stack; 

	std::vector<std::string> code;
	std::map<std::string, Function*> functions;

	void _collect();
	void _execute();
};