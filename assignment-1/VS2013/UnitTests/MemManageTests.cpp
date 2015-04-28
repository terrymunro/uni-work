#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\MemManage\MemManage.h"

#define MEM_SIZE 16

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
    TEST_CLASS(MemManageTests)
    {
    public:
        TEST_METHOD(MemManage_AllocatesMemory)
        {
            MemManage m(MEM_SIZE);
            Assert::AreEqual<int>(MEM_SIZE, m.Avail());

            stringstream sstream;
            int *intPtr = (int*)m.Alloc(sizeof(int));
            float *floatPtr = (float*)m.Alloc(sizeof(float));

            Assert::AreEqual<int>(MEM_SIZE - sizeof(int) - sizeof(float), m.Avail());
            Assert::IsNotNull(intPtr);
            Assert::IsNotNull(floatPtr);

            *intPtr = 10;
            *floatPtr = 1.2f;

            Assert::AreEqual<int>(10, *intPtr);
            Assert::AreEqual<float>(1.2f, *floatPtr);

            sstream << m;
            Assert::AreEqual<basic_string<char>>(
                "0A 00 00 00 9A 99 99 3F 00 00 00 00 00 00 00 00\n",
				sstream.str());
        }

        TEST_METHOD(MemManage_FreesMemory)
        {
            MemManage m(MEM_SIZE);

            stringstream sstream;
            int *intPtr = (int*)m.Alloc(sizeof(int));
            float *floatPtr = (float*)m.Alloc(sizeof(float));

            Assert::IsNotNull(intPtr);
            Assert::IsNotNull(floatPtr);
			Assert::AreEqual<int>(MEM_SIZE - sizeof(int)-sizeof(float), m.Avail());

            *intPtr = 10;
            *floatPtr = 1.2f;

            sstream << m;
            Assert::AreEqual<basic_string<char>>(
                "0A 00 00 00 9A 99 99 3F 00 00 00 00 00 00 00 00\n",
				sstream.str());
			sstream.str("");

            m.Free(intPtr);
            Assert::AreEqual<int>(MEM_SIZE - sizeof(int), m.Avail());
            sstream << m;
            Assert::AreEqual<basic_string<char>>(
                "00 00 00 00 9A 99 99 3F 00 00 00 00 00 00 00 00\n",
				sstream.str());
			sstream.str("");

            m.Free(floatPtr);
            Assert::AreEqual<int>(MEM_SIZE, m.Avail());
            sstream << m;
            Assert::AreEqual<basic_string<char>>(
                "00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00\n",
				sstream.str());

			char *a, *b, *c;
			a = (char*)m.Alloc(10);
			b = (char*)m.Alloc(2);
			c = (char*)m.Alloc(4);

			Assert::AreEqual<int>(0, m.Avail());
			m.Free(b);
			Assert::AreEqual<int>(2, m.Avail());
			m.Free(c);
			Assert::AreEqual<int>(6, m.Avail());
        }

		TEST_METHOD(MemManage_ReallocatesMemory)
		{
			MemManage m(16);
			stringstream sstream;

			size_t size = sizeof(char) * 6;
			char *str = (char*)m.Alloc(size);
			sprintf_s(str, size, "hello");

            sstream << m;
            Assert::AreEqual<basic_string<char>>(
                "68 65 6C 6C 6F 00 00 00 00 00 00 00 00 00 00 00\n",
				sstream.str());
			sstream.str("");

			size = size + (sizeof(char)* 7);
			str = (char*)m.Realloc(str, size);
			sprintf_s(str, size, "hello, world");

            sstream << m;
            Assert::AreEqual<basic_string<char>>(
                "68 65 6C 6C 6F 2C 20 77 6F 72 6C 64 00 00 00 00\n",
				sstream.str());
			sstream.str("");
		}

		TEST_METHOD(MemManage_Compact)
		{
			MemManage m(16), cpy;
			stringstream sstream;
			char *strs[4] = { "one", "two", "three", "k" };
			char *ptrs[4];

			for (int i = 0; i < sizeof(strs) / sizeof(char*); i++)
			{
				size_t len = strlen(strs[i]) + 1;
				ptrs[i] = (char*)m.Alloc(len);
				sprintf_s(ptrs[i], len, strs[i]);
			}

			Assert::AreEqual<int>(0, m.Avail());

            sstream << m;
            Assert::AreEqual<basic_string<char>>(
				"6F 6E 65 00 74 77 6F 00 74 68 72 65 65 00 6B 00\n",
				sstream.str(), L"1");
			sstream.str("");

			m.Free(ptrs[2]);

            sstream << m;
            Assert::AreEqual<basic_string<char>>(
				"6F 6E 65 00 74 77 6F 00 00 00 00 00 00 00 6B 00\n",
				sstream.str(), L"2");
			sstream.str("");

			cpy = m;

			m.Compact();
            sstream << m;
			Assert::AreEqual<basic_string<char>>(
				"6F 6E 65 00 74 77 6F 00 6B 00 00 00 00 00 00 00\n",
				sstream.str(), L"3");
			sstream.str("");
			Assert::AreEqual<int>(6, m.Avail());

			cpy.Compact();
            sstream << cpy;
            Assert::AreEqual<basic_string<char>>(
				"6F 6E 65 00 74 77 6F 00 6B 00 00 00 00 00 00 00\n",
				sstream.str(), L"4");
			sstream.str("");
			Assert::AreEqual<int>(6, cpy.Avail());

            sstream << m;
            Assert::AreEqual<basic_string<char>>(
				"6F 6E 65 00 74 77 6F 00 6B 00 00 00 00 00 00 00\n",
				sstream.str(), L"5");
		}

		TEST_METHOD(MemManage_DeepCopy)
		{
			MemManage m(16), cpy(m);
			stringstream s1, s2;

			Assert::AreEqual<int>(16, m.Avail());
			Assert::AreEqual<int>(16, cpy.Avail());
			
			char* str = (char*)m.Alloc(6);
			Assert::AreEqual<int>(10, m.Avail());
			Assert::AreEqual<int>(16, cpy.Avail());
			sprintf_s(str, 6, "hello");

			cpy = m;
			Assert::AreEqual<int>(10, m.Avail());
			Assert::AreEqual<int>(10, cpy.Avail());

			s1 << m;
			s2 << cpy;
			Assert::AreEqual<basic_string<char>>(s1.str(), s2.str());
		}
    };
}