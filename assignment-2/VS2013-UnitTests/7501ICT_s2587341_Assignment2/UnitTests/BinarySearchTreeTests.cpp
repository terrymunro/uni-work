#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\DSA\BinarySearchTree.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
    /* helper functions to reduce repetition */
    const int MAX = 6;
    static int intArr[MAX];
    static int index;

    void addToArray(const int &i)
    {
        intArr[index++] = i;
    }

    void initializeArray()
    {
        for (int i = 0; i < MAX; i++)
            intArr[i] = 0;
        index = 0;
    }

    void initializeBst(BinarySearchTree<int> &bst)
    {
        bst.insert(2);
        bst.insert(9);
        bst.insert(1);
        bst.insert(4);
        bst.insert(12);
        bst.insert(5);
    }

    TEST_CLASS(BinarySearchTreeTests)
    {
    public:
        TEST_METHOD(BST_Insert)
        {
            BinarySearchTree<int> bst;

            Assert::AreEqual<int>(0, bst.size());

            initializeBst(bst);

            Assert::AreEqual<int>(6, bst.size());
        }

        TEST_METHOD(BST_Remove)
        {
            BinarySearchTree<int> bst;

            initializeBst(bst);
            Assert::AreEqual<int>(6, bst.size());

            bst.remove(4);
            Assert::AreEqual<int>(5, bst.size());
            Assert::IsFalse(bst.contains(4));
        }

        TEST_METHOD(BST_Contains)
        {
            BinarySearchTree<int> bst;

            initializeBst(bst);

            Assert::IsTrue(bst.contains(2));
            Assert::IsFalse(bst.contains(49));
        }

        TEST_METHOD(BST_Empty)
        {
            BinarySearchTree<int> bst;

            Assert::IsTrue(bst.isEmpty());

            bst.insert(12);

            Assert::IsFalse(bst.isEmpty());
        }

        TEST_METHOD(BST_Find)
        {
            BinarySearchTree<int> bst;

            initializeBst(bst);

            int itemToFind = 1;
            int *item = bst.find(&itemToFind);
            
            Assert::AreEqual<int>(itemToFind, *item);
        }

        TEST_METHOD(BST_Inorder)
        {
            BinarySearchTree<int> bst;
            int answers[] = { 1, 2, 4, 5, 9, 12 };

            initializeArray();
            initializeBst(bst);

            bst.inorder(addToArray);
            for (int i = 0; i < MAX; i++)
                Assert::AreEqual<int>(answers[i], intArr[i]);
        }

        TEST_METHOD(BST_Preorder)
        {
            BinarySearchTree<int> bst;
            int answers[] = { 4, 2, 1, 9, 5, 12 };
            
            initializeArray();
            initializeBst(bst);

            bst.preorder(addToArray);
            for (int i = 0; i < MAX; i++)
                Assert::AreEqual<int>(answers[i], intArr[i]);
        }

        TEST_METHOD(BST_Postorder)
        {
            BinarySearchTree<int> bst;
            int answers[] = { 1, 2, 5, 12, 9, 4 };

            initializeArray();
            initializeBst(bst);

            bst.postorder(addToArray);
            for (int i = 0; i < MAX; i++)
                Assert::AreEqual<int>(answers[i], intArr[i]);
        }

        TEST_METHOD(BST_LeavesCount)
        {
            BinarySearchTree<int> bst;

            initializeBst(bst);

            Assert::AreEqual<int>(3, bst.leavesCount());
        }

        TEST_METHOD(BST_Destroy)
        {
            BinarySearchTree<int> bst;

            initializeBst(bst);

            Assert::AreEqual<int>(6, bst.size());

            bst.destroyTree();

            Assert::IsTrue(bst.isEmpty());
        }

        TEST_METHOD(BST_Height)
        {
            BinarySearchTree<int> bst;

            initializeBst(bst);

            Assert::AreEqual<int>(3, bst.height());

            bst.remove(1);
            bst.remove(2);
            bst.remove(9);
            bst.remove(5);

            Assert::AreEqual<int>(2, bst.height());
        }
    };
}