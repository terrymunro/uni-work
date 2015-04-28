/////////////////////////////////////////////////////////////////////////////////
// Filename:    BinarySearchTree.h                                             //
// Author:      Terence Munro (s2587341)                                       //
// Email:       terence.munro@griffithuni.edu.au                               //
// Created:     05/05/2014                                                     //
// Modified:    04/06/2014 (Reworded some comments)                            //
// Description: This class represents a binary search tree which is a sorted   //
//              tree structure that consists of a root node and zero to two    //
//              child nodes that are ordered such that all nodes to the left   //
//              of the root node store data that is evaluated as <0 by a       //
//              comparison function and all right nodes store data that is     //
//              evaluated as >0 and so on for each node. Comparison function   //
//              returns 0 for equivalent node.                                 //
//                                                                             //
//              example:            (4)                                        //
//                                 /   \                                       //
//                               (2)   (9)                                     //
//                               /    /   \                                    //
//                             (1)  (5)   (12)                                 //
//                                                                             //
// Note:        Mostly written with recursive functions however the traversals //
//              were written with stacks to demonstrate the difference         //
/////////////////////////////////////////////////////////////////////////////////

#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

//////////////////////////// INCLUDES ///////////////////////////////////////////
#include <iostream>
#include <cassert>
#include <algorithm>
#include "Stack.h"

//////////////////////////// NAMESPACES /////////////////////////////////////////
using namespace std;

//////////////////////////// FORWARD DECLARATIONS ///////////////////////////////
template <typename T> struct BSTNode;
template <typename T> class  BSTIterator;

//////////////////////////// DEFAULT COMPARISON FUNCTION ////////////////////////

