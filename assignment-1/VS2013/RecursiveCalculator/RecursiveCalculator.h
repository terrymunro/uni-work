#ifndef RECURSIVECALCULATOR_H
#define RECURSIVECALCULATOR_H

class RecursiveCalculator
{
private:
    typedef enum
    {
        Number, Plus, Minus, Multiply,
        Divide, Power, LParenthesis,
        RParenthesis, End, Error
    } Symbol; 

    Symbol symbol;
    char const *input;
    char *buffer;
    int bufferIndex;

    Symbol getSymbol();
    void error(const char*);
    void readNumberIntoBuffer();
    bool accept(Symbol);
    bool expect(Symbol);
    double getValueFromBuffer();
    double expression();
    double exponent();
    double term();
    double factor();

public:
    RecursiveCalculator(int bufferSize = 1024);
    ~RecursiveCalculator();
    double calculate(char const *);
};

#endif