#include "Interpreter.h"


Interpreter::Interpreter(std::string robocode_file, std::string labyrinth_file)
{
	std::ifstream file(robocode_file);
	if (!file.is_open()) throw (std::string)("File not found!");

	std::string t_line;
	while (getline(file, t_line))
	{
		this->code.push_back(t_line);
	}

	this->robot = new Robot(labyrinth_file);

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
			//exp_stack.push("return");

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

		/*
		if (regex_match(code[i].c_str(), return_rx))
		{
			if (!exp_stack.empty() && exp_stack.top() != "return") throw ("Expected 'return', at line: " + std::to_string(i+1));
			if (exp_stack.empty()) throw ("Extra 'return' met, at line: " + std::to_string(i+1));
			exp_stack.pop();
			continue;
		}
		*/
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
	std::regex var_assignment_rx("([a-zA-Z][a-zA-Z0-9]*):=(([a-zA-Z][a-zA-Z0-9]*)|(.+));"); //cg[1]=var_name, cg[3]=variable, cg[4]=expr

	std::regex var_operand("([a-zA-Z][a-zA-Z0-9]*)"); //cg[1]=variable
	std::regex int_operand("(-?[0-9]+)|([a-zA-Z][a-zA-Z0-9]*)"); //cg[1]=number, cg[2]=variable
	std::regex str_operand("([\"].+[\"])|([a-zA-Z][a-zA-Z0-9]*)"); //cg[1]=str, cg[2]=variable
	
	std::regex checkzero_rx("checkzero[ ][(](.+)[)]"); //cg[1]=expr
	std::regex while_rx("while[ ][(](.+)[)]"); //cg[1]=expr

	std::regex finish_rx("finish");
	std::regex break_rx("break;");

	std::regex fun_call_rx("(([a-zA-Z][a-zA-Z0-9]*):=)?call[ ]([a-zA-Z][a-zA-Z0-9]*)[ ]with[ ][(](.*)[)][;]"); //cg[2]=beneficiary_name, cg[3]=fun_name, cg[4]=args
	std::regex return_rx("return[ ]((-?[0-9]+)|([\"].+[\"])|([a-zA-Z][a-zA-Z0-9]*));"); //cg[2]=number, cg[3]=str, cg[4]=variable

	std::regex robo_methods_rx("(top)|(right)|(bot)|(left)|(timeshift[(](-?[0-9]+)[)])"); //cg[1]=top, cg[2]=right, cg[3]=bot, cg[4]=left, cg[5]=timeshift, cg[6]=timeshift_arg

	Function* cur_context = this->functions["main"]; //�������, ������ ��������� ������� �������� �������������
	int GI = this->call_stack.top(); //���������� ������
	this->call_stack.pop();

	//������ ������ �������������� ������� GI, ���� ����������� ��� ���������� return ������� main ��� ��� ������ ������ �� ���������
	while (!robot->is_win())
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
					//�������� �� ������������� ������ ������
					std::string tmp_crutch = cg[8];
					std::cmatch local_cg;
					if (regex_match(tmp_crutch.c_str(), local_cg, robo_methods_rx))
					{
						if (local_cg[1].length() != 0) //top
						{
							Integer* new_var = new Integer(cg[5]);
							cur_context->add_var(new_var);
							new_var->set_value(this->robot->top());
						}
						else if (local_cg[2].length() != 0) //right
						{
							Integer* new_var = new Integer(cg[5]);
							cur_context->add_var(new_var);
							new_var->set_value(this->robot->right());
						}
						else if (local_cg[3].length() != 0) //bot
						{
							Integer* new_var = new Integer(cg[5]);
							cur_context->add_var(new_var);
							new_var->set_value(this->robot->bot());
						}
						else if (local_cg[4].length() != 0) //left
						{
							Integer* new_var = new Integer(cg[5]);
							cur_context->add_var(new_var);
							new_var->set_value(this->robot->left());
						}
						else //timeshift
						{
							throw (std::string)("Timeshift doesn`t return any value");
						}

						GI++;
						continue;
					}

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
			else if (cg[3] == "string")
			{
				if (cg[2].length() == 0) //��� ��������� � �� ���������� ����������������
				{
					if (cg[7].length() != 0) throw (std::string)("Constant value not set, at line: " + std::to_string(GI + 1));

					std::cmatch local_cg;
					std::string tmp_crutch = cg[8];
					if (regex_match(tmp_crutch.c_str(), local_cg, str_operand)) //�������� �� ��������� ���������� ��� ������ ������
					{
						if (local_cg[1].length() != 0) //str
						{
							cur_context->add_var(new String(cg[5], local_cg[1]));
						}
						else //variable
						{
							String* ref_var = dynamic_cast<String*>(cur_context->get_var(local_cg[2]));
							if (ref_var == nullptr) throw (std::string)("Incompatible types, at line: " + std::to_string(GI + 1));

							cur_context->add_var(new String(cg[5], ref_var->get_value()));
						}
					}
				}
				else //��� ���������� � �� ������������� �����������
				{
					String* new_var = new String(cg[5]);
					cur_context->add_var(new_var);

					if (cg[7].length() == 0) //����� ������������ �������������
					{
						std::cmatch local_cg;
						std::string tmp_crutch = cg[8];
						if (regex_match(tmp_crutch.c_str(), local_cg, str_operand)) //�������� �� ��������� ���������� ��� ������ ������
						{
							if (local_cg[1].length() != 0) //str
							{
								new_var->set_value(local_cg[1]);
							}
							else //variable
							{
								String* ref_var = dynamic_cast<String*>(cur_context->get_var(local_cg[2]));
								if (ref_var == nullptr) throw (std::string)("Incompatible types, at line: " + std::to_string(GI + 1));

								new_var->set_value(ref_var->get_value());
							}
						}
					}
				}
			}

			GI++;
			continue;
		}
		
		//����� �������
		if (regex_match(code[GI].c_str(), cg, fun_call_rx))
		{
			Function* called_fun = this->functions[cg[3]];
			if (called_fun == nullptr) throw (std::string)("There is no such function '" + cg[3].str() + "', at line: " + std::to_string(GI + 1));

			//�������� �����������
			if (cg[2].length() != 0)
			{
				Variable* beneficiary = cur_context->get_var(cg[2]);
				if (beneficiary == nullptr) throw (std::string)("There is no such variable '" + cg[2].str() + "', at line: " + std::to_string(GI + 1));
				if (beneficiary->get_type() != called_fun->get_ret_type()) throw (std::string)("The return type does not match the variable, at line: " + std::to_string(GI + 1));

				called_fun->set_beneficiary(beneficiary);
			}
			else called_fun->set_beneficiary(nullptr);

			//�������� ����������
			if (cg[4].length() != 0)
			{
				std::string value = cg[4];
				static const std::regex rdelim{ ", " };
				std::vector<std::string> arg_pairs{
						std::sregex_token_iterator(value.begin(), value.end(), rdelim, -1),
						std::sregex_token_iterator()
				};

				if (arg_pairs.size() != called_fun->get_args_size()) throw (std::string)("Argument list does not match function requirements, at line: " + std::to_string(GI + 1));

				//�������� ���������� �������
				for (int i = 0; i < arg_pairs.size(); i++)
				{
					std::cmatch local_cg;

					if (regex_match(arg_pairs[i].c_str(), local_cg, var_operand)) //����������
					{
						Variable* var_arg = cur_context->get_var(local_cg[1]);
						if (var_arg == nullptr) throw (std::string)("There is no such variable '" + local_cg[1].str() + "', at line: " + std::to_string(GI + 1));

						switch (var_arg->get_type())
						{
						case vt_Integer:
							called_fun->set_arg(i, dynamic_cast<Integer*>(var_arg)->get_value());
							break;
						default:
							throw (std::string)("This type of argument is not supported (yet?)");
							break;
						}

						continue;
					}

					if (regex_match(arg_pairs[i].c_str(), local_cg, int_operand)) //�����
					{
						called_fun->set_arg(i, std::stoi(local_cg[1]));
					}
				}

			}
			else //���������� ���
			{
				if (called_fun->get_args_size() != 0) throw (std::string)("Argument list does not match function requirements, at line: " + std::to_string(GI + 1));
			}

			//������� � ���������� �������
			this->call_stack.push(GI + 1);
			GI = called_fun->get_start_i();

			called_fun->set_called_context(cur_context);
			cur_context = called_fun;

			continue;
		}

		//������������ ����������
		if (regex_match(code[GI].c_str(), cg, var_assignment_rx))
		{
			Variable* orig_var;
			Variable* fun_var;

			//�������� �� ������������� ������ ������
			std::string crutch = cg[3];
			std::cmatch local_cg;
			if (regex_match(crutch.c_str(), local_cg, robo_methods_rx))
			{
				orig_var = dynamic_cast<Integer*>(cur_context->get_var(cg[1]));
				if (orig_var == nullptr) throw (std::string)("Incompatible types, at line: " + std::to_string(GI + 1));

				if (local_cg[1].length() != 0) //top
				{
					dynamic_cast<Integer*>(orig_var)->set_value(this->robot->top());
				}
				else if (local_cg[2].length() != 0) //right
				{
					dynamic_cast<Integer*>(orig_var)->set_value(this->robot->right());
				}
				else if (local_cg[3].length() != 0) //bot
				{
					dynamic_cast<Integer*>(orig_var)->set_value(this->robot->bot());
				}
				else if (local_cg[4].length() != 0) //left
				{
					dynamic_cast<Integer*>(orig_var)->set_value(this->robot->left());
				}
				else //timeshift
				{
					throw (std::string)("Timeshift doesn`t return any value");
				}

				GI++;
				continue;
			}

			switch (cur_context->get_var(cg[1])->get_type())
			{
			case vt_Integer:
				orig_var = dynamic_cast<Integer*>(cur_context->get_var(cg[1]));
				fun_var = nullptr;

				if (cg[3].length() != 0) //����������
				{
					fun_var = dynamic_cast<Integer*>(cur_context->get_var(cg[3]));
					dynamic_cast<Integer*>(orig_var)->set_value(dynamic_cast<Integer*>(fun_var)->get_value());
				}
				else if (cg[4].length() != 0) //����� ��� ���������
				{
					std::cmatch local_cg;
					std::string crutch = cg[4];
					regex_match(crutch.c_str(), local_cg, int_operand);

					if (local_cg[1].length() != 0) //�����
					{
						dynamic_cast<Integer*>(orig_var)->set_value(std::stoi(local_cg[1]));
					}
					else //���������
					{
						Integer* result = new Integer("result");
						ST_Calculator int_calculator(crutch);
						int_calculator.calculate(result, cur_context, int_calculator.get_root());
						dynamic_cast<Integer*>(orig_var)->set_value(result->get_value());
						delete result;
					}
				}

				break;
			case vt_String:
				orig_var = dynamic_cast<String*>(cur_context->get_var(cg[1]));
				fun_var = nullptr;

				if (cg[3].length() != 0) //����������
				{
					fun_var = dynamic_cast<Integer*>(cur_context->get_var(cg[3]));
					dynamic_cast<String*>(orig_var)->set_value(dynamic_cast<String*>(fun_var)->get_value());
				}
				else if (cg[4].length() != 0) //������
				{
					std::cmatch local_cg;
					std::string crutch = cg[4];
					regex_match(crutch.c_str(), local_cg, str_operand);

					if (local_cg[1].length() != 0) //������
					{
						dynamic_cast<String*>(orig_var)->set_value(local_cg[1]);
					}
					else throw (std::string)("Attempt to assign an unknown value to a string, at line: " + std::to_string(GI + 1));
				}

				break;
			default:
				throw (std::string)("This type not supported, at line: " + std::to_string(GI+1));
				break;
			}

			GI++;
			continue;
		}

		//�������� ��������
		if (regex_match(code[GI].c_str(), cg, checkzero_rx))
		{
			//���� ���������� start, �� _collect ����������� ������� finish
			if (code[GI + 1] != "start") throw (std::string)("Expected 'start', at line: " + std::to_string(GI+2));
			
			/*
			* ���� ��������� ����� 0, �� ����������� ������ ���� � ����� 
			* ��������� �� call_stack � ����� ��������� ���������			
			*/

			int checkzero_i = GI; //������ ��������� checkzero
			int instead_i = -1; //������ ��������� instead
			int finish_i = -1; //������ ����� ��������� ���������
			int finish_order = 1; //������� ������������ finish, ������������ ��� ������ ������ ����� ����� ������ ��������� ���������

			GI++; //����� �� ��������� ��� ��� start
			while (1)
			{
				GI++;
				if (code[GI] == "start") finish_order++;

				if (code[GI] == "finish" && finish_order == 1)
				{
					if (code[GI + 1] == "instead")
					{
						if (code[GI + 2] != "start") throw (std::string)("Expected 'start', at line: " + std::to_string(GI + 3));
						instead_i = GI + 1;
						GI += 2; //������� instead � ��� start, ��� ����������� ���������
					}
					else
					{ 
						finish_i = GI;
						break;
					}
				}
				else if (code[GI] == "finish" && finish_order > 1)
				{
					finish_order--;
				}
			}

			Integer* result = new Integer("result");

			std::string crutch = cg[1];
			std::cmatch local_cg;
			if (regex_match(crutch.c_str(), local_cg, robo_methods_rx))
			{
				if (local_cg[1].length() != 0) //top
				{
					result->set_value(this->robot->top());
				}
				else if (local_cg[2].length() != 0) //right
				{
					result->set_value(this->robot->right());
				}
				else if (local_cg[3].length() != 0) //bot
				{
					result->set_value(this->robot->bot());
				}
				else if (local_cg[4].length() != 0) //left
				{
					result->set_value(this->robot->left());
				}
				else //timeshift
				{
					throw (std::string)("Timeshift doesn`t return any value");
				}
			}
			else 
			{
				ST_Calculator int_calculator(cg[1]);
				int_calculator.calculate(result, cur_context, int_calculator.get_root());
			}

			if (result->get_value() == 0)
			{
				GI = checkzero_i + 2;
				this->call_stack.push(finish_i + 1);
			}
			else if (instead_i != -1)
			{
				GI = instead_i + 2;
				this->call_stack.push(finish_i + 1);
			}
			else
			{
				GI = finish_i;
				this->call_stack.push(finish_i + 1);
			}

			delete result;
			continue;
		}

		//���� while
		if (regex_match(code[GI].c_str(), cg, while_rx))
		{
			//���� ���������� start, �� _collect ����������� ������� finish
			if (code[GI + 1] != "start") throw (std::string)("Expected 'start', at line: " + std::to_string(GI + 2));

			/*
			* ���� ��������� ����� 1, �� � call_stack ��������� GI ������ while
			* ����������� ���� ����� �� finish, ����� ��������� ����� �� �������� 
			* ���� ��������� ����� 0, �� ������ � ����������� ���� instead
			* GI ����������� �� ����� while
			*/

			ST_Calculator int_calculator(cg[1]);
			Integer* result = new Integer("result");
			int_calculator.calculate(result, cur_context, int_calculator.get_root());

			if (result->get_value() == 1) //���� ������� �����������, �� �������� � ����� �����
			{
				call_stack.push(GI);
				GI+=2; //������� � ���� �����
				delete result;
				continue;
			}

			//���� �� ��������, �� ���� instead ��� ����� while`�
			GI++; //�� while ��������� � start
			int finish_order = 1;
			int instead_i = -1; //GI instead`a, �� ������ ���� ��������
			int while_end_i = -1;
			while (1)
			{
				GI++;
				if (code[GI] == "start") finish_order++;

				if (code[GI] == "finish" && finish_order == 1)
				{
					if (code[GI + 1] == "instead")
					{
						if (code[GI + 2] != "start") throw (std::string)("Expected 'start', at line: " + std::to_string(GI + 3));		
						instead_i = GI + 1;
						GI += 2; //���������� instead � start, ����� ���������� ������ ����� while`a
					}
					else
					{
						while_end_i = GI;
						call_stack.push(GI + 1); //����� while`a
						break;
					}
				}
				else if (code[GI] == "finish" && finish_order > 1)
				{
					finish_order--;
				}
			}

			if (instead_i != -1) //������� instead, ���������
			{
				GI = instead_i + 2; //����� ��� �������� �� instead � ��� ����
			}
			else 
			{
				GI = while_end_i; //������� �� finish � ����� while`a, ����� ���������� call_stack
			}

			continue;
		}

		//Break (��� �������� �� ������ "Break out of cycle")
		if (regex_match(code[GI].c_str(), break_rx))
		{
			if (this->call_stack.empty()) throw (std::string)("Call_stack was empty, at line: " + std::to_string(GI + 1));

			GI = call_stack.top();
			while (!regex_match(code[GI].c_str(), while_rx))
			{
				call_stack.pop();
				GI = call_stack.top();
			}

			call_stack.pop();

			//���� ����� while`�
			GI++; //�� while ��������� � start
			int finish_order = 1;
			while (1)
			{
				GI++;
				if (code[GI] == "start") finish_order++;

				if (code[GI] == "finish" && finish_order == 1)
				{
					if (code[GI + 1] == "instead")
					{
						GI += 2; //���������� instead � start, ����� ���������� ������ ����� while`a
					}
					else
					{
						call_stack.push(GI + 1); //����� while`a
						break;
					}
				}
				else if (code[GI] == "finish" && finish_order > 1)
				{
					finish_order--;
				}
			}

			continue;
		}

		//����� ������
		if (regex_match(code[GI].c_str(), finish_rx))
		{
			if (this->call_stack.empty()) throw (std::string)("Call_stack was empty, at line: " + std::to_string(GI+1));

			GI = this->call_stack.top();
			this->call_stack.pop();
			continue;
		}

		//������������ ��������
		if (regex_match(code[GI].c_str(), cg, return_rx))
		{
			if (cg[2].length() != 0) //������������ �����
			{
				if (cur_context->get_ret_type() != vt_Integer) throw (std::string)("The return type does not match the type of the function, at line: " + std::to_string(GI+1));
				dynamic_cast<Integer*>(cur_context->get_return_var())->set_value(std::stoi(cg[2]));
			}
			else if (cg[3].length() != 0) //������������ ������
			{
				if (cur_context->get_ret_type() != vt_String) throw (std::string)("The return type does not match the type of the function, at line: " + std::to_string(GI + 1));
				dynamic_cast<String*>(cur_context->get_return_var())->set_value(cg[2]);
			}
			else //������������ ����������
			{
				Variable* ret_var = cur_context->get_var(cg[4]);
				if (ret_var == nullptr) throw (std::string)("There is no such variable '" + cg[4].str() + "', at line: " + std::to_string(GI + 1));
				if (cur_context->get_ret_type() != ret_var->get_type()) throw (std::string)("The return type does not match the type of the function, at line: " + std::to_string(GI + 1));
				
				switch (ret_var->get_type())
				{
				case vt_Integer:
					dynamic_cast<Integer*>(cur_context->get_return_var())->set_value(dynamic_cast<Integer*>(ret_var)->get_value());
					break;
				case vt_String:
					dynamic_cast<String*>(cur_context->get_return_var())->set_value(dynamic_cast<String*>(ret_var)->get_value());
					break;
				default:
					throw (std::string)("This type of return is not supported (yet?), at line: " + std::to_string(GI + 1));
					break;
				}
			}

			cur_context->make_benefit();
			cur_context = cur_context->get_called_context();

			if (cur_context == nullptr) break; //Return �� main, ���������� ������ ���������

			GI = call_stack.top();
			call_stack.pop();

			continue;
		}

		//������� ������
		if (regex_match(code[GI].c_str(), cg, robo_methods_rx))
		{
			if (cg[1].length() != 0) //top
			{
				Integer* new_var = new Integer(cg[5]);
				cur_context->add_var(new_var);
				new_var->set_value(this->robot->top());
			}
			else if (cg[2].length() != 0) //right
			{
				Integer* new_var = new Integer(cg[5]);
				cur_context->add_var(new_var);
				new_var->set_value(this->robot->right());
			}
			else if (cg[3].length() != 0) //bot
			{
				Integer* new_var = new Integer(cg[5]);
				cur_context->add_var(new_var);
				new_var->set_value(this->robot->bot());
			}
			else if (cg[4].length() != 0) //left
			{
				Integer* new_var = new Integer(cg[5]);
				cur_context->add_var(new_var);
				new_var->set_value(this->robot->left());
			}
			else //timeshift
			{
				this->robot->time_shift(std::stoi(cg[6]));
			}

			GI++;
			continue;
		}

		GI++; //��������(?), �� ����� ���������� ������ ���� ������������� ����� �� �� ��������� 
		//std::cout << code[GI] << std::endl;
	}

	if (robot->is_win())
	{
		std::cout << "The robot got out of the labyrinth!" << std::endl;
	}
	else
	{
		//������ ������� main ���������� int, �� ��� ������� � ������������� ����� ��������� case �� ������� ����� �����

		std::cout << "Function main returned " + std::to_string(dynamic_cast<Integer*>(this->functions["main"]->get_return_var())->get_value());
		//std::cout << "Function main returned " + dynamic_cast<String*>(this->functions["main"]->get_return_var())->get_value();
	}

}