/********************************************************************************
 * Description:     Default comparison function used during searches etc. This  *
 *                  should be replaced in situations that require a more        *
 *                  involved comparison.                                        *
 * Returns:         (integer)   >0 if item is larger than node info             *
 *                               0 if item is equal to node info                *
 *                              <0 if item is smaller than node info            *
 * Parameters:      BSTNode<T> node;    node to be checked                      *
 *                  const void *item;   item being searched for                 *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
int bstDefaultCompare(const T &n, const void *itemInfo)
{
    T *i = (T*)itemInfo;

    return (*i == n) ? 0 : (*i > n ? 1 : -1);
}

//////////////////////////// BINARY SEARCH TREE /////////////////////////////////
template <typename T>
class BinarySearchTree
{
public:
    //- Comparison function prototype
    typedef int(*compFunc)(const T&, const void*);
    //- Visit node function prototype
    typedef void(*visitFunc)(const T&);

    int height() const;                 //- Height of the BST
    int size() const;                   //- Number of nodes in the BST
    int leavesCount() const;            //- Number of leaves in the BST

    bool isEmpty() const;               //- Check if tree is empty
    bool contains(const T&) const;      //- Check if info is in tree

    T* find(const void*);               //- Find info in tree with default compFunc
    T* find(compFunc, const void*);     //- Find info in tree

    void insert(const T&);              //- Insert new info into tree
    void insert(compFunc, const T&);    //-  ^ using non-default compFunc

    void remove(const T&);              //- Delete info from tree
    void remove(compFunc, const T&);    //-  ^ using non-default compFunc

    void inorder(visitFunc);            //- In-order traversal of tree
    void preorder(visitFunc);           //- Pre-order traversal of tree
    void postorder(visitFunc);          //- Post-order traversal of tree

    void destroyTree();                 //- Deallocate the memory for tree

    //- Overload the assignment operator
    const BinarySearchTree<T>& operator= (const BinarySearchTree<T>&);

    BinarySearchTree();                             //- Default constructor
    BinarySearchTree(const BinarySearchTree<T>&);   //- Copy constructor
    ~BinarySearchTree();                            //- Destructor

protected:
    BSTNode<T> *root;       //- Root node for the tree

private:
    //- Copies a Tree into another Tree - Does not balance
    void copyTree(BSTNode<T>*&, BSTNode<T>*&);

    //- Destroys a binary tree - Does not re-balance
    void destroy(BSTNode<T>*&);

    //- Find node containing info
    BSTNode<T>* search(compFunc, const void*) const;

    //- Traversals starting from specific node
    void inorder(BSTNode<T>*, visitFunc);
    void preorder(BSTNode<T>*, visitFunc);
    void postorder(BSTNode<T>*, visitFunc);

    //- Recursive functions called from public functions
    int height(BSTNode<T>*) const;
    int nodeCount(BSTNode<T>*) const;
    int leavesCount(BSTNode<T>*) const;

    //- Balancing functions
    void rotateToLeft(BSTNode<T>*&);
    void rotateToRight(BSTNode<T>*&);
    void balanceFromLeft(BSTNode<T>*&);
    void balanceFromRight(BSTNode<T>*&);
    void insertIntoTree(compFunc, BSTNode<T>*&, BSTNode<T>*&, bool&);

    //- Delete node and rebalance
    void deleteFromTree(BSTNode<T>*&);  //- Performs the deletion
    void deleteFromTree(compFunc, BSTNode<T>*&, const T&, bool&);
};

//////////////////////////// CLASS FUNCTIONS ////////////////////////////////////

/********************************************************************************
 * Description:     Retrieves the height of the Binary Search Tree              *
 * Returns:         (integer) height of the binary search tree                  *
 * Parameters:      NIL                                                         *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
int BinarySearchTree<T>::height() const
{
    return height(root);
}

/********************************************************************************
 * Description:     Retrieves a count of the number of nodes in the BST         *
 * Returns:         (integer) number of nodes in the tree                       *
 * Parameters:      NIL                                                         *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
int BinarySearchTree<T>::size() const
{
    return nodeCount(root);
}

/********************************************************************************
 * Description:     Retrieves the number of leaves in the BST                   *
 * Returns:         (integer) number of leaves in the tree                      *
 * Parameters:      NIL                                                         *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
int BinarySearchTree<T>::leavesCount() const
{
    return leavesCount(root);
}

/********************************************************************************
 * Description:     Check if the tree is empty                                  *
 * Returns:         (boolean)                                                   *
 * Parameters:      NIL                                                         *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
bool BinarySearchTree<T>::isEmpty() const
{
    return root == NULL;
}

/********************************************************************************
 * Description:     Check if the item is in the tree                            *
 * Returns:         (boolean)                                                   *
 * Parameters:      const T& item;  Item to be checked                          *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
bool BinarySearchTree<T>::contains(const T& item) const
{
    return search(bstDefaultCompare, &item) != NULL;
}

/********************************************************************************
 * Description:     Searches for an item in the tree and returns a pointer to   *
 *                  it in the tree                                              *
 * Returns:         (T*) Information searched for                               *
 * Parameters:      const T& item;  Item being searched for                     *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
T* BinarySearchTree<T>::find(const void *item)
{
    return find(bstDefaultCompare, item);
}

/********************************************************************************
 * Description:     Searches for an item in the tree and returns a pointer to   *
 *                  it in the tree (using comparison function)                  *
 * Returns:         (T*) Information searched for                               *
 * Parameters:      compFunc func;  Comparison function                         *
 *                  const T& item;  Item being searched for                     *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
T* BinarySearchTree<T>::find(compFunc func, const void *item)
{
    BSTNode<T>* result = search(func, item);
    return &(result->info);
}

/********************************************************************************
 * Description:     Insert item into the tree                                   *
 * Returns:         NIL                                                         *
 * Parameters:      const T& item;  Item to be inserted                         *
 * Preconditions:   NIL                                                         *
 * Postconditions:  Item is inserted and sorted in the tree                     *
 ********************************************************************************/
template <typename T>
void BinarySearchTree<T>::insert(const T& item)
{
    insert(bstDefaultCompare, item);
}

/********************************************************************************
 * Description:     Insert item into tree                                       *
 * Returns:         NIL                                                         *
 * Parameters:      compFunc compare;   Comparison function                     *
 *                  const T& item;      Item to be inserted                     *
 * Preconditions:   NIL                                                         *
 * Postconditions:  Item is inserted and sorted in the tree                     *
 ********************************************************************************/
template <typename T>
void BinarySearchTree<T>::insert(compFunc compare, const T& item)
{
    bool isTaller = false;

    BSTNode<T> *newNode = new BSTNode<T>;
    assert(newNode != NULL);

    newNode->info = item;
    newNode->balanceFactor = 0;
    newNode->left = NULL;
    newNode->right = NULL;

    insertIntoTree(compare, root, newNode, isTaller);
}

/********************************************************************************
 * Description:     Deletes an item from the tree using default comparison func *
 * Returns:         NIL                                                         *
 * Parameters:      const T& item;  Item to be removed                          *
 * Preconditions:   NIL                                                         *
 * Postconditions:  Item is removed from tree                                   *
 ********************************************************************************/
