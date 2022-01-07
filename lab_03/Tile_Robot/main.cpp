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
	ST_Calculator stree("(4+8*1)/2");
	Integer* num = new Integer("num");
	stree.calculate(num, new Function(num, (string)("main"), 4), stree.get_root());

	cout << num->get_value();

	stree.draw_syntax_tree("godhelpme.txt");

    return 0;
}
