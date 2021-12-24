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
    string reg = "a.b.c";
    
    My_Regex my_rx1(reg);
    std::list<std::string> mtchs = my_rx1.findall("abcdaaaa", "a+.(^|(b.a)+)");

    for (auto it = mtchs.begin(); it != mtchs.end(); it++)
    {
        cout << (*it) << " ";
    }
    
    //my_rx1.addition();

    //cout << my_rx1.check_str("") << endl;

    return 0;
}

