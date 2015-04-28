#include "RecursiveCalculator.h"
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

bool quitCalculator(char const* input)
{
	return *input == 'q' || *input == 'Q' || *input == '\0';
}

int main()
{
	RecursiveCalculator calc;
	string userInput;

	// Display welcome
	cout << "This is a very basic recursive descent parsing calculator." << endl;
	cout << "At the prompt you can enter a mathematical expression comprised of" << endl;
	cout << " +, -, *, /, ^, (, ) symbols/operators and the result will be printed." << endl << endl;

	// Loop until quit
	while (1)
	{
		// Display prompt
		cout << "> ";

		// Receive user input
		getline(cin, userInput);

		// Trim trailing whitespace '\n' characters
		userInput.erase(userInput.find_last_not_of(" \n\r\t")+1);

		// Check if the user wants to quit
		if (quitCalculator(userInput.c_str()))
			break;

		// Calculate answer and display result
		cout << "= " << fixed << setprecision(3) << calc.calculate(userInput.c_str()) << endl;
	}

	// Display goodbye message
	cout << endl <<"Have a nice day" << endl;

	return 0;
}