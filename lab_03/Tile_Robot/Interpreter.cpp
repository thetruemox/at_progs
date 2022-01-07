#include "Interpreter.h"


Interpreter::Interpreter(std::string file_name)
{
	std::ifstream file(file_name);
	if (!file.is_open()) throw("File not found!");

	std::string t_line;
	while (getline(file, t_line))
	{
		this->code.push_back(t_line);
	}

	try
	{
		_collect();
		_execute();
	}
	catch (const std::string& ex)
	{
		std::cerr << ex.c_str();
	}
	
}

void Interpreter::_collect()
{
	/* todo or not todo
	* �������� � ���� ������ �����: 
	*	
	*/

	//���� ��������. ������������ ��� �������� ����������� return � ������� � �������� ����� ����������� �����
	std::stack<std::string> exp_stack;

	std::string type("integer|string|(pointer)<(integer|string)>");
	std::string name("[a-zA-Z][a-zA-Z0-9]*");

	std::regex function_rx("(" + type + ")[ ](" + name + ")[(]((" + type + ")[ ](" + name + ")([,][ ]?(" + type + ")[ ](" + name + "))*)?[)]");
	std::regex start_rx("start");
	std::regex finish_rx("finish");
	std::regex return_rx("return[ ]((" + name + ")|(.*))[;]");
	
	std::cmatch cap_gr;

	Function* function = nullptr;

	for (int i = 0; i < code.size(); i++)
	{
		//����� ���������� �������
		if (regex_match(code[i].c_str(), cap_gr, function_rx))
		{
			/*
			* Group 1 � fun type
			* ���� ���������, ��
			*	Group 2 � fun type
			*	Group 3 � pointer type
			* Group 4 � fun name
			* Group 5 � fun argumets
			*/

			if (this->functions[cap_gr[4]] != nullptr) throw ("Function '" + cap_gr[4].str() + "' alredy exists, at line: " + std::to_string(i+1));
			if (code[i + 1] != "start") throw ("Expected 'start', after function declaration, at line: " + std::to_string(i+1));
			exp_stack.push("finish");
			exp_stack.push("return");

			//����������� ���� � ����� �������
			if (cap_gr[2].length() != 0 && cap_gr[3].length() != 0)
			{
				//������� ���������� ���������
				if (cap_gr[3] == "integer")
				{
					function = new Function(new Pointer("return", vt_Integer), cap_gr[4], i + 2);
				}
				else if (cap_gr[3] == "string")
				{
					function = new Function(new Pointer("return", vt_String), cap_gr[4], i + 2);
				}
			}
			else
			{
				if (cap_gr[1] == "integer")
				{
					function = new Function(new Integer("return"), cap_gr[4], i + 2);
				} 
				else if (cap_gr[1] == "string")
				{
					function = new Function(new String("return"), cap_gr[4], i + 2);
				}
			}

			//������� ���������� �������
			if (cap_gr[5].length() != 0)
			{			
				std::string value = cap_gr[5];
				static const std::regex rdelim{ ", " };
				std::vector<std::string> arg_pairs{
						std::sregex_token_iterator(value.begin(), value.end(), rdelim, -1),
						std::sregex_token_iterator()
				};

				std::cmatch arg_cg;
				std::regex arg_rx("(" + type + ")[ ](" + name + ")");

				/*
				* Group 1 � var type
				* ���� ���������, ��
				*	Group 2 � var type
				*	Group 3 � pointer type
				* Group 4 � var name
				*/

				for (int i = 0; i < arg_pairs.size(); i++)
				{
					regex_match(arg_pairs[i].c_str(), arg_cg, arg_rx);

					if (arg_cg[2].length() != 0 && arg_cg[3].length() != 0)
					{
						//�������� ���������
						if (arg_cg[3] == "integer")
						{
							function->add_arg(new Pointer(arg_cg[4], vt_Integer));
						}
						else if (cap_gr[3] == "string")
						{
							function->add_arg(new Pointer(arg_cg[4], vt_String));
						}
					}
					else
					{
						if (cap_gr[1] == "integer")
						{
							function->add_arg(new Integer(arg_cg[4]));
						}
						else if (cap_gr[1] == "string")
						{
							function->add_arg(new String(arg_cg[4]));
						}
					}
				}


			}
			
			//���������� ��������� ������� � ����
			this->functions[cap_gr[4]] = function;

			//���� ���������� ������� main, �� ��� ����������� � ���� ������
			if (cap_gr[4] == "main") this->call_stack.push(this->functions["main"]->get_start_i());

			i++;
			continue;
		}

		if (regex_match(code[i].c_str(), start_rx))
		{
			exp_stack.push("finish");
			continue;
		}

		if (regex_match(code[i].c_str(), finish_rx))
		{
			if (!exp_stack.empty() && exp_stack.top() != "finish") throw ("Expected '" + exp_stack.top() + "', at line: " + std::to_string(i+1));
			if (exp_stack.empty()) throw ("Extra 'finish' met, at line: " + std::to_string(i+1));
			exp_stack.pop();
			continue;
		}

		if (regex_match(code[i].c_str(), return_rx))
		{
			if (!exp_stack.empty() && exp_stack.top() != "return") throw ("Expected 'return', at line: " + std::to_string(i+1));
			if (exp_stack.empty()) throw ("Extra 'return' met, at line: " + std::to_string(i+1));
			exp_stack.pop();
			continue;
		}
	}

	if (!exp_stack.empty()) throw "Error '" + exp_stack.top() + "' excpected somewhere";
	if (this->call_stack.empty()) throw (std::string)("The 'main' function was not met");
}