template <typename T>
void BinarySearchTree<T>::remove(const T& item)
{
    remove(bstDefaultCompare, item);
}

/********************************************************************************
 * Description:     Deletes an item from the tree                               *
 * Returns:         NIL                                                         *
 * Parameters:      compFunc compare;   Comparison function                     *
 *                  const T& item;      Item to be removed from the tree        *
 * Preconditions:   NIL                                                         *
 * Postconditions:  Item will no longer be in tree                              *
 ********************************************************************************/
template <typename T>
void BinarySearchTree<T>::remove(compFunc compare, const T& item)
{
    bool isShorter = false;

    deleteFromTree(compare, root, item, isShorter);
}

/********************************************************************************
 * Description:     In-order traversal of tree starting at root                 *
 * Returns:         NIL                                                         *
 * Parameters:      visitFunc visit;    Function used when visiting each node   *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
void BinarySearchTree<T>::inorder(visitFunc visit)
{
    inorder(root, visit);
}

/********************************************************************************
 * Description:     Pre-order traversal of tree starting at root                *
 * Returns:         NIL                                                         *
 * Parameters:      visitFunc visit;    Function used when visiting each node   *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
void BinarySearchTree<T>::preorder(visitFunc visit)
{
    preorder(root, visit);
}

/********************************************************************************
 * Description:     Post-order traversal of tree starting at root               *
 * Returns:         NIL                                                         *
 * Parameters:      visitFunc visit;    Function used when visiting each node   *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
void BinarySearchTree<T>::postorder(visitFunc visit)
{
    postorder(root, visit);
}

/********************************************************************************
 * Description:     Destroys the BST. Deallocates the memory                    *
 * Returns:         NIL                                                         *
 * Parameters:      NIL                                                         *
 * Preconditions:   NIL                                                         *
 * Postconditions:  root = NULL                                                 *
 ********************************************************************************/
template <typename T>
void BinarySearchTree<T>::destroyTree()
{
    destroy(root);
}

/********************************************************************************
 * Description:     Assignment operator                                         *
 ********************************************************************************/
template <typename T>
const BinarySearchTree<T>& BinarySearchTree<T>::operator=(const BinarySearchTree<T> &other)
{
    if (this != &other)
    {
        if (root != NULL)
            destroy(root);

        if (other.root != NULL)
            copyTree(root, other.root);
    }
    else
        cerr << "operator=: Can not assign me to myself." << endl;

    return *this;
}

/********************************************************************************
 * Description:     Default constructor                                         *
 ********************************************************************************/
template <typename T>
BinarySearchTree<T>::BinarySearchTree()
{
    root = NULL;
}

/********************************************************************************
 * Description:     Copy constructor                                            *
 ********************************************************************************/
template <typename T>
BinarySearchTree<T>::BinarySearchTree(const BinarySearchTree<T>& other)
{
    if (other.root == NULL)
        root = NULL;
    else
        copyTree(root, other.root);
}

/********************************************************************************
 * Description:     Destructor                                                  *
 ********************************************************************************/
template <typename T>
BinarySearchTree<T>::~BinarySearchTree()
{
    destroy(root);
}

