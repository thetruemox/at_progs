#include "FileGenerator.h"
#define MAX_STRINGS 1
#define MAX_VARS 100



int FileGenerator::rand_num(int first, int last)
{
	return (first + rand() % last);
}

void FileGenerator::generate(std::string file_name)
{
	std::ofstream fout;
	fout.open(file_name, std::ios_base::out);

	char temp;
	int var_size;

	for (int i = 0; i < MAX_STRINGS; i++)
	{
		fout << "<-";

		for (int j = 0; j < MAX_VARS; j++)
		{
			var_size = this->rand_num(1, 16);

			for (int k = 0; k < var_size; k++)
			{
				temp = 'a' + this->rand_num(0, 'z' - 'a');
				fout << temp;
			}

			if (j != MAX_VARS - 1) fout << '&';	
		}

		fout << "#";
		if (i != MAX_STRINGS - 1) fout << '\n';
	}

	fout.close();
}
