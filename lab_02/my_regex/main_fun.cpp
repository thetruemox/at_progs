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
    //string reg = "((((a)).(^|((a)|(a).(a))+).((a)))|(((a))|((a)).(^|((a)|(a).(a))+).((a).(a))).(^|(((a))|((a)).(^|((a)|(a).(a))+).((a).(a)))+).(((a)).(^|((a)|(a).(a))+).((a))))";
    //string reg = "(5:m.e.(p.h.i|o.w))";
    string reg = "a.b.c.d";
    
    My_Regex my_rx1(reg);
    //my_rx1.restore_regex("rstr_reg.txt");
    My_Regex* my_rx2 = my_rx1.inversion();

    cout << my_rx2->check_str("dcba") << endl;

    //map<int, string> cg;
    //cout << my_rx1.check_str("meeepo", cg) << endl;

    //cout << cg[10] << " " << cg[8];

    return 0;
}

