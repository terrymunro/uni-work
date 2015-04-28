/////////////////////////////////////////////////////////////////////////////////
// Filename:    LinkedList.h                                                   //
// Author:      Terence Munro (s2587341)                                       //
// Email:       terence.munro@griffithuni.edu.au                               //
// Created:     01/04/2014                                                     //
// Modified:    06/06/2014 (Cleaned up comments and touched up abstractions)   //
// Description: Templated implementation of a LinkedList. Nodes contain info   //
//              and a pointer to the next node in the list.                    //
/////////////////////////////////////////////////////////////////////////////////

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

////////////////////////// INCLUDES /////////////////////////////////////////////
#include <iostream>
#include <cassert>

////////////////////////// FORWARD DECLARATIONS /////////////////////////////////
template <typename T> struct Node;
template <typename T> class LinkedListIterator;

////////////////////////// DEFAULT COMPARISON FUNCTION //////////////////////////

/********************************************************************************
 * Description:     The default comparison function for searching through the   *
 *                  Linked List.                                                *
 * Returns:         (boolean) true if the node parameter contains the item      *
 *                  parameter as its information.                               *
 * Parameters:      const Node<T>& node;    The node to be checked              *
 *                  const void *item;       The item being searched for         *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
bool defaultCompare(const Node<T>& node, const void *item)
{
    return node.info == *((T*)item);
}

////////////////////////// CLASS DEFINITION /////////////////////////////////////
template <typename T>
class LinkedList
{
private:
    Node<T> *first; //- First node in the list, NULL if empty list
    Node<T> *last;  //- Last node in the list, NULL if empty, equal to the first
    //  if only one node
    int count;      //- Number of nodes in the linked list

    void copyList(const LinkedList<T>&); //- Copy another list into this

public:
    //- Typedefs
    typedef bool(*compFunc)(const Node<T>&, const void*);
    typedef bool(*filterFunc)(const T*);
    typedef T(*mapFunc)(const T*);

    //- Standard typedefs for iterators of a list class
    typedef LinkedListIterator<T> iterator;
    typedef size_t size_type;
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;

    friend class LinkedListIterator<T>;    //- Linked List Iterator class declared later
    iterator begin();                      //- Get an iterator starting at the beginning
    iterator end();                        //- Get an iterator starting at the end

    bool isEmpty() const;                  //- Check if the list is empty
    int length() const;                    //- Get the number of nodes in the list
    T front() const;                        //- Get the first element in the lists data
    T back() const;                         //- Get the last element in the lists data

    LinkedList<T> map(mapFunc) const;       //- Perform a function on every item in the list
    //  and return a new list with the modified data
    LinkedList<T> filter(filterFunc) const;
    //- Filter items in the list by a predicate function

    bool contains(const T&) const;          //- Find if an item is in this list
    Node<T>* search(const T&) const;        //- Find the node given data is in

    Node<T>* search(compFunc, const void*) const;
    //- Find the node given data is in by a predicate function

    void insertFirst(const T&);            //- Insert a new node at the beginning of the list
    void insertLast(const T&);             //- Insert a new node at the end of the list
    void insertAfter(const T&, const T&);  //- Insert a new node after the node with the given data
    void insertAfter(compFunc, const T&, const T&);
    //- Insert a new node after the node with the given data
    //  found using a given predicate function

    void deleteNode(const T&);             //- Delete a node with the given data
    void deleteNode(compFunc, const void*);//- Delete a node with the given data using a predicate
    //  function to find it
    void destroyList();                    //- Delete all elements from this list

    template <typename T>
    friend std::ostream& operator<< (std::ostream&, const LinkedList<T>&);
    //- Stream operator for outputting the contents of the list

    const LinkedList<T>& operator=(const LinkedList<T>&);
    //- Assignment operator copy rhs to this list

    void initializeList();                //- Initialize the Linked List (clears the list)

    //- Constructors and destructors
    LinkedList();
    LinkedList(const LinkedList<T>&);     //- Copy constructor
    ~LinkedList();
};

///////////////////////// LINKED LIST FUNCTIONS ////////////////////////////////

/********************************************************************************
 * Description:     Default constructor - initializes a clean empty list        *
 * Returns:         NIL                                                         *
 * Parameters:      NIL                                                         *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
LinkedList<T>::LinkedList()
{
    first = last = NULL;
    count = 0;
}

/********************************************************************************
 * Description:     Copy constructor - initializes a new list with contents     *
 *                  copied from another list                                    *
 * Returns:         NIL                                                         *
 * Parameters:      NIL                                                         *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
LinkedList<T>::LinkedList(const LinkedList &rhs) : LinkedList()
{
    destroyList();
    copyList(rhs);
}

/********************************************************************************
 * Description:     Destructor - Cleans up the list making sure nothing is left*
 *                  on the heap                                                 *
 * Returns:         NIL                                                         *
 * Parameters:      NIL                                                         *
 * Preconditions:   NIL                                                         *
 * Postconditions:  List is removed from memory                                 *
 ********************************************************************************/
