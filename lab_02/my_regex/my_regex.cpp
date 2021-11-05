#include <iostream>
#include <string>
#include "Syntax_Tree.h"

using namespace std;

int main()
{
    string reg = "a.(b|2)+";
    Syntax_Tree stree(reg);

    cout << "regex is " + reg << endl;
    stree.out();
   

    return 0;
}
