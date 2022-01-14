#pragma once
#include "Variables/Variable.h"
#include "Variables/Integer.h"
#include "Variables/String.h"
#include <vector>
#include <map>

class Function
{
public:
	Function(Variable* return_var, std::string name, int start_i);
	
	void add_var(Variable* variable);
	void add_arg(Variable* variable);
	Variable* get_var(std::string var_name);
	std::string get_fun_name();
	
	int get_start_i();
	int get_args_size();

	void set_arg(int index, int value); //������������� �������� ��������� ��� ������� �������
	
	var_type get_ret_type();
	Variable* get_return_var();

	void set_beneficiary(Variable* beneficiary); //��������� ����������� �������
	void make_benefit(); //�������� ����������� ������������ ��������

	Function* make_copy();

	void set_called_context(Function* context); //��������� ��������� �� �������� ���������� �������
	Function* get_called_context(); //��������� ��������� �� �������� ���� �������� �������
private:
	std::string name;
	std::vector<Variable**> args;
	std::map<std::string, Variable*> scope;

	Variable* return_var; //���������� �������, ���������� � ���� ������������ ��������
	Variable* beneficiary; //���������� �������, ����������, � ������� ����� ��������� �������� return_var

	Function* called_context; //������� ������� �� ������� ���� ������� ������

	int start_i;
};
