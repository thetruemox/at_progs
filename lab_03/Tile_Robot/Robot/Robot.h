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
	void place_wall(); //������ ����� � ������������ �� ���������� ������ (�� ���������� �����)
	void time_shift(int offset);

	void draw();
	int is_win(); //1 � ���� ����� ������ ������, 0 � � ��������� �������
private:
	int win; //1 � ���� ����� ������ ������, 0 � � ��������� �������

	std::pair<int, int> XY_enter; //���������� ������ �������� ������
	std::pair<int, int> XY_exit; //���������� ������ �� ���������
	std::pair<int, int> XY_cur; //���������� ��������� ������

	std::stack<std::pair<int, int>> move_memory;

	std::vector<std::string> labyrinth; 
};