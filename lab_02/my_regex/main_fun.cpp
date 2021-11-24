#include <iostream>
#include <string>
#include <list>
#include "My_Regex.h"


using namespace std;

int main()
{
    string reg = "a|^";
    My_Regex my_regex(reg);
    cout << my_regex.check_str("a") << std::endl;
    my_regex.restore_regex("rstr_reg.txt");

    return 0;
}