template <typename T>
LinkedList<T>::~LinkedList()
{
    destroyList();
}

/********************************************************************************
 * Description:     Copy another list into this list                            *
 * Returns:         NIL                                                         *
 * Parameters:      const LinkedList<T>& otherList;  The list to be copied      *
 * Preconditions:   NIL                                                         *
 * Postconditions:  This list is a carbon copy of the other list                *
 ********************************************************************************/
template <typename T>
void LinkedList<T>::copyList(const LinkedList<T> &otherList)
{
    destroyList();
    for (Node<T> *ptr = otherList.first; ptr != NULL; ptr = ptr->link)
        insertLast(ptr->info);
}

/********************************************************************************
 * Description:     Create an iterator pointing at the start of the list        *
 * Returns:         (LinkedListIterator object) starting at the first node      *
 * Parameters:      NIL                                                         *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
LinkedListIterator<T> LinkedList<T>::begin()
{
    return iterator(*this, first);
}

/********************************************************************************
 * Description:     Create an iterator pointing at the end of the list          *
 * Returns:         (LinkedListIterator object) starting at the last node       *
 * Parameters:      NIL                                                         *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
LinkedListIterator<T> LinkedList<T>::end()
{
    return iterator(*this, NULL);
}

/********************************************************************************
 * Description:     Get the length of the list.                                 *
 * Returns:         (integer) number of nodes in the list                       *
 * Parameters:      NIL                                                         *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
int LinkedList<T>::length() const
{
    return count;
}

/********************************************************************************
 * Description:     Find out if the Linked List is empty                        *
 * Returns:         (boolean) true if the linked list has no nodes              *
 * Parameters:      NIL                                                         *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
bool LinkedList<T>::isEmpty() const
{
    return count == 0;
}

/********************************************************************************
 * Description:     Find out if an item is stored in this list                  *
 * Returns:         (boolean) true if the item is found in the list             *
 * Parameters:      const T item;  Item being looked for                        *
 * Preconditions:   List should not be empty                                    *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
bool LinkedList<T>::contains(const T& item) const
{
    return search(defaultCompare, &item) != NULL;
}

/********************************************************************************
 * Description:     Map all the data in this list onto a new list modified by   *
 *                    a given function.                                         *
 * Returns:         (LinkedList<T>) New list with modified data                 *
 * Parameters:      mapFunc func;   function that modifies data in some way     *
 *                                  the function must take in a T element and   *
 *                                  return the T after being modified           *
 * Preconditions:   List should not be empty                                    *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
LinkedList<T> LinkedList<T>::map(mapFunc func) const
{
    LinkedList<T> mappedList;

    for (Node<T> *ptr = first; ptr != NULL; ptr = ptr->link)
    {
        T item = func(ptr->info);
        mappedList.insertLast(item);
    }

    return mappedList;
}

/********************************************************************************
 * Description:     Produce a new list with elements from this list that pass   *
 *                    a given predicate function.                               *
 * Returns:         (LinkedList<T>) New list with filtered elements             *
 * Parameters:      filterFunc predicate; predicate function that filters data  *
 * Preconditions:   List should not be empty                                    *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
LinkedList<T> LinkedList<T>::filter(filterFunc predicate) const
{
    LinkedList<T> filteredList;

    for (Node<T> *ptr = first; ptr != NULL; ptr = ptr->link)
    {
        if (predicate(ptr->info))
            filteredList.insertLast(ptr->info);
    }

    return filteredList;
}

/********************************************************************************
 * Description:     Find the node given data is held by using the default       *
 *                    comparison function.                                      *
 * Returns:         (Node<T>*) Pointer to the node holding the data given or    *
 *                    NULL if not found                                         *
 * Parameters:      const T &item;    data being searched for in the list       *
 * Preconditions:   List should not be empty                                    *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
Node<T>* LinkedList<T>::search(const T &item) const
{
    return search(defaultCompare, &item);
}

/********************************************************************************
 * Description:     Find the node given data is held by using a specified       *
 *                    comparison function.                                      *
 * Returns:         (Node<T>*) Pointer to the node holding the data given or    *
 *                    NULL if not found                                         *
 * Parameters:      compFunc predicate;    Comparison function                  *
 *                  const T &item;    data being searched for in the list       *
 * Preconditions:   List should not be empty                                    *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
Node<T>* LinkedList<T>::search(compFunc predicate, const void *item) const
{
    for (Node<T> *ptr = first; ptr != NULL; ptr = ptr->link)
    {
        if (predicate(*ptr, item))
            return ptr;
    }
    return NULL;
}

/********************************************************************************
 * Description:     Insert a new item into the list at the front of the list    *
 * Returns:         NIL                                                         *
 * Parameters:      const T &item;    Item to be inserted                       *
 * Preconditions:   NIL                                                         *
 * Postconditions:  List has the new element in and count is incremeneted       *
 ********************************************************************************/
