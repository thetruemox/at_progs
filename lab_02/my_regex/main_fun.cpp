#include <iostream>
#include <string>
#include <list>
#include "My_Regex.h"


using namespace std;

int main()
{
    string reg = "(m.e.((o.w)|(p.h.i)))+";
    My_Regex my_regex(reg);
    cout << my_regex.check_str("1");
    return 0;
}

