
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <random>
#include <Windows.h>
#include "CalculationAlgorithm.cpp"

// Function to put console to sleep so makes programming job easier
inline void sleep(int ms) { std::this_thread::sleep_for(std::chrono::milliseconds(ms)); }

// Function to initialise text colour so makes programming job easier
inline void colour(int colnum) {
	HANDLE hColour = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hColour, colnum);
}

// Function for outputting characters slowly
void slowcharfn(bool nline, std::string slowchar) {
	int size = 0;

	size = slowchar.size();
	for (int i = 0; i <= size; i++) {
		sleep(26);
		std::cout << slowchar[i];
	}
	if (nline == true) {
		std::cout << std::endl;
	}
	return;
}

// Function to choose random number
long long int randnum(long long int max, long long int min) {
	std::random_device dev;
	std::mt19937_64 rng(dev());
	std::uniform_int_distribution<std::mt19937_64::result_type> dist(min, max); // distribution in range [min, max]

	long long int randnum = dist(rng);
	return randnum;
}

// CentreText - Takes in a string and outputs it with padding in relation to the console window size.
std::string CentreText(std::string sText) {
	// Work out width
	int nWidth = 0;
	// Get console window size
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	// Assign window width to nWidth
	nWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;

	// Check to prevent memory overspill
	if (sText.length() >= (nWidth + 1)) return sText;

	return std::string((nWidth - sText.length()) / 2, ' ') + sText;
}

void CentreColouredText(std::string sText, int nColour) {
	// Work out width
	int nWidth = 0;
	// Get console window size
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	// Assign window width to nWidth
	nWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;

	// Check to prevent memory overspill
	if (sText.length() >= (nWidth + 1)) return;

	std::cout << std::string((nWidth - sText.length()) / 2, ' ');
	colour(nColour);
	std::cout << sText;
	colour(15);

	return; 
}

int main() {

	std::string sCalculationString = "";
	char cPrompt = 0;
	CalculationAlgorithm caTest;

	// Remove hard-to-read colours from random mix
	int nColourRandNum = randnum(15, 1);
	if (nColourRandNum == 8 || nColourRandNum == 7 || nColourRandNum == 5) nColourRandNum = 15;

	// Set random colour for title
	colour(nColourRandNum);
	SetConsoleTitleA("BIDMAS Console Calculator");

	std::cout << CentreText("____Welcome to the BIDMAS Console Calculator v1.1!____") << '\n';

	// Set to default colours
	colour(15);

	// Prompt for working-out displaying
	while (true) {
		std::cout << "\nIn this session, would you like the calculator to display working out steps? [y/n] > ";
		colour(14);
		std::cin >> cPrompt;
		colour(15);

		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore((std::numeric_limits<int>::max)(), '\n');
			colour(4);
			std::cerr << "Your input was incorrect. Please try again.\n";
			colour(15);
			continue;
		}
		else {
			std::cin.ignore((std::numeric_limits<int>::max)(), '\n');
			break;
		}
	}

	// Set environment variables
	if (cPrompt == 'y') {
		caTest.bDisplayWorkingOutProcess = true;
	}
	else {
		caTest.bDisplayWorkingOutProcess = false;
	}

	std::cout << "\n";
	CentreColouredText("___USAGE OF THE BIDMAS CALCULATOR___", 95);
	colour(15);
	std::cout << "\n- Type in any symbol or function from the following in your calculation input:\n\n";

	colour(11);
	std::cout << "___BASIC MATHEMATICAL OPERATORS___";
	colour(15);
	std::cout << "\n\t --> '/': Division\n\t --> '*': Multiplication\n\t --> '+': Addition\n\t --> '-': Subtraction\n\t --> '^': To the power of\n\n";

	colour(9); 
	std::cout << "___SQUARE/CUBE ROOT AND TRIGONOMETRY___";
	colour(15);
	std::cout << "\n\t --> 'sqrt(n)': Square Root function, where n is the desired number\n\t --> 'cbrt(n)': Cube Root function, where n is the desired number\n\t --> 'sin(n)': Sine function, where n is the desired number"
		<< "\n\t --> 'cos(n)': Cosine function, where n is the desired number\n\t --> 'tan(n)': Tangent function, where n is the desired number\n\t --> 'asin(n)': Arc Sine function, where n is the desired number"
		<< "\n\t --> 'acos(n)': Arc Cosine function, where n is the desired number\n\t --> 'atan(n)': Arc Tangent function, where n is the desired number\n\n";

	colour(10);
	std::cout << "___MATHEMATICAL CONSTANTS___";
	colour(15);
	std::cout << "\n\t --> '(pi)': The PI number constant.\n\t --> '(e)': The Euler's number constant.\n\n";

	colour(14);
	std::cout << "Example of usage:";
	colour(15);
	std::cout << " (10+5^2)((5*-2)+9-3^3)/2\n\n\n";

	while (true) {
		std::cout << "Enter calculation to calculate (type 'exit' to exit): > ";
		colour(14);
		std::getline(std::cin, sCalculationString, '\n');
		colour(15);

		if (sCalculationString == "exit") break;

		long double dFinalAnswer = caTest.SafeCalculate(sCalculationString);
		std::string sFinalAnswerString = std::to_string(dFinalAnswer);

		if (caTest.GetLastCalculationErrorValue() > 0) {
			sFinalAnswerString = caTest.GetLastCalculationErrorInfo();
		}

		std::cout << "\n Final Answer: ";
		colour(11);
		std::cout << sFinalAnswerString << "\n\n";
		colour(15);
	}

	colour(10);
	slowcharfn(true, "\nExiting the BIDMAS Console Calculator...");
	colour(15);

	return 0;
}