template <typename T>
void LinkedList<T>::insertFirst(const T &item)
{
    Node<T> *ptr = new Node<T>;
    ptr->info = item;
    ptr->link = first;

    count++;
    first = ptr;
    if (last == NULL)
        last = first;
}

/********************************************************************************
 * Description:     Insert a new item into the list at the end of the list      *
 * Returns:         NIL                                                         *
 * Parameters:      const T &item;    Item to be inserted                       *
 * Preconditions:    NIL                                                        *
 * Postconditions:  List has the new element in and count is incremeneted       *
 ********************************************************************************/
template <typename T>
void LinkedList<T>::insertLast(const T &item)
{
    Node<T> *ptr = new Node<T>;
    assert(ptr != NULL);

    ptr->info = item;
    ptr->link = NULL;
    count++;

    //- If there's already an element in the list set that elements link to this
    //  new node
    if (last != NULL)
        last->link = ptr;

    // Obviously this is now the last element so we set the last pointer to this
    last = ptr;

    // But in the case there is no elements in the list we should set the first
    if (first == NULL)
        first = last;
}

/********************************************************************************
 * Description:     Insert a new item into the list after a given item          *
 * Returns:         NIL                                                         *
 * Parameters:      const T &preceedingItem;    Item to place new item after    *
 *                  const T &item;              Item to be inserted             *
 * Preconditions:   NIL                                                         *
 * Postconditions:  List has the new element in and count is incremeneted       *
 ********************************************************************************/
template <typename T>
void LinkedList<T>::insertAfter(const T &preceedingItem, const T &newItem)
{
    insertAfter(defaultCompare, preceedingItem, newItem);
}

/********************************************************************************
 * Description:     Insert a new item into the list after a given item using a  *
 *                    given predicate function for location the given item      *
 * Returns:         NIL                                                         *
 * Parameters:      compFunc predicate;         Predicate function used to      *
 *                                                find the given item           *
 *                  const T &preceedingItem;    Item to place new item after    *
 *                  const T &item;              Item to be inserted             *
 * Preconditions:   NIL                                                         *
 * Postconditions:  List has the new element in and count is incremeneted       *
 ********************************************************************************/
template <typename T>
void LinkedList<T>::insertAfter(compFunc predicate, const T &preceedingItem, const T &newItem)
{
    Node<T> *newItemPtr = new Node<T>;
    assert(newItemPtr != NULL);

    newItemPtr->info = newItem;
    for (Node<T> *ptr = first; ptr != NULL; ptr = ptr->link)
    {
        if (predicate(*ptr, &preceedingItem))
        {
            if (ptr->link == last)
                last = newItemPtr;
            newItemPtr->link = ptr->link;
            ptr->link = newItemPtr;
            return;
        }
    }
}

/********************************************************************************
 * Description:     Delete an item from the list                                *
 * Returns:         NIL                                                         *
 * Parameters:      const T &item;    Item to be deleted                        *
 * Preconditions:   NIL                                                         *
 * Postconditions:  List no longer has the element and count is decremented     *
 ********************************************************************************/
template <typename T>
void LinkedList<T>::deleteNode(const T &item)
{
    deleteNode(defaultCompare, &item);
}

/********************************************************************************
 * Description:     Delete an item from the list using a given predicate        *
 *                    function to find it                                       *
 * Returns:         NIL                                                         *
 * Parameters:      compFunc predicate;   Function used to find the item        *
 *                  const T &item;        Item to be deleted                    *
 * Preconditions:   NIL                                                         *
 * Postconditions:  List no longer has the element and count is decremented     *
 ********************************************************************************/
template <typename T>
void LinkedList<T>::deleteNode(compFunc predicate, const void *item)
{
    Node<T> *cur, *prev = NULL;
    for (cur = first; cur != NULL; prev = cur, cur = cur->link)
    {
        if (predicate(*cur, item))
        {
            if (prev == NULL)
                // adjust first pointer if this is the old first
                first = cur->link;
            else
                // not the first element so link the previous to the next
                prev->link = cur->link;

            if (cur->link == NULL)
                // adjust the last pointer if this is the old last
                last = prev;

            --count;
            delete cur;

            return;
        }
    }
}

/********************************************************************************
 * Description:     Initialize the linked list to a default clean slate         *
 *                    (Alias of Destroy List)                                   *
 * Returns:         NIL                                                         *
 * Parameters:      NIL                                                         *
 * Preconditions:   NIL                                                         *
 * Postconditions:  List is reset to a clean slate all information is removed   *
 ********************************************************************************/
template <typename T>
void LinkedList<T>::initializeList()
{
    destroyList();
}

/********************************************************************************
 * Description:     Destroy the linked list returning it to a default state     *
 * Returns:         NIL                                                         *
 * Parameters:      NIL                                                         *
 * Preconditions:   NIL                                                         *
 * Postconditions:  List is reset to a clean slate all information is removed   *
 ********************************************************************************/
template <typename T>
void LinkedList<T>::destroyList()
{
    if (first == NULL)
        return;

    Node<T> *cur, *next;
    for (cur = first; cur != NULL; cur = next)
    {
        next = cur->link;
        delete cur;
    }

    first = last = NULL;
    count = 0;
}

/********************************************************************************
 * Description:     Get the information from the first element in the list      *
 * Returns:         (T) Information stored in the first item in the list        *
 * Parameters:      NIL                                                         *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
T LinkedList<T>::front() const
{
    return first->info;
}

/********************************************************************************
 * Description:     Get the information from the last element in the list       *
 * Returns:         (T) Information stored in the last item in the list         *
 * Parameters:      NIL                                                         *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
T LinkedList<T>::back() const
{
    return last->info;
}

/********************************************************************************
 * Description:     Copy operator: Copies given list into this list             *
 * Returns:         (LinkedList<T>) This list                                   *
 * Parameters:      const LinkedList & rhs;    List to be copied                *
 * Preconditions:   NIL                                                         *
 * Postconditions:  This list is a copy of the given list                       *
 ********************************************************************************/
template <typename T>
LinkedList<T> const& LinkedList<T>::operator=(const LinkedList<T> &rhs)
{
    copyList(rhs);
    return *this;
}

/********************************************************************************
 * Description:     Output stream operator - Used to export the contents of a   *
 *                    list as a stream comma separated                          *
 * Returns:         (ostream&) The output stream                                *
 * Parameters:      ostream& os;                The output stream               *
 *                  const LinkedList<T>& list;  The list bein outputted         *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
std::ostream& operator<<(std::ostream &os, const LinkedList<T> &list)
{
    for (Node<T> *ptr = list.first; ptr != NULL; ptr = ptr->link)
    {
        os << ptr->info;
        if (ptr->link != NULL)
            os << ", ";
    }

    return os;
}

////////////////////////// NODE DEFINITION /////////////////////////////////////
template <typename T>
struct Node
{
    T        info;  //- Data stored in the node
    Node<T> *link;  //- Link to the next node in the list
};

////////////////////////// ITERATOR DEFINITION /////////////////////////////////
template <typename T>
class LinkedListIterator
{
private:
    LinkedList<T>    *linkedList;   //- The list to be iterated over
    Node<T>          *ptr;          //- A pointer to the current node

public:
    //- Constructors
    LinkedListIterator(LinkedList<T>&);              //- Initialize iterator beginning at the front
    LinkedListIterator(LinkedList<T>&, Node<T>*);    //- Initialize at a specific starting node

    T &next();                                       //- Move the pointer to the next element
    T &operator*();                                  //- Dereference the information from the node

    //- Comparison operators
    bool operator==(LinkedListIterator<T>);
    bool operator!=(LinkedListIterator<T>);

    //- Copy iterator operator
    LinkedListIterator<T> &operator=(LinkedListIterator<T>&);

    //- Move the pointer to the next element
    LinkedListIterator<T> &operator++();
    LinkedListIterator<T> operator++(int);
};

////////////////////////// ITERATOR FUNCTIONS //////////////////////////////////

/********************************************************************************
 * Description:     Constructor - Initializes a Linked List iterator with a     *
 *                    pointer to the Linked List starting at the beginning      *
 * Returns:         NIL                                                         *
 * Parameters:      NIL                                                         *
 * Preconditions:   NIL                                                         *
 * Postconditions:  Iterator has a pointer to the list and the first element    *
 ********************************************************************************/
