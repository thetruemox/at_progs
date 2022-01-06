#include <iostream>
#include <vector>
#include <regex>

#include "Variable.h"
#include "Integer.h"
#include "String.h"
#include "Pointer.h"
#include "Array.h"

#include "Interpreter.h"
#include "Calculator/ST_Calculator.h"

using namespace std;

int main()
{
	//Interpreter ntrptr("robocode.txt");
	ST_Calculator stree("a");
	stree.draw_syntax_tree("godhelpme.txt");

    return 0;
}
