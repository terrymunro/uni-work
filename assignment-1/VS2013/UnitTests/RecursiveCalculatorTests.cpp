#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\RecursiveCalculator\RecursiveCalculator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
    struct Answer
    {
        const char* expression;
        double		answer;
    };

	TEST_CLASS(RecursiveCalculatorTests)
	{
	public:
        TEST_METHOD(RecursiveCalculator_TestCases)
        {
            RecursiveCalculator calc;

            Answer answers[] = {
                { "1 + 1",			2 },
                { "3 + 2",			5 },
                { "1 + 23 + 102",	126 },
                { "12 - 3",			9 },
                { "3 - 4",			-1 },
                { "4 - 1 - 2",		1 },
                { "5 * 2",			10 },
                { "5 * 2 * 3",		30 },
                { "3 / 1",			3 },
                { "1 / 4",			0.25 },
                { "12 / 3 / 2",		2 },
                { "1 + 2 - 3",		0 },
                { "1 + 2 * 3",		7 },
				{ "6 / 2 - 1",		2 },
				{ "3 * 2 / 3",		2 },
				{ "0.2 + 0.3",		0.5 },
				{ "1 + 0.2 - 0.3",	1 + 0.2 - 0.3 },
				{ "3+4/2*4", 3 + 4 / 2 * 4 },
				{ "2^2 + 3 - 4", pow(2, 2) + 3 - 4 },
				{ "(2 + 3) * 5", (2 + 3) * 5 },
				{ "(2 + 3) * (5 - 1)^2", (2 + 3) * pow((5 - 1), 2) },
				{ "2 + ((3 + 1) * (6 - 2) - 1) / (4 - 2)", (2.0 + ((3.0 + 1.0) * (6.0 - 2.0) - 1.0) / (4.0 - 2.0)) },
				{ "2 + 3 * (1 + 4) - 9 / 2 * 3 + 1", (2.0 + 3.0 * (1.0 + 4.0) - 9.0 / 2.0 * 3.0 + 1.0) },
				{ "-1 + -4", -1 + -4 }
            };
        
            for (int i = 0; i < sizeof(answers) / sizeof(Answer); ++i)
            {
				size_t n;
				wchar_t errMsg[255];
				mbstowcs_s(&n, errMsg, 255, answers[i].expression, strlen(answers[i].expression));

				Assert::AreEqual<double>(
					answers[i].answer,
					calc.calculate(answers[i].expression),
					errMsg);
            }
        }
    };
}