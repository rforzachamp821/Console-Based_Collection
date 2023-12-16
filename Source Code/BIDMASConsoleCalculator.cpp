//
// BIDMAS Console Calculator
//

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>
#include <random>
#include <Windows.h>

void UserErrorDisplay(std::string);
void VerbosityDisplay(std::string);

// GetNumberDigitLength - Gets the number of characters before the decimal point of a long double floating-point number.
// Arguments: dNumber - The number to check and use.
// Return values: The number of characters before the decimal point.
// NOTE: If the number is less than 1, the number -1 will be returned.
//
int GetWholeNumberDigitLength(long double dNumber) 
{
	// -1 because if number was less than 1, it can support 16dp when long double is 64-bit (no. 0 would be taking no space of the 64-bit container)
	int nNumberDigitLength = -1;
	constexpr uint64_t nMaxMultiplierNum = (std::numeric_limits<uint64_t>::max)() / 10; // divide by 10 so it stops early before overflow occurs

	// Count number of place value digits
	for (uint64_t i = 1; i < dNumber && i < nMaxMultiplierNum; i *= 8, nNumberDigitLength++) {
	}

	return nNumberDigitLength;
}

// EradicateTrailingZeroes - Remove any extra trailing zeroes and, if the string is truly a whole number, remove the decimal point as well.
// Arguments: sString - The string to perform the eradication on.
// Return values: The updated string with the eradication processes completed on.
//
std::string EradicateTrailingZeroes(std::string sString)
{
	// Get position of last decimal place
	size_t nFinalDecimalPlace = sString.find_last_of('.');

	// Get rid of trailing 0s
	for (size_t i = sString.size() - 1; i >= nFinalDecimalPlace; i--) {
		if (sString.at(i) == '0') {
			sString.pop_back(); // Remove if last digit is '0'.
		}
		else if (sString.at(i) == '.') {
			sString.pop_back(); // Remove dot.
			break; // Break after '.' is removed.
		}
		else {
			break; // Or break before a digit is removed.
		}
	}

	return sString;
}

