#include <iostream>
#include <string>
#include "../DSA/MinimumSpanningTree.h"

using namespace std;

int main()
{
    int const NO_VERT = 100;
    MinimumSpanningTree<int, NO_VERT> mst;
    int source;

    cout << "Textbook, Chapter 12, Programming Exercise 5" << endl;
    cout << " from Data Structures Using C++ by D.S. Malik" << endl << endl;

    while (true)
    {
        string input;

        cout << "Please choose one of the following or press enter for default:" << endl;
        cout << "\t[ 0] Graph from Textbook figure 12-16 pg. 708 (default)" << endl;
        cout << "\t[-1] Read graph in from file" << endl;
        cout << "\t[>0] Create random graph with number of vertices specified (max: 100)" << endl;
        cout << "\t[ q] Quit the application" << endl << endl;
        cout << "[0]# ";

        getline(cin, input);
        
        if (input.compare("q") == 0)
        {
            cout << "Quitting application." << endl;
            break;
        }

        else if (input.compare("-1") == 0)
        {
            cout << "Enter the filename or press enter for 'graph.txt':" << endl << endl;
            cout << "[graph.txt]# ";

            getline(cin, input);

            if (input.empty())
                mst.createSpanningGraph(-1);
            else 
                mst.createGraph(input.c_str());
        }

        else
        {
            int value = atoi(input.c_str());
            mst.createSpanningGraph(value);
        }

        cout << endl;
        mst.printGraph();

        cout << "Enter source vertex: ";
        cin >> source;

        // Out of bounds cases
        if (source < 0)
            source = 0;
        if (source >= mst.numberOfVertices())
            source = mst.numberOfVertices() - 1;

        // Ignore any junk
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // Print out required information
        cout << endl << "Printing minimumSpanning:" << endl;
        mst.minimumSpanning(source);
        mst.printTreeAndWeight();

        cout << endl << "Printing prim2:" << endl;
        mst.prims2(source);
        mst.printTreeAndWeight();

        cout << endl;
    }

    system("pause");

    return 0;
}