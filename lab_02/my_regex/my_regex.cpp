#include <iostream>
#include <string>
#include <list>
#include "NFA_Builder.h"

using namespace std;

int main()
{
    string reg = "(m.e.((o.w)|(p.h.i)))+";
    NFA_Builder* builder = new NFA_Builder(reg);

    builder->draw_graph("nfa_graphviz.txt");
    builder->draw_syntax_tree("stree_graphviz.txt");

    return 0;
}