/********************************************************************************
 * Description:     Copies a tree recursively                                   *
 * Returns:         NIL                                                         *
 * Parameters:      BSTNode<T>* &dst;   Destination for the copy                *
 *                  BSTNode<T>* &src;   Source for the copy                     *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
void BinarySearchTree<T>::copyTree(BSTNode<T>* &dst, BSTNode<T>* &src)
{
    if (src == NULL)
    {
        dst = NULL;
        return;
    }

    dst = new BSTNode<T>;
    dst->info = src->info;
    dst->balanceFactor = src->balanceFactor;

    copyTree(dst->left, src->left);
    copyTree(dst->right, src->right);
}

/********************************************************************************
 * Description:     Deletes a tree starting at node (recursive)                 *
 * Returns:         NIL                                                         *
 * Parameters:      BSTNode<T>* &node;  Root of the tree to be deleted          *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
void BinarySearchTree<T>::destroy(BSTNode<T>* &node)
{
    if (node != NULL)
    {
        destroy(node->left);
        destroy(node->right);
        delete node;
        node = NULL;
    }
}

/********************************************************************************
 * Description:     Searches for the item and returns the node holding it using *
 *                  a custom comparison function.                               *
 * Returns:         (BSTNode<T>*) Node that holds the information               *
 * Parameters:      compFunc compare;   Comparison function                     *
 *                  const void *item;   Item being searched for                 *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
BSTNode<T>* BinarySearchTree<T>::search(compFunc compare, const void *item) const
{
    BSTNode<T> *node = root;

    while (node != NULL)
    {
        int ret = compare(node->info, item);
        if (ret == 0)       return node;
        else if (ret < 0)   node = node->left;
        else                node = node->right;
    }

    return NULL;
}

/********************************************************************************
 * Description:     In-order traversal of tree starting at node (non-recursive) *
 * Returns:         NIL                                                         *
 * Parameters:      BSTNode<T>* &node;  Node to start at                        *
 *                  visitFunc visit;    Function used when visited each node    *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
void BinarySearchTree<T>::inorder(BSTNode<T> *node, visitFunc visit)
{
    //- Recursive version
    //if (node != NULL)
    //{
    //    inorder(node->left, visit);
    //    visit(node->info);
    //    inorder(node->right, visit);
    //}

    Stack<BSTNode<T>*>  stack;
    BSTNode<T>          *cur = node;

    while (cur != NULL || !stack.isEmpty())
    {
        if (cur != NULL)
        {
            stack.push(cur);
            cur = cur->left;
        }
        else
        {
            cur = stack.pop();
            visit(cur->info);
            cur = cur->right;
        }
    }
}

/********************************************************************************
 * Description:     Pre-order traversal of tree starting at node (non-recursive)*
 * Returns:         NIL                                                         *
 * Parameters:      BSTNode<T>* &node;  Node to start at                        *
 *                  visitFunc visit;    Function used when visited each node    *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
void BinarySearchTree<T>::preorder(BSTNode<T> *node, visitFunc visit)
{
    //- Recursive version:
    //if (node != NULL)
    //{
    //    visit(node->info);
    //    preorder(node->left, visit);
    //    preorder(node->right, visit);
    //}

    Stack<BSTNode<T>*>  stack;
    BSTNode<T>          *cur = node;

    while (cur != NULL || !stack.isEmpty())
    {
        if (cur != NULL)
        {
            visit(cur->info);
            stack.push(cur);
            cur = cur->left;
        }
        else
        {
            cur = stack.pop();
            cur = cur->right;
        }
    }
}

/********************************************************************************
 * Description:     Post-order traversal of tree starting at node(non-recursive)*
 * Returns:         NIL                                                         *
 * Parameters:      BSTNode<T>* &node;  Node to start at                        *
 *                  visitFunc visit;    Function used when visited each node    *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
void BinarySearchTree<T>::postorder(BSTNode<T> *node, visitFunc visit)
{
    //- Recursive version
    //if (node != NULL)
    //{
    //    postorder(node->left, visit);
    //    postorder(node->right, visit);
    //    visit(node->info);
    //}
    if (node == NULL)
        return;

    Stack<BSTNode<T>*>  nodeStack;
    Stack<int>          intStack;
    BSTNode<T>          *cur = node;
    int                 v = 0;

    nodeStack.push(cur);
    intStack.push(1);
    cur = cur->left;

    while (!nodeStack.isEmpty())
    {
        if (cur != NULL && v == 0)
        {
            nodeStack.push(cur);
            intStack.push(1);
            cur = cur->left;
        }
        else
        {
            cur = nodeStack.pop();
            v = intStack.pop();
            if (v == 1)
            {
                nodeStack.push(cur);
                intStack.push(2);
                cur = cur->right;
                v = 0;
            }
            else
                visit(cur->info);
        }
    }
}

/********************************************************************************
 * Description:     Retrieves the height of the BST (recursive function)        *
 * Returns:         (integer) height of the node                                *
 * Parameters:      BSTNode<T>* &node;  Node to start at                        *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
int BinarySearchTree<T>::height(BSTNode<T> *node) const
{
    if (node == NULL)
        return 0;

    return 1 + max(height(node->left), height(node->right));
}

/********************************************************************************
 * Description:     Retreives the number of nodes in the BST (recursive)        *
 * Returns:         (integer) number of nodes                                   *
 * Parameters:      BSTNode<T>* &node;  Node to start at                        *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
int BinarySearchTree<T>::nodeCount(BSTNode<T> *node) const
{
    if (node == NULL)
        return 0;

    return 1 + nodeCount(node->left) + nodeCount(node->right);
}

/********************************************************************************
 * Description:     Retreives the number of leaves in the BST (recursive)       *
 * Returns:         (integer) number of leaves                                  *
 * Parameters:      BSTNode<T>* &node;  Node to start at                        *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
int BinarySearchTree<T>::leavesCount(BSTNode<T> *node) const
{
    if (node == NULL)
        return 0;

    if (node->left == NULL && node->right == NULL)
        return 1;

    return leavesCount(node->left) + leavesCount(node->right);
}

/********************************************************************************
 * Description:     Rotates the node to the left                                *
 * Returns:         NIL                                                         *
 * Parameters:      BSTNode<T>* &node;  Node to rotate                          *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
void BinarySearchTree<T>::rotateToLeft(BSTNode<T>* &node)
{
    if (node == NULL)
    {
        cerr << "rotateToLeft: Attempted to rotate NULL ptr." << endl;
    }
    else if (node->right == NULL)
    {
        cerr << "rotateToLeft: Attempted to rotate with no right subtree." << endl;
    }
    else
    {
        BSTNode<T> *rightNode = node->right;
        node->right = rightNode->left;
        rightNode->left = node;
        node = rightNode;
    }
}

/********************************************************************************
 * Description:     Rotates the node to the right                               *
 * Returns:         NIL                                                         *
 * Parameters:      BSTNode<T>* &node;  Node to rotate                          *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
void BinarySearchTree<T>::rotateToRight(BSTNode<T>* &node)
{
    if (node == NULL)
    {
        cerr << "rotateToRight: Attempted to rotate NULL ptr." << endl;
    }
    else if (node->left == NULL)
    {
        cerr << "rotateToRight: Attempted to rotate with no left subtree." << endl;
    }
    else
    {
        BSTNode<T> *leftNode = node->left;
        node->left = leftNode->right;
        leftNode->right = node;
        node = leftNode;
    }
}

/********************************************************************************
 * Description:     Balances the tree from the left side                        *
 * Returns:         NIL                                                         *
 * Parameters:      BSTNode<T>* &node;  Node to balance                         *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
void BinarySearchTree<T>::balanceFromLeft(BSTNode<T>* &node)
{
    BSTNode<T>  *leftNode = node->left,
        *rightNode;

    switch (leftNode->balanceFactor)
    {
    case -1:
        node->balanceFactor = 0;
        leftNode->balanceFactor = 0;
        rotateToRight(node);
        break;

    case 0:
        cerr << "balanceFromLeft: Cannot balance from the left, balance factor already 0" << endl;
        break;

    case 1:
        rightNode = leftNode->right;
        switch (rightNode->balanceFactor)
        {
        case -1:
            node->balanceFactor = 1;
            leftNode->balanceFactor = 0;
            break;

        case 0:
            node->balanceFactor = 0;
            leftNode->balanceFactor = 0;
            break;

        case 1:
            node->balanceFactor = 0;
            leftNode->balanceFactor = -1;
            break;
        }
        rightNode->balanceFactor = 0;
        rotateToLeft(leftNode);
        node->left = leftNode;
        rotateToRight(node);
        break;
    }
}

/********************************************************************************
 * Description:     Balances the tree from the right side                       *
 * Returns:         NIL                                                         *
 * Parameters:      BSTNode<T>* &node;  Node to balance                         *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
void BinarySearchTree<T>::balanceFromRight(BSTNode<T>* &node)
{
    BSTNode<T>  *leftNode,
                *rightNode = node->right;

    switch (rightNode->balanceFactor)
    {
    case -1:
        leftNode = rightNode->left;
        switch (leftNode->balanceFactor)
        {
        case -1:
            node->balanceFactor = 0;
            rightNode->balanceFactor = 1;
            break;

        case 0:
            node->balanceFactor = 0;
            rightNode->balanceFactor = 0;
            break;

        case 1:
            node->balanceFactor = -1;
            rightNode->balanceFactor = 0;
            break;
        }
        leftNode->balanceFactor = 0;
        rotateToRight(rightNode);
        node->right = rightNode;
        rotateToLeft(node);
        break;

    case 0:
        cerr << "balanceFromRight: Cannot balance from the right, already balance factor 0" << endl;
        break;

    case 1:
        node->balanceFactor = 0;
        rightNode->balanceFactor = 0;
        rotateToLeft(node);
        break;
    }
}

/********************************************************************************
 * Description:     Inserts into tree while keeping balance                     *
 * Returns:         NIL                                                         *
 * Parameters:      BSTNode<T>* &node;  Node to balance                         *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
void BinarySearchTree<T>::insertIntoTree(compFunc compare, BSTNode<T>* &root,
    BSTNode<T>* &newNode, bool &isTaller)
{
    if (root == NULL)
    {
        root = newNode;
        isTaller = true;
        return;
    }

    int ret = compare(root->info, &newNode->info);
    if (ret == 0)
    {
        cerr << "insertIntoTree: Item already exists in the tree." << endl;
        delete newNode;
    }
    else if (ret < 0)
    {
        insertIntoTree(compare, root->left, newNode, isTaller);

        if (!isTaller)
            return;

        switch (root->balanceFactor)
        {
        case -1:
            balanceFromLeft(root);
            isTaller = false;
            break;

        case 0:
            root->balanceFactor = -1;
            break;

        case 1:
            root->balanceFactor = 0;
            isTaller = false;
            break;
        }
    }
    else
    {
        insertIntoTree(compare, root->right, newNode, isTaller);

        if (!isTaller)
            return;

        switch (root->balanceFactor)
        {
        case -1:
            root->balanceFactor = 0;
            isTaller = false;
            break;

        case 0:
            root->balanceFactor = 1;
            break;

        case 1:
            balanceFromRight(root);
            isTaller = false;
            break;
        }
    }
}

/********************************************************************************
 * Description:     Delete a node from the tree.                                *
 * Returns:         NIL                                                         *
 * Parameters:      BSTNode<T>* &node;  Node to delete                          *
 * Preconditions:   NIL                                                         *
 * Postconditions:  node = NULL                                                 *
 ********************************************************************************/
