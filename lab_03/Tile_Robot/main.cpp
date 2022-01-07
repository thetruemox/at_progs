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
	ST_Calculator stree("\"14\"+\"88\"");
	String* str = new String("str");
	stree.calculate(str, new Function(str, (string)("main"), 4), stree.get_root());

	cout << str->get_value();

	stree.draw_syntax_tree("godhelpme.txt");

    return 0;
}
