/////////////////////////////////////////////////////////////////////////////////
// Filename:    StringHelpers.cpp                                              //
// Author:      Terence Munro (s2587341)                                       //
// Email:       terence.munro@griffithuni.edu.au                               //
// Created:     06/06/2014                                                     //
// Modified:    06/06/2014 (Quickly threw this together)                       //
// Description: This is a collection of string helper functions. Normally a    //
//              library like this would be more focused with less repetition.  //
/////////////////////////////////////////////////////////////////////////////////

#include "StringHelpers.h"

namespace StringHelpers
{

    /****************************************************************************
     * Description:     Read a file in using C style functions but use C++      *
     *                  style strings                                           *
     *                                  (uses C++ strings)                      *
     * Returns:         (string) string of the entire file                      *
     * Parameters:      char *fileName;     The name of the file to read        *
     ****************************************************************************/
    string readFile(char *fileName)
    {
        int errNo = 0;
        FILE *file;
        string ret;

        if (errNo = fopen_s(&file, fileName, "r") != 0)
        {
            cerr << "[" << errNo << "] Error occurred trying to read file: '"
                << fileName << "'." << endl;
        }
        else
        {
            char buf[BUFFER_SIZE];
            while (!feof(file))
            {
                int bytesRead = fread(buf, sizeof(char), BUFFER_SIZE - 1, file);
                buf[bytesRead] = '\0';
                ret += buf;
            }
        }
        fclose(file);

        return ret;
    }

    /****************************************************************************
     * Description:     Read a file in using C++ style streams but use C style  *
     *                  strings that are malloced meaning they need to be freed *
     *                                  (uses C strings)                        *
     * Returns:         (char*) cstring of the entire file                      *
     * Parameters:      char *fileName;     The name of the file to read        *
     * Note:            It's not good practice to allocate the memory in a      *
     *                  function and assume the user will free it. Better would *
     *                  be to pass in as a parameter the already allocated      *
     *                  memory, that way the user knows to free it however it   *
     *                  means I would have to make a way to get the length of   *
     *                  the file in advanced or guess at the memory size ...    *
     *                  in the end whatever this works fine because its only    *
     *                  for a toy project.                                      *
     ****************************************************************************/
    char* readFileStream(char *fileName)
    {
        unsigned int len = 0;
        unsigned int index = 0;
        char *ret = 0;

        ifstream fs(fileName);
        if (fs.is_open())
        {
            fs.seekg(0, ios::end);          //- Seek to the end of the file
            len = (unsigned int)fs.tellg(); //- This location is the length now
            ret = (char*)malloc(len+1);     //- Allocate C string of appropriate length
            memset(ret, 0, len+1);          //- Zero out the memory
            fs.seekg(0, ios::beg);          //- Return to the start of the file
            while (fs.good() && index < len)
                ret[index++] = fs.get();
            fs.close();
        }
        else
            cerr << "readFilestream: Unable to read file '" << fileName << "'."
                << endl;

        return ret;
    }

    /****************************************************************************
     * Description:     Save a file using C style functions but use C++ style   *
     *                  strings                                                 *
     *                                  (uses C++ strings)                      *
     * Returns:         NIL                                                     *
     * Parameters:      char *fileName;     The name of the file to save into   *
     *                  const string &data; The string that will be saved       *
     ****************************************************************************/
    void saveFile(char *fileName, const string &data)
    {
        FILE *file;
        int errNo = 0;
        if (errNo = fopen_s(&file, fileName, "w") != 0)
        {
            cerr << "[" << errNo << "] Error occurred attempting to write to"
                "file: '" << fileName << "'." << endl;
        }
        else
            fprintf(file, "%s", data.c_str());

        fclose(file);
    }

    /****************************************************************************
     * Description:     Check if a string is a valid word                       *
     * Returns:         (boolean)                                               *
     * Parameters:      const string &str;  String to be checked                *
     ****************************************************************************/
    bool isWord(const string &str)
    {
        char state = DEFAULT;    //- could just have two bools this is just more fun
        for (string::const_iterator it = str.cbegin(); it != str.cend(); it++)
        {
            //- If we find a hyphen and there are no other hyphens already
            //  and its not at the beginning or end of a word.... its acceptable
            if (*it == '-')
            {
                if (TESTBIT(state, HAS_HYPHEN)  //- If we already have a hyphen
                    || it == str.cbegin()       //- Or this hyphen is at the start
                    || *it == *str.crbegin())   //- of a word, or the end
                    return false;               //- Not considered valid

                SETBIT(state, HAS_HYPHEN);
            }

            //- If we find an apostrophy and there are no other apostrophies
            //  it is acceptable.
            if (*it == '\'')
            {
                if (TESTBIT(state, HAS_APOSTROPHY))
                    return false;

                SETBIT(state, HAS_APOSTROPHY);
            }

            //- Otherwise pass through allowed characters filter
            if (!allowedChar(*it))
                return false;
        }

        return true;
    }

