#include <iostream>
#include <string>
#include <list>
#include <map>
#include "My_Regex.h"
#include "DFA_Builder.h"
#include "NFA_Builder.h"

using namespace std;

int main()
{
    //string reg = "(((^|(1.0)+).0)|1.(^|(0.1)+).1).(^|(^|(0.(^|(0.1)+).(1|0.0)|1.(^|(1.0)+).(0|1.1)))+)";
    string reg = "(8:m.(10:e+).p.o)";

    My_Regex my_rx1(reg);
    //my_rx1.restore_regex("rstr_reg.txt");

    map<int, string> cg;
    cout << my_rx1.check_str("meepo", cg) << endl;

    cout << cg[8] << " " << cg[10];

    return 0;
}

