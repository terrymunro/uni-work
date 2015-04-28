#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\DSA\StringHelpers.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace StringHelpers;

namespace UnitTests
{
    TEST_CLASS(StringHelperTests)
    {
    public:
        TEST_METHOD(StringHelpers_IsWord_Standard)
        {
            Assert::IsTrue(isWord("Normal"));
        }

        TEST_METHOD(StringHelpers_IsWord_ShortWord)
        {
            Assert::IsTrue(isWord("a"));
        }

        TEST_METHOD(StringHelpers_IsWord_RandomCase)
        {
            Assert::IsTrue(isWord("RaNdOmCasE"));
        }

        TEST_METHOD(StringHelpers_IsWord_Hyphenated)
        {
            Assert::IsTrue(isWord("Hyphen-ated"));
        }

        TEST_METHOD(StringHelpers_IsWord_Apostrophy)
        {
            Assert::IsTrue(isWord("Test's"));
        }

        TEST_METHOD(StringHelpers_IsWord_MultipleHyphens)
        {
            Assert::IsFalse(isWord("Too-many-hyphens"));
        }

        TEST_METHOD(StringHelpers_IsWord_BeginHyphen)
        {
            Assert::IsFalse(isWord("-begin"));
        }

        TEST_METHOD(StringHelpers_IsWord_EndHyphen)
        {
            Assert::IsFalse(isWord("end-"));
        }

        TEST_METHOD(StringHelpers_IsWord_MultipleApostrophys)
        {
            Assert::IsFalse(isWord("G'rea't"));
        }

        TEST_METHOD(StringHelpers_IsWord_DigitsInWord)
        {
            Assert::IsFalse(isWord("Numb3rs"));
        }

        TEST_METHOD(StringHelpers_IsWord_SemiColon)
        {
            Assert::IsFalse(isWord("Te;st"));
        }

        TEST_METHOD(StringHelpers_IsWord_Comma)
        {
            Assert::IsFalse(isWord("Te,st"));
        }

        TEST_METHOD(StringHelpers_Split)
        {
            vector<string> results;
            string str = "can't co-processor\ntest another-\ntest";
            results = split(str, isspace);

            Assert::AreEqual<int>(4, results.size());
        }
    };
}