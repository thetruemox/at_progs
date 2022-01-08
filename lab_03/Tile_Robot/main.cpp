#include <iostream>
#include <vector>
#include <regex>

#include "Variables/Variable.h"
#include "Variables/Integer.h"
#include "Variables/String.h"
#include "Variables/Pointer.h"
#include "Variables/Array.h"

#include "Interpreter.h"
#include "Calculator/ST_Calculator.h"

using namespace std;

/* Идеи
* 
*/

/* Баги (Фичи?)
* 1. При работе калькулятора, в область видимости функции может добавиться
*	объект с именем константного значения и NULL в поле переменной, на 
*	работу программы это никак не влияет (Возможно все дело в обращении к map по ключу константы)
*	
*/

int main()
{
	//std::regex str("[\"](.*)[\"]");

	Interpreter ntrptr("robocode.txt");

	/*
	ST_Calculator stree("\"14\"+\"88\"");
	String* str = new String("str");
	stree.calculate(str, new Function(str, (string)("main"), 4), stree.get_root());

	cout << str->get_value();

	stree.draw_syntax_tree("godhelpme.txt");
	*/
    return 0;
}
