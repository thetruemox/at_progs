#include <iostream>
#include <string>
#include <list>
#include "Syntax_Tree.h"
#include "NFA_Builder.h"

using namespace std;

int main()
{
    string reg = "a.(b|2)+";
    Syntax_Tree* stree = new Syntax_Tree(reg);
    NFA_Builder* builder = new NFA_Builder();

    //cout << "regex is " + reg << endl;
    //stree->out();
    builder->build(stree, stree->get_root());
    builder->draw_graph();
    

    return 0;
}
