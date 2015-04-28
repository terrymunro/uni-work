#include "RecursiveCalculator.h"
#include <cmath>
#include <cassert>
#include <iomanip>
#include <iostream>

RecursiveCalculator::RecursiveCalculator(int bufferSize)
{
    bufferIndex = 0;
    buffer = new char[bufferSize];
	assert(buffer != NULL);
	for (int i = 0; i < bufferSize; i++)
		buffer[i] = 0;
}

RecursiveCalculator::~RecursiveCalculator()
{
    delete [] buffer;
}

// - Each level indicates the order of operations from
// the top down. The grammar is as follows:
//
//     factor = [+|-] number | ( expression )
//    exponent = factor { [^] factor }
//   term = exponent { [*|/] exponent }
//  expression = [+|-] term { [+|-] term }
// answer = expression 
double RecursiveCalculator::calculate(char const* expr)
{
    double answer = 0;

    input = expr;
    symbol = getSymbol();
    answer = expression();
    expect(End);

    return answer;
}

double RecursiveCalculator::expression()
{
    double value = term();

    while (symbol == Plus || symbol == Minus)
    {
        bool isPlus = symbol == Plus;
        symbol = getSymbol();

        if (isPlus) value += term();
        else		value -= term();
    }

    return value;
}

double RecursiveCalculator::term()
{
    double value = exponent();

    while (symbol == Multiply || symbol == Divide)
    {
        bool isMultiply = symbol == Multiply;
        symbol = getSymbol();

        if (isMultiply)	value *= exponent();
        else			value /= exponent();
    }

    return value;
}

double RecursiveCalculator::exponent()
{
    double value = factor();

    while (symbol == Power)
    {
        symbol = getSymbol();
        value = pow(value, factor());
    }

    return value;
}

double RecursiveCalculator::factor()
{
    double value = 0;

    if (accept(Number) || (accept(Plus) && accept(Number)))
    {
        value = getValueFromBuffer();
    }
    else if (accept(Minus) && accept(Number))
    {
        value = -getValueFromBuffer();
    }
    else if (accept(LParenthesis))
    {
        value = expression();
        expect(RParenthesis);
    }
    else
    {
        error("Factor: Syntax error.");
        symbol = getSymbol();
    }

    return value;
}

void RecursiveCalculator::readNumberIntoBuffer()
{
    bool hasDecimal = false;

    // rewind pointer to re-read first number discovered
    --input; 

    // while still finding numbers add to buffer
    while (isdigit(*input) || *input == '.')
    {
        if (*input == '.')
        {
            if (hasDecimal)
            {
                error("Number: Too many decimal marks.");
                return;
            }
            hasDecimal = true;
        }
        
        buffer[bufferIndex++] = *input++;
    }
}

RecursiveCalculator::Symbol RecursiveCalculator::getSymbol()
{
    char sym = *input++;

    switch (sym)
    {
    // accepted symbols
    case '\0': return End;
    case '+':  return Plus;
    case '-':  return Minus;
    case '*':  return Multiply;
    case '/':  return Divide;
    case '^':  return Power;
    case '(':  return LParenthesis;
    case ')':  return RParenthesis;

    // whitespace
    case ' ': case '\t': case '\n':
    case '\v': case '\f': case '\r':
        return getSymbol();

    // numbers
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
    case '.':
        readNumberIntoBuffer();
        return Number;

    default:
        error("Get Symbol: Unknown symbol.");
        return Error;
    }
}

double RecursiveCalculator::getValueFromBuffer()
{
    double value = atof(buffer);

    // Reset buffer
    while (bufferIndex > 0)
        buffer[--bufferIndex] = '\0';

    return value;
}

void RecursiveCalculator::error(char const* errmsg)
{
    std::cerr << "Error in " << errmsg << std::endl;
}

bool RecursiveCalculator::accept(Symbol s)
{
    if (symbol == s)
    {
		if (symbol != End)
			symbol = getSymbol();
        return true;
    }

    return false;
}

bool RecursiveCalculator::expect(Symbol s)
{
    bool accepted = accept(s);

    if (!accepted)
        error("Expect: Unexpected symbol");

    return accepted;
}