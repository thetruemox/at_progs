#include "Collector.h"
#include <string>
#include <fstream>
#include <iostream>

Collector::Collector(std::string file_name)
{
	std::ifstream file(file_name);
	if (!file.is_open()) std::cout << "file not found!";

	/*
	������ ����� ������������ � ������� � ���� ��� ������� � ���������� � �� ������� ���������
	*/

	



}