void Interpreter::_execute()
{
	/*
	* ��������� �������� �� ������� ������� return � ������� main
	* ��� � ������ ���� ��� ���������� return, call_stack �������� ����
	*/

	/*
	* ����� ��������� ������������ ���� �������� �� �������������
	* ����� ����������, ���� ����������������� �������� ���� ����������
	* � ���� ������ ��������� �������� ������������
	*/

	std::cmatch cg;
	std::regex var_declaration_rx("((mutable)[ ])?(integer|string|pointer<(integer|string)>)[ ]([a-zA-Z][a-zA-Z0-9]*)((;)|:=(.+);)"); //cg[2]=mutable, cg[3]=var_type, cg[4]=pointer_type, cg[5]=var_name, cg[7]=only_declaration, cg[8]=assigment
	std::regex int_operand("(-?[0-9]+)|([a-zA-Z][a-zA-Z0-9]*)"); //cg[1]=number, cg[2]=variable
	std::regex str_operand("([\"].+[\"])|([a-zA-Z][a-zA-Z0-9]*)"); //cg[1]=str, cg[2]=variable

	Function* cur_context = this->functions["main"]; //�������, ������ ��������� ������� �������� �������������
	int GI = this->call_stack.top(); //���������� ������
	this->call_stack.pop();

	//������ ������ �������������� ������� GI
	while (true)
	{
		//���������� ����������
		if (regex_match(code[GI].c_str(), cg, var_declaration_rx))
		{
			if (cg[3] == "integer")
			{
				if (cg[2].length() == 0) //��� ��������� � �� ���������� ����������������
				{
					if (cg[7].length() != 0) throw (std::string)("Constant value not set, at line: " + std::to_string(GI+1));

					std::cmatch local_cg;
					std::string tmp_crutch = cg[8]; 
					if (regex_match(tmp_crutch.c_str(), local_cg, int_operand)) //�������� �� ��������� ���������� ��� ������ �����
					{
						if (local_cg[1].length() != 0) //number
						{
							cur_context->add_var(new Integer(cg[5], std::stoi(local_cg[1])));
						}
						else //variable
						{
							Integer* ref_var = dynamic_cast<Integer*>(cur_context->get_var(local_cg[2]));
							if (ref_var == nullptr) throw (std::string)("Incompatible types, at line: " + std::to_string(GI+1));

							cur_context->add_var(new Integer(cg[5], ref_var->get_value()));
						}
					}
					else //����� ��� �������������� ���������
					{
						ST_Calculator int_calculator(cg[8]);
						Integer* int_result = new Integer("tmp");
						int_calculator.calculate(int_result, cur_context, int_calculator.get_root());

						cur_context->add_var(new Integer(cg[5], int_result->get_value()));
						delete int_result;
					}
				}
				else //��� ���������� � �� ������������� �����������
				{
					Integer* new_var = new Integer(cg[5]);
					cur_context->add_var(new_var);

					if (cg[7].length() == 0) //����� ������������ �������������
					{
						std::cmatch local_cg;
						std::string tmp_crutch = cg[8];
						if (regex_match(tmp_crutch.c_str(), local_cg, int_operand)) //�������� �� ��������� ���������� ��� ������ �����
						{
							if (local_cg[1].length() != 0) //number
							{
								new_var->set_value(std::stoi(local_cg[1]));
							}
							else //variable
							{
								Integer* ref_var = dynamic_cast<Integer*>(cur_context->get_var(local_cg[2]));
								if (ref_var == nullptr) throw (std::string)("Incompatible types, at line: " + std::to_string(GI+1));

								new_var->set_value(ref_var->get_value());
							}
						}
						else //����� ��� �������������� ���������
						{
							ST_Calculator int_calculator(cg[8]);
							Integer* int_result = new Integer("tmp");
							int_calculator.calculate(int_result, cur_context, int_calculator.get_root());

							new_var->set_value(int_result->get_value());
							delete int_result;
						}
					}
				}
			}

			GI++;
			continue;
		}


	}

}
