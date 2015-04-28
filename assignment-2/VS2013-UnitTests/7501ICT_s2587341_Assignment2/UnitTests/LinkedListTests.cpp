#include "stdafx.h"
#include "..\DSA\LinkedList.h"
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTests
{
    TEST_CLASS(LinkedListTests)
    {
    public:
        static LinkedList<int> l;

        TEST_METHOD_INITIALIZE(setUp)
        {
            l.insertLast(1);
            l.insertLast(2);
            l.insertLast(3);
            l.insertLast(4);
        }

        TEST_METHOD_CLEANUP(tearDown)
        {
            l.destroyList();
        }

        TEST_METHOD(LinkedList_InitializeList)
        {
            Assert::IsFalse(l.isEmpty());

            l.initializeList();

            Assert::IsTrue(l.isEmpty());
            Assert::AreEqual<int>(0, l.length());
        }

        TEST_METHOD(LinkedList_Front)
        {
            Assert::AreEqual<int>(1, l.front());
        }

        TEST_METHOD(LinkedList_Iterator)
        {
            int count = 0;

            for (LinkedList<int>::iterator it = l.begin(); it != l.end(); it++)
                count++;

            Assert::AreEqual<int>(l.length(), count);
        }

        TEST_METHOD(LinkedList_Back)
        {
            Assert::AreEqual<int>(4, l.back());
        }

        TEST_METHOD(LinkedList_Length)
        {
            Assert::AreEqual<int>(4, l.length());
        }

        TEST_METHOD(LinkedList_LessThanLessThanOperator)
        {
            ostringstream os;
            os << l;
            
            Assert::AreEqual<string>("1, 2, 3, 4", os.str());
        }

        TEST_METHOD(LinkedList_InsertLast)
        {
            l.insertLast(5);

            Assert::AreEqual<int>(5, l.back());
        }

        TEST_METHOD(LinkedList_InsertFirst)
        {
            l.insertFirst(23);

            Assert::AreEqual<int>(23, l.front());
        }

        TEST_METHOD(LinkedList_Contains)
        {
            Assert::IsTrue(l.contains(3));
            Assert::IsFalse(l.contains(9));
        }

        TEST_METHOD(LinkedList_CopyMethods)
        {
            LinkedList<int> newList;
            newList = l;

            // Clean the old list to make sure its a deep copy
            l.initializeList();

            Assert::AreEqual<int>(4, newList.length());
            Assert::AreEqual<int>(0, l.length());
        }

        TEST_METHOD(LinkedList_DeleteNode)
        {
            l.deleteNode(2);
            Assert::AreEqual<int>(3, l.length());
            Assert::IsFalse(l.contains(2));

            l.deleteNode(1);
            Assert::AreEqual<int>(2, l.length());
            Assert::IsFalse(l.contains(1));
        }
    };

    LinkedList<int> LinkedListTests::l;
}