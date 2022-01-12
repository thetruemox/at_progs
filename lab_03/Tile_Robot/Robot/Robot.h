#pragma once
#include <string>
#include <vector>
#include <stack>
#include <fstream>
#include <iostream>
#include <Windows.h>

class Robot
{
public:
	Robot(std::string file_name);

	int top();
	int bot();
	int left();
	int right();
	void place_wall(); //Ставит стену и перемещается на предыдущую клетку (Не актуальный метод)
	void time_shift(int offset);

	void draw();
	int is_win(); //1 – если робот достиг выхода, 0 – в остальных случаях
private:
	int win; //1 – если робот достиг выхода, 0 – в остальных случаях

	std::pair<int, int> XY_enter; //Координаты начала движения робота
	std::pair<int, int> XY_exit; //Координаты выхода из лабиринта
	std::pair<int, int> XY_cur; //Координаты положения робота

	std::stack<std::pair<int, int>> move_memory;

	std::vector<std::string> labyrinth; 
};