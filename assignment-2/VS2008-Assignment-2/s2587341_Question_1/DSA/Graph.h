////////////////////////////////////////////////////////////////////////////////
// Filename:    Graph.h                                                       //
// Author:      Terence Munro (s2587341)                                      //
// Email:       terence.munro@griffithuni.edu.au                              //
// Created:     28/05/2014                                                    //
// Modified:    30/05/2014 (Clean up comments)                                //
// Description: Templated Graph class                                         //
////////////////////////////////////////////////////////////////////////////////

#ifndef GRAPH_H
#define GRAPH_H

////////////////////////// INCLUDES ////////////////////////////////////////////
#include <time.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "LinkedList.h"
#include "Queue.h"

////////////////////////// NAMESPACES //////////////////////////////////////////
using namespace std;

////////////////////////// CONSTANTS ///////////////////////////////////////////
const int infinity = 10000000;

////////////////////////// CLASS DEFINITION ////////////////////////////////////
template <typename Vertex, int size>
class Graph
{
public:
    bool isEmpty();                 // Determines whether the graph is empty
                                    // Postcondition: Returns true if the graph
                                    //                is empty; otherwise, false

    void createGraph(int);          // Creates the graph from one of three
                                    // different sources (calls other overloads
                                    // for 0 and <0 cases)
                                    // Postcondition: The graph is created in the
                                    //                form of adjacenty lists

    void createGraph();             // Creates the graph using a hardcoded 
                                    // example

    void createGraph(char const*);  // Creates the graph using a file input
                                    // filename specified by the input
                                    

    void clearGraph();              // Deallocates the memory occupied by the 
                                    // linked lists and the array of pointers
                                    // pointing to the linked lists

    void printGraph() const;        // Prints the graph

    void depthFirstTraversal();     // Performs a depth first traversal of the
                                    // entire graph

    void dftAtVertex(Vertex);       // Performs a depth first traversal of the
                                    // graph at a vertex specified by the parameter
                                    // vertex

    void breadthFirstTraversal();   // Performs a breadth first traversal of the
                                    // entire graph

    int numberOfVertices();         // Retrieve the size of the graph

    Graph();                        // Default constructor
                                    // Postcondition: The graph size is set to 
                                    //                0, that is, vertexCount = 0; 

    ~Graph();                       // Destructor
                                    // Postcondition: The storage occupied by 
                                    //                the graph is deallocated

protected:
    int vertexCount;                // Current number of vertices
    int edgeCount;                  // Current number of edges
    LinkedList<Vertex> graph[size]; // Array of pointers to create the adjacency
                                    // lists (linked lists)

    virtual void addEdge(Vertex, Vertex, int);  // Add a new edge to the graph

private:
    void dft(Vertex v, bool visited[]); // Performs the depth first traversal of the
                                        // graph at a particular vertex

    void bftVisitVertex(Vertex, bool[], Queue<Vertex>&);    // Visits a vertex;
                                                            // used in the breadth
                                                            // first traversal
};

////////////////////////// CLASS FUNCTIONS /////////////////////////////////////

/*******************************************************************************
 * Description:     Add an edge between startVertex and endVertex              *
 * Returns:         NIL                                                        *
 * Parameters:      Vertex start;   Starting point                             *
 *                  Vertex end;     Ending point                               *
 * Preconditions:   NIL                                                        *
 * Postconditions:  Increment edge count and add the edge to the linked list   *
 *                  entry for start vertex                                     *
 *******************************************************************************/
template <typename Vertex, int size>
void Graph<Vertex, size>::addEdge(Vertex start, Vertex end, int weight)
{
    if (weight <= 0)
        return;

    edgeCount++;
    graph[start].insertLast(end);
}

/*******************************************************************************
 * Description:     Creates a graph from one of three sources;                 *
 * Returns:         NIL                                                        *
 * Parameters:      int input;  determines how the graph is created            *
 *                      input > 0: Random edges with input number of vertices  *
 *                      input = 0: Specific case hardcoded from the text book  *
 *                      input < 0: Input read from file named graph.txt        *
 * Preconditions:   size must be larger than input                             *
 * Postconditions:  LinkedList contains vertices populated from inputted source* 
 *******************************************************************************/
template <typename Vertex, int size>
void Graph<Vertex, size>::createGraph(int const input)
{
    if (input == 0) //--------- Hardcoded example ------------------------------
    {
        createGraph();
    }
    else if (input < 0) //----- Read from file ---------------------------------
    {
        createGraph("graph.txt");
    }
    else //-------------------- Randomly generated values ----------------------
    {
        // Assigned size needs to be more than the vertices we're going to add
        assert(input <= size);

        if (!isEmpty())
            clearGraph();

        srand((unsigned int)time(NULL)); // seed the rng

        for (int row = 0; row < input; row++)
            for (int col = 0; col < input; col++)
                // Scale the number of adjacencies based on the number of vertices
                if (rand() % int(ceil(input * 0.35)) == 0)
                    addEdge(row, col, (rand() % (input * 2)) + 1);
        vertexCount = input;
    }
}

