#include "..\DSA\BinarySearchTree.h"
#include "..\DSA\StringHelpers.h"
#include "..\DSA\HashTable.h"

using namespace StringHelpers;

void print(const string &s);
void BSTtoString(const string &s);
int compare(const string &nodeStr, const void *item);

//- Argh Global Variables are bad! Usually they are bad but here we have no race
//  conditions and this string is used for a single purpose. Finally we're noting
//  these facts in comments
string bstString;

int main()
{
    BinarySearchTree<string> bst;
    HashTable<500> ht;
    string cppStr;
    char *cStr;
    vector<string> cppWords;
    vector<char*> cWords;

    /////////////////////////// QUESTION 2 a ////////////////////////////////////
    cout << "Reading in 'Ass2 Dictionary.txt'..." << endl;
    cppStr = readFile("Ass2 Dictionary.txt");
    cppWords = split(cppStr, isspace);

    //- Add words to binary search tree
    cout << "Adding words to binary search tree..." << endl;
    for (vector<string>::iterator it = cppWords.begin(); it != cppWords.end(); it++)
    {
        toLowerCase(*it);
        if (isWord(*it) && !bst.contains(*it))
            bst.insert(compare, *it);
    }

    //- Print out statistics
    cout << "Binary Search Tree complete." << endl;
    cout << "Height:\t\t" << bst.height() << endl;
    cout << "# Nodes:\t" << bst.size() << endl;
    cout << "# Leaves:\t" << bst.leavesCount() << endl;

    //- Offer to print out alphabetic listing
    cout << endl << "Would you like an alphabetical listing of the words? [yN] ";
    getline(cin, cppStr);

    toLowerCase(cppStr);
    if (cppStr.substr(0, 1) == "y")
        bst.inorder(print);

    cout << endl << endl;
    cout << "Saving dictionary to 'dict.txt'..." << endl;

    //- To keep abstraction I don't want to tie the saving of the binary search
    //  tree into the StringHelper library.. so convert it to a string first
    bst.inorder(BSTtoString);
    saveFile("dict.txt", bstString);

    /////////////////////////// QUESTION 2 b ////////////////////////////////////
    cout << "Reading dictionary back in and inputting into Hash Table" << endl;
    cStr = readFileStream("dict.txt");
    cWords = split(cStr, isspace);
    for (vector<char*>::iterator it = cWords.begin(); it != cWords.end(); it++)
        ht.insert(*it);
    free(cStr); //- remembering to deallocate malloc'd memory

    cout << "Reading in 'Eisenhower spell.txt' for spell checking." << endl;
    cStr = readFileStream("Eisenhower spell.txt");
    cWords = split(cStr, isspace);
    free(cStr); //- remembering to deallocate malloc'd memory

    for (vector<char*>::iterator it = cWords.begin(); it != cWords.end(); it++)
    {
        toLowerCase(*it);
        if (isWord(*it) && !ht.contains(*it))
        {
            vector<char*> sugs = ht.suggestions(*it);

            cout << "Found a possible spelling mistake '" << *it << "'." << endl;
            if (sugs.size() > 0)
            {
                cout << "Did you mean one of the following: " << endl;
                for (vector<char*>::iterator s = sugs.begin();
                    s != sugs.end(); s++)
                    cout << "- " << *s << endl;
            }
            else
                cout << "No suggestions." << endl;
            cout << endl;
            system("pause");
        }
    }

    cout << "Thank you." << endl;
    system("pause");
}

//- Simple function to print string to standard out
void print(const string &s)
{
    cout << s << endl;
}

//- Simple function to concatinate strings into a global string
void BSTtoString(const string &s)
{
    bstString += s + '\n';
}

//- Replace default comparison function with the string comparison function
int compare(const string &n, const void *item)
{
	const string i = *((const string*)item);
	return i.compare(n);
}