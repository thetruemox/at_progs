//
// The contents of this file are subject to the Mozilla Public
// License Version 1.1 (the "License"); you may not use this file
// except in compliance with the License. You may obtain a copy
// of the License at http://www.mozilla.org/MPL/
// 
// Software distributed under the License is distributed on an
// "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
// implied. See the License for the specific language governing
// rights and limitations under the License.
// 
// The Original Code is State Machine Compiler (SMC).
// 
// The Initial Developer of the Original Code is Charles W. Rapp.
// Portions created by Charles W. Rapp are
// Copyright (C) 2000 - 2009. Charles W. Rapp.
// All Rights Reserved.
// 
// Contributor(s): 
//
// Class
//	AppClass
//
// Inline Member Functions
//	AppClass()				   - Default constructor.
//	CheckString(const char *)  - Is this string acceptable?
//
// RCS ID
// $Id$
//
// CHANGE LOG
// $Log$
// Revision 1.10  2015/08/02 19:44:34  cwrapp
// Release 6.6.0 commit.
//
// Revision 1.9  2014/09/07 17:16:44  fperrad
// explicit condition
//
// Revision 1.8  2014/09/06 19:53:15  fperrad
// remove hard tab
//
// Revision 1.7  2014/09/06 09:02:18  fperrad
// pragma only for MS compiler
//
// Revision 1.6  2014/07/12 10:48:49  fperrad
// remove _rcs_id
//
// Revision 1.5  2009/03/01 18:20:37  cwrapp
// Preliminary v. 6.0.0 commit.
//
// Revision 1.4  2005/05/28 13:31:16  cwrapp
// Updated C++ examples.
//
// Revision 1.0  2003/12/14 19:06:42  charlesr
// Initial revision
//

#ifdef _MSC_VER
#pragma warning(disable: 4355)
#endif

#include "AppClass.h"

AppClass::AppClass()
#ifdef CRTP
: isAcceptable(false)
#else
: _fsm(*this),
  isAcceptable(false)
#endif
{
#ifdef FSM_DEBUG
#ifdef CRTP
    setDebugFlag(true);
#else
    _fsm.setDebugFlag(true);
#endif
#endif
}

bool AppClass::CheckString(const char *theString)
{ 
    this->word_list.clear();
    this->temp_word = "";

    _fsm.enterStartState();
   // std::cout << "Start" << std::endl;

    while (*theString != '\0')
    {
        if (*theString == '<' && *(theString + 1) == '-')
        {
            _fsm.OpenSymb();
            ++theString;
            //std::cout << "OpenSymb" << std::endl;
        } else if ((*theString >= 'a' && *theString <= 'z') || (*theString >= 'A' && *theString <= 'Z'))
        {
      
            if (length > max_length)
            {
                _fsm.Unknown();
             //   std::cout << "Unknown" << std::endl;
            }
            else 
            {
                _fsm.Letter();
                this->temp_word += *theString;
              //  std::cout << "Letter" << std::endl;
            }

        }
        else if (*theString >= '0' && *theString <= '9')
        {
     
            if (length > max_length)
            {
                _fsm.Unknown();
               // std::cout << "Unknown" << std::endl;
            }
            else
            {
                _fsm.Number();
                this->temp_word += *theString;
               // std::cout << "Number" << std::endl;
            }
        }
        else if (*theString == '!')
        {         
            _fsm.Exc_point();
            this->temp_word += *theString;
           // std::cout << "Exc_point" << std::endl;
        }
        else if (*theString == '&' || *theString == '|' || *theString == '^')
        {      
            _fsm.Math();
            this->word_list.push_back(this->temp_word);
            this->temp_word = "";
          //  std::cout << "Math" << std::endl;
        }
        else if (*theString == '#')
        {
            _fsm.Hash();
            this->word_list.push_back(this->temp_word);
            this->temp_word = "";
          //  std::cout << "Hash" << std::endl;
        }
        else
        {
            _fsm.Unknown();
          //  std::cout << "Unknown" << std::endl;
        }

        ++theString;
    }
    
    // end of string has been reached - send the EOS transition.
    _fsm.EOS();
  //  std::cout << "EOS" << std::endl;
    
    _fsm.Epsilon();
    
  //  std::cout << "Epsilon" << std::endl;


    if (isAcceptable)
    {
        std::list<std::string>::iterator list_it = this->word_list.begin();

        for (int i = 0; list_it != this->word_list.end(); list_it++)
        {
            ++map[*list_it];
        }
    }

    return isAcceptable;
}
