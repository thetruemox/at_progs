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
	}
	catch (const std::string& ex)
	{
		std::cerr << ex.c_str();
	}
	
}

void Interpreter::_collect()
{
	/* todo or not todo
	* ¬озможно в этом методе стоит: 
	*	
	*/

	//—тек ожидани€. »спользуетс€ дл€ проверки присутстви€ return у функций и закрытых групп предложений €зыка
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
		//ѕоиск объ€влени€ функции
		if (regex_match(code[i].c_str(), cap_gr, function_rx))
		{
			/*
			* Group 1 Ц fun type
			* ≈сли указатель, то
			*	Group 2 Ц fun type
			*	Group 3 Ц pointer type
			* Group 4 Ц fun name
			* Group 5 Ц fun argumets
			*/

			if (this->functions[cap_gr[4]] != nullptr) throw ("Function '" + cap_gr[4].str() + "' alredy exists, at line: " + std::to_string(i+1));
			if (code[i + 1] != "start") throw ("Expected 'start', after function declaration, at line: " + std::to_string(i+1));
			exp_stack.push("finish");
			exp_stack.push("return");

			//ќпределение типа и имени функции
			if (cap_gr[2].length() != 0 && cap_gr[3].length() != 0)
			{
				//‘ункци€ возвращает указатель
				if (cap_gr[3] == "integer")
				{
					function = new Function(new Pointer("return", vt_Integer), cap_gr[4], i + 3);
				}
				else if (cap_gr[3] == "string")
				{
					function = new Function(new Pointer("return", vt_String), cap_gr[4], i + 3);
				}
			}
			else
			{
				if (cap_gr[1] == "integer")
				{
					function = new Function(new Integer("return"), cap_gr[4], i + 3);
				} 
				else if (cap_gr[1] == "string")
				{
					function = new Function(new String("return"), cap_gr[4], i + 3);
				}
			}

			//ѕарсинг параметров функции
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
				* Group 1 Ц var type
				* ≈сли указатель, то
				*	Group 2 Ц var type
				*	Group 3 Ц pointer type
				* Group 4 Ц var name
				*/

				for (int i = 0; i < arg_pairs.size(); i++)
				{
					regex_match(arg_pairs[i].c_str(), arg_cg, arg_rx);

					if (arg_cg[2].length() != 0 && arg_cg[3].length() != 0)
					{
						//јргумент указатель
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
			
			//ƒобавление собранной функции в мапу
			this->functions[cap_gr[4]] = function;

			//≈сли обработана функци€ main, то она добавл€етс€ в стек вызова
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
