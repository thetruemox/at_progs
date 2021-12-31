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

    string reg = "a.b.c";
    
    My_Regex my_rx1(reg);

    /* restore_regex
    my_rx1.restore_regex("rstr_reg.txt");
    */

    /* inversion
    My_Regex* my_rx2 = my_rx1.inversion();
    cout << my_rx2->check_str("cba");
    */
    
    /* addon -> addon.txt
    my_rx1.addition();
    */ 

   
    std::list<std::string> mtchs = my_rx1.findall("ababcdaaaa", "a+.(^|(b.a)+)");
    for (auto it = mtchs.begin(); it != mtchs.end(); it++)
    {
        cout << (*it) << " ";
    }
    

    return 0;
}

