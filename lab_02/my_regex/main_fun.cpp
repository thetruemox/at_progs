#include <iostream>
#include <string>
#include <list>
#include "My_Regex.h"
#include "DFA_Builder.h"
#include "NFA_Builder.h"

using namespace std;

/*todo 
Восстановление почему-то не работает для случая, когда в исходной регулярке есть операция конкатенации
Для случая (a|b)+ воспринимает "или" как конкатенацию (почему?....)
*/
int main()
{
    string reg = "(a|b)+";
    string str = "a";

    My_Regex my_rx1(reg);
    //my_rx1.restore_regex("rstr_reg.txt");



    //My_Regex my_rx2(my_rx1.restore_regex("rstr_reg.txt"));
    //cout << "Restored: " << my_rx2.check_str(str) << std::endl;


    DFA_Builder dfa(my_rx1.restore_regex("rstr_reg.txt"));
    dfa.draw_min_dfa_graph("rstr_min_dfa.txt");

    //NFA_Builder nfa(reg);
    //nfa.draw_nfa_graph("nfa.txt");
    
    
   // cout << "Original: " << my_rx1.check_str("a") << std::endl;
   
    //My_Regex my_rx2(my_rx1.restore_regex("rstr_reg.txt"));

    //DFA_Builder nfa(my_rx1.restore_regex("rstr_reg.txt"));
   //cout << "Restored: " << my_rx2.check_str(str) << std::endl;

    return 0;
}