template <typename T>
void BinarySearchTree<T>::deleteFromTree(BSTNode<T>* &node)
{
    if (node->left == NULL && node->right == NULL)
    {
        BSTNode<T> *temp = node;
        node = NULL;
        delete temp;
    }
    else if (node->left == NULL)
    {
        BSTNode<T> *temp = node;
        node = node->right;
        delete temp;
    }
    else if (node->right == NULL)
    {
        BSTNode<T> *temp = node;
        node = node->left;
        delete temp;
    }
    else
    {
        BSTNode<T>  *cur = node->left,  // ptr for tree traversal
                    *prev = NULL;       // ptr behind cur(rent)

        while (cur->right != NULL)
        {
            prev = cur;
            cur = cur->right;
        }

        node->info = cur->info;
        if (prev == NULL)   //- current did not move; adjust node
            node->left = cur->left;
        else
            prev->right = cur->left;

        delete cur;
    }
}

/********************************************************************************
 * Description:     Delete a node from the tree.                                *
 * Returns:         NIL                                                         *
 * Parameters:      compFunc compare;   Comparison function                     *
 *                  BSTNode<T>* &root;  Node to check                           *
 *                  const T &item;      Item to be removed from tree            *
 *                  bool &isShorter;    Has this branch become shorter          *
 * Preconditions:   NIL                                                         *
 * Postconditions:  NIL                                                         *
 ********************************************************************************/
