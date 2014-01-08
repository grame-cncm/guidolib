/*
 GUIDO Library
 Copyright (C) 2013 Grame
 
 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
 
 Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
 research@grame.fr
 
 */

#include <locale.h>
#include <istream>
#include <sstream>
#include <stack>

using namespace std;

#include "GuidoStream.h"


//--------------------------------------------------------------------------
GuidoStream::GuidoStream() : stringstream()
{
    fTheGlobalStringStream = new stringstream();
}

//--------------------------------------------------------------------------
GuidoStream::~GuidoStream() 
{
    delete fTheGlobalStringStream;
}

//--------------------------------------------------------------------------
void GuidoStream::WriteToStream(const char* inStr) 
{
    /* Append inStr (written by user) to global string */
    *fTheGlobalStringStream << inStr;

    /* Create a temporary string from the global string */
    string *tmpString = new string(fTheGlobalStringStream->str());

    /* Build tags stack from the global string */
    stack<char> *charStack = AnalyzeString(fTheGlobalStringStream);

    /* Complete temporary string with appropriate tags from tags stack */
    WriteNewString(charStack, tmpString);

    /* Set the final stringstream (this) from the temporary string */
    this->str(*tmpString);
    delete tmpString;
}

//--------------------------------------------------------------------------
stack<char> *GuidoStream::AnalyzeString(stringstream *inStr)
{
    stack<char> *charStack = new stack<char>();

    string stringToAnalyze(inStr->str());
    bool errorFound = false;

    /* Get current char, and push its opposite in the stack if it's a tag ("{," "[," "(," "<") */
    for (size_t i = 0; i < stringToAnalyze.size() && errorFound == false; i++)
    {
        switch (stringToAnalyze[i])
        {
        case '{':
            charStack->push('}');
            break;
        case '[':
            charStack->push(']');
            break;
        case '(':
            charStack->push(')');
            break;
        case '<':
            charStack->push('>');
            break;
        /* If current char is a closing tag and corresponds to the top of the stack, stack's first element is popped.
           Else, there is a syntax error and we leave. */
        case '}':
            if (charStack->size() && charStack->top() == '}')
                charStack->pop();
            else
            {
                /* Syntax error */
                errorFound = true;
            }
            break;
        case ']':
            if (charStack->size() && charStack->top() == ']')
                charStack->pop();
            else
            {
                /* Syntax error */
                errorFound = true;
            }
            break;
        case ')':
            if (charStack->size() && charStack->top() == ')')
                charStack->pop();
            else
            {
                /* Syntax error */
                errorFound = true;
            }
            break;
        case '>':
            if (charStack->size() && charStack->top() == '>')
                charStack->pop();
            else
            {
                /* Syntax error */
                errorFound = true;
            }
            break;
        default:
            break;
        }
    }
    
    
    return charStack;
}

//--------------------------------------------------------------------------
void GuidoStream::WriteNewString(stack<char> *inStack, string *stringToComplete)
{
    /* Append all stack's tags to the stringToComplete */
    while (inStack->size())
    {
        stringToComplete->push_back(inStack->top());
        inStack->pop();
    }
}

//--------------------------------------------------------------------------
void GuidoStream::Prepare()
{
    this->clear();
    this->str(this->str()); // Needed !
}

//--------------------------------------------------------------------------
void GuidoStream::ReinitStream()
{
    this->clear();
    this->str("");

    fTheGlobalStringStream->clear();
    fTheGlobalStringStream->str("");
}