/*******************************************************************************
 * Description:     Creates a graph from one of three sources;                 *
 * Returns:         NIL                                                        *
 * Parameters:      int input;  determines how the graph is created            *
 *                      input > 0: Random edges with input number of vertices  *
 *                      input = 0: Specific case hardcoded from the text book  *
 *                      input < 0: Input read from file named graph.txt        *
 * Preconditions:   size must be larger than input                             *
 * Postconditions:  LinkedList contains vertices populated from inputted source* 
 *******************************************************************************/
template <typename Vertex, int size>
void Graph<Vertex, size>::createGraph()
{
    const int NO_VERTICES = 7;

    // Assigned size needs to be more than the vertices we're going to add
    assert(size >= NO_VERTICES);

    if (!isEmpty())
        clearGraph();

    int exampleGraph[NO_VERTICES][NO_VERTICES] = {
        {  0,  6,  5,  2,  0,  0,  0 },
        {  6,  0,  0,  0,  2,  0,  4 },
        {  5,  0,  0,  0,  0,  7,  5 },
        {  2,  0,  0,  0,  8,  0,  0 },
        {  0,  2,  0,  8,  0, 10,  0 },
        {  0,  0,  7,  0, 10,  0,  0 },
        {  0,  4,  5,  0,  0,  0,  0 }
    };

    for (int row = 0; row < NO_VERTICES; row++)
        for (int col = 0; col < NO_VERTICES; col++)
            addEdge(row, col, exampleGraph[row][col]);

    vertexCount = NO_VERTICES;
}

/*******************************************************************************
 * Description:     Creates a graph from one of three sources;                 *
 * Returns:         NIL                                                        *
 * Parameters:      int input;  determines how the graph is created            *
 *                      input > 0: Random edges with input number of vertices  *
 *                      input = 0: Specific case hardcoded from the text book  *
 *                      input < 0: Input read from file named graph.txt        *
 * Preconditions:   size must be larger than input                             *
 * Postconditions:  LinkedList contains vertices populated from inputted source* 
 *******************************************************************************/
template <typename Vertex, int size>
void Graph<Vertex, size>::createGraph(char const *fileName)
{
    if (!isEmpty())
        clearGraph();

    ifstream infile;
    int col, value;
    char c;

    col = value = 0;

    infile.open(fileName);
    if (!infile)
    {
        cout << "Cannot open '" << fileName << "' please check it is in the "
            "correct location." << endl;
        return;
    }

    do
    {
        c = infile.peek();
        if (c == '\n' || col == size) // new line and column limit specifically
        {
            if (col == size) // skip the rest of the line
                infile.ignore(numeric_limits<streamsize>::max(), '\n');

            vertexCount++;
            col = 0; 

            // If we hit max size stop reading for more
            if (vertexCount == size)
                break;
        }

        if (iswspace(c) && infile.get()) // skip whitespace
            continue;

        infile >> value;
        addEdge(vertexCount, col, value);

        col++;
    } while (!infile.eof());

    infile.close();
}

/*******************************************************************************
 * Description:     Clears the graph                                           *
 * Returns:         NIL                                                        *
 * Parameters:      NIL                                                        *
 * Preconditions:   NIL                                                        *
 * Postconditions:  graph is empty and vertexCount set to 0                          *
 *******************************************************************************/
template <typename Vertex, int size>
void Graph<Vertex, size>::clearGraph()
{
    for (Vertex v = 0; v < vertexCount; v++)
        graph[v].destroyList();

    vertexCount = 0;
    edgeCount = 0;
}

/*******************************************************************************
 * Description:     Print the graph to standard out                            *
 * Returns:         NIL                                                        *
 * Parameters:      NIL                                                        *
 * Preconditions:   NIL                                                        *
 * Postconditions:  NIL                                                        *
 *******************************************************************************/
template <typename Vertex, int size>
void Graph<Vertex, size>::printGraph() const
{
    for (Vertex v = 0; v < vertexCount; v++)
        cout << "V[" << v << "]: " << graph[v] << endl;

    cout << endl;
}

/*******************************************************************************
 * Description:     Determines whether the graph has any vertices              *
 * Returns:         (boolean) true if vertexCount is 0; otherwise, false       *
 * Parameters:      NIL                                                        *
 * Preconditions:   NIL                                                        *
 * Postconditions:  NIL                                                        *
 *******************************************************************************/
template <typename Vertex, int size>
bool Graph<Vertex, size>::isEmpty()
{
    return vertexCount == 0;
}

/*******************************************************************************
 * Description:     Retrieves the number of vertices in the graph              *
 * Returns:         (integer) number of vertices in the graph                  *
 * Parameters:      NIL                                                        *
 * Preconditions:   NIL                                                        *
 * Postconditions:  NIL                                                        *
 *******************************************************************************/
