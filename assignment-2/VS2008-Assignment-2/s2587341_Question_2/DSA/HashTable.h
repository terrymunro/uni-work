/////////////////////////////////////////////////////////////////////////////////
// Filename:    HashTable.h                                                    //
// Author:      Terence Munro (s2587341)                                       //
// Email:       terence.munro@griffithuni.edu.au                               //
// Created:     06/06/2014                                                     //
// Modified:    06/06/2014 (Quickly threw this together)                       //
// Description: A data structure that can map keys to values. This particular  //
//              hash table maps the value as its key using a hash function. A  //
//              hash table uses a hash function to compute an index into an    //
//              array of buckets, from which the correct value can be found.   //
//                  (ref. en.wikipedia.org/wiki/Hash_table)                    //
/////////////////////////////////////////////////////////////////////////////////

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdlib.h>
#include <string.h>
#include <vector>

template <int size>
class HashTable
{
public:
    int length() const;                             //- Number of items in the table
    vector<char*> suggestions(const char *) const;  //- Get a list of close matching
    bool contains(const char *) const;              //- Check if string is in table
    void insert(const char*);                       //- Insert string into table
    void remove(const char*);                       //- Remove stirng from table

    //- Constructors and destructors
    HashTable();
    ~HashTable();

protected:
    vector<char*> buckets[size];
    int numberOfEntries;

private:
    int hash(const char *) const;   //- Hash function
};

/********************************************************************************
 * Description:     Retrieves a count of the number of entries in the table     *
 * Returns:         (integer) number of strings in the table                    *
 * Parameters:      NIL                                                         *
 ********************************************************************************/
template <int size>
int HashTable<size>::length() const
{
    return numberOfEntries;
}

/********************************************************************************
 * Description:     Retrieve a list of close matching strings from the table    *
 * Returns:         (vector <char*>)    vector of strings                       *
 * Parameters:      const char *input;  string being looked for                 *
********************************************************************************/
template <int size>
vector<char*> HashTable<size>::suggestions(const char *input) const
{
    const vector<char*> *bucket;
    vector<char*> sugs;
    int index = hash(input);

    bucket = &buckets[index];
    for (vector<char*>::const_iterator i = bucket->begin();
        i != bucket->end(); ++i)
        sugs.push_back(*i);

    //- Pull in nearby buckets also
    bucket = &buckets[(index > 0 ? index : size) - 1];
    for (vector<char*>::const_iterator i = bucket->begin();
        i != bucket->end(); ++i)
        sugs.push_back(*i);

    bucket = &buckets[(index < size - 1 ? (index + 1) : 0)];
    for (vector<char*>::const_iterator i = bucket->begin();
        i != bucket->end(); ++i)
        sugs.push_back(*i);

    return sugs;
}

/********************************************************************************
 * Description:     Checks if the given string is in the table                  *
 * Returns:         (boolean)                                                   *
 * Parameters:      const char *input;  string being looked for                 *
 ********************************************************************************/
template <int size>
bool HashTable<size>::contains(const char *input) const
{
    int index = hash(input);
    for (vector<char*>::const_iterator i = buckets[index].begin();
        i != buckets[index].end(); ++i)
        if (strcmp(input, *i) == 0)
            return true;
    return false;
}

/********************************************************************************
 * Description:     Insert a string into the table                              *
 * Returns:         NIL                                                         *
 * Parameters:      const char *input;  string being inserted                   *
 ********************************************************************************/
template <int size>
void HashTable<size>::insert(const char* input)
{
    int index = hash(input);
    int len = strlen(input) + 1;
    char *newWord = new char[len];
    strcpy_s(newWord, len, input);
    buckets[index].push_back(newWord);
    numberOfEntries++;
}

/********************************************************************************
 * Description:     Remove the string from the table                            *
 * Returns:         NIL                                                         *
 * Parameters:      const char *input;  string being removed                    *
 ********************************************************************************/
template <int size>
void HashTable<size>::remove(const char* input)
{
    int index = hash(input);
    buckets[index].remove(input);
    --numberOfEntries;
}

/********************************************************************************
 * Description:     Default Constructor                                         *
 ********************************************************************************/
template <int size>
HashTable<size>::HashTable()
{
    numberOfEntries = 0;
}

/********************************************************************************
 * Description:     Destructor                                                  *
 ********************************************************************************/
template <int size>
HashTable<size>::~HashTable()
{
    for (int i = 0; i < size; i++)
    {
        for (vector<char*>::size_type j = 0; j < buckets[i].size(); j++)
        {
            char *tmp = buckets[i][j];
            delete [] tmp;
            buckets[i][j] = NULL;
        }
        buckets[i].clear();
    }
}

/********************************************************************************
 * Description:     Hash function - Uses a scalar multiplier to weigh the       *
 *                  initial few letters higher than the rest. Also only uses    *
 *                  the first 6 letters.                                        *
 * Returns:         (integer) hashed value                                      *
 * Parameters:      const char *input;  string being hashed                     *
 ********************************************************************************/
template <int size>
int HashTable<size>::hash(const char *input) const
{
    const int scalar[] = { 10, 8, 7, 2, 2, 1 };
    int v = 0, len = strlen(input);
    for (int n = 0; n < len && n < 6; ++n)
        v += static_cast<int>(input[n]) * scalar[n];
    return v % size;
}

#endif