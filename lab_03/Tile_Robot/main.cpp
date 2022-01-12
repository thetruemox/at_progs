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

/* todo
* Использование методов перемещения робота в: 
*	1. Объявлении переменной
*	2. Присвоении переменной
*	3. Условном операторе
* Вызов функции в условии цикла while
* Перевести код алгоритма робота с С++ на Robocode
*/

/* Баги (Фичи?)
* 1. При работе калькулятора, в область видимости функции может добавиться
*	объект с именем константного значения и NULL в поле переменной, на 
*	работу программы это никак не влияет (Возможно все дело в обращении к map по ключу константы)
*	
*/

int check_top(Robot* rbt)
{
	int wall = rbt->top();
	
	if (wall == 1)
	{
		rbt->time_shift(1);
		return 1;
	}
	else 
	{

		return 0;
	}
}

int check_right(Robot* rbt)
{
	int wall = rbt->right();

	if (wall == 1)
	{
		rbt->time_shift(1);
		return 1;
	}
	else
	{
		return 0;
	}
}

int check_bot(Robot* rbt)
{
	int wall = rbt->bot();

	if (wall == 1)
	{
		rbt->time_shift(1);
		return 1;
	}
	else
	{
		return 0;
	}
}

int check_left(Robot* rbt)
{
	int wall = rbt->left();

	if (wall == 1)
	{
		rbt->time_shift(1);
		return 1;
	}
	else
	{
		return 0;
	}
}

int main()
{
	Interpreter ntrptr("robocode.txt");
	
	/*
	Robot rbt("labyrinth.txt");
	
	int prev = 3;
	while(!rbt.is_win())
	{
		//вектор вниз
		if (prev == 3)
		{
			while (!check_left(&rbt))
			{
				if (!rbt.bot()) break;
			}

			if (rbt.left())
			{
				prev = 4;
			} 
			else if (rbt.right())
			{
				prev = 2;
			}
			else
			{
				prev = 1;
			}
		}

		//вектор влево
		if (prev == 4)
		{
			while (!check_top(&rbt))
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
			while (!check_bot(&rbt))
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
			while (!check_right(&rbt))
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