template <typename Vertex, int size>
int Graph<Vertex, size>::numberOfVertices()
{
    return vertexCount;
}

/*******************************************************************************
 * Description:     Default constructor                                        *
 *******************************************************************************/
template <typename Vertex, int size>
Graph<Vertex, size>::Graph()
{
    vertexCount = 0;
    edgeCount = 0;
}

/*******************************************************************************
 * Description:     Destructor                                                 *
 *******************************************************************************/
template <typename Vertex, int size>
Graph<Vertex, size>::~Graph()
{
    clearGraph();
}

/*******************************************************************************
 * Description:     Depth first traversal - Calls a private function that uses *
 *                  recursion to traverse the graph                            *
 * Returns:         NIL                                                        *
 * Parameters:      NIL                                                        *
 * Preconditions:   NIL                                                        *
 * Postconditions:  NIL                                                        *
 *******************************************************************************/
template <typename Vertex, int size>
void Graph<Vertex, size>::depthFirstTraversal()
{
    int index;
    // array to keep track of the visited vertices
    bool *visited = new bool[vertexCount];

    for (index = 0; index < vertexCount; index++)
        visited[index] = false;

    for (index = 0; index < vertexCount; index++) // Every vertex
        if (!visited[index])                // that is not visited,
            dft(index, visited);            // do a depth first traversal

    delete[] visited;

    cout << endl;
}

/*******************************************************************************
 * Description:     Depth first traversal algorithm - recursively calls itself *
 * Returns:         NIL                                                        *
 * Parameters:      Vertex v;       Vertex being visited                       *
 *                  bool visited[]; Vertices that have already been visited    *
 * Preconditions:   NIL                                                        *
 * Postconditions:  NIL                                                        *
 *******************************************************************************/
template <typename Vertex, int size>
void Graph<Vertex, size>::dft(Vertex v, bool visited[])
{
    // Iterator for getting adjacent vertices
    LinkedList<Vertex>::iterator it = graph[v].begin();  

    // visit the vertex
    visited[v] = true;
    cout << " " << v << " ";                        

    if (graph[v].length() == 0)
        return;

    do
    {
        if (!visited[*it])
            dft(*it, visited);
    } while (it++ != graph[v].end());
}

/*******************************************************************************
 * Description:     Depth first traversal - starting at a specific vertex      *
 * Returns:         NIL                                                        *
 * Parameters:      Vertex vertex;   Vertex to start the traversal at          *
 * Preconditions:   NIL                                                        *
 * Postconditions:  NIL                                                        *
 *******************************************************************************/
template <typename Vertex, int size>
void Graph<Vertex, size>::dftAtVertex(Vertex vertex)
{
    bool *visited;

    visited = new bool[vertexCount];

    for (int index = 0; index < vertexCount; index++)
        visited[index] = false;

    dft(vertex, visited);

    delete[] visited;
}
 
/*******************************************************************************
 * Description:     Breadth first traversal - technique using a queue to       *
 *                    traverse the graph one level at a time printing visited  *
 *                    vertices to standard output                              *
 * Returns:         NIL                                                        *
 * Parameters:      NIL                                                        *
 * Preconditions:   NIL                                                        *
 * Postconditions:  NIL                                                        *
 *******************************************************************************/
template <typename Vertex, int size>
void Graph<Vertex, size>::breadthFirstTraversal()
{
    Queue<Vertex> queue;
    bool *visited = new bool[vertexCount];
    Vertex *adjacencyList = new Vertex[vertexCount];
    Vertex u;
    int alLength = 0;

    // initialize the array
    for (int ind = 0; ind < vertexCount; ind++)
        visited[ind] = false; 

    for (int index = 0; index < vertexCount; index++)
    {
        bftVisitVertex(index, visited, queue);

        while (!queue.isEmpty())
        {
            u = queue.pop_front();

            if (graph[u].isEmpty()) // Disconnected node
                continue;

            LinkedList<Vertex>::iterator it = graph[u].begin();
            do
            {
                bftVisitVertex(*it, visited, queue);
            } while (it++ != graph[u].end());
        }
    }

    delete[] visited;

    cout << endl;
}

/*******************************************************************************
 * Description:     Breadth first traversal - visit a vertex                   *
 * Returns:         NIL                                                        *
 * Parameters:      Vertex v;           Vertex to be visited                   *
 *                  bool visited[];     Visited vertices                       *
 *                  Queue<Vertex> &q;   Queue of vertices to be visited        *
 * Preconditions:   NIL                                                        *
 * Postconditions:  NIL                                                        *
 *******************************************************************************/
template <typename Vertex, int size>
void Graph<Vertex, size>::bftVisitVertex(Vertex v, bool visited[], Queue<Vertex> &q)
{
    if (visited[v])
        return;

    q.push_back(v);
    visited[v] = true;
    cout << " " << v << " ";
}

#endif