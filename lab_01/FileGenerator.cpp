#include "FileGenerator.h"
#define MAX_STRINGS 20
#define MAX_VARS 150

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
		/*if (rand_num(1, 20) != 1) fout << "<-";
		
		if (rand_num(1,10) == 1)
		{
			fout << rand_num(0, 9);
		}*/

		for (int j = 0; j < MAX_VARS; j++)
		{
			var_size = this->rand_num(1, 16);

			for (int k = 0; k < var_size; k++)
			{
				temp = 'a' + this->rand_num(0, 'z' - 'a');
				fout << temp;
			}

			/*if (rand_num(1, 100) == 1)
			{
				fout << "abfgyfhstrfghbgkj";
			}*/

			if (j != MAX_VARS - 1) fout << '&';	
		}

		fout << "#";
		if (i != MAX_STRINGS - 1) fout << '\n';
	}

	fout.close();
}
