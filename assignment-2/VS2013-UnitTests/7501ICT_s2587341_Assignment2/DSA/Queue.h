////////////////////////////////////////////////////////////////////////////////
// Filename:    Queue.h                                                       //
// Author:      Terence Munro (s2587341)                                      //
// Email:       terence.munro@griffithuni.edu.au                              //
// Created:     26/05/2014                                                    //
// Modified:    26/05/2014 (Quickly put together this queue class)            //
// Description: Templated implementation of a Queue. A first in first out     //
//              data structure                                                //
// Note:        Comments are lacking but the functionality is very basic and  //
//              and should not pose any real confusion. Any problems email me //
////////////////////////////////////////////////////////////////////////////////

#ifndef QUEUE_H
#define QUEUE_H

#include "LinkedList.h"

template <typename T>
class Queue : public LinkedList<T>
{
public:
    Queue<T>();
    Queue<T>(const Queue<T>&);

    void push_back(T item);
    T pop_front();

protected:
    LinkedList<T>::insertLast;
    LinkedList<T>::insertAfter;
    LinkedList<T>::insertFirst;
    LinkedList<T>::deleteNode;
};

template <typename T>
Queue<T>::Queue() : LinkedList()
{ }

template <typename T>
Queue<T>::Queue(Queue const &otherQueue)
: LinkedList(otherQueue)
{ }

template <typename T>
void Queue<T>::push_back(T item)
{
    insertLast(item);
}

template <typename T>
T Queue<T>::pop_front()
{
    if (isEmpty())
        return NULL;

    T ret = front();
    deleteNode(ret);
    return ret;
}

#endif