// FormatValueForDisplay - Formats a specific value for display output.
//                       - Has a max of 15-16dp.
// Arguments: dValue - The number to format.
// Return Value: Formatted number as a string.
//
std::string FormatValueForEuropeanNotation(long double dValue) {
	// Create stringstream and send info to it
	std::stringstream ssFormatted;
	ssFormatted << std::fixed << std::setprecision(std::numeric_limits<long double>::digits10 - GetWholeNumberDigitLength(dValue)) << dValue;

	std::string sFormatBuffer = EradicateTrailingZeroes(ssFormatted.str());

	// Change decimal point to European notation if necessary
	//
	// Find last decimal point and replace with european one
	size_t nDecimalPointPos = sFormatBuffer.find_last_of('.');

	// Exit when not found
	if (nDecimalPointPos == std::string::npos) return sFormatBuffer;

	// Replace
	sFormatBuffer.replace(nDecimalPointPos, 1, ",");

	// Return string from stringstream
	return sFormatBuffer;
}

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
	size_t size = 0;

	size = slowchar.size();
	for (size_t i = 0; i <= size; i++) {
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

bool bErrorToggle = false; // FALSE for no errors, TRUE for errors

// Display user-space errors
inline void UserErrorDisplay(std::string sOutput) {
	if (bErrorToggle) {
		colour(4);
		std::cerr << sOutput << '\n';
		colour(15);
	}
}

// Display complex, verbose errors or messages
inline void VerbosityDisplay(std::string sOutput) {
	if (bErrorToggle) {
		colour(8);
		std::cerr << "Verbose Message: " << sOutput << '\n';
		colour(15);
	}
}

int main(int argc, char* argv[]) {

	std::string sCalculationString = "";
	char cPrompt = 0;
	bool bEuropeanMathNotationToggle = false;
	CalculationAlgorithm caTest;

	// Calculate random colour, and remove hard-to-read colours from random mix
	int nColourRandNum = randnum(15, 1);
	if (nColourRandNum == 8 || nColourRandNum == 7 || nColourRandNum == 5) nColourRandNum = 15;

	// Set title for window
	SetConsoleTitleA("BIDMAS Console Calculator");

	// If arguments are found
	if (argc > 1)
	{
		// Compare with c++ string
		sCalculationString = argv[1];

		if (sCalculationString == "exit") return 0;
		else if (sCalculationString == "toggleerrors") {
			if (bErrorToggle == false) {
				bErrorToggle = true;
				colour(10);
				std::cout << "Algorithm error/message output enabled successfully.\n";
				colour(15);
			}
			else {
				bErrorToggle = false;
				colour(10);
				std::cout << "Algorithm error/message output disabled successfully.\n";
				colour(15);
			}

			return 0;
		}
		else if (sCalculationString == "toggleeuropenotation") {
			if (bEuropeanMathNotationToggle == false) {
				bEuropeanMathNotationToggle = true;
				colour(10);
				std::cout << "European Math Notation input/output enabled successfully.\n";
				colour(15);
			}
			else {
				bEuropeanMathNotationToggle = false;
				colour(10);
				std::cout << "European Math Notation input/output disabled successfully.\n";
				colour(15);
			}

			return 0;
		}

		// Calculate calculation string
		caTest.UseEuropeanNotation(bEuropeanMathNotationToggle);
		long double dFinalAnswer = caTest.SafeCalculate(sCalculationString);

		// Error occured - get error info and output
		if (caTest.GetLastCalculationErrorValue() > 0) {
			colour(12);
			std::cout << "\nError: " << caTest.GetLastCalculationErrorInfo() << "\n\n";
			colour(15);
			return 0;
		}

		std::cout << "\n Final Answer: ";
		colour(11);
		// Output result
		if (bEuropeanMathNotationToggle) {
			std::cout << FormatValueForEuropeanNotation(dFinalAnswer) << "\n\n";
		}
		else {
			// Send all floating-point information and formatting information to stringstream
			std::ostringstream ossFinalAns;
			ossFinalAns << std::fixed << std::setprecision(std::numeric_limits<long double>::digits10 - GetWholeNumberDigitLength(dFinalAnswer)) << dFinalAnswer;

			std::cout << EradicateTrailingZeroes(ossFinalAns.str()) << "\n\n";
		}
		colour(15);

		return 0;
	}

	// Set random colour for title
	colour(nColourRandNum);
	std::cout << CentreText("____Welcome to the BIDMAS Console Calculator v1.3.0!____") << '\n';

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

	colour(12);
	std::cout << "___EXTRAS___";
	colour(15);
	std::cout << "\n\t --> 'toggleerrors': Display verbose errors from the internal calculation algorithm.\n\t --> 'toggleeuropenotation': Output calculation working and answer in European math notation.\n\t --> '(ans)': The Ans user-space variable. Use this to substitute the previous calculated answer into a new one.\n\n";

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
		else if (sCalculationString == "toggleerrors") {
			if (bErrorToggle == false) {
				bErrorToggle = true;
				colour(10);
				std::cout << "Algorithm error/message output enabled successfully.\n";
				colour(15);
			}
			else {
				bErrorToggle = false;
				colour(10);
				std::cout << "Algorithm error/message output disabled successfully.\n";
				colour(15);
			}

			continue;
		}
		else if (sCalculationString == "toggleeuropenotation") {
			if (bEuropeanMathNotationToggle == false) {
				bEuropeanMathNotationToggle = true;
				colour(10);
				std::cout << "European Math Notation input/output enabled successfully.\n";
				colour(15);
			}
			else {
				// Assume it is currently true, so toggle to false
				bEuropeanMathNotationToggle = false;
				colour(10);
				std::cout << "European Math Notation input/output disabled successfully.\n";
				colour(15);
			}

			continue;
		}

		// Calculate
		caTest.UseEuropeanNotation(bEuropeanMathNotationToggle);
		long double dFinalAnswer = caTest.SafeCalculate(sCalculationString);

		if (caTest.GetLastCalculationErrorValue() > 0) {
			colour(12);
			std::cout << "\nError: " << caTest.GetLastCalculationErrorInfo() << "\n\n";
			colour(15);
			continue;
		}

		std::cout << "\n Final Answer: ";
		colour(11);
		if (bEuropeanMathNotationToggle) {
			std::cout << FormatValueForEuropeanNotation(dFinalAnswer) << "\n\n";
		}
		else {
			// Send all floating-point information and formatting information to stringstream
			std::ostringstream ossFinalAns;
			ossFinalAns << std::fixed << std::setprecision(std::numeric_limits<long double>::digits10 - GetWholeNumberDigitLength(dFinalAnswer)) << dFinalAnswer;

			std::cout << EradicateTrailingZeroes(ossFinalAns.str()) << "\n\n";
		}
		colour(15);
	}

	colour(10);
	slowcharfn(true, "\nExiting the BIDMAS Console Calculator...");
	colour(15);

	return 0;
}