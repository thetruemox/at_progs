#include <iostream>
#include <vector>

#include "Collector.h"
#include "Variable.h"
#include "Integer.h"
#include "String.h"
#include "Pointer.h"
#include "Array.h"

using namespace std;

int main()
{
    vector<Variable*> arr;

    arr.push_back(new Integer("a"));
    arr.push_back(new Integer("b"));
    arr.push_back(new String("c"));
    arr.push_back(new Pointer("d"));
    arr.push_back(new Array("arr", vt_Integer));

    for (int i = 0; i < arr.size(); i++)
    {
        cout << arr[i]->get_name() << " " << arr[i]->get_type() << endl;
    }

    return 0;
}
