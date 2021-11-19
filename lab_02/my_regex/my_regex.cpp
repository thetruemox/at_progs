﻿#include <iostream>
#include <string>
#include <list>
#include "NFA_Builder.h"
#include "DFA_Builder.h"

using namespace std;

int main()
{
    string reg = "(m.e.((o.w)|(p.h.i)))+";

    DFA_Builder* dfa = new DFA_Builder(reg);

    dfa->draw_min_dfa_graph("min_dfa.txt");
    dfa->draw_dfa_graph("dfa.txt");
    dfa->draw_nfa_graph("nfa.txt");
    dfa->draw_syntax_tree("stree.txt");
    

    return 0;
}
