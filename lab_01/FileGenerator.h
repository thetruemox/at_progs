#pragma once

#include <string>
#include <fstream>
#include <ctime>

class FileGenerator
{
private:
	
public:
	FileGenerator()
	{
		srand(time(0));
	}
	int rand_num(int first, int last);
	void generate(std::string file_name);
};
