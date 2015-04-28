#include "stdafx.h"
#include "CppUnitTest.h"
#include "../DSA/Queue.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
    TEST_CLASS(QueueTests)
    {
    public:
        TEST_METHOD(Queue_PushBackAndPopFront)
        {
            Queue<int> q;

            q.push_back(7);
            q.push_back(9);
            q.push_back(11);

            Assert::AreEqual<int>(3, q.length());

            int ret = q.pop_front();

            Assert::AreEqual<int>(7, ret);
            Assert::AreEqual<int>(2, q.length());
        }

        TEST_METHOD(Queue_PopEmptyQueueReturnsNull)
        {
            Queue<int> q;

            int ret = q.pop_front();

            // NULL is equivalent to 0 in C and < C++ 11
            Assert::AreEqual<int>(NULL, ret);
        }
    };
}