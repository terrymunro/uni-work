/////////////////////////////////////////////////////////////////////////////////
// Filename:    StringHelpers.h                                                //
// Author:      Terence Munro (s2587341)                                       //
// Email:       terence.munro@griffithuni.edu.au                               //
// Created:     06/06/2014                                                     //
// Modified:    06/06/2014 (Quickly threw this together)                       //
// Description: This is a collection of string helper functions. Normally a    //
//              library like this would be more focused with less repetition.  //
/////////////////////////////////////////////////////////////////////////////////

#ifndef STRINGHELPERS_H
#define STRINGHELPERS_H

//- For character array buffers
#define BUFFER_SIZE     256

/* I have decided to use bitwise operations for the fun of it here instead of
perhaps an enumeration or multiple boolean variables. */
#define DEFAULT         0x00
#define HAS_HYPHEN      0x01
#define HAS_APOSTROPHY  0x02

#define SETBIT(x, b)    ((x) |= b)
#define CLEARBIT(x, b)  ((x) &= ~b)
#define TESTBIT(x, b)   ((x) & b)

//- Includes
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <string.h>

namespace StringHelpers
{
    using namespace std;

    string readFile(char *fileName);
    char* readFileStream(char *fileName);

    void saveFile(char *fileName, const string &data);

    vector<string> split(const string &src, int(*delimiter)(int));
    vector<char*> split(const char* src, int(*delimiter)(int));

    int allowedChar(const char &c);
    bool isWord(const string &str);

    void toLowerCase(string &src);
    void toLowerCase(char *src);
}

#endif