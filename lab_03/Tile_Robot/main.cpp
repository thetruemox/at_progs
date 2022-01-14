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

#include "Robot/Robot.h"

using namespace std;

/* todo
* Пример с рекурсией
* 15:30
*/

/* Баги (Фичи?)
* 1. При работе калькулятора, в область видимости функции может добавиться
*	объект с именем константного значения и NULL в поле переменной, на 
*	работу программы это никак не влияет (Возможно все дело в обращении к map по ключу значения value)
*	
*/

int main()
{
	Interpreter ntrptr("robocode.txt", "labyrinth.txt");
	//Interpreter ntrptr("recursive_test.txt", "labyrinth.txt");

	//int num = foo(0);
	//cout << num;
    return 0;
}
