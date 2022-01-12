#pragma once
#include "Robot.h"

Robot::Robot(std::string file_name)
{
	std::ifstream file(file_name);
	if (!file.is_open()) throw (std::string)("File " + file_name + " not found!");

	std::string t_line;
	while (getline(file, t_line))
	{
		this->labyrinth.push_back(t_line);
	}

	for (int x = 0; x < labyrinth.size(); x++)
	{
		for (int y = 0; y < labyrinth[x].size(); y++)
		{
			if (labyrinth[x][y] == 'P')
			{
				this->XY_enter.first = x;
				this->XY_enter.second = y;
				this->XY_cur = this->XY_enter;
			}
			else if (labyrinth[x][y] == 'X')
			{
				this->XY_exit.first = x;
				this->XY_exit.second = y;
			}
		}
	}

	this->move_memory.push(this->XY_cur);

	this->win = 0;
}

int Robot::top()
{
	char dest; //Клетка, в которую попытается переместиться робот 

	try
	{
		dest = this->labyrinth.at(XY_cur.first - 1).at(XY_cur.second);
	}
	catch (const std::out_of_range& ex)
	{
		return 0;
	}
	
	if (dest == ' ') //Клетка свободна
	{
		this->labyrinth.at(XY_cur.first).at(XY_cur.second) = ' ';

		XY_cur.first--;
		move_memory.push(XY_cur);

		this->labyrinth.at(XY_cur.first).at(XY_cur.second) = 'P';

		draw();
		return 1;
	}
	else if (dest == 'X') //Робот достиг выхода
	{
		this->labyrinth.at(XY_cur.first).at(XY_cur.second) = ' ';
		this->win = 1;

		XY_cur.first--;
		move_memory.push(XY_cur);

		this->labyrinth.at(XY_cur.first).at(XY_cur.second) = 'P';

		draw();
		return 1;
	} 
	else //Клетка занята
	{
		return 0;
	}
	
	return 0;
}

int Robot::bot()
{
	char dest; //Клетка, в которую попытается переместиться робот 

	try
	{
		dest = this->labyrinth.at(XY_cur.first + 1).at(XY_cur.second);
	}
	catch (const std::out_of_range& ex)
	{
		return 0;
	}

	if (dest == ' ') //Клетка свободна
	{
		this->labyrinth.at(XY_cur.first).at(XY_cur.second) = ' ';

		XY_cur.first++;
		move_memory.push(XY_cur);

		this->labyrinth.at(XY_cur.first).at(XY_cur.second) = 'P';

		draw();
		return 1;
	}
	else if (dest == 'X') //Робот достиг выхода
	{
		this->labyrinth.at(XY_cur.first).at(XY_cur.second) = ' ';
		this->win = 1;

		XY_cur.first++;
		move_memory.push(XY_cur);

		this->labyrinth.at(XY_cur.first).at(XY_cur.second) = 'P';

		draw();
		return 1;
	}
	else //Клетка занята
	{
		return 0;
	}

	return 0;
}

int Robot::left()
{
	char dest; //Клетка, в которую попытается переместиться робот 

	try
	{
		dest = this->labyrinth.at(XY_cur.first).at(XY_cur.second - 1);
	}
	catch (const std::out_of_range& ex)
	{
		return 0;
	}

	if (dest == ' ') //Клетка свободна
	{
		this->labyrinth.at(XY_cur.first).at(XY_cur.second) = ' ';

		XY_cur.second--;
		move_memory.push(XY_cur);

		this->labyrinth.at(XY_cur.first).at(XY_cur.second) = 'P';

		draw();
		return 1;
	}
	else if (dest == 'X') //Робот достиг выхода
	{
		this->labyrinth.at(XY_cur.first).at(XY_cur.second) = ' ';

		this->win = 1;
		XY_cur.second--;
		move_memory.push(XY_cur);

		this->labyrinth.at(XY_cur.first).at(XY_cur.second) = 'P';

		draw();
		return 1;
	}
	else //Клетка занята
	{
		return 0;
	}

	return 0;
}

int Robot::right()
{
	char dest; //Клетка, в которую попытается переместиться робот 

	try
	{
		dest = this->labyrinth.at(XY_cur.first).at(XY_cur.second + 1);
	}
	catch (const std::out_of_range& ex)
	{
		return 0;
	}

	if (dest == ' ') //Клетка свободна
	{
		this->labyrinth.at(XY_cur.first).at(XY_cur.second) = ' ';

		XY_cur.second++;
		move_memory.push(XY_cur);

		this->labyrinth.at(XY_cur.first).at(XY_cur.second) = 'P';

		draw();
		return 1;
	}
	else if (dest == 'X') //Робот достиг выхода
	{
		this->labyrinth.at(XY_cur.first).at(XY_cur.second) = ' ';
		this->win = 1;

		XY_cur.second++;
		move_memory.push(XY_cur);

		this->labyrinth.at(XY_cur.first).at(XY_cur.second) = 'P';

		draw();
		return 1;
	}
	else //Клетка занята
	{
		return 0;
	}

	return 0;
}

void Robot::place_wall()
{
	if (XY_cur == XY_enter)
	{
		std::cout << "You are trying to place a wall at spawn";
		return;
	}

	labyrinth.at(XY_cur.first).at(XY_cur.second) = '#';
	move_memory.pop();
	XY_cur = move_memory.top();
	labyrinth.at(XY_cur.first).at(XY_cur.second) = 'P';
	
	draw();
}

void Robot::time_shift(int offset)
{
	if (offset <= 0) return;

	for (int i = 0; i < offset; i++)
	{
		if (move_memory.empty())
		{
			labyrinth.at(XY_cur.first).at(XY_cur.second) = ' ';
			XY_cur = XY_enter;
			labyrinth.at(XY_cur.first).at(XY_cur.second) = 'P';
			draw();
			return;
		}

		move_memory.pop();
	}

	labyrinth.at(XY_cur.first).at(XY_cur.second) = ' ';

	if (move_memory.empty())
	{
		XY_cur = XY_enter;
	}
	else 
	{
		XY_cur = move_memory.top();	
	}

	labyrinth.at(XY_cur.first).at(XY_cur.second) = 'P';
	draw();
}

void Robot::draw()
{
	Sleep(500);
	system("cls");
	for (int x = 0; x < labyrinth.size(); x++)
	{
		std::cout << labyrinth[x] << std::endl;
	}
}

int Robot::is_win()
{
	return this->win;
}
