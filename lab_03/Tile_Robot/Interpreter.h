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
	* ������ �������������� ������� � ���������� ���� �������: 
	* Collect (�������� ���������� � ��������)
	* �
	* Execute (��������� ��� ������ �������)
	*/

	Interpreter(std::string file_name);

private:
	std::stack<int> call_stack; //� ����� �������� ������� ������ ���������� �������
	std::vector<std::string> code;
	std::map<std::string, Function*> functions;

	void _collect();
	void _execute();
};