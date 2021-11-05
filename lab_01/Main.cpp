//
// The contents of this file are subject to the Mozilla Public
// License Version 1.1 (the "License"); you may not use this file
// except in compliance with the License. You may obtain a copy of
// the License at http://www.mozilla.org/MPL/
// 
// Software distributed under the License is distributed on an "AS
// IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
// implied. See the License for the specific language governing
// rights and limitations under the License.
// 
// The Original Code is State Machine Compiler (SMC).
// 
// The Initial Developer of the Original Code is Charles W. Rapp.
// Portions created by Charles W. Rapp are
// Copyright (C) 2000 - 2003 Charles W. Rapp.
// All Rights Reserved.
// 
// Contributor(s): 
//
// Function
//	Main
//
// Description
//  This routine starts the finite state machine running.
//
// RCS ID
// $Id$
//
// CHANGE LOG
// $Log$
// Revision 1.10  2014/09/07 07:19:00  fperrad
// exception const reference
//
// Revision 1.9  2014/09/06 19:53:15  fperrad
// remove hard tab
//
// Revision 1.8  2014/07/12 10:48:49  fperrad
// remove _rcs_id
//
// Revision 1.7  2006/06/03 19:39:24  cwrapp
// Final v. 4.3.1 check in.
//
// Revision 1.6  2006/04/22 12:45:24  cwrapp
// Version 4.3.1
//
// Revision 1.5  2005/06/08 11:09:12  cwrapp
// + Updated Python code generator to place "pass" in methods with empty
//   bodies.
// + Corrected FSM errors in Python example 7.
// + Removed unnecessary includes from C++ examples.
// + Corrected errors in top-level makefile's distribution build.
//
// Revision 1.4  2005/05/28 13:31:16  cwrapp
// Updated C++ examples.
//
// Revision 1.1  2004/09/06 15:23:39  charlesr
// Updated for SMC v. 3.1.0.
//
// Revision 1.0  2003/12/14 19:12:12  charlesr
// Initial revision
//

#ifndef _REGEX_MAX_COMPLEXITY_COUNT
#define _REGEX_MAX_COMPLEXITY_COUNT   0   /* set to 0 to disable */
#endif /* _REGEX_MAX_COMPLEXITY_COUNT */


#ifndef _REGEX_MAX_STACK_COUNT
#ifdef _WIN64
#define _REGEX_MAX_STACK_COUNT   0    /* set to 0 to disable */
#else /* _WIN64 */
#define _REGEX_MAX_STACK_COUNT   0   /* set to 0 to disable */
#endif /* _WIN64 */
#endif /* _REGEX_MAX_STACK_COUNT */

#include <stdio.h>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <time.h>
#include <regex>

#include "FileGenerator.h"
#include "AppClass.h"

using namespace std;
using namespace statemap;

bool CheckStringRegex(string str)
{
    try {
        regex regular("[<][-][a-zA-Z!][a-zA-Z0-9]{0,15}([&|\\||\\^][a-zA-Z!][a-zA-Z0-9]{0,15})*[#]");
        return regex_match(str.c_str(), regular);
    }
    catch (const std::exception& ex)
    {
        cout << ex.what() << endl;
    }
    
}


int main(int argc, char *argv[])
{
    FileGenerator fg;
    fg.generate("test.txt");
    
    clock_t start, stop;
    
    std::ifstream fin;
    fin.open("test.txt", ios_base::in);

    std::ofstream fout;
    fout.open("time.txt", ios_base::out);

    if (!fin.is_open()) 
    { 
        cout << "Can`t open the test.txt!" << endl; 
        return -1;
    }

    if (!fout.is_open())
    {
        cout << "Can`t open the time.txt!" << endl;
        return -2;
    }

    string word;
    std::map<string, int> map;
    AppClass thisContext;

    bool isAccpetable;

    int time_index = 0;
    while (!fin.eof())
    {
        fin >> word;
    
        //cout << "The string \"" << word << "\" is ";

        try
        {
            start = clock();
           
            //isAccpetable = thisContext.CheckString(word.c_str());
            isAccpetable = CheckStringRegex(word);

            stop = clock();

            

            if (isAccpetable)
            {
                fout << time_index <<" is acceptable, " << "time: " << (stop - start) / CLK_TCK << endl;
                ++time_index;
                //cout << "acceptable" << endl;          
            }
            else
            {
                fout << time_index << " is not acceptable, " << "time: " << (stop - start) / CLK_TCK << endl;
                ++time_index;
                //cout << "not acceptable" << endl;
            }
        }
        catch (const SmcException& smcex)
        {
            cout << "not acceptable - " << smcex.what() << endl;
        }
    }

    map = thisContext.get_map();

    fin.close();
    fout.close();

    /*
    std::map<string, int>::iterator map_it = map.begin();
    for (int i = 0; map_it != map.end(); map_it++)
    {
        cout << map_it->first << " : " << map_it->second << endl;
    }
    */

    return 0;
}