    /****************************************************************************
     * Description:     Check if a character is an allowed character            *
     * Returns:         (int)                                                   *
     * Parameters:      const char &c;  Character to be checked                 *
     ****************************************************************************/
    int allowedChar(const char &c)
    {
        return (isascii(c)      //- Only allowing ascii no unicode at this point
            && (isalpha(c)      //- Only allow alphabetic letters
            || c == '-'         //- or hyphens
            || c == '\'')       //- or apostrophies
        );
    }

    /****************************************************************************
     * Description:     Split the string into a vector of strings using a       *
     *                  function that determines if a character is a delimiter  *
     *                              (uses C++ strings)                          *
     * Returns:         (vector<string>)                                        *
     * Parameters:      const string &src;      String to be split              *
     *                  int (*delimiter)(int);  Delimiter function              *
     ****************************************************************************/
    vector<string> split(const string &src, int(*delimiter)(int))
    {
        vector<string> ret;
        string tmp;

        for (string::const_iterator it = src.cbegin(); it != src.cend(); it++)
        {
            //- Only splitting ascii characters
            if (!isascii(*it))
                continue;

            //- Special case
            if (*it == '\n' && tmp.length() > 0 && tmp.at(tmp.length() - 1) == '-')
            {
                tmp.erase(tmp.length() - 1);
                continue;
            }

            //- Split on delimiter
            else if (delimiter(*it))
            {
                if (!tmp.empty())
                    ret.push_back(tmp);
                tmp = "";
            }

            //- Concatenate otherwise
            else
                tmp += *it;
        }

        //- Add the last word
        if (!tmp.empty())
            ret.push_back(tmp);

        return ret;
    }

    /****************************************************************************
     * Description:     Split the string into a vector of strings using a       *
     *                  function that determines if a character is a delimiter  *
     *                              (uses C strings)                            *
     * Returns:         (vector<char*>)                                         *
     * Parameters:      const char *src;        String to be split              *
     *                  int (*delimiter)(int)   Delimiter function              *
     ****************************************************************************/
    vector<char*> split(const char *src, int(*delimiter)(int))
    {
        vector<char*> ret;
        int buflen = 0;
        char buf[BUFFER_SIZE];
        memset(buf, 0, BUFFER_SIZE);

        for (; *src != '\0'; ++src)
        {
            //- Only splitting ascii characters
            if (!isascii(*src))
                continue;

            //- Special case
            if (*src == '\n' && buflen > 0 && buf[buflen - 1] == '-')
            {
                buf[buflen - 1] = '\0';
                continue;
            }

            //- Split on delimiter
            else if (delimiter(*src) || *(src+1) == '\0')
            {
                if (buflen > 0)
                {
                    char *tmp = (char*)malloc(buflen+1);
                    strcpy_s(tmp, buflen+1, buf);
                    tmp[buflen] = '\0';
                    ret.push_back(tmp);
                }
                memset(buf, 0, BUFFER_SIZE);
                buflen = 0;
            }

            //- Concatenate otherwise
            else
                buf[buflen++] = *src;
        }

        return ret;
    }

    /****************************************************************************
     * Description:     Convert a string to lower case. (Modifies the input     *
     *                  string)                                                 *
     *                              (uses C++ strings)                          *
     * Returns:         NIL                                                     *
     * Parameters:      string &src;    String to be converted to lower case    *
     ****************************************************************************/
    void toLowerCase(string &src)
    {
        transform(src.begin(), src.end(), src.begin(), tolower);
    }

    /****************************************************************************
     * Description:     Convert a string to lower case. (Modifies the input     *
     *                  string)                                                 *
     *                              (uses C strings)                            *
     * Returns:         NIL                                                     *
     * Parameters:      char *src;      String to be converted to lowercase     *
     ****************************************************************************/
    void toLowerCase(char *src)
    {
        for (; *src; ++src) *src = tolower(*src);
    }
}