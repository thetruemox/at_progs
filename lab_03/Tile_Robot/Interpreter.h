#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <regex>
#include <stack>

#include "Calculator/ST_Calculator.h"
#include "Function.h"
#include "Variables/Variable.h"
#include "Variables/String.h"
#include "Variables/Pointer.h"

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
	/*
	* � ���� ����� ��������� ����� ������� � ����, � ������� 
	* ���������� ��������� � ������ ������� return ��� ������ 
	* ���������
	*/
	std::stack<int> call_stack; 

	std::vector<std::string> code;
	std::map<std::string, Function*> functions;

	void _collect();
	void _execute();
};