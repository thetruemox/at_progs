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
* Фиббоначи 
* 
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
	//Тест с роботом
	//Interpreter ntrptr("robocode.txt", "labyrinth.txt");
	//Тест с рекурсией
	//Interpreter ntrptr("recursive_test.txt", "labyrinth.txt");
	//Тест с математическим выражением
	//Interpreter ntrptr("math_test.txt", "labyrinth.txt");
	//Тест с конкатенацией строк
	//Interpreter ntrptr("str_test.txt", "labyrinth.txt");
	//Тест с числами Фибоначчи
	//Interpreter ntrptr("fibonacci_test.txt", "labyrinth.txt");

	Interpreter ntrptr("array_test.txt", "labyrinth.txt");

    return 0;
}