template <typename T>
void BinarySearchTree<T>::deleteFromTree(compFunc compare, BSTNode<T>* &root,
    const T &item, bool &isShorter)
{
    if (root == NULL)
    {
        cerr << "deleteFromTree: Item does not exist in the tree." << endl;
        return;
    }

    int ret = compare(root->info, &item);
    if (ret == 0)
    {
        deleteFromTree(root);
        isShorter = true;
    }
    else if (ret < 0)
    {
        deleteFromTree(compare, root->left, item, isShorter);

        if (!isShorter)
            return;

        switch (root->balanceFactor)
        {
        case -1:
            root->balanceFactor = 0;
            break;

        case 0:
            root->balanceFactor = 1;
            isShorter = false;
            break;

        case 1:
            balanceFromRight(root);
            isShorter = false;
            break;
        }
    }
    else
    {
        deleteFromTree(compare, root->right, item, isShorter);

        if (!isShorter)
            return;

        switch (root->balanceFactor)
        {
        case -1:
            balanceFromLeft(root);
            isShorter = false;
            break;

        case 0:
            root->balanceFactor = -1;
            isShorter = false;
            break;

        case 1:
            root->balanceFactor = 0;
            break;
        }
    }
}

///////////////////////////// NODE ELEMENT //////////////////////////////////////
template <typename T>
struct BSTNode
{
    T           info;           //- Stores data in the node
    int         balanceFactor;  //- Used to keep the tree balanced height
    BSTNode<T>  *left;          //- Left of this node are all nodes that return
                                //  <0 from the comparison function
    BSTNode<T>  *right;         //- Conversely right of this node are all nodes
                                //  that return >0 from the comparison function
};

#endif