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

	_collect();
}

void Interpreter::_collect()
{
	//—тек ожидани€. »спользуетс€ дл€ проверки присутстви€ return у функций и закрытых групп предложений €зыка
	std::stack<std::string> exp_stack;

	std::string type("integer|string|(pointer)<(integer|string)>");
	std::string name("[a-zA-Z][a-zA-Z0-9]*");

	std::regex function_rx("(" + type + ")[ ](" + name + ")[(]((" + type + ")[ ](" + name + ")([,][ ]?(" + type + ")[ ](" + name + "))*)?[)]");
	std::regex start_rx("start");
	std::regex finish_rx("finish");
	std::regex return_rx("return[ ]((" + name + ")|(.*))[;]");

	std::cmatch cap_gr;

	for (int i = 0; i < code.size(); i++)
	{
		//ѕоиск объ€влени€ функции
		if (regex_match(code[i].c_str(), cap_gr, function_rx))
		{
			if (code[i + 1] != "start") throw "Expected 'start', after function declaration";
			
			/*
			* Group 1 Ц fun type 
			* ≈сли указатель, то
			*	Group 2 Ц fun type
			*	Group 3 Ц pointer type
			* Group 4 Ц fun name 
			* Group 5 Ц fun argumets
			*/

			if (cap_gr[2] != false && cap_gr[3] != false)
			{
				//‘ункци€ возвращает указатель

			}
			else
			{
				
			}

		}




	}


	//¬ конце работы collect, необходимо добавить main в call_stack
}
