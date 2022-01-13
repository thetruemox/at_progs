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
#include "Robot/Robot.h"

class Interpreter
{
public:
	/*
	* ������ �������������� ������� � ���������� ���� �������: 
	* Collect (�������� ���������� � ��������)
	* �
	* Execute (��������� ��� ������ �������)
	*/
	Interpreter(std::string robocode_file, std::string labyrinth_file);

private:
	//� ���� ����� ��������� ����� ������� � ����, � ������� ���������� ��������� � ������ ������� return, finish ��� ������ ���������
	std::stack<int> call_stack;

	std::vector<std::string> code;
	std::map<std::string, Function*> functions;

	Robot* robot;

	void _collect();
	void _execute();
};