template <typename T>
LinkedListIterator<T>::LinkedListIterator(LinkedList<T> &l)
{
    linkedList = &l;
    ptr = linkedList->first;
}

/********************************************************************************
 * Description:     Constructor - Initializes a linked list iterator with a     *
 *                    pointer to given node                                     *
 * Returns:         NIL                                                         *
 * Parameters:      NIL                                                         *
 * Preconditions:   NIL                                                         *
 * Postconditions:  Iterator has a pointer to the list and given node           *
 ********************************************************************************/
template <typename T>
LinkedListIterator<T>::LinkedListIterator(LinkedList<T> &l, Node<T> *p)
: linkedList(&l), ptr(p)
{
}

/********************************************************************************
 * Description:     Get the information from the next node in the list          *
 * Returns:         (T) Information stored in the next node                     *
 * Parameters:      NIL                                                         *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
T &LinkedListIterator<T>::next()
{
    return ptr->link->info;
}

/********************************************************************************
 * Description:     Reference operator - for retrieving the information from    *
 *                    the current node                                          *
 * Returns:         (T) Information stored in the current node                  *
 * Parameters:      NIL                                                         *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
T &LinkedListIterator<T>::operator*()
{
    return ptr->info;
}

/********************************************************************************
 * Description:     Equality operator - for comparing if iterators are the same*
 * Returns:         (boolean) true if they are the same                         *
 * Parameters:      NIL                                                         *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
bool LinkedListIterator<T>::operator==(LinkedListIterator<T> rhs)
{
    return linkedList == rhs.linkedList && ptr == rhs.ptr;
}

/********************************************************************************
 * Description:     Not equal operator - for comparing if iterators are not     *
 *                    the same                                                  *
 * Returns:         (boolean) true if they are not the same                     *
 * Parameters:      NIL                                                         *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
bool LinkedListIterator<T>::operator!=(LinkedListIterator<T> rhs)
{
    return !this->operator==(rhs);
}

/********************************************************************************
 * Description:     Assignment operator - points this iterator to another       *
 *                    iterators linked list and position in the list            *
 * Returns:         (LinkedListIterator<T>&) this iterator                      *
 * Parameters:      NIL                                                         *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
LinkedListIterator<T> &LinkedListIterator<T>::operator=(LinkedListIterator<T> &rhs)
{
    linkedList = rhs.linkedList;
    ptr = rhs.ptr;
    return *this;
}

/********************************************************************************
 * Description:     Pre-increment operator - Moves the iterator to the next     *
 *                    element                                                   *
 * Returns:         (LinkedListIterator<T>&) this iterator                      *
 * Parameters:      NIL                                                         *
 * Preconditions:   NIL                                                         *
 * Postconditions:  Pointer moved to the next link in the list                  *
 ********************************************************************************/
template <typename T>
LinkedListIterator<T> &LinkedListIterator<T>::operator++()
{
    ptr = ptr->link;
    return *this;
}

/********************************************************************************
 * Description:     Post-increment operator - Moves the iterator to the next    *
 *                    element                                                   *
 * Returns:         (LinkedListIterator<T>&) this iterator                      *
 * Parameters:      NIL                                                         *
 * Preconditions:   NIL                                                         *
 * Postconditions:  Pointer moved to the next link in the list                  * 
 ********************************************************************************/
template <typename T>
LinkedListIterator<T> LinkedListIterator<T>::operator++(int)
{
    LinkedList<T>::iterator clone(*this);
    ptr = ptr->link;
    return clone;
}

#endif