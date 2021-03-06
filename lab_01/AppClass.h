#ifndef _H_THECONTEXT
#define _H_THECONTEXT

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
// Name
//	AppClass
//
// Description
//	When a state map executes an action, it is really calling a
//	member function in the context class.
//
// RCS ID
// $Id$
//
// CHANGE LOG
// $Log$
// Revision 1.6  2015/08/02 19:44:34  cwrapp
// Release 6.6.0 commit.
//
// Revision 1.5  2014/09/06 19:53:15  fperrad
// remove hard tab
//
// Revision 1.4  2005/05/28 13:31:16  cwrapp
// Updated C++ examples.
//
// Revision 1.0  2003/12/14 19:11:44  charlesr
// Initial revision
//

#include "AppClass_sm.h"
#include <map>
#include <string>
#include <list>
#define MAX_LENGTH 16

#ifdef CRTP
class AppClass : public AppClassContext<AppClass>
#else
class AppClass
#endif
{
private:
#ifndef CRTP
    AppClassContext _fsm;
#endif

    bool isAcceptable;
    int length = 0, max_length = MAX_LENGTH - 1;

    std::list<std::string> word_list;
    std::string temp_word;
    std::map<std::string, int> map;

public:
    AppClass();
        // Default constructor.

    ~AppClass() {};
        // Destructor.

    bool CheckString(const char*);
        // Checks if the string is acceptable.

    inline void Acceptable()
    { isAcceptable = true; };

    inline void Unacceptable()
    { isAcceptable = false; };

    inline void inc_length()
    {
        this->length++;
    };
    inline void reset_length()
    {
        this->length = 0;
    };
    inline int check_length()
    {
        if (this->length > this->max_length)
        {
            return 0;
        }
        else return 1;
    };

    inline std::map<std::string, int> get_map()
    {
        return this->map;
    };
    
        // State map actions.
};

#endif
