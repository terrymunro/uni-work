#ifndef LINKEDLIST_H
#define LINKEDLIST_H

////////////////// HEADER PORTION ///////////////////
#include <iostream>
#include <cassert>

// Forward Declarations
template <typename T> struct Node;
template <typename T> class LinkedListIterator;

// Linked List class
template <typename T>
class LinkedList
{
public:
	typedef LinkedListIterator<T> iterator;
	typedef size_t size_type;
	typedef T value_type;
	typedef T* pointer;
	typedef T& reference;

	friend class LinkedListIterator<T>;
	iterator begin() { return iterator(*this, first); }
	iterator end() { return iterator(*this, last); }

    int length() const;
    bool isEmpty() const;
    bool contains(const T&) const;
    LinkedList<T> map(T (*)(T*)) const;
    LinkedList<T> filter(bool (*)(T*)) const;
    T* search(const T&) const;
    T* search(bool (*)(const Node<T>&, const void*), const void*) const;
    void initializeList();
    void insertFirst(const T&);
    void insertLast(const T&);
	void insertAfter(const T&, const T&);
	void insertAfter(bool (*)(const Node<T>&, const void*), const T&, const T&);
    void deleteNode(const T&);
    void deleteNode(bool (*)(const Node<T>&, const void*), const void*);
    void destroyList();
    T front() const;
    T back() const;
    template <typename T> friend std::ostream& operator<< (std::ostream&, const LinkedList<T>&);
    const LinkedList<T>& operator=(const LinkedList<T>&);
    LinkedList();
    LinkedList(const LinkedList<T>&);
    ~LinkedList();

protected:
    int count;
    Node<T> *first;
    Node<T> *last;

private:
    void copyList(const LinkedList<T>&);
};

// Node element
template <typename T>
struct Node
{
    T		info;
    Node<T> *link;
};

template <typename T>
class LinkedListIterator
{
private:
	LinkedList<T>	*linkedList;
	Node<T>			*ptr;

public:
	LinkedListIterator(LinkedList<T> &l)
	{
		linkedList = &l;
		ptr = linkedList->first;
	}
	LinkedListIterator(LinkedList<T> &l, Node<T> *p)
		: linkedList(&l), ptr(p) { }

	T &operator*()
	{
		return ptr->info;
	}

	T &next()
	{
		return ptr->link->info;
	}

	bool operator==(LinkedListIterator<T> rhs)
	{
		return linkedList == rhs.linkedList && ptr == rhs.ptr;
	}

	bool operator!=(LinkedListIterator<T> rhs)
	{
		return !this->operator==(rhs);
	}

	LinkedListIterator<T> &operator=(LinkedListIterator<T> &rhs)
	{
		linkedList = rhs.linkedList;
		ptr = rhs.ptr; 
		return *this;
	}

	LinkedListIterator<T> &operator++()
	{
		ptr = ptr->link;
		return *this;
	}

	LinkedListIterator<T> operator++(int)
	{
		LinkedList<T>::iterator clone(*this);
		ptr = ptr->link;
		return clone;
	}
};

/////////// IMPLEMENTATION PORTION ////////////
template <typename T>
bool defaultCompare(const Node<T>& node, const void *item)
{
    return node.info == *((T*)item);
}

template <typename T>
int LinkedList<T>::length() const
{
    return count;
}

template <typename T>
bool LinkedList<T>::isEmpty() const
{
    return length() == 0;
}

template <typename T>
bool LinkedList<T>::contains(T const& item) const
{
    return search(defaultCompare, &item) != NULL;
}

template <typename T>
LinkedList<T> LinkedList<T>::map(T (*func)(T*)) const
{
	LinkedList<T> mappedList;

    for (Node<T> *ptr = first; ptr != NULL; ptr = ptr->link)
    {
		T item = func(ptr->info);
		mappedList.insertLast(item);
    }

	return mappedList;
}

template <typename T>
LinkedList<T> LinkedList<T>::filter(bool (*predicate)(T*)) const
{
	LinkedList<T> filteredList;

    for (Node<T> *ptr = first; ptr != NULL; ptr = ptr->link)
    {
		if (predicate(ptr->info))
			filteredList.insertLast(ptr->info);
    }

	return filteredList;
}

template <typename T>
T* LinkedList<T>::search(T const& item) const
{
    return search(defaultCompare, &item);
}

template <typename T>
T* LinkedList<T>::search(bool (*predicate)(Node<T> const&, void const*), void const *item) const
{
    for (Node<T> *ptr = first; ptr != NULL; ptr = ptr->link)
    {
        if (predicate(*ptr, item))
            return &(ptr->info);
    }
    return NULL;
}

template <typename T>
void LinkedList<T>::initializeList()
{
    destroyList();

    first = last = NULL;
    count = 0;
}

template <typename T>
void LinkedList<T>::insertFirst(T const& item)
{
    Node<T> *ptr = new Node<T>;
    ptr->info = item;
    ptr->link = first;

    count++;
    first = ptr;
    if (last == NULL)
        last = first;
}

template <typename T>
void LinkedList<T>::insertLast(T const& item)
{
    Node<T> *ptr = new Node<T>;
    assert(ptr != NULL);
    ptr->info = item;
    ptr->link = NULL;

    count++;
    if (last != NULL)
        last->link = ptr;

    last = ptr;

    if (first == NULL)
        first = last;
}

template <typename T>
void LinkedList<T>::insertAfter(T const &preceedingItem, T const& newItem)
{
	insertAfter(defaultCompare, preceedingItem, newItem);
}

template <typename T>
void LinkedList<T>::insertAfter(bool (*predicate)(Node<T> const&, void const*), T const &preceedingItem, T const& newItem)
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

template <typename T>
void LinkedList<T>::deleteNode(T const& item)
{
    deleteNode(defaultCompare, &item);
}

template <typename T>
void LinkedList<T>::deleteNode(bool (*predicate)(const Node<T>&, const void*), const void *item)
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

template <typename T>
T LinkedList<T>::front() const
{
    return first->info;
}

template <typename T>
T LinkedList<T>::back() const
{
    return last->info;
}

template <typename T>
LinkedList<T> const& LinkedList<T>::operator=(LinkedList const& rhs)
{
    copyList(rhs);
    return *this;
}

template <typename T>
LinkedList<T>::LinkedList()
{
    first = last = NULL;
    count = 0;
}

template <typename T>
LinkedList<T>::LinkedList(LinkedList const& rhs) : LinkedList()
{
	destroyList();
    copyList(rhs);
}

template <typename T>
LinkedList<T>::~LinkedList()
{
    destroyList();
}

template <typename T>
void LinkedList<T>::copyList(LinkedList const& otherList)
{
    for (Node<T> *ptr = otherList.first; ptr != NULL; ptr = ptr->link)
    {
        insertLast(ptr->info);
    }
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const LinkedList<T>& list)
{
    for (Node<T> *ptr = list.first; ptr != NULL; ptr = ptr->link)
    {
        os << ptr->info;
        if (ptr->link != NULL)
            os << ", ";
    }

    return os;
}

#endif