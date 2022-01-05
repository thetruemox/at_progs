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
	//Стек ожидания. Используется для проверки присутствия return у функций и закрытых групп предложений языка
	std::stack<std::string> exp_stack;

	std::string type("integer|string|(pointer)<(integer|string)>");
	std::string name("[a-zA-Z][a-zA-Z0-9]*");

	std::regex function_rx("(" + type + ")[ ](" + name + ")[(]((" + type + ")[ ](" + name + ")([,][ ]?(" + type + ")[ ](" + name + "))*)?[)]");
	std::regex start_rx("start");
	std::regex finish_rx("finish");
	std::regex return_rx("return[ ]((" + name + ")|(.*))[;]");
	
	std::cmatch cap_gr;

	Function* function;

	for (int i = 0; i < code.size(); i++)
	{
		//Поиск объявления функции
		if (regex_match(code[i].c_str(), cap_gr, function_rx))
		{
			if (code[i + 1] != "start") throw ("Expected 'start', after function declaration, at line: " + std::to_string(i+1));
			exp_stack.push("finish");
			exp_stack.push("return");

			/*
			* Group 1 – fun type 
			* Если указатель, то
			*	Group 2 – fun type
			*	Group 3 – pointer type
			* Group 4 – fun name 
			* Group 5 – fun argumets
			*/

			if (cap_gr[2].length() != 0 && cap_gr[3].length() != 0)
			{
				//Функция возвращает указатель
				if (cap_gr[3] == "integer")
				{
					function = new Function(new Pointer("return", vt_Integer), cap_gr[4], i + 1);
				}
				else if (cap_gr[3] == "string")
				{
					function = new Function(new Pointer("return", vt_String), cap_gr[4], i + 1);
				}
			}
			else
			{
				if (cap_gr[1] == "integer")
				{
					function = new Function(new Integer("return"), cap_gr[4], i + 1);
				} 
				else if (cap_gr[1] == "string")
				{
					function = new Function(new String("return"), cap_gr[4], i + 1);
				}
			}

			/* todo
			* Распарсить параметры функции
			* Возможно стоит делать i + 2 
			* Добавить функцию в мапу
			* ???
			* profit!
			*/

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
			if (!exp_stack.empty() && exp_stack.top() != "finish") throw ("Expected 'finish', at line: " + std::to_string(i));
			if (exp_stack.empty()) throw ("Extra 'finish' met, at line: " + std::to_string(i));
			exp_stack.pop();
		}

		if (regex_match(code[i].c_str(), cap_gr, return_rx))
		{
			if (!exp_stack.empty() && exp_stack.top() != "return") throw ("Expected 'return', at line: " + std::to_string(i));
			if (exp_stack.empty()) throw ("Extra 'return' met, at line: " + std::to_string(i));
			exp_stack.pop();
		}
	}

	if (!exp_stack.empty()) throw "Error '" + exp_stack.top() + "' excpected somewhere";




	//В конце работы collect, необходимо добавить main в call_stack
}
