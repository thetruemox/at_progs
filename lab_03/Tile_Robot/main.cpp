#include <iostream>
#include <vector>
#include <regex>

#include "Variables/Variable.h"
#include "Variables/Integer.h"
#include "Variables/String.h"
#include "Variables/Pointer.h"
#include "Variables/Array.h"

#include "Interpreter.h"
#include "Calculator/ST_Calculator.h"

#include "Robot/Robot.h"

using namespace std;

/* Идеи
* 
*/

/* Баги (Фичи?)
* 1. При работе калькулятора, в область видимости функции может добавиться
*	объект с именем константного значения и NULL в поле переменной, на 
*	работу программы это никак не влияет (Возможно все дело в обращении к map по ключу константы)
*	
*/

int main()
{
	//std::regex str("[\"](.*)[\"]");

	Interpreter ntrptr("robocode.txt");

	/*
	Robot rbt("labyrinth.txt");
	
	int prev = 3;
	while(!rbt.is_win())
	{
		//вектор вниз
		if (prev == 3)
		{
			while (!rbt.left())
			{
				if (!rbt.bot()) break;
			}

			if (rbt.right())
			{
				prev = 2;
			} 
			else if (rbt.left())
			{
				prev = 4;
			}
			else
			{
				prev = 1;
			}
		}

		//вектор влево
		if (prev == 4)
		{
			while (!rbt.top())
			{
				if (!rbt.left()) break;
			}

			if (rbt.top())
			{
				prev = 1;
			}
			else if (rbt.bot())
			{
				prev = 3;
			}
			else
			{
				prev = 2;
			}
		}

		//вектор вправо
		if (prev == 2)
		{
			while (!rbt.bot())
			{
				if (!rbt.right()) break;
			}

			if (rbt.bot())
			{
				prev = 3;
			}
			else if (rbt.top())
			{
				prev = 1;
			}
			else
			{
				prev = 4;
			}
		}

		//вектор вверх
		if (prev == 1)
		{
			while (!rbt.right())
			{
				if (!rbt.top()) break;
			}

			if (rbt.right())
			{
				prev = 2;
			}
			else if (rbt.left())
			{
				prev = 4;
			}
			else
			{
				prev = 3;
			}
		}
	}
	*/
    return 0;
}
