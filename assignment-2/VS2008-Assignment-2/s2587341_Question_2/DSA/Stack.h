/////////////////////////////////////////////////////////////////////////////////
// Filename:    Stack.h                                                        //
// Author:      Terence Munro (s2587341)                                       //
// Email:       terence.munro@griffithuni.edu.au                               //
// Created:     26/05/2014                                                     //
// Modified:    04/06/2014 (Cleaned up comment formatting)                     //
// Description: Templated implementation of a Stack. A last in first out       //
//              data structure                                                 //
// Note:        Comments are lacking but the functionality is very basic and   //
//              and should not pose any real confusion. Any problems email me  //
/////////////////////////////////////////////////////////////////////////////////

#ifndef STACK_H
#define STACK_H

#include "LinkedList.h"

template <typename T>
class Stack : public LinkedList<T>
{
public:
    Stack<T>();
    Stack<T>(const Stack<T>&);

    void push(T item);  // Push item on top of stack
    T pop();            // Remove top and return it
    T top();            // Alias to front()

protected:
    LinkedList<T>::front; //- Realias'd to top();
    LinkedList<T>::insertLast;
    LinkedList<T>::insertAfter;
    LinkedList<T>::insertFirst;
    LinkedList<T>::deleteNode;
};

template <typename T>
Stack<T>::Stack() : LinkedList()
{ }

template <typename T>
Stack<T>::Stack(Stack const &otherStack)
: LinkedList(otherStack)
{ }

template <typename T>
void Stack<T>::push(T item)
{
    insertFirst(item);
}

template <typename T>
T Stack<T>::pop()
{
    if (isEmpty())
        return NULL;

    T ret = front();
    deleteNode(ret);
    return ret;
}

template <typename T>
T Stack<T>::top()
{
    return front();
}

#endif