// Include libraries
#include <iostream>
#include <string>
#include <Windows.h>

int main()
{
	// Declare numbers
	std::string text;
	int time = 0;
	// Ask user for input
	std::cout << "Welcome!\n\n";
start:
	do {
		std::cout << "Please input the string of text you want to output as 'slow characters': > ";
		getline(std::cin, text);
		if (text == "0") {
			return 0;
		}
		if (text == "") {
			std::cout << "Please input something.\n";
		}
		else break;
	} while (true);

	do {
		std::cout << "\nInput time intervals in milliseconds: > ";
		std::cin >> time;
		if (time <= 0) {
			return 0;
		}
		while (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			std::cout << "Please input a number.\n";
		}
		std::cin.ignore();
		break;
	} while (true);

	// Outputs text using a for loop
	std::cout << "\nText:\n\n_______________________________________________________________________________________________________\n";
	for (int i = 0; text[i] != '\0'; i++)
	{
		std::cout << text[i];
		Sleep(time);
	}
	std::cout << "\n_______________________________________________________________________________________________________\n\n";
	goto start;
	return 0;
}