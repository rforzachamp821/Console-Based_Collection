
//////////////////////////////////////////////////////////////////////////////////////////////
/* TERMINALAPP GEN2- This is the second generation of the TerminalApp series of programs.	//
Any code in this program is allowed to be reproduced and used for any kind of purpose.		//
This program is open source. You may take any code without permission.						//
------------Have fun programming!---------------											//
*/																							//
//////////////////////////////////////////////////////////////////////////////////////////////
// 
// Include files
#include <iostream> 
#include <string>
#include <fstream>
#include <chrono>
#include <thread>
#include <Windows.h>
#include <conio.h>

// Game defenitions
#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70

// Terminal definitions
#define DEFAULT_COL 7

// Global variables
std::string		command;
std::string		previnput = "";
std::string		coltxt;
std::string		spare;
std::string		filename;
std::string		text;
std::string		sFileParseName;

unsigned long long int nNumofUsrCommands = 0;

long long int	hertz;
long long int	milsecs;
long long int	letters = 1;
long long int	words = 1;
long long int	chars = 1;
long long int	nNumOfFileParseLines = 1;

int				coloption;
int				lastcolour = 7; // 7 because hex number 7 is white
int				inputnum;

short int		ansicol = 0;

char			input;

bool			colcheck = false;
bool			bIsRunFromFile = false;

// Game variables
std::string		player1Name;
std::string		player2Name;

long long int	guessnum;
long long int	rightnum;
long long int	score;
long long int	highscoreguess = 0;
long long int	highscorecarhard = 0;
long long int	highscorecareasy = 0;
long long int	scorecar = 0;

int				enemy_y[3];
int				enemy_x[3];
int				enemyF[3];
int				carPos = WIN_WIDTH / 2;

short int		uppernum;
short int		lowernum;
short int		tries;
short int		CarType = 1;
short int		nRandcolNum = 7;

char			mycar1[4][4] = { ' ', '±', '±', ' ', '±', '±', '±', '±', ' ', '±', '±', ' ', '±', '±', '±', '±' };
char			mycar2[4][4] = { ' ', '|', '|', ' ', '0', '*', '*', '0', '|', '*', '*', '|', '0', '_', '_', '0' };
char			mycar3[4][4] = { '/', 'T', 'T', '\\', '|', '_', '_', '|', '|', ' ', ' ', '|', '/', '\\', '/', '\\' };
char			mycar4[4][4] = { '_', '=', '=', '_', '±', '|', '|', '±', '|', '0', '0', '|', '±', ')', '(', '±' };
char			mycar5[4][4] = { ' ', '±', '±', ' ', '±', '*', '*', '±', ' ', '%', '%', ' ', '±', '=', '=', '±' };

// Other variables
HANDLE			console = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE			h;

COORD			CursorPosition;
COORD			v4;

CHAR_INFO		v3;
SMALL_RECT		v5;
CONSOLE_SCREEN_BUFFER_INFO cbsi;

// Function to put console to sleep so makes programming job easier
void sleep(int ms) { std::this_thread::sleep_for(std::chrono::milliseconds(ms)); }

// Function to clear keyboard buffer
void clearkeebbuf() {
	while (_kbhit()) {
		_getch();
	}
	return;
}

// Function to initialise text colour so makes programming job easier
void colour(int colnum) {
	if (ansicol == 0) {
		SetConsoleTextAttribute(console, colnum);
	}
	else return;
}

// Mario kart song in beeps function
void mariobeep();

// FileParse feature activator function
bool fileParseFeatureFn(std::string);

// Function to choose random number
int randnum(long long int max, long long int min) {
	if (max == 0) max = 1;
	srand((unsigned)time(0));
	int randnum = rand() % (max - min + 1) + min;
	if (randnum == 0) randnum = 1;
	nRandcolNum = randnum;
	return randnum;
}

// Function for outputting characters slowly with different colours
void slowcolourrandfn(std::string slowchar) {
	for (int i = 1, a = 0; slowchar[a] != '\0'; i++, a++) {
		if (i == 15) i = 1;
		colour(i);
		sleep(45);
		std::cout << slowchar[a];
	}
	colour(lastcolour);
	clearkeebbuf();
	return;
}

// Function to get console cursor position
COORD GetConsoleCursorPosition(){

	if (GetConsoleScreenBufferInfo(console, &cbsi)) {
		return cbsi.dwCursorPosition;
	}
	else {
		// The function failed. Call GetLastError() for details.
		COORD invalid = { 0, 0 };
		return invalid;
	}
}

// Function for outputting characters slowly
void slowcharfn(bool nLine, std::string slowChar) {
	int size;
	size = 0;
	size = slowChar.size();
	for (int i = 0; i <= size; i++) {
		sleep(45);
		std::cout << slowChar[i];
	}
	if (nLine == true) {
		std::cout << std::endl;
	}
	clearkeebbuf();
	std::cin.clear();
	return;
}

// Clear screen function (Flushes screen buffer)
void cls() {
	if ((h = (HANDLE)GetStdHandle(0xFFFFFFF5), (unsigned int)GetConsoleScreenBufferInfo(h, &cbsi)))
	{
		v5.Right = cbsi.dwSize.X;
		v5.Bottom = cbsi.dwSize.Y;
		v3.Char.UnicodeChar = 32;
		v4.Y = -cbsi.dwSize.Y;
		v3.Attributes = cbsi.wAttributes;
		v4.X = 0;
		*(DWORD*)&v5.Left = 0;
		ScrollConsoleScreenBufferW(h, &v5, 0, v4, &v3);
		cbsi.dwCursorPosition = { 0 };
		HANDLE v1 = GetStdHandle(0xFFFFFFF5);
		SetConsoleCursorPosition(v1, cbsi.dwCursorPosition);
	}
	return;
}

// Colourful text function
void colourtxt(std::string text) {
	for (int i = 1, a = 0; text[a] != '\0'; i++, a++) {
		if (i == 15) { i = 1; }
		colour(i);
		std::cout << text[a];
	}
}

// Function to output stuff for converters and calculators
long double num(std::string prompt) {
	long double num = 0.0;
	while(true) {
		std::cout << prompt;
		if (lastcolour < 16) {
			colour(14);
		}

		if (bIsRunFromFile == false) {
			std::cin >> num;
			if (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(INT_MAX, '\n');
				std::cerr << "Your input was incorrect. Please try again.\n";
				continue;
			} else break;
		}
		else {
			std::ifstream FileParseNum(sFileParseName);
			// reiterate through the file to get to the correct line number
			long long int line_no = 0;
			while (line_no <= nNumOfFileParseLines) {
				getline(FileParseNum, spare, '\n');
				line_no++;
			}
			// send data to num
			FileParseNum >> num;
			// close stream to save memory space
			FileParseNum.close();
			// increment nNumOfFileParseLines to switch to next file line
			nNumOfFileParseLines++;
			break;
		}
		colour(lastcolour);
	}
	return num;
}

// Function that makes inputting strings one step easier
std::string str(std::string prompt) {
	std::string string;

	// Prompt
	std::cout << prompt;
	if (bIsRunFromFile == false) {
		std::cin.ignore();
		getline(std::cin, string);
	}
	else {
		std::ifstream FileParseStr(sFileParseName);
		// reiterate through the file to get to the correct line number
		long long int line_no = 0;
		while (line_no <= nNumOfFileParseLines) {
			getline(FileParseStr, spare, '\n');
			line_no++;
		}
		// send data to num
		getline(FileParseStr, string);
		// close stream to save memory space
		FileParseStr.close();
		// increment nNumOfFileParseLines to switch to next file line
		nNumOfFileParseLines++;
	}
	
	return string;
}

// fail function - used if command not found
void fail() {
	colour(12);
	std::cerr << "Sorry, your input was incorrect. Please try again.\n";
	colour(2);
	std::cerr << "Previous input: ";
	colour(4);
	std::cerr << "'" << command << "'";
	colour(2);
	std::cerr << ".\n";
	colour(7);
	return;
}

// Help function, used to reduce copy pasting
void help() {
	colour(2);
	std::cout << "HELP- Displays all commands with their meaning(s) and usage.\n";
	colour(lastcolour);
	std::cout << "\n1. Tutorial- Starts a tutorial on how to use this application.\n2. Exit- Exits the application.\n3. Hello- Outputs 'Hello, World!'.\n"
		<< "4. Cls- Wipes the console text buffer fully (clears the terminal).\n5. Clear- Removes current console text from view using ANSI escape codes (Only works on specific terminals).\n"
		<< "6. Beep- Create a sound for a specific amount of time and hertz.\n7. TextInfo- Output statistics of chosen text.\n8. Sleep- Timer in milliseconds.\n"
		<< "9. Game- Open a list of text-based games to play.\n10. Coltxt- Colours string of chosen text.\n11. Slowchar- Outputs string of text slowly.\n"
		<< "12. Ansi- A terminal editor where you can modify the look and workings of the terminal with ansi escape codes.\n13. Echo- Type this followed by a string of characters to output whatever you want.\n"
		<< "14. Prev- Output your most previous input into the command section of the terminal.\n15. About- Terminal credits and contributors, with an exclusive message.\n16. Colour- Change the terminal text colour.\n"
		<< "17. Randcol- Randomly pick terminal text and background colour.\n18. Cow- Outputs a cow made with characters.\n19. Cowsay- Make a cow say whatever you want.\n20. Einstein- Output one of 50 Albert Einstein's quotes.\n"
		<< "21. Note- Permanently save notes and access them later.\n22. Shutdownsys- Shuts down your computer.\n23. Restartsys- Restarts your computer.\n24. Dundundun- Outputs a 'dundundun' sound.\n25. Saynote- Output notes that were saved using the note command.\n"
		<< "26. Memtest- Simple memory address checker.\n27. Ipinfo- Shows information about your IP.\n28. Cpustress- A simple single core cpu stress tester.\n29. Calculator- Use a list of calculators built into the program.\n"
		<< "30. Converter- Use a list of converters built into the program.\n31. Mario- Output the Super Mario theme song in beeps.\n"
		<< "32. Ansireset- Resets all set ANSI colour codes with the Ansi command.\n33. Explreset- Restarts Explorer.exe to fix problems with the Desktop shell.\n34. Commandnum- Shows the number of commands you have inputted since the start of the terminal execution.\n"
		<< "35. Cry- Make the terminal cry!\n36. Stopwatch- A simple stopwatch that counts in seconds.\n37. Simplecalc- A simple calculator that supports the 4 basic operators (+,-,/,*).\n38. Edison- Outputs a randomly-picked choice of one of 50 Thomas Edison quotes.\n"
		<< "39. FileParse- Begin the FileParse feature which make the terminal run off a chosen file and use its commands.\n40. Pause- pauses the terminal and awaits any keypress to continue.\n41. Nodisp- Don't display the prompt for commands (saves room in terminal window- similar to cmd's '@echo off' command)\n"
		<< "42. Disp- Opposite of Nodisp.\n43. Colreset- Resets all the terminal colours to default (ignoring ANSI colour codes)\n";
	return;
}

// Tutorial function
void tutorial(bool FromTerminal) {
	cls();
	slowcharfn(true, "Using this terminal is very straight-forward.");
	slowcharfn(true, "On the starting screen of the terminal, you can always type 'help' in the terminal to see all commands and what they do.");
	sleep(500);
	for (;;) {
		slowcharfn(false, "Type ");
		colour(3);
		slowcharfn(false, "'help'");
		colour(lastcolour);
		slowcharfn(false, " without quotes to see what happens: > ");
		command = "";
		colour(14);
		getline(std::cin, command);
		colour(lastcolour);
		if (command == "help") {
			help();
			colour(47);
			slowcharfn(false, "Congratulations! You now know how to input properly in this terminal.");
			colour(6);
			std::cout << "\n\nPress ENTER to progress through the tutorial...\n";
			colour(lastcolour);
			std::cin.get();
			break;
		}
		else {
			std::cin.clear();
			colour(12);
			std::cerr << "Try again.\n";
			colour(lastcolour);
		}
	}
	// If the user did not enter the tutorial from the actual terminal, create a file to say that the terminal has been completed.
	if (FromTerminal == false) {
		CreateDirectoryA("TerminalApp_Temp", 0);
		std::ofstream IsOld("TerminalApp_Temp\\IsNew.confi");
		IsOld << "Has completed enough of the tutorial.";
		IsOld.close();
	}
	slowcharfn(true, "Some commands also require you to input a number next to a desired option.");
	slowcharfn(true, "Here is a list of 3 options:");
	colour(1);
	std::cout << "[1] Blue\n";
	colour(8);
	std::cout << "[2] Black\n";
	colour(4);
	std::cout << "[3] Red\n";
	colour(lastcolour);
	sleep(500);
	for (;;) {
		slowcharfn(false, "Please input the number next to the option 'Blue': > ");
		command = "";
		colour(14);
		std::cin.clear();
		getline(std::cin, command);
		if (command == "1") {
			colour(10);
			std::cout << "Correct!\n\n";
			break;
		}
		else {
			// std::cin.clear used to clear error flags in class cin (obviously for error handling)
			std::cin.clear();
			colour(12);
			std::cerr << "Try that again.";
			colour(lastcolour);
			std::cerr << "\n";
		}
	}
	for (;;) {
		colour(lastcolour);
		slowcharfn(false, "Please input the number next to the option 'Red': > ");
		command = "";
		colour(14);
		std::cin.clear();
		getline(std::cin, command);
		if (command == "3") {
			colour(10);
			std::cout << "Correct!!\n\n";
			break;
		}
		else {
			std::cin.clear();
			colour(12);
			std::cerr << "Try that again.";
			colour(lastcolour);
			std::cerr << "\n";
		}
	}
	for (;;) {
		colour(lastcolour);
		slowcharfn(false, "Please input the number next to the option 'Black': > ");
		command = "";
		colour(14);
		std::cin.clear();
		getline(std::cin, command);
		if (command == "2") {
			colour(47);
			std::cout << "Congrats, you've passed the tutorial!";
			colour(lastcolour);
			std::cout << "\nYou are now ready to use the terminal.\n";
			break;
		}
		else {
			std::cin.clear();
			colour(12);
			std::cerr << "Try that again.";
			colour(lastcolour);
			std::cerr << "\n";
		}
	}
	// Return to the next line after this function was called
	return;
}

// Random Number Calculator
void randNumcalc() {
	long long int min, max;
	slowcharfn(true, "Welcome to the Random Number Picker!");
	// Prompt
	do {
		std::cout << "Please input your lower number boundary (Input character to exit): > ";
		std::cin >> min;
		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			return;
		}
		else break;
	} while (true);
	
	do {
		std::cout << "Please input your higher number boundary: > ";
		std::cin >> max;
		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			return;
		}
		else break;
	} while (true);
	colour(34052);
	// Calculate and output
	std::cout << "Result: " << randnum(max, min) << "\n";
	colour(lastcolour);
	std::cin.ignore(INT_MAX, '\n');
	return;
}

// Fahrenheit to celsius
void fahrenheitToCelsius() {
	long double		fahrenheit;
	long double		celsius;

	fahrenheit = num("Please input your fahrenheit value (Input character to exit): > ");

	celsius = (fahrenheit - 32) * 5 / 9;

	colour(34052);
	std::cout << "Results:\n";
	colour(lastcolour);
	std::cout << "Fahrenheit value: " << fahrenheit << "\nConverted Celsius value: " << celsius << ".\n\n";
	return;
}

// Celsius to fahrenheit
void celsiusToFahrenheit() {
	long double		celsius;
	long double		fahrenheit;

	celsius = num("Please input your celsius value (Input character to exit): > ");

	fahrenheit = celsius * (9 / 5) + 32;
	// Output result
	colour(34052);
	std::cout << "Results:\n";
	colour(lastcolour);
	std::cout << "Celsius value: " << celsius << "\nConverted fahrenheit value: " << fahrenheit << ".\n\n";
	return;
}

// Simple Interest Calculator
void simpIntCalc() {
	slowcharfn(true, "Welcome to the Simple Interest Calculator!\n");
	// prompt
	long double		money = num("Please input the amount of money: > ");
	long double		percnum = num("Please input the interest rate in percentage: > ");
	long long int	yearnum = num("Please input the number of years: > ");

	// convert percentage into decimal
	percnum /= 100;

	// formula calculation
	long double endnumtotalinterest = money * percnum * yearnum;
	long double endnumyearinterest = money * percnum;
	long double endnumadded = money * percnum * yearnum + money;
	// Output result
	colour(34052);
	std::cout << "Results:\n";
	colour(lastcolour);
	std::cout << "Calculated interest for a single year: " << endnumyearinterest << "\nCalculated interest for all "
		<< yearnum << " years: " << endnumtotalinterest << "\nTotal money that should be recieved: " << endnumadded << std::endl;
	return;
}

// Compound Interest Calculator
void compIntCalc() {
	long double		money;
	long double		percent;
	long long int	time;
	long double		endnumtotalinterest = 0;
	long double		interest;
	slowcharfn(true, "Welcome to the Compound Interest Calculator!");
	money = num("Please input your principal amount of money: > ");
	percent = num("Please input the interest rate in percentage: > ");
	time = num("Please input the amount of time in years: > ");
	// Calculate numbers
	endnumtotalinterest = money;
	std::cout << "Results:\n";
	for (long long int i = 0; i != time; i++) {
		interest = (endnumtotalinterest * percent * 1) / 100;
		endnumtotalinterest += interest;
	}

	long double		endnumyearinterest = money * (1 + (percent / 100));
	long double		investment = endnumtotalinterest;

	// Output results
	colour(34052);
	std::cout << "Results:";
	colour(lastcolour);
	std::cout << "\nCalculated interest for the first year: " << (endnumyearinterest - money) << "\nCalculated interest for all "
		<< time << " years: " << (endnumtotalinterest - money) << "\nTotal money that should be recieved: " << investment << std::endl;
}

// Power of 2 calculator
void powerOf2Calc() {
	short int		power = 1;
	long long int	powernum = 1;

	slowcharfn(true, "Welcome to the Power of 2 calculator!");
	power = num("Please input the power to use for the calculation: > ");
	if (power > 62) {
		std::cout << "Your number was too high. The max number is 62. Please try again.\n";
	}
	power++;

	// Work out and output value
	colour(34052);
	std::cout << "\n___Results___\n";
	colour(lastcolour);
	// Increase power by 2 every loop
	for (short int i = 1; i < power; i++) {
		powernum *= 2;
		std::cout << i << ": " << powernum << std::endl;
	}
	std::cout << '\n';
	return;
}

void ratioCalc() {
	long float *nRatioArray = new long float[128];
	short int nNumofNums = 0;
	long float nAddedRatioNum = 0;
	long float nGivenNum = 0;
	slowcharfn(true, "Welcome to the Ratio Divider Calculator!");
	nGivenNum = num("Please input ratio number (0 to exit): > ");
	if (nGivenNum <= 0) {
		return;
	}
	std::cout << "Input anything other than a number to exit at any time.\n";
	for (nNumofNums = 0; nNumofNums <= 127; nNumofNums++) {
		std::cout << "Please input ratio number " << (nNumofNums + 1) << ": > ";
		std::cin >> nRatioArray[nNumofNums];
		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			nNumofNums--;
			break;
		}
	}
	// --Calculate Ratio--
	// 
	// Add ratio numbers
	for (int i = 0; i <= nNumofNums; i++) {
		nAddedRatioNum += nRatioArray[i];
	}
	// Divide by given number
	nGivenNum /= nAddedRatioNum;
	// Now multiply that number by every ratio array cell
	colour(34052);
	std::cout << "Result: ";
	for (int i = 0; i <= nNumofNums; i++) {
		nRatioArray[i] *= nGivenNum;
	}
	// Output results
	for (int i = 0; i <= nNumofNums; i++) {
		std::cout << nRatioArray[i];
		if (nNumofNums != i) {
			std::cout << " : ";
		}
	}
	std::cout << '\n';
	nGivenNum = 0;
	nAddedRatioNum = 0;
	colour(lastcolour);
	return;
}

void binaryToDenary() {
	std::string		binary;
	long long int	denary2 = 0;
	long long int	stringlen = 0;
	long long int	ind = 1;
	bool			didItWork;

	slowcharfn(true, "Welcome to the Binary to Denary converter!");
	// Prompt
	std::cin.ignore(INT_MAX, '\n');
	do {
		didItWork = true;
		std::cout << "\nPlease input your string of binary numbers ('exit' without quotes to exit): > ";
		colour(14);
		getline(std::cin, binary);
		colour(lastcolour);
		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			std::cerr << "An error occured. Please try again.\n";
			didItWork = false;
			continue;
		}
		if (binary == "exit") return;
		// Get string length
		stringlen = binary.length();
		// Check in case if the user inputted too many binary numbers
		if (stringlen > 62) {
			std::cerr << "The max number of binary numbers is 62. You put " << stringlen << " characters. Please try again.\n";
			didItWork = false;
			continue;
		}
		// Check characters in string; Make sure that they are only 1 and 0
		for (int i = 0; i < stringlen; i++) {
			if (binary[i] != '1' && binary[i] != '0') {
				std::cerr << "Sorry, you have inputted a character other than 1 or 0. Please try again.\n";
				didItWork = false;
				break;
			}
		}
		// Only if everything worked will execution be allowed to advance from this do loop
		if (didItWork != false) break;
	} while (true);

	// Calculate denary value
	for (int i = stringlen - 1; i >= 0; i--) {
		if (binary[i] == '1') {
			denary2 += ind;
		}
		ind *= 2;

	}
	// Output result
	colour(34052);
	std::cout << "Result: " << denary2 << ".\n";
	colour(lastcolour);
	// End
	return;
}

void denaryToBinary() {
	short int		binary2[64];
	long long int	denary;
	int				i = 0;

	slowcharfn(true, "Welcome to the Denary to Binary converter!");
	// Prompt for denary number
	do {
		denary = num("Please enter your denary number: > ");
		if (denary == 0) {
			colour(34052);
			std::cout << "Result:\nBinary form of " << denary << ": 0.\n";
			colour(lastcolour);
			return;
		}
		colour(34052);
		std::cout << "Result:\nBinary form of " << denary << ": ";
		// Negative numbers aren't allowed
		if (denary < 0) {
			std::cout << "Sorry, negative numbers aren't allowed. Please try again.\n";
			continue;
		}
		else break;
	} while (true);

	// Calculate and output binary number
	while (denary > 0) {
		binary2[i] = denary % 2;
		denary /= 2;
		i++;
	}
	for (int j = i - 1; j >= 0; j--) {
		std::cout << binary2[j];
	}
	std::cout << '.';
	colour(lastcolour);
	// End
	return;
}

// Percentage Increase/Decrease Calculator
void percIncDecCalc() {
	long double		ognum;
	long double		endnum = 0;
	long double		topnum;
	std::cout << '\n';
	slowcharfn(true, "Welcome to the Percentage Increase/Decrease Calculator!");
	ognum = num("Please input the original number: > ");
	topnum = num("Please input the new amount: > ");
	std::cout << '\n';
	if (topnum > ognum) {
		std::cout << "This is percentage increase.\n";
		endnum = topnum - ognum;
	}
	else if (topnum < ognum) {
		std::cout << "This is percentage decrease.\n";
		endnum = ognum - topnum;
	}

	endnum = endnum / ognum * 100;
	std::cout << "Result: " << endnum << "%.";
}

void milesToKilometres() {
	// variables
	long double miles;
	long double kilometres;
	// prompt
	slowcharfn(true, "Welcome to the Miles to Kilometres converter!\n");
	miles = num("Please input the number of miles: > ");
	if (miles < 0) {
		std::cerr << "Negative numbers aren't allowed.\nExiting...\n";
		return;
	}
	// Work out value
	kilometres = miles * 1.609344;
	// Output value
	colour(34052);
	std::cout << "\n___Results___\n";
	colour(lastcolour);
	std::cout << "Inputted miles value: " << miles << " miles\nConverted kilometres value: " << kilometres << "km\n";
	return;
}

void kilometresToMiles() {
	// variables
	long double	kilometres;
	long double	miles;
	// prompt
	slowcharfn(true, "Welcome to the Kilometres to Miles converter!\n");
	kilometres = num("Please input the number of kilometres: > ");
	if (kilometres < 0) {
		std::cerr << "Negative numbers aren't allowerd.\nExiting...\n";
		return;
	}
	// Work out value
	miles = kilometres / 1.609344;
	// Output value
	colour(34052);
	std::cout << "\n___Results___\n";
	colour(lastcolour);
	std::cout << "Inputted kilometres value: " << kilometres << "km\nConverted miles value: " << miles << " miles\n";
	return;
}

// Main snap game part
void mainGameFn() {
	short int	player1Cards = 50;
	short int	player2Cards = 50;
	short int	player1Num = 0;
	short int	player2Num = 0;
	short int	pileCardNum = 0;
	short int	i = 0;

	colour(173);
	cls();
	std::cout << "\n\n";
	while (true) {
		if (i == 150) {
			cls();
			std::cout << "150 reiterations of tries are over! Player 2 (" << player2Name << ") has " << player2Cards << " cards, and Player 1 (" << player1Name << ") has " << player1Cards << " cards! So...\n";
			if (player1Cards < player2Cards) {
				std::cout << "\n--Player 2 (" << player2Name << ") WINS!!!!--\n";
			}
			else std::cout << "\n--Player 1 (" << player1Name << ") WINS!!!!--\n";

			std::cout << "Press ENTER to exit the game arena...\n";
			std::cin.get();
			cls();
			return;
		}
		if (player2Cards == 0) {
			std::cout << "OOPS! Player 2 (" << player2Name << ") has reached 0 CARDS!!!! Player 1 (" << player1Name << ") WON!!!!";
			break;
		}
		else if (player1Cards == 0) {
			std::cout << "OOPS! Player 1 (" << player1Name << ") has reached 0 CARDS!!!! Player 2 (" << player2Name << ") WON!!!!";
			break;
		}
			player1Num = randnum(15, 1);
		std::cout << "\nPlayer 1 (" << player1Name << "): \nPress any key to send a card from your stack to the pile (ESC to exit game)...";
		inputnum = _getch();
		if (inputnum == 27) {
			std::cout << "Game over because you pressed ESC.\n";
			break;
		}
		else std::cout << "\n\n";
		std::cout << "Player 1 (" << player1Name << ") Card: " << player1Num << '\n';
		player1Cards--;
		pileCardNum++;
		if (player1Num == player2Num) {
			std::cout << "SNAP!! Player 1 (" << player1Name << ") gets " << pileCardNum << " cards from the pile!\n";
			player1Cards += pileCardNum;
			pileCardNum = 0;
			std::cout << "Player 1 now has " << player1Cards << " cards!\n\n";
		}
			player2Num = randnum(15, 1);
			if (player2Name == "Computer") {
				std::cout << "Player 2 (" << player2Name << ") Card: " << player2Num << '\n';
			}
			else {
				std::cout << "\nPlayer 2 (" << player2Name << "): \nPress ENTER to send a card from your stack to the pile (ESC to exit game)...";
				inputnum = _getch();
				if (inputnum == 27) {
					std::cout << "Game over because you pressed ESC.\n";
					break;
				}
				else std::cout << "\n\n";
				std::cout << "Player 2 (" << player2Name << ") Card: " << player2Num << '\n';
			}
			player2Cards--;
			pileCardNum++;
			if (player2Num == player1Num) {
				std::cout << "SNAP!! Player 2 (" << player2Name << ") gets " << pileCardNum << " cards from the pile!\n";
				player2Cards += pileCardNum;
				std::cout << "Player 2 now has " << player2Cards << " cards!\n\n";
				pileCardNum = 0;
			}
			i++;
		}
		return;
	}

// How-to-play for snap game
void howtoplay() {
	cls();
	colour(173);
	std::cout << "################################### SNAP GAME - HOW TO PLAY ######################################\n\n";
	slowcharfn(true, "2 players take turns to put numbered cards on a pile.");
	slowcharfn(true, "If player 1 and player 2's cards match, the last person who laid down their card gets the whole pile and uses it in their stack.");
	slowcharfn(true, "First to 0 cards loses. You can also lose if you have the lowest amount of cards after 150 reiterations through turns.");
	slowcharfn(true, "The computer will pick the cards from 1 to 15 at random by itself for both players. \nGood luck!");
	std::cout << "##################################################################################################\n\n";
	std::cout << "Press ENTER to continue...\n";
	std::cin.ignore();
	std::cin.get();
	return;
}

// Snap game
void snapGame() {
		char	inputchar;

		while (true) {
			colour(173);
			cls();
			std::cout << "################################### SNAP GAME - MENU ######################################\n\n\t\t\t\t[1] Play\n\t\t\t\t[2] How to Play\n\t\t\t\t[3] Exit\n\n######################################################################################\n";
			std::cout << "\nPlease input your desired number here: > ";
			std::cin >> inputnum;
			if (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(INT_MAX, '\n');
				std::cout << "Sorry, your input was incorrect. Please try again.\nPress ENTER to continue...";
				std::cin.get();
				cls();
				continue;
			}
			switch (inputnum) {
			case 1:
				player1Name = str("\nPlease input Player 1's name ('exit' to go back): > ");
				if (player1Name == "exit") break;
				std::cout << "Would you like to play against the computer or another person?\n[1] Real Person\n[2] Computer\n[3] Go to Main Menu";
				inputnum = num("\nPlease input your desired number here: > ");
				if (inputnum == 3) break;
				switch (inputnum) {
				case 1:
					player2Name = str("Please input Player 2's name: > ");
					break;
				case 2:
					player2Name = "Computer";
					break;
				default:
					std::cout << "Sorry, your input was incorrect. Please try again.\n";
					break;
				}
				while (true) {
					mainGameFn();
					if (inputnum == 27) break;
					std::cout << "\nWould you like to play again? [y/n]: > ";
					std::cin >> inputchar;
					if (inputchar == 'y') {
						continue;
					}
					else break;
				}
				break;
			case 2:
				howtoplay();
				break;
			case 3:
				colour(lastcolour);
				cls();
				return;
			default:
				std::cout << "Sorry, your input was incorrect. Please try again.\nPress ENTER to continue...";
				std::cin.get();
				cls();
				break;
			}
		}
		return;
}

// Guess game hard game
void guessgamehard() {
	colour(30);
	std::ifstream scoreguesshardin("TerminalApp_Temp\\scoreguesshard.confi");
	if (scoreguesshardin.fail()) {
		std::ofstream scoreguesshard("TerminalApp_Temp\\scoreguesshard.confi");
		scoreguesshard << "0";
		scoreguesshard.close();
	}
	scoreguesshardin >> highscoreguess;
	scoreguesshardin.close();
	cls();
	std::cout << "################################# GAME 2 - GUESS THE NUMBER - HARD ##############################################\n";
	std::cout << "Welcome to Guess The Number!";
	do {
		std::cout << "\n[1] Play\n[2] Tutorial\n[3] High Score\n[4] Exit\n";
		std::cout << "Input number here: > ";
		std::cin >> inputnum;
		if (inputnum == 1) {
			break;
		}
		else if (inputnum == 2) {
			cls();
			std::cout << "################################# GAME 2 - GUESS THE NUMBER - HARD ######################################\n";
			slowcharfn(true, "Welcome to Guess The Number - Hard! \nYou have 5 tries to guess a number between 2 randomly picked numbers. \n");
			slowcharfn(true, "These 2 numbers are between 1 and 100. The game will log your highest score.");
			slowcharfn(true, "If you get the answer wrong, the game will tell you if your answer was higher or lower than the correct number.\n");
			slowcharfn(true, "Good luck!\n");
			std::cout << "Press ENTER to continue...\n";
			std::cout << "########################################################################################################\n";
			std::cin.ignore(INT_MAX, '\n');
			std::cin.get();
		}
		else if (inputnum == 3) {
			colour(34052);
			std::cout << "Current high score: " << highscoreguess << ".";
			colour(30);
			std::cout << "\n";
		}
		else {
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			colour(lastcolour);
			cls();
			return;
		}
	} while (true);

	for (;;) {
		lowernum = randnum(49, 0);
		uppernum = randnum(100, 51);
		rightnum = randnum(uppernum, lowernum);
		tries = 0;
		score = 0;
		cls();
		std::cout << "\nLet the games begin!\n";
		sleep(1000);
		do {
			std::cout << "\nThis number is between " << lowernum << " and " << uppernum << ".";
			std::cout << "\nInput guess number: > ";
			std::cin >> guessnum;
			std::cin.ignore();

			while (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(INT_MAX, '\n');
				colour(lastcolour);
				cls();
				return;
			}

			if (guessnum == rightnum) {
				score++;
				std::cout << "\nYou got the number right in " << tries << " tries!\nYour new score is " << score << ".\n\n";
				std::cin.clear();
				rightnum = randnum(100, 0);
				lowernum = randnum(rightnum, 0);
				uppernum = randnum(100, rightnum);
				tries = 0;
				goto endloop;
			}
			if (guessnum > uppernum) {
				std::cout << "\nThat guess number was higher than " << uppernum << ". Please try again.\n";
				goto endloop;
			}
			if (guessnum < lowernum) {
				std::cout << "\nThat guess number was less than " << lowernum << ". Please try again.\n";
				goto endloop;
			}
			if (guessnum < rightnum) {
				tries++;
				std::cout << "\nWrong! Your guess was lower than the correct number.\nYou have " << (5 - tries) << " tries left (out of 5).\n";
			}
			if (guessnum > rightnum) {
				tries++;
				std::cout << "\nWrong! Your guess was higher than the correct number.\nYou have " << (5 - tries) << " tries left (out of 5).\n";
			}
		endloop:
			if (tries == 5) {
				cls();
				if (highscoreguess < score) {
					highscoreguess = score;
					std::cout << "New high score!!!\n\n";
				}
				std::ofstream scoreguesshardout("TerminalApp_Temp\\scoreguesshard.confi");
				scoreguesshardout << "";
				scoreguesshardout << highscoreguess;
				scoreguesshardout.close();

				std::cout << "Oh no! You have used up all of your 5 tries.\nYour score was " << score << ", and the correct number was " << rightnum << ".\nYou lost! Would you like to play again? [y/n] > ";
				std::cin >> input;
				if (input == 'y') {
					cls();
					break;
				}
				else {
					std::cin.clear();
					std::cin.ignore(INT_MAX, '\n');
					colour(lastcolour);
					cls();
					return;
				}
			}
		} while (true);
	}
}

// Put cursor in specific area function
void gotoxy(int x, int y) {
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}

// Set cursor position function
void setcursor(bool visible, DWORD size) {
	if (size == 0) {
		size = 20;
	}
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console, &lpCursor);
}

// Draw the car game border
void drawBorder() {
	for (int i = 0; i < SCREEN_HEIGHT; i++) {
		for (int j = 0; j < 17; j++) {
			gotoxy(0 + j, i); std::cout << "±";
			gotoxy(WIN_WIDTH - j, i); std::cout << "±";
		}
	}
	for (int i = 0; i < SCREEN_HEIGHT; i++) {
		gotoxy(SCREEN_WIDTH, i); std::cout << "±";
	}
}

// Generate location for enemy cars
void genEnemy(int ind) {
	enemy_x[ind] = 17 + rand() % (33);
}

// Design/draw enemy cars
void drawEnemy(int ind) {
	if (enemyF[ind] == 1) {
		gotoxy(enemy_x[ind], enemy_y[ind]);
		std::cout << "0**0";
		gotoxy(enemy_x[ind], enemy_y[ind] + 1);
		std::cout << " ** ";
		gotoxy(enemy_x[ind], enemy_y[ind] + 2);
		std::cout << "0**0";
		gotoxy(enemy_x[ind], enemy_y[ind] + 3);
		std::cout << " || ";
	}
}

// Erase the enemy cars from their spot
void eraseEnemy(int ind) {
	if (enemyF[ind] == 1) {
		gotoxy(enemy_x[ind], enemy_y[ind]); std::cout << "    ";
		gotoxy(enemy_x[ind], enemy_y[ind] + 1); std::cout << "    ";
		gotoxy(enemy_x[ind], enemy_y[ind] + 2); std::cout << "    ";
		gotoxy(enemy_x[ind], enemy_y[ind] + 3); std::cout << "    ";
	}
}

// Reset the enemy location and redraw it
void resetEnemy(int ind) {
	eraseEnemy(ind);
	enemy_y[ind] = 1;
	genEnemy(ind);
}

// Easy car game draw car function
void drawCareasy() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			gotoxy(j + carPos, i + 22);
			if (CarType == 1) std::cout << mycar1[i][j];
			else if (CarType == 2) std::cout << mycar2[i][j];
			else if (CarType == 3) std::cout << mycar3[i][j];
		}
	}
}

// Hard car game draw car function
void drawCarhard() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			gotoxy(j + carPos, i + 22);
			if (CarType == 1) std::cout << mycar1[i][j];
			else if (CarType == 2) std::cout << mycar4[i][j];
			else if (CarType == 3) std::cout << mycar5[i][j];
		}
	}
}

// Erase the car
void eraseCar() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			gotoxy(j + carPos, i + 22); std::cout << " ";
		}
	}
}

// Collision function to see if car collided
int collision() {
	if (enemy_y[0] + 4 >= 22) {
		if (enemy_x[0] + 4 - carPos >= 0 && enemy_x[0] + 4 - carPos < 9) {
			return 1;
		}
	}
	return 0;
}

// Game over function for hard car game
void gameoverhard() {
	cls();

	if (highscorecarhard < scorecar) {
		highscorecarhard = scorecar;
		colourtxt("New high score!!!\n\n");
	}
	std::ofstream scorecarhardout("TerminalApp_Temp\\scorecardodgehard.confi");
	scorecarhardout << "";
	scorecarhardout << highscorecarhard;
	scorecarhardout.close();

	std::cout << std::endl;
	std::cout << "\t\t--------------------------" << std::endl;
	std::cout << "\t\t-------- Game Over -------" << std::endl;
	std::cout << "\t\t--------------------------" << std::endl;
	std::cout << "\t\tYou crashed!\n\t\tYou racked up " << scorecar << " points in this round";
	if (scorecar <= 9) {
		std::cout << " (Below Average).";
	}
	else if (scorecar <= 37) {
		std::cout << " (Average).";
	}
	else if (scorecar <= 70) {
		std::cout << " (Above Average).";
	}
	else if (scorecar <= 95) {
		std::cout << " (Insane).";
	}
	else if (scorecar <= 150) {
		std::cout << " (G.O.A.T Level).";
	}
	else if (scorecar > 150) {
		std::cout << " (GOD Level).";
	}
	std::cout << "\n\nPress ENTER to go back to menu... ";
	scorecar = 0;
	std::cin.clear();
	std::cin.get();
	return;
}

// Game over function for easy car game
void gameovereasy() {
	cls();

	if (highscorecareasy < scorecar) {
		highscorecareasy = scorecar;
		std::cout << "New high score!!!\n\n";
	}
	std::ofstream scorecareasyout("TerminalApp_Temp\\scorecardodgeeasy.confi");
	scorecareasyout << "";
	scorecareasyout << highscorecareasy;
	scorecareasyout.close();

	std::cout << std::endl;
	std::cout << "\t\t--------------------------" << std::endl;
	std::cout << "\t\t-------- Game Over -------" << std::endl;
	std::cout << "\t\t--------------------------" << std::endl;
	std::cout << "\t\tYou crashed!\n\t\tYou racked up " << scorecar << " points in this round";
	if (scorecar <= 50) {
		std::cout << " (Below Average).";
	}
	else if (scorecar <= 100) {
		std::cout << " (Average).";
	}
	else if (scorecar <= 200) {
		std::cout << " (Above Average).";
	}
	else if (scorecar <= 325) {
		std::cout << " (Insane).";
	}
	else if (scorecar <= 525) {
		std::cout << " (G.O.A.T Level).";
	}
	else if (scorecar > 525) {
		std::cout << " (GOD Level).";
	}
	std::cout << "\n\nPress ENTER to go back to menu... ";
	scorecar = 0;
	std::cin.clear();
	std::cin.get();
	return;
}

// Score updater function
void updateScore() {
	gotoxy(WIN_WIDTH + 7, 5);
	std::cout << "Points: " << scorecar << std::endl;
}

// Instruction function for car game
void instructions() {
	cls();
	slowcharfn(false, "How to Play");
	std::cout << "\n----------------";
	slowcharfn(false, "\nAvoid hitting cars by moving left or right.");
	slowcharfn(false, "\n\nPress A to move left.");
	slowcharfn(false, "\nPress D to move right.");
	slowcharfn(false, "\nPress ESC to exit the current match.");
	std::cout << "\n\nPress ENTER to go back to the menu...";
	std::cin.clear();
	std::cin.ignore();
	std::cin.get();
}

// Play function (easy)
void play() {
	setcursor(0, 0);
	carPos = -1 + WIN_WIDTH / 2;
	score = 0;
	enemyF[0] = 1;
	enemyF[1] = 0;
	enemy_y[0] = enemy_y[1] = 1;

	colour(95);
	cls();
	drawBorder();
	updateScore();
	genEnemy(0);
	genEnemy(1);

	gotoxy(WIN_WIDTH + 4, 2); std::cout << "Car Dodge- Easy";
	gotoxy(WIN_WIDTH + 6, 4); std::cout << "----------";
	gotoxy(WIN_WIDTH + 6, 6); std::cout << "----------";
	gotoxy(WIN_WIDTH + 7, 12); std::cout << "Controls ";
	gotoxy(WIN_WIDTH + 7, 13); std::cout << "-------- ";
	gotoxy(WIN_WIDTH + 2, 14); std::cout << " A Key - Left";
	gotoxy(WIN_WIDTH + 2, 15); std::cout << " D Key - Right";

	gotoxy(18, 5);
	std::cout << "Press ENTER to start... ";
	std::cin.clear();
	std::cin.ignore();
	std::cin.get();
	gotoxy(44, 5);
	colour(47);
	std::cout << "3 ";
	sleep(1000);
	std::cout << "2 ";
	sleep(1000);
	std::cout << "1";
	sleep(1000);

	colour(95);
	gotoxy(18, 5);
	std::cout << "                               ";

	while (1) {
		if (_kbhit()) {
			char ch = _getch();
			if (ch == 'a' || ch == 'A') {
				if (carPos > 18) {
					carPos -= 4;
				}
			}
			if (ch == 'd' || ch == 'D') {
				if (carPos < 50) {
					carPos += 4;
				}
			}
			if (ch == 27) {
				gotoxy(72, 22);
				std::cout << "You are PAUSED.";
				gotoxy(72, 23);
				std::cout << "Press ENTER to play.";
				gotoxy(72, 24);
				std::cout << "Press ESC to exit.";
				ch = _getch();
				if (ch == 27) {
					setcursor(1, 1);
					scorecar = 0;
					break;
				}
				else {
					gotoxy(72, 22);
					std::cout << "               ";
					gotoxy(72, 23);
					std::cout << "                  ± ";
					gotoxy(72, 24);
					std::cout << "                  ";
				}

			}
		}

		drawCareasy();
		drawEnemy(0);
		drawEnemy(1);

		if (collision() == 1) {
			setcursor(1, 1);
			gameovereasy();
			return;
		}

		sleep(45);
		eraseCar();
		eraseEnemy(0);
		eraseEnemy(1);

		if (enemy_y[0] == 10) {
			if (enemyF[1] == 0) {
				enemyF[1] = 1;
			}
		}

		if (enemyF[0] == 1) {
			enemy_y[0] += 1;
		}

		if (enemyF[1] == 1) {
			enemy_y[1] += 1;
		}

		if (enemy_y[0] > SCREEN_HEIGHT - 4) {
			resetEnemy(0);
			scorecar++;
			updateScore();
		}

		if (enemy_y[1] > SCREEN_HEIGHT - 4) {
			resetEnemy(1);
			scorecar++;
			updateScore();
		}

	}
}

// Change car function (easy)
void changecar() {
	gotoxy(1, 1);
	cls();
	std::cout << "Car options:\n\n";
	std::cout << "  ±± \n ±±±±\n  ±± \n ±±±±\n\nStandard";
	gotoxy(10, 2);
	std::cout << "  || ";
	gotoxy(10, 3);
	std::cout << " 0**0";
	gotoxy(10, 4);
	std::cout << " |**|";
	gotoxy(10, 5);
	std::cout << " 0__0";
	gotoxy(10, 7);
	std::cout << "Slicer";
	gotoxy(18, 2);
	std::cout << " /TT\\";
	gotoxy(18, 3);
	std::cout << " |__|";
	gotoxy(18, 4);
	std::cout << " |  |";
	gotoxy(18, 5);
	std::cout << " /\\/\\";
	gotoxy(18, 7);
	std::cout << "Rocket Car";
	gotoxy(0, 9);
	std::cout << "[1] Standard\n[2] Slicer\n[3] Rocket Car\n";
	do {
		std::cout << "Please input your option (0 to exit): > ";
		std::cin >> inputnum;
		if (inputnum == 1) {

			CarType = 1;
			std::cout << "Option 1 set!\n";
		}
		else if (inputnum == 2) {
			CarType = 2;
			std::cout << "Option 2 set!\n";
		}
		else if (inputnum == 3) {
			CarType = 3;
			std::cout << "Option 3 set!\n";
		}
		else if (inputnum == 0) {
			return;
		}
		else {
			std::cin.clear();
			std::cin.ignore();
			std::cout << "Please retry.\n";
		}
		if (inputnum == 1 || inputnum == 2 || inputnum == 3) {
			sleep(1000);
			break;
		}
	} while (true);
	return;
}


void cardodgeeasy() {
	std::ifstream scorecareasyin("TerminalApp_Temp\\scorecardodgeeasy.confi");

	if (scorecareasyin.fail()) {
		std::ofstream scorecareasy("TerminalApp_Temp\\scorecardodgeeasy.confi");
		scorecareasy << "0";
		scorecareasy.close();
	}
	scorecareasyin >> highscorecareasy;
	scorecareasyin.close();

	srand((unsigned)time(nullptr));

	do {
		colour(47);
		cls();
		gotoxy(10, 5); std::cout << " --------------------------------- ";
		gotoxy(10, 6); std::cout << " |  Welcome to Car Dodge- Easy!  | ";
		gotoxy(10, 7); std::cout << " ---------------------------------";
		gotoxy(10, 9); std::cout << "[1] Play";
		gotoxy(10, 10); std::cout << "[2] Choose Car";
		gotoxy(10, 11); std::cout << "[3] High Score";
		gotoxy(10, 12); std::cout << "[4] How to Play";
		gotoxy(10, 13); std::cout << "[5] Exit";
		gotoxy(10, 14); std::cout << "Input your desired number: > ";
		std::cin.clear();
		std::cin >> inputnum;

		switch (inputnum) {
		case 1:
			play();
			break;
		case 2:
			changecar();
			break;
		case 3:
			colour(32778);
			std::cout << "Current high score: " << highscorecareasy;
			if (highscorecareasy <= 50) {
				std::cout << " (Below Average).";
			}
			else if (highscorecareasy <= 100) {
				std::cout << " (Average).";
			}
			else if (highscorecareasy <= 200) {
				std::cout << " (Above Average).";
			}
			else if (highscorecareasy <= 325) {
				std::cout << " (Insane).";
			}
			else if (highscorecareasy <= 525) {
				std::cout << " (G.O.A.T Level).";
			}
			else if (highscorecareasy > 525) {
				std::cout << " (GOD Level).";
			}
			colour(47);
			std::cout << "\nPress ENTER to continue...";
			std::cin.ignore();
			std::cin.get();
			break;
		case 4:
			instructions();
			break;
		case 5:
			CarType = 1;
			scorecar = 0;
			colour(lastcolour);
			cls();
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			return;
		default:
			Beep(500, 250);
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
		}
	} while (true);

	return;
}

void playhard() {
	setcursor(0, 0);
	carPos = -1 + WIN_WIDTH / 2;
	score = 0;
	enemyF[0] = 1;
	enemyF[1] = 0;
	enemy_y[0] = enemy_y[1] = 1;

	colour(111);
	cls();
	drawBorder();
	updateScore();
	genEnemy(0);
	genEnemy(1);

	gotoxy(WIN_WIDTH + 4, 2); std::cout << "Car Dodge- Hard";
	gotoxy(WIN_WIDTH + 6, 4); std::cout << "----------";
	gotoxy(WIN_WIDTH + 6, 6); std::cout << "----------";
	gotoxy(WIN_WIDTH + 7, 12); std::cout << "Controls ";
	gotoxy(WIN_WIDTH + 7, 13); std::cout << "-------- ";
	gotoxy(WIN_WIDTH + 2, 14); std::cout << " A Key - Left";
	gotoxy(WIN_WIDTH + 2, 15); std::cout << " D Key - Right";

	gotoxy(18, 5);
	std::cout << "Press ENTER to start... ";
	std::cin.clear();
	std::cin.ignore();
	std::cin.get();
	gotoxy(44, 5);
	colour(176);
	std::cout << "3 ";
	sleep(1000);
	std::cout << "2 ";
	sleep(1000);
	std::cout << "1";
	sleep(1000);

	colour(111);
	gotoxy(18, 5);
	std::cout << "                               ";

	while (1) {
		if (_kbhit()) {
			char ch = _getch();
			if (ch == 'a' || ch == 'A') {
				if (carPos > 18) {
					carPos -= 4;
				}
			}
			if (ch == 'd' || ch == 'D') {
				if (carPos < 50) {
					carPos += 4;
				}
			}
			if (ch == 27) { // 27 because ASCII ESC code is 27
				gotoxy(72, 22);
				std::cout << "You are PAUSED.";
				gotoxy(72, 23);
				std::cout << "Press ENTER to play.";
				gotoxy(72, 24);
				std::cout << "Press ESC to exit.";
				ch = _getch();
				if (ch == 27) {
					setcursor(1, 1);
					scorecar = 0;
					break;
				}
				else {
					gotoxy(72, 22);
					std::cout << "               ";
					gotoxy(72, 23);
					std::cout << "                  ± ";
					gotoxy(72, 24);
					std::cout << "                  ";
				}

			}
		}

		drawCarhard();
		drawEnemy(0);
		drawEnemy(1);

		if (collision() == 1) {
			setcursor(1, 1);
			gameoverhard();
			return;
		}

		sleep(20);
		eraseCar();
		eraseEnemy(0);
		eraseEnemy(1);

		if (enemy_y[0] == 10) {
			if (enemyF[1] == 0) {
				enemyF[1] = 1;
			}
		}

		if (enemyF[0] == 1) {
			enemy_y[0] += 1;
		}

		if (enemyF[1] == 1) {
			enemy_y[1] += 1;
		}

		if (enemy_y[0] > SCREEN_HEIGHT - 4) {
			resetEnemy(0);
			scorecar++;
			updateScore();
		}

		if (enemy_y[1] > SCREEN_HEIGHT - 4) {
			resetEnemy(1);
			scorecar++;
			updateScore();
		}

	}
}

// Chnage car for car game hard
void changecarhard() {
	gotoxy(1, 1);
	cls();
	std::cout << "Car options:\n\n";
	std::cout << "  ±± \n ±±±±\n  ±± \n ±±±±\n\nStandard";
	gotoxy(10, 2);
	std::cout << " _==_";
	gotoxy(10, 3);
	std::cout << " ±||±";
	gotoxy(10, 4);
	std::cout << " |00|";
	gotoxy(10, 5);
	std::cout << " ±)(±";
	gotoxy(10, 7);
	std::cout << "F1 Car";
	gotoxy(18, 2);
	std::cout << "  ±± ";
	gotoxy(18, 3);
	std::cout << " ±**±";
	gotoxy(18, 4);
	std::cout << "  %% ";
	gotoxy(18, 5);
	std::cout << " ±==±";
	gotoxy(18, 7);
	std::cout << "Army Car";
	gotoxy(0, 9);
	std::cout << "[1] Standard\n[2] F1 Car\n[3] Army Car\n";
	do {
		std::cout << "Please input your option (0 to exit): > ";
		std::cin >> inputnum;
		if (inputnum == 1) {

			CarType = 1;
			std::cout << "Option 1 set!\n";
		}
		else if (inputnum == 2) {
			CarType = 2;
			std::cout << "Option 2 set!\n";
		}
		else if (inputnum == 3) {
			CarType = 3;
			std::cout << "Option 3 set!\n";
		}
		else if (inputnum == 0) {
			return;
		}
		else {
			std::cin.clear();
			std::cin.ignore();
			std::cout << "Please retry.\n";
		}
		if (inputnum == 1 || inputnum == 2 || inputnum == 3) {
			sleep(1000);
			break;
		}
	} while (true);
	return;
}

// Car dodge hard function/game
void cardodgehard() {
	// Check high score
	std::ifstream scorecarhardin("TerminalApp_Temp\\scorecardodgehard.confi");

	if (scorecarhardin.fail()) {
		// Set high score if no highscore file is present
		std::ofstream scorecarhard("TerminalApp_Temp\\scorecardodgehard.confi");
		scorecarhard << "0";
		scorecarhard.close();
	}
	scorecarhardin >> highscorecarhard;
	scorecarhardin.close();

	srand((unsigned)time(nullptr));

	do {
		// Display menu screen
		colour(176);
		cls();
		gotoxy(10, 5); std::cout << " --------------------------------- ";
		gotoxy(10, 6); std::cout << " |  Welcome to Car Dodge- Hard!  | ";
		gotoxy(10, 7); std::cout << " ---------------------------------";
		gotoxy(10, 9); std::cout << "[1] Play";
		gotoxy(10, 10); std::cout << "[2] Choose Car";
		gotoxy(10, 11); std::cout << "[3] High Score";
		gotoxy(10, 12); std::cout << "[4] How to Play";
		gotoxy(10, 13); std::cout << "[5] Exit";
		gotoxy(10, 14); std::cout << "Input your desired number: > ";
		std::cin.clear();
		std::cin >> inputnum;

		switch (inputnum) {
		case 1:
			playhard();
			break;
		case 2:
			changecarhard();
			break;
		case 3:
			colour(32780);
			std::cout << "Current high score: " << highscorecarhard;
			// Check score skill level
			if (highscorecarhard <= 9) {
				std::cout << " (Below Average).";
			}
			else if (highscorecarhard <= 37) {
				std::cout << " (Average).";
			}
			else if (highscorecarhard <= 70) {
				std::cout << " (Above Average).";
			}
			else if (highscorecarhard <= 95) {
				std::cout << " (Insane).";
			}
			else if (highscorecarhard <= 150) {
				std::cout << " (G.O.A.T Level).";
			}
			else if (highscorecarhard > 150) {
				std::cout << " (GOD Level).";
			}
			colour(176);
			std::cout << "\nPress ENTER to continue...";
			std::cin.ignore();
			std::cin.get();
			break;
		case 4:
			instructions();
			break;
		case 5:
			CarType = 1;
			scorecar = 0;
			colour(lastcolour);
			cls();
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			return;
		default:
			Beep(500, 250);
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
		}

	} while (true);

	return;
}
// Easy guess game function
void guessgameeasy() {
	colour(55);
	cls();
	std::cout << "################################# GAME 1 - GUESS THE NUMBER - EASY ##############################################\n";
	std::cout << "Welcome to Guess The Number!";
	do {
		colour(55);
		std::cout << "\n[1] Play\n[2] Tutorial\n[3] Exit\n";
		std::cout << "Input number here: > ";
		std::cin >> inputnum;
		if (inputnum == 1) {
			break;
		}
		else if (inputnum == 2) {
			cls();
			std::cout << "################################# GAME 1 - GUESS THE NUMBER - EASY ##############################################\n";
			slowcharfn(true, "You have 10 tries to guess a number between 0 and 100 inclusive. \n");
			slowcharfn(true, "If you get the answer wrong, the game will tell you if your answer was higher or lower than the correct number.");
			std::cout << "\n";
			slowcharfn(true, "Good luck!\n");
			std::cout << "Press ENTER to continue...\n";
			std::cout << "########################################################################################################\n";
			std::cin.ignore(INT_MAX, '\n');
			std::cin.get();
		}
		else {
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			colour(lastcolour);
			cls();
			return;
		}
	} while (true);

	for (;;) {
		rightnum = randnum(100, 0);
		tries = 0;
		cls();
		std::cout << "\nLet the games begin!\n";
		sleep(1000);
		cls();
		do {
			std::cout << "\nInput guess number: > ";
			std::cin >> guessnum;

			while (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(INT_MAX, '\n');
				colour(lastcolour);
				cls();
				return;
			}

			if (guessnum == rightnum) {
				std::cout << "Congratulations! You won!\nWould you like to play again? [y/n] > ";
				std::cin >> input;
				if (input == 'y') {
					break;
				}
				else {
					std::cin.clear();
					std::cin.ignore(INT_MAX, '\n');
					colour(lastcolour);
					cls();
					return;
				}
			}
			if (guessnum > 100) {
				std::cout << "That guess number was higher than 100. Please try again.\n";
				goto endloop;
			}
			if (guessnum < 0) {
				std::cout << "That guess number was less than 0. Please try again.\n";
				goto endloop;
			}
			if (guessnum < rightnum) {
				tries++;
				std::cout << "Wrong! Your guess was lower than the correct number.\nYou have " << (10 - tries) << " tries left (out of 10).\n";
			}
			if (guessnum > rightnum) {
				tries++;
				std::cout << "Wrong! Your guess was higher than the correct number.\nYou have " << (10 - tries) << " tries left (out of 10).\n";
			}
		endloop:
			if (tries == 10) {
				cls();
				std::cout << "Oh no! You have used up all of your 10 tries.\nYou lost! Would you like to play again? [y/n] > ";
				std::cin >> input;
				if (input == 'y') {
					cls();
					break;
				}
				else {
					std::cin.clear();
					std::cin.ignore(INT_MAX, '\n');
					colour(lastcolour);
					cls();
					return;
				}
			}
		} while (true);
	}


}

// Commands function- this is where all commands will go
void commands() {
	nNumofUsrCommands++;
	// help
	if (command == "help" || command == "0") {
		help();
		return;
	}
	// tutorial
	else if (command == "tutorial" || command == "1") {
		std::cout << "Are you sure you want to enter a tutorial? [y/n] > ";
		colour(14);
		std::cin >> input;
		colour(lastcolour);
		std::cin.clear();
		std::cin.ignore(INT_MAX, '\n');
		if (input == 'y') tutorial(true);
		return;
	}
	// hello
	else if (command == "hello" || command == "3") {
		std::cout << "Hello, World!\n";
		colour(lastcolour);
		return;
	}
	// cls
	else if (command == "cls" || command == "4") {
		cls();
		return;
	}
	// clear
	else if (command == "clear" || command == "5") {
		// ANSI escape code for just clearing the screen
		std::cout << "\x1B[2J\x1B[H";
		return;
	}
	// coltxt
	else if (command == "coltxt" || command == "10") {
		if (lastcolour == 7) colour(3);
		slowcharfn(false, "Input text that you want to colour: > ");
		colour(14);
		std::cin.clear();
		coltxt = str("");
		colour(7);
		// This function colours the text
		colourtxt(coltxt);
		colour(lastcolour);
		std::cout << "\n";
		return;
	}
	// slowchar
	else if (command == "slowchar" || command == "11") {
		std::cout << "Input what you want to output as 'slow characters': > ";
		colour(14);
		spare = str("");
		colour(lastcolour);
		// A boolean is passed along with the function so it knows if you need a newline or not
		slowcharfn(false, spare);
		std::cout << std::endl;
		return;
	}
	// ansi
	else if (command == "ansi" || command == "12") {
		colour(2);
		slowcharfn(true, "Welcome to the ANSI Editor.");
		colour(lastcolour);
		do {
			std::cout << "Please input your ANSI escape code without any square brackets or \\x1b (type 0 to exit, 1 for list of ansi codes): > ";
			spare = str("");
			if (spare == "0") {
				std::cout << "Do you want to save your changes? [y/n] > ";
				std::cin.clear();
				getline(std::cin, spare);
				if (spare == "y") {
					ansicol = 1;
				}
				else ansicol = 0;
				return;
			}
			if (spare == "1") {
				std::cout << "Graphics/Colours:\n\n0m - Reset all\n1m- Bold mode (Reset: 22m)\n2m- Dim/faint mode (Reset: 22m)\n3m- Italic mode (Reset: 23m)\n"
					<< "4m- Underline mode (Reset: 24m)\n5m- Blinking mode (Reset: 25m)\n7m- Inverse/Reverse mode (Reset: 27m)\n8m- Hidden/Invisible mode (Reset: 28m)\n"
					<< "9m- Strikethrough mode (Reset: 29m)\n";
				spare = "a";
			}
			if (spare == "0m") {
				ansicol = 0;
			}
			// This is passed along with the value that the user inputted
			std::cout << "\x1b[" << spare << std::endl;
		} while (true);
	}
	// beep
	else if (command == "beep" || command == "6") {
		// Variables
		char text[40] = "Your sound is being outputted!!!";
		long long int hertz;
		long long int milsecs;

		std::cout << "Input anything other than a number or 0 to exit.\n\nThe maximum an average teenager can hear is 20,000 hertz (please don't try it).\n";

		// Enter hertz number
		std::cout << "\nPlease input your hertz value: > ";
		colour(14);
		hertz = num("");
		colour(lastcolour);

		// Clear input buffer if input failed
		while (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			return;
		}

		// If hertz input is 0, exit program
		if (hertz == 0) {
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			return;
		}
		// Enter milliseconds number
		std::cout << "Enter the amount of time in milliseconds: > ";
		colour(14);
		milsecs = num("");
		colour(lastcolour);

		// Clear input buffer if input failed
		while (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			return;
		}

		// If millisecond input is 0, exit program
		if (milsecs == 0) {
			std::cin.clear();
			std::cin.ignore();
			return;
		}

		// Creates colourful text saying that the sound is being outputted
		colourtxt(text);
		// Set the text colour to what it was before
		colour(lastcolour);
		// The 'Beep' function located in conio.h.
		Beep(hertz, milsecs);
		std::cout << "\nDone.\n\n";
		clearkeebbuf();
		std::cin.clear();
		std::cin.ignore(INT_MAX, '\n');
		return;
	}
	// sleep
	else if (command == "sleep" || command == "8") {
		int sleeptime;
		std::cout << "Input a character or 0 to exit.\n";
		std::cout << "Please input ";
		colour(34052);
		std::cout << "milliseconds";
		colour(lastcolour);
		std::cout << " that you want to time: > ";
		colour(14);
		sleeptime = num("");
		colour(lastcolour);
		if (sleeptime == 0) {
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			return;
		}
		// While loop to prevent the program from crashing if a character is inputted
		while (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			return;
		}
		std::cout << "Timing...\n";
		sleep(sleeptime);
		std::cout << "\nTimer finished.\n";
		// Clear keyboard buffer
		clearkeebbuf();
		std::cin.clear();
		std::cin.ignore(INT_MAX, '\n');
		return;
		// echo
	}
	else if (command[0] == 'e' && command[1] == 'c' && command[2] == 'h' && command[3] == 'o' || command == "13") {
		if (command == "13" || command[4] != ' ') {
			std::cout << "Input what you want me to output: > ";
			spare = str("");
			std::cout << spare << "\n";
			return;
		}
		for (int i = 5; command[i] != '\0'; i++) {
			std::cout << command[i];
		}
		std::cout << std::endl;
		return;
	}
	// textinfo
	else if (command == "textinfo" || command == "7") {

		// Ask for text input
		std::cout << "Input 0 to exit. \nPlease input the string of text that you want to see statistics of: > ";
		std::cin.clear();
		colour(14);
		text = str("");
		colour(lastcolour);
		if (text == "0") {
			return;
		}
		// Count the number of words and letters
		for (int i = 0; text[i] != '\0'; chars++, letters++, i++) {
			if (text[i] == ' ') {
				words++;
				letters--;
			}
			if (text[i] == '.') letters--;
		}
		// Output results
		std::cout << "\n---------------------------\nStatistics:\nLetters: " << letters << "\nTotal number of characters: " << chars << "\nWords: " << words << "\n---------------------------\n" << std::endl;
		// Zero out variables back to normal
		text = "";
		letters = 1;
		words = 1;
		chars = 1;
		return;
	}
	// game
	else if (command == "game" || command == "9") {
		// Games cannot be run from scripts in this program.
		if (bIsRunFromFile == true) {
			std::cout << "Sorry, you are currently using a file as your main source of input. You can't use games right now.\nExiting...\n";
			return;
		}
		std::cout << "Welcome to the Game Selector! Please choose your game (0 to exit):";
		// Here's an example of a numbered list in this program
		std::cout << "\n[1] Guess the number! (Easy)";
		std::cout << "\n[2] Guess the number! (Hard)";
		std::cout << "\n[3] Car Dodge (Easy)";
		std::cout << "\n[4] Car Dodge (Hard)";
		std::cout << "\n[5] Snap Game";
		std::cout << "\nPlease input the number next to your desired option: > ";
		colour(14);
		std::cin >> inputnum;
		colour(lastcolour);
		// switch case for inputted number
		switch (inputnum) {
		case 1:
			guessgameeasy();
			break;
		case 2:
			guessgamehard();
			break;
		case 3:
			cardodgeeasy();
			break;
		case 4:
			cardodgehard();
			break;
		case 5:
			snapGame();
		default:
			std::cin.clear();
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			break;
		}
		colour(lastcolour);
		return;
	}
	// prev
	else if (command == "prev" || command == "14") {
		colour(randnum(15, 1));
		std::cout << "Previous input: ";
		colour(4);
		std::cout << "'" << previnput << "'";
		colour(lastcolour);
		std::cout << ".\n";
		return;
	}
	// about
	else if (command == "about" || command == "abt" || command == "15") {
		cls();
		std::cout << "TerminalApp Gen 2-\n";
		colour(randnum(14, 1));
		slowcharfn(true, "The terminal PACKED with colours!");
		colour(randnum(14, 1));
		slowcharfn(false, "Creator: ");
		std::cout << "Ryan\n";
		sleep(500);
		colour(randnum(14, 1));
		slowcharfn(false, "Designers and Helpers: ");
		std::cout << "Ryan and Belal\n";
		sleep(500);
		colour(70);
		slowcharfn(true, "Massive thank-you to those who contributed to the project and gave feedback, such as my dad, school friends, ");
		slowcharfn(true, "and my dad's friends.\n");
		sleep(500);
		colour(3);
		slowcharfn(true, "This program was designed and tested on 2 different laptops, which are the Surface Book 3 and Dell Precision M4600.\nVirtual machines made in Hyper-V were also used to test this program at times.");
		sleep(500);
		std::cout << "A short message from the maker:\n";
		slowcharfn(true, "I hope you all like this program so far. I really enjoy making this program with my friends and family,\nand recieveing feedback that is helpful to the development of this program. I will hopefully learn more\nwith regards to programming while developing programs to make your experience with my programs a \nlot nicer.\nThanks guys!");
		colour(lastcolour);
		return;
	}
	// colourtxt
	else if (command[0] == 'c' && command[1] == 'o' && command[2] == 'l' && command[3] == 'o' && command[4] == 'u' && command[5] == 'r') {
	if (colcheck == true) {
		fail();
		colcheck = false;
		return;
	}
		if (command[6] != ' ' || command == "13") {
			coloption = 0;
			std::cout << "Text colour options:\n [1] Black\n [2] Blue\n [3] Green\n [4] Aqua\n [5] Red\n [6] Purple\n [7] Yellow\n [8] White\n [9] Grey\n[10] Light Blue\n"
				<< "[11] Light Green\n[12] Light Aqua\n[13] Light Red\n[14] Light Purple\n[15] Light Yellow\n[16] Bright White\n";
			do {
				std::cout << "Please input your option here (0 to exit): > ";
				colour(14);
				coloption = num("");
				colour(lastcolour);

				if (coloption <= 0) {
					std::cin.clear();
					std::cin.ignore();
					return;
				}

				while (std::cin.fail()) {
					std::cin.clear();
					std::cin.ignore(INT_MAX, '\n');
					std::cerr << "Please input a number from the list and try again.\n";
				}

				if (coloption > 16 || coloption < 1) {
					std::cin.clear();
					std::cerr << "Please input a number from the list and try again.\n";
				}
				else break;
			} while (true);

			coloption--;
			colour(coloption);
			lastcolour = coloption;
			// ansicol set to 0 to show that this is not an ansi colour
			ansicol = 0;
			std::cin.ignore();
			return;
		}
		else {
			short int coloption2;
			coloption = command[7];
			coloption -= 48;
			if (command[8] >= 48) {
				coloption2 = (command[8] - 48);
				coloption *= 10;
				coloption += coloption2;
			}
			if (coloption <= 0) {
				std::cin.clear();
				return;
			}
			if (coloption > 16 || coloption < 1) {
				std::cin.clear();
				std::cerr << "Please input a number from the list and try again.\n";
				return;
			}
			coloption--;
			colour(coloption);
			lastcolour = coloption;
			// ansicol set to 0 to show that this is not an ansi colour
			ansicol = 0;
			return;
		}

	}
	// cow
	else if (command == "cow" || command == "18") {
		// output cow made with chars
		std::cout << "\n  ^__^\n";
		std::cout << "  (OO)\_______     \n";
		std::cout << "  (__)\       )_/\\/\\\n";
		std::cout << "      ||----W |\n";
		std::cout << "      ||     ||\n";
		std::cout << '\n';
		std::cout << "Cow :)\n";
		return;
	}
	// cowsay
	else if (command == "cowsay" || command == "csay" || command == "19") {
		int numofchars;
		std::cout << "Please input what you want Henry the Cow to say: > ";
		colour(14);
		spare = str("");
		colour(lastcolour);
		for (numofchars = 0; spare[numofchars] != '\0'; numofchars++) {}
		for (int i = 0; i != (numofchars + 2); i++) { std::cout << "_"; }
		std::cout << "\n|" << spare << "|\n";
		for (int i = 0; i != (numofchars + 2); i++) { std::cout << "*"; }
		std::cout << "\n";
		std::cout << "    |\n";
		std::cout << "    |  ^__^\n";
		std::cout << "    -- (OO)\_______     \n";
		std::cout << "       (__)\       )_/\\/\\ \n";
		std::cout << "           ||----W |\n";
		std::cout << "           ||     ||\n";
		std::cout << "\nHenry's a good boy, even better than Alfred!\n";
		return;
	}
	// textedit
	else if (command == "note" || command == "21") {
		int innum = 1;
		// You can't use this with the FileParse feature.
		if (bIsRunFromFile == true) {
			std::cout << "Sorry, you cannot run this command from the FileParse feature.\n";
			return;
		}
		std::cout << "\nWelcome to Notes!\n\nYou can set notes and they will be saved permanently, unless if you delete the ";
		colour(3);
		std::cout << "Notes.txt";
		colour(lastcolour);
		std::cout << " file\nin the ";
		colour(11);
		std::cout << "TerminalApp_Temp";
		colour(lastcolour);
		std::cout << " folder.\n\nType ";
		colour(12);
		std::cout << "\"^exit\"";
		colour(lastcolour);
		std::cout << " without quotes to exit.\n\n";
		std::ofstream filee("TerminalApp_Temp\\Notes.txt");
		// loop
		do {
			// Ask for notes input
			std::cout << "Note ";
			colour(6);
			std::cout << innum;
			colour(lastcolour);
			std::cout << ": ";
			colour(14);
			getline(std::cin, text);
			colour(lastcolour);
			// Exit if ^exit was typed
			if (text == "^exit") {
				// close to save memory
				filee.close();
				std::cerr << "File saved. Leaving...\n";
				return;
			}
			filee << text << std::endl;
			// increment the number of notes taken indicator
			innum++;
		} while (true);
		return;
	}
	// restartsys
	else if (command == "restartsys" || command == "rsys" || command == "23") {
		std::cerr << "Are you sure you want to shut down your computer? [y/n] > ";
		colour(14);
		std::cin >> input;
		colour(lastcolour);

		while (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			return;
		}
		if (input == 'y') {
			// cmd restart command
			system("shutdown /r /t 0");
		}
		else {
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			return;
		}
	}
	// shutdownsys
	else if (command == "shutdownsys" || command == "ssys" || command == "22") {
		std::cerr << "Are you sure you want to shut down your computer? [y/n] > ";
		colour(14);
		std::cin >> input;
		colour(lastcolour);

		while (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			return;
		}
		if (input == 'y') {
			// cmd shutdown command
			system("shutdown /t 0");
		}
		else {
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			return;
		}
	}
	// randcol
	else if (command == "randcol" || command == "17") {
		lastcolour = randnum(255, 0);
		colour(lastcolour);
		std::cerr << "Random colour set." << std::endl;
	}
	// dundundun
	else if (command == "dundundun" || command == "24") {
		std::cout << "Outputting \"dundundun\" sound...";
		Beep(1568, 250);
		Beep(1568, 250);
		Beep(1568, 250);
		Beep(1245, 1250);
		Beep(1397, 250);
		Beep(1397, 250);
		Beep(1397, 250);
		Beep(1175, 1250);
		clearkeebbuf();
		return;
	}
	// saynote
	else if (command == "saynote" || command == "snote" || command == "25") {
		std::ifstream fileein("TerminalApp_Temp\\Notes.txt");
		// check if fileein stream is working
		if (fileein.fail()) {
			colour(12);
			std::cerr << "The Notes.txt file is unavailable for access.\n";
			colour(lastcolour);
			std::cerr << "Please make sure you have written any notes or you haven't deleted the ";
			colour(11);
			std::cerr << "Notes.txt";
			colour(lastcolour);
			std::cerr << " file.\n";
			fileein.close();
			return;
		}
		// output notes here
		colour(11);
		std::cout << "\nNotes:\n\n";
		colour(lastcolour);
		spare = "";
		for (int i = 1; std::getline(fileein, spare); i++) {
			std::cout << i << ": " << spare << std::endl;
		}
		// output that there are no more notes to show
		colour(12);
		std::cerr << "\nThere are no more notes to show.\n";
		colour(lastcolour);
		fileein.close();
		return;
	}
	// memtest
	else if (command == "memtest" || command == "26") {
		// variable 1
		long long int* memtest = new long long int;
		slowcharfn(true, "Welcome to MemTest.");
			std::cout << "Please enter a number: > ";
			colour(14);
			*memtest = num("");
			colour(lastcolour);
		// pointer variable 1
		long long int** pMemtest = new long long int*;
		// assign variable address to pointer
		pMemtest = &memtest;
		std::cout << "__Results__\n\nValue of number inputted: " << *memtest << "\nValue of pointer: " << **pMemtest << "\nAddress stored in pointer: " << pMemtest << "\n\n";
		std::cerr << "Returning memory to heap...\n";
		// Return memory to heap
		delete memtest;
		memtest = nullptr;
		delete* pMemtest;
		pMemtest = nullptr;
		// ignore other input before returning
		std::cin.ignore();
		return;
	}
	// ipinfo
	else if (command == "ipinfo" || command == "27") {
		system("ipconfig /all");
		return;
	}
	// cpustress
	else if (command == "cpustress" || command == "28") {
		std::cout << "Welcome to the CPU Stress test.\nThis is a simple single core stress test.\nPress any key in the middle of the stress test to exit.\n\nPress ENTER to begin stress test...";
		std::cin.clear();
		std::cin.get();

		// stress test part
		float num = 1.5;
		while (true) {
			// increment variable
			num *= num;
			// kbhit is used to see if a key was pressed
			if (_kbhit()) {
				colour(12);
				std::cerr << "Stress test terminated. Exiting...\n";
				colour(lastcolour);
				clearkeebbuf();
				return;
			}
		}
	}
	// Calculator
	else if (command == "calculator" || command == "calc" || command == "29") {
		do {
			std::cout << "\nCalculator Options (0 to exit):\n[1] Percentage Increase/Decrease\n[2] Simple Interest\n[3] Compound Interest\n[4] Power of 2 Calculator\n"
					<< "[5] Random Number Calculator\n[6] Dividing into Ratios Calculator";
			std::cout << "\nPlease input your option: > ";
			colour(14);
			inputnum = num("");
			colour(lastcolour);
			std::cout << '\n';
			// This time, it's a switch statement of course
			switch (inputnum) {
			case 0:
				std::cin.clear();
				std::cin.ignore(INT_MAX, '\n');
				return;
			case 1:
				percIncDecCalc();
				break;
			case 2:
				simpIntCalc();
				break;
			case 3:
				compIntCalc();
				break;
			case 4:
				powerOf2Calc();
				break;
			case 5:
				randNumcalc();
				break;
			case 6:
				ratioCalc();
				break;
			default:
				std::cin.clear();
				std::cin.ignore(INT_MAX, '\n');
				std::cerr << "Please input a valid option.\n";
				break;
			}
		} while (true);
	}
	// Converter
	else if (command == "converter" || command == "conv" || command == "30") {
		do {
			std::cout << "\nConverter Options (0 to exit):\n[1] Celsius to Fahrenheit\n[2] Fahrenheit to Celsius\n[3] Kilometres to Miles\n[4] Miles to Kilometres\n[5] Binary to Denary\n[6] Denary to Binary";
			std::cout << "\nPlease input your option: > ";
			colour(14);
			inputnum = num("");
			colour(lastcolour);
			std::cout << '\n';
			// This time, it's a switch statement of course
			switch (inputnum) {
			case 0:
				std::cin.clear();
				std::cin.ignore(INT_MAX, '\n');
				return;
			case 1:
				celsiusToFahrenheit();
				break;
			case 2:
				fahrenheitToCelsius();
				break;
			case 3:
				kilometresToMiles();
				break;
			case 4:
				milesToKilometres();
				break;
			case 5:
				binaryToDenary();
				break;
			case 6:
				denaryToBinary();
				break;
			default:
				std::cin.clear();
				std::cin.ignore(INT_MAX, '\n');
				std::cerr << "Please input a valid option.\n";
				break;
			}
		} while (true);
		std::cin.clear();
		std::cin.ignore(INT_MAX, '\n');
		return;
	}
	// Mario
	else if (command == "mario" || command == "31") {
		slowcolourrandfn("Mario theme song in beeps! Listen...");
		colour(lastcolour);
		std::cout << '\n';
		// This function has all the beep codes for the mario theme song
		mariobeep();
		// Clear keyboard buffer
		clearkeebbuf();
		return;
	}
	// Ansireset
	else if (command == "ansireset" || command == "32") {
		// Reset ansi indicator to 0
		ansicol = 0;
		std::cout << "ANSI colours have been reset.\n";
		return;
	}
	// explreset
	else if (command == "explreset" || command == "33") {
	colour(11);
	std::cerr << "Ok, Restarting Explorer.exe...\n";
	// System commands to restart explorer.exe
	system("taskkill /f /im explorer.exe && start explorer.exe");
	colour(lastcolour);
	return;
	}
	else if (command == "commandnum" || command == "cnum" || command == "34") {
	colour(randnum(15, 1));
	std::cout << "You have inputted ";
	colour(9);
	std::cout << nNumofUsrCommands;
	colour(nRandcolNum);
	std::cout << " commands since the start of this terminal.\n";
	colour(lastcolour);
	}
	else if (command == "cry" || command == "35") {
		// Here's a small cry storyline
		colour(34052);
		std::cout << "Cry- A short story by Ryan (the developer):\n\n";
		colour(lastcolour);
		sleep(1000);
		colourtxt("*Story starts*\n\n");
		sleep(500);
		colour(11);
		std::cout << "Computer: ";
		colour(lastcolour);
		sleep(500);
		slowcharfn(true, "Aight, I'm hungry. I want a cookie!");
		std::cout << "          ";
		slowcharfn(true, "But where could I find one? ");
		sleep(750);
		colour(11);
		std::cout << "Web Server: ";
		colour(lastcolour);
		sleep(500);
		slowcharfn(true, "*begins talking to other computers and giving them cookies*");
		sleep(750);
		slowcharfn(false, "(The computer now knows where to get cookies. The computer walks up to the web server and says:)");
		sleep(750);
		colour(11);
		std::cout << "\nComputer: ";
		colour(lastcolour);
		sleep(500);
		slowcharfn(true, "Hey web server, could I please have a cookie-");
		sleep(500);
		colour(11);
		std::cout << "Web Server: ";
		colour(lastcolour);
		sleep(500);
		slowcharfn(false, "Sorry, your request could not be processed. \n");
		std::cout << "            ";
		slowcharfn(false, "Error info: \"We're out of cookies, dude. Come back later.\"");
		sleep(750);
		colour(11);
		std::cout << "\nComputer: \n";
		colour(lastcolour);
		sleep(800);
		for (short int i = 1; i < 16; i++) {
			colour(i);
			std::cout << "WAAAAAAAAAAAAAAAAAAAAHAHAHHAHAHAAAAAAAAAAAAAAAAAAHA\n";
			sleep(50);
		}
		sleep(500);
		colourtxt("\n*Story done*\n");
		return;
	}
	else if (command == "stopwatch" || command == "stopw" || command == "36") {
	long double time = 0;
	std::cout << "This is a simple stopwatch which measures time in seconds.\nPress any key while the stopwatch is running to exit it.\n";
	colour(11);
	std::cout << "Timing...\n\n";
	CursorPosition = GetConsoleCursorPosition();
	std::cout << "Time: ";
	colour(lastcolour);
	while (!_kbhit()) {
		gotoxy(6, CursorPosition.Y);
		std::cout << "               ";
		gotoxy(6, CursorPosition.Y);
		std::cout << time;
		sleep(500);
		time = time + 0.5;
	}
	clearkeebbuf();
	std::cout << '\n';
	return;
	}
	else if (command == "simplecalc" || command == "scalc" || command == "simpcalc" || command == "37") {
	long double		nNum1 = 0.0;
	long double		nNum2 = 0.0;
	char			cMethod;
	long double		nAns = 0.0;
	slowcharfn(true, "Welcome to a Simple Calculator!");
	std::cout << "Accepted operators are: +,-,*,/\n\n";
	nNum1 = num("Please input the first number: > ");
	std::cout << "Please input your operator: > ";
	std::cin >> cMethod;
	if (nNum1 == 0.0 && cMethod == '/') {
		std::cout << "Sorry, you cannot use a 0 as your first number when dividing (divide by 0 error). Please retry later.\n";
		return;
	}
	nNum2 = num("Please input the second number: > ");

	std::cin.ignore(INT_MAX, '\n');
	switch (cMethod) {
	case '+':
		nAns = nNum1 + nNum2;
		break;
	case '-':
		nAns = nNum1 - nNum2;
		break;
	case '*':
		nAns = nNum1 * nNum2;
		break;
	case 'x':
		nAns = nNum1 * nNum2;
		break;
	case '/':
		nAns = nNum1 / nNum2;
		break;

	default:
		std::cout << "Sorry, an error occured. Please input one of the correct operators mentioned.\n";
		break;
	}
	std::cout << "Answer: [";
	colour(3);
	std::cout << nAns;
	colour(lastcolour);
	std::cout << "].\n";
	return;
	}
	// fileparse
	else if (command == "fileparse" || command == "filemode" || command == "file" || command == "39") {
	std::string sFileParse;

	// Check if program is already in FileParse mode
	if (bIsRunFromFile == true) {
		std::cerr << "You are already running from FileParse and you do not need any further action of enabling it.\n";
		return;
	}
	while (true) {
		std::cout << "Please input the exact filepath of the file you want to run from (0 to exit): > ";
		colour(14);
		getline(std::cin, sFileParse);
		colour(lastcolour);
		if (sFileParse == "0") return;

		// Test if the file is legit
		std::ifstream FileParseTest(sFileParse);
		if (FileParseTest.fail() == true) {
			colour(4);
			std::cerr << "Sorry, an error occured while opening the file.\nYou probably put the wrong filename into the program. Please try again.\n\n";
			colour(lastcolour);
			continue;
		}
		else {
			std::cout << "File successfully opened!\nStarting up fileParse...";
			break;
		}
	}
	// Start up the feature by calling its associated function
	cls();
	bIsRunFromFile = true;
	if (fileParseFeatureFn(sFileParse) == true) {
		colour(2);
		std::cout << "\nFile execution Success!\nFile used: <";
		colour(11);
		std::cout << sFileParse;
		colour(2);
		std::cout << ">\n";
	}
	else {
		std::cout << "\nFile execution Failed!\nAn error occured while running the file. File used: <";
		colour(11);
		std::cout << sFileParse;
		colour(lastcolour);
		std::cout << ">\n";
	}
	return;
	}
	else if (command == "pause" || command == "40") {
	std::cout << "Press any key to exit PAUSE command...";
	_getch();
	std::cout << '\n';
	return;
	}
	else if (command == "colreset" || command == "43") {
	// Reset colours to default
	colour(DEFAULT_COL);
	return;
	}
	// edison
	else if (command == "edison" || command == "38") {
	short int		edison;

	// Randomly pick number
	edison = randnum(50, 1);
	std::cout << "\n\"";

	switch (edison) {
	case 1:
		std::cout << "Our greatest weakness lies in giving up. The most certain way to succeed is always to try just one more time.\n";
		break;
	case 2:
		std::cout << "I have not failed. I’ve just found 10,000 ways that won’t work.\n";
		break;
	case 3:
		std::cout << "Having a vision for what you want is not enough… Vision without execution is hallucination.\n";
		break;
	case 4:
		std::cout << "Opportunity is missed by most people because it is dressed in overalls and looks like work.\n";
		break;
	case 5:
		std::cout << "There is a better way for everything. Find it.\n";
		break;
	case 6:
		std::cout << "The successful person makes a habit of doing what the failing person doesn’t like to do.\n";
		break;
	case 7:
		std::cout << "Success is 90% perspiration and 10% inspiration.\n";
		break;
	case 8:
		std::cout << "I never view mistakes as failures. They are simply opportunities to find out what doesn’t work.\n";
		break;
	case 9:
		std::cout << "Tomorrow is my exam but I don’t care because a single sheet of paper can’t decide my future.\n";
		break;
	case 10:
		std::cout << "Never go to sleep without a request to your subconscious.\n";
		break;
	case 11:
		std::cout << "Your worth consists in what you are and not in what you have.\n";
		break;
	case 12:
		std::cout << "The greatest invention in the world is the mind of a child.\n";
		break;
	case 13:
		std::cout << "Each time you fail, you have eliminated another wrong option.\n";
		break;
	case 14:
		std::cout << "People are not remembered by how few times they fail, but by how often they succeed. Every wrong step is another step forward.\n";
		break;
	case 15:
		std::cout << "The value of an idea lies in the using of it.\n";
		break;
	case 16:
		std::cout << "Time is really the only capital that any human being has, and the only thing he can’t afford to lose.\n";
		break;
	case 17:
		std::cout << "Restlessness is discontent and discontent is the first necessity of progress. Show me a thoroughly satisfied man and I will show you a failure.\n";
		break;
	case 18:
		std::cout << "What you are will show in what you do.\n";
		break;
	case 19:
		std::cout << "Do the thing and the power will come.\n";
		break;
	case 20:
		std::cout << "I find out what the world needs. Then I go ahead and try to invent it.\n";
		break;
	case 21:
		std::cout << "Every failure is a lesson learned about your strategy.\n";
		break;
	case 22:
		std::cout << "Before you reject an idea, find at least five good things about it.\n";
		break;
	case 23:
		std::cout << "Absorb ideas from every source.\n";
		break;
	case 24:
		std::cout << "A good intention, with a bad approach, often leads to a poor result.\n";
		break;
	case 25:
		std::cout << "There seems to be no limit to which some men will go to avoid the labor of thinking. Thinking is hard work.\n";
		break;
	case 26:
		std::cout << "When you have exhausted all possibilities, remember this – you haven’t.\n";
		break;
	case 27:
		std::cout << "Many of life’s failures are people who did not realize how close they were to success when they gave up.\n";
		break;
	case 28:
		std::cout << "I owe my success to the fact that I never had a clock in my workroom.\n";
		break;
	case 29:
		std::cout << "I have far more respect for the person with a single idea who gets there than for the person with a thousand ideas who does nothing.\n";
		break;
	case 30:
		std::cout << "If we did all the things we are capable of doing, we would literally astound ourselves.\n";
		break;
	case 31:
		std::cout << "Continued innovation is the best way to beat the competition.\n";
		break;
	case 32:
		std::cout << "Good fortune is what happens when opportunity meets with planning.\n";
		break;
	case 33:
		std::cout << "I didn’t fail 1000 times. The light bulb was an invention with 1000 steps.\n";
		break;
	case 34:
		std::cout << "You can’t realize your dreams unless you have one to begin with.\n";
		break;
	case 35:
		std::cout << "To get a great idea, come up with lots of them.\n";
		break;
	case 36:
		std::cout << "The trouble with most people is that they quit before they start.\n";
		break;
	case 37:
		std::cout << "The doctor of the future will no longer treat the human frame with drugs, but rather will cure and prevent disease with nutrition.\n";
		break;
	case 38:
		std::cout << "A teacher sent the following note home with a six-year-old boy ‘He is too stupid to learn.’ That boy was Thomas A. Edison.\n";
		break;
	case 39:
		std::cout << "I never perfected an invention that I did not think about in terms of the service it might give others… I find out what the world needs, then I proceed to invent.\n";
		break;
	case 40:
		std::cout << "Fools call wise men fools. A wise man never calls any man a fool.\n";
		break;
	case 41:
		std::cout << "I failed my way to success.\n";
		break;
	case 42:
		std::cout << "Most of my ideas belonged to other people who never bothered to develop them.\n";
		break;
	case 43:
		std::cout << "The strength of the constitution lies in the will of the people to defend it.\n";
		break;
	case 44:
		std::cout << "I’d put my money on the sun and solar energy. What a source of power! I hope we don’t have to wait until oil and coal run out before we tackle that.\n";
		break;
	case 45:
		std::cout << "Non-violence leads to the highest ethics, which is the goal of all evolution. Until we stop harming all other living beings, we are still savages.\n";
		break;
	case 46:
		std::cout << "The most necessary task of civilization is to teach people how to think. It should be the primary purpose of our public schools. The mind of a child is naturally active, it develops through exercise. Give a child plenty of exercise, for body and brain. The trouble with our way of educating is that it does not give elasticity to the mind. It casts the brain into a mold. It insists that the child must accept. It does not encourage original thought or reasoning, and it lays more stress on memory than observation.\n";
		break;
	case 47:
		std::cout << "To do much clear thinking a person must arrange for regular periods of solitude when they can concentrate and indulge the imagination without distraction.\n";
		break;
	case 48:
		std::cout << "The world owes nothing to any man, but every man owes something to the world.\n";
		break;
	case 49:
		std::cout << "It’s obvious that we don’t know one millionth of one percent about anything.\n";
		break;
	case 50:
		std::cout << "To invent, you need a good imagination and a pile of junk.\n";
		break;
	default:
		std::cerr << "Oops, sorry! An error occured with this command/module.\n";
		break;
	}
	// Append 'Thomas Edison' to the end
	std::cout << "\"";
	colour(randnum(15, 1));
	std::cout << "\n-Thomas Edison\n";
	std::cout << '\n';
	colour(lastcolour);
	return;
	}
	// einstein
	else if (command == "einstein" || command == "20") {
		short int		einstein;

		// Randomly pick number for choosing random quote
		einstein = randnum(50, 0);
		std::cout << "\n\"";
		// Quotes here:
		switch (einstein) {
		case 1:
			std::cout << "Two things are infinite: the universe and human stupidity; and I'm not sure about the universe.";
			break;
		case 2:
			std::cout << "There are only two ways to live your life. One is as though nothing is a miracle. The other is as though everything is a miracle.";
			break;
		case 3:
			std::cout << "I am enough of an artist to draw freely upon my imagination. Imagination is more important than knowledge. Knowledge is limited. Imagination encircles the world.";
			break;
		case 4:
			std::cout << "If you can't explain it to a six year old, you don't understand it yourself.";
			break;
		case 5:
			std::cout << "If you want your children to be intelligent, read them fairy tales. If you want them to be more intelligent, read them more fairy tales.";
			break;
		case 6:
			std::cout << "Logic will get you from A to Z; imagination will get you everywhere.";
			break;
		case 7:
			std::cout << "Life is like riding a bicycle. To keep your balance, you must keep moving.";
			break;
		case 8:
			std::cout << "Anyone who has never made a mistake has never tried anything new.";
			break;
		case 9:
			std::cout << "I speak to everyone in the same way, whether he is the garbage man or the president of the university.";
			break;
		case 10:
			std::cout << "Never memorize something that you can look up.";
			break;
		case 11:
			std::cout << "When you are courting a nice girl an hour seems like a second. When you sit on a red-hot cinder a second seems like an hour. That's relativity.";
			break;
		case 12:
			std::cout << "A clever person solves a problem. A wise person avoids it.";
			break;
		case 13:
			std::cout << "Science without religion is lame, religion without science is blind.";
			break;
		case 14:
			std::cout << "Reality is merely an illusion, albeit a very persistent one.";
			break;
		case 15:
			std::cout << "Any fool can know. The point is to understand.";
			break;
		case 16:
			std::cout << "If we knew what it was we were doing, it would not be called research, would it?";
			break;
		case 17:
			std::cout << "I have no special talents. I am only passionately curious.";
			break;
		case 18:
			std::cout << "A human being is a part of the whole called by us universe, a part limited in time and space. He experiences himself, his thoughts and feeling as something separated from the rest, a kind of optical delusion of his consciousness. This delusion is a kind of prison for us, restricting us to our personal desires and to affection for a few persons nearest to us. Our task must be to free ourselves from this prison by widening our circle of compassion to embrace all living creatures and the whole of nature in its beauty.";
			break;
		case 19:
			std::cout << "Try not to become a man of success. Rather become a man of value.";
			break;
		case 20:
			std::cout << "The important thing is not to stop questioning. Curiosity has its own reason for existence. One cannot help but be in awe when he contemplates the mysteries of eternity, of life, of the marvelous structure of reality. It is enough if one tries merely to comprehend a little of this mystery each day.";
			break;
		case 21:
			std::cout << "The world as we have created it is a process of our thinking. It cannot be changed without changing our thinking.";
			break;
		case 22:
			std::cout << "Once you can accept the universe as matter expanding into nothing that is something, wearing stripes with plaid comes easy.";
			break;
		case 23:
			std::cout << "If I were not a physicist, I would probably be a musician. I often think in music. I live my daydreams in music. I see my life in terms of music.";
			break;
		case 24:
			std::cout << "The most beautiful experience we can have is the mysterious. It is the fundamental emotion that stands at the cradle of true art and true science.";
			break;
		case 25:
			std::cout << "I know not with what weapons World War III will be fought, but World War IV will be fought with sticks and stones.";
			break;
		case 26:
			std::cout << "You never fail until you stop trying.";
			break;
		case 27:
			std::cout << "Great spirits have always encountered violent opposition from mediocre minds.";
			break;
		case 28:
			std::cout << "The measure of intelligence is the ability to change.";
			break;
		case 29:
			std::cout << "Gravitation is not responsible for people falling in love.";
			break;
		case 30:
			std::cout << "It is not that I'm so smart. But I stay with the questions much longer.";
			break;
		case 31:
			std::cout << "Creativity is intelligence having fun.";
			break;
		case 32:
			std::cout << "The world is a dangerous place to live, not because of the people who are evil, but because of the people who don't do anything about it.";
			break;
		case 33:
			std::cout << "If A is a success in life, then A equals x plus y plus z. Work is x; y is play; and z is keeping your mouth shut.";
			break;
		case 34:
			std::cout << "Black holes are where God divided by zero.";
			break;
		case 35:
			std::cout << "Everything must be made as simple as possible. But not simpler.";
			break;
		case 36:
			std::cout << "The best way to cheer yourself is to cheer somebody else up.";
			break;
		case 37:
			std::cout << "When you trip over love, it is easy to get up. But when you fall in love, it is impossible to stand again.";
			break;
		case 38:
			std::cout << "Whoever is careless with the truth in small matters cannot be trusted with important matters.";
			break;
		case 39:
			std::cout << "What is right is not always popular and what is popular is not always right.";
			break;
		case 40:
			std::cout << "Peace cannot be kept by force; it can only be achieved by understanding.";
			break;
		case 41:
			std::cout << "In the middle of difficulty lies opportunity.";
			break;
		case 42:
			std::cout << "A question that sometimes drives me hazy: am I or are the others crazy?";
			break;
		case 43:
			std::cout << "Imagination is everything. It is the preview of life's coming attractions.";
			break;
		case 44:
			std::cout << "The pursuit of truth and beauty is a sphere of activity in which we are permitted to remain children all our lives.";
			break;
		case 45:
			std::cout << "We dance for laughter, we dance for tears, we dance for madness, we dance for fears, we dance for hopes, we dance for screams, we are the dancers, we create the dreams.";
			break;
		case 46:
			std::cout << "The woman who follows the crowd will usually go no further than the crowd. The woman who walks alone is likely to find herself in places no one has ever been before.";
			break;
		case 47:
			std::cout << "Time is an illusion.";
			break;
		case 48:
			std::cout << "It would be possible to describe everything scientifically, but it would make no sense; it would be without meaning, as if you described a Beethoven symphony as a variation of wave pressure.";
			break;
		case 49:
			std::cout << "I must be willing to give up what I am in order to become what I will be.";
			break;
		case 50:
			std::cout << "Love is a better master than duty.";
			break;
		default:
			std::cerr << "Oops, sorry! An error occured with this command/module.\n";
		}
		// Append 'Albert Einstein' to the end
		std::cout << "\"";
		colour(randnum(15, 1));
		std::cout << "\n-Albert Einstein\n";
		std::cout << '\n';
		colour(lastcolour);
		return;
	}
	else if (command == "disp" || command == "nodisp" || command == "41" || command == "42") return;
	// incorrect input part
	else {
		fail();
		return;
	}
	return;
}

// Main
int main(int nArgsNum, char* pszArgs[]) {
	// Create fstream streams
	std::ifstream IsNew("TerminalApp_Temp\\IsNew.confi");
	// Activate the FileParse feature/method if requested to do so (through command-line arguments)
	if (nArgsNum > 1) {
		bIsRunFromFile = true;
		if (fileParseFeatureFn(pszArgs[1]) == true) {
			colour(randnum(15, 1));
			std::cerr << "\nProgram execution success!\nClosing in ";
			CursorPosition = GetConsoleCursorPosition();
			short int nCounter = 3;
			colour(4);
			while (nCounter > 0) {
				gotoxy(11, CursorPosition.Y);
				std::cerr << nCounter << "...";
				sleep(1000);
				nCounter--;
			}
		}
		else {
			colour(randnum(15, 1));
			std::cerr << "\nProgram execution failed!\nClosing in ";
			CursorPosition = GetConsoleCursorPosition();
			short int nCounter = 3;
			colour(4);
			while (nCounter > 0) {
				gotoxy(11, CursorPosition.Y);
				std::cerr << nCounter << "...";
				sleep(1000);
				nCounter--;
			}
		}
		return 0;
	}
	colour(randnum(15, 1));
	slowcharfn(true, "Welcome to TerminalApp Gen 2!");
	colourtxt("(Now with colours!)");
	std::cout << std::endl;
	// If IsNew failed to open, start the tutorial
	if (IsNew.fail()) {
		sleep(500);
		IsNew.close();
		colour(7);
		slowcharfn(true, "It looks like you haven't used this application before.");
		std::cout << "Would you like a tutorial? [y/n] > ";
		colour(14);
		std::cin >> input;
		colour(7);
		std::cin.ignore();
		if (input == 'y') {
			tutorial(false);
		}
		else {
			// Create a new folder/directory
			CreateDirectoryA("TerminalApp_Temp", 0);
			// Create a file and write to it to say that the tutorial has been completed.
			std::ofstream IsOld("TerminalApp_Temp\\IsNew.confi");
			IsOld << "TutCompletedEnough";
			IsOld.close();
			std::cout << "Ok, get ready!\n";
		}
	}
	IsNew.close();
	sleep(100);
	colour(randnum(15, 1));
	std::cout << "Press ENTER to begin...\n";
	std::cin.get();
	colour(2);
	std::cout << "[Type ";
	colour(12);
	std::cout << "'help'";
	colour(2);
	std::cout << " or ";
	colour(12);
	std::cout << "'0'";
	colour(2);
	std::cout << " without quotes to see all commands and what they do.]\n";
	colour(7);
	clearkeebbuf();
	bool nodisp = false;
	do {
		if (command == "nodisp" || command == "41") { 
			if (nodisp == true) {
				colour(randnum(15, 1));
				std::cout << "NODISP is already on.\n";
			}
			nodisp = true; 
		} else if (command == "disp" || command == "42") { 
			if (nodisp == false) {
				colour(randnum(15, 1));
				std::cout << "DISP is already on.\n";
			}
			nodisp = false; 
		}

		colour(lastcolour);
		if (nodisp == false) {
			std::cout << "Command: > ";
		}
		if (lastcolour < 16) {
			colour(14);
		}

		// Ask for command input
		command[6] = ' ';
		getline(std::cin, command);
		if (command == "") continue;
		colour(lastcolour);

		// If command doesn't equal echo, convert all characters to lowercase, else convert only the echo parts of the command.
		if (command[0] != 'e' && command[1] != 'c' && command[2] != 'h' && command[3] != 'o') {
			for (int i = 0; i < command.length(); i++) { command[i] = tolower(command[i]); }
		}
		else for (int i = 0; i < 5; i++) { command[i] = tolower(command[i]); }

		// Colour command check
		if (command != "colour" && command[6] != ' ') colcheck = true; else colcheck = false;
		// If command equals exit, return 0
		if (command == "exit" || command == "2" || command == "goodbye") {
			colour(randnum(15, 1));
			slowcharfn(false, "Bye-bye!");
			sleep(100);
			return 0;
		}

		// Only call the command function if command has something in it, else go back to the input statement above
		commands();

		previnput = command;
	} while (true);
	return 0;
}

bool fileParseFeatureFn(std::string sFileName) {
	bool bSuccess = true;
	bool nodisp = false;
	sFileParseName = sFileName;
	bIsRunFromFile = true;

	// Create fileParse stream
	std::ifstream fileParseReadIn(sFileName);

	while (!fileParseReadIn.eof()) {
		// Check if the file is valid
		if (fileParseReadIn.good() != true) {
			bSuccess = false;
			break;
		}
		// If file is valid, begin read from file to command buffer
		command[6] = ' ';
		getline(fileParseReadIn, command);
		if (command == "nodisp" || command == "41") {
			if (nodisp == true) {
				std::cout << "NODISP is already on.\n";
			}
			nodisp = true;
			nNumOfFileParseLines++;
			continue;
		}
		else if (command == "disp" || command == "42") {
			if (nodisp == false) {
				std::cout << "DISP is already on.\n";
			}
			nodisp = false;
			nNumOfFileParseLines++;
			continue;
		}

		// Check if 'nodisp' file command is present
		if (nodisp == false) {
			std::cout << "Command: > ";
			if (lastcolour < 16) {
				colour(14);
			}
			std::cout << command << '\n';
		}
		colour(lastcolour);

		// If command doesn't equal echo, convert all characters to lowercase, else convert only the echo parts of the command.
		if (command[0] != 'e' && command[1] != 'c' && command[2] != 'h' && command[3] != 'o') {
			for (int i = 0; i < command.length(); i++) { command[i] = tolower(command[i]); }
		}
		else for (int i = 0; i < 5; i++) { command[i] = tolower(command[i]); }

		// Colour command check
		if (command != "colour" && command[6] != ' ') colcheck = true; else colcheck = false;
		// If command equals exit, return 0
		if (command == "exit" || command == "2" || command == "goodbye") {
			colour(randnum(15, 1));
			slowcharfn(false, "Bye-bye!");
			sleep(100);
			return true;
		}

		// Only call the command function if command has something in it, else go back to the input statement above
		if (command != "") commands();

		previnput = command;
		nNumOfFileParseLines++;
	}

	return bSuccess;
}

void mariobeep() {
	Beep(659, 125);
	Beep(659, 125);
	sleep(125);
	Beep(659, 125);
	sleep(167);
	Beep(523, 125);
	Beep(659, 125);
	sleep(125);
	Beep(784, 125);
	sleep(375);
	Beep(392, 125);
	sleep(375);
	Beep(523, 125);
	sleep(250);
	Beep(392, 125);
	sleep(250);
	Beep(330, 125);
	sleep(250);
	Beep(440, 125);
	sleep(125);
	Beep(494, 125);
	sleep(125);
	Beep(466, 125);
	sleep(42);
	Beep(440, 125);
	sleep(125);
	Beep(392, 125);
	sleep(125);
	Beep(659, 125);
	sleep(125);
	Beep(784, 125);
	sleep(125);
	Beep(880, 125);
	sleep(125);
	Beep(698, 125);
	Beep(784, 125);
	sleep(125);
	Beep(659, 125);
	sleep(125);
	Beep(523, 125);
	sleep(125);
	Beep(587, 125);
	Beep(494, 125);
	sleep(125);
	Beep(523, 125);
	sleep(250);
	Beep(392, 125);
	sleep(250);
	Beep(330, 125);
	sleep(250);
	Beep(440, 125);
	sleep(125);
	Beep(494, 125);
	sleep(125);
	Beep(466, 125);
	sleep(42);
	Beep(440, 125);
	sleep(125);
	Beep(392, 125);
	sleep(125);
	Beep(659, 125);
	sleep(125);
	Beep(784, 125);
	sleep(125);
	Beep(880, 125);
	sleep(125);
	Beep(698, 125);
	Beep(784, 125);
	sleep(125);
	Beep(659, 125);
	sleep(125);
	Beep(523, 125);
	sleep(125);
	Beep(587, 125);
	Beep(494, 125);
	sleep(375);
	Beep(784, 125);
	Beep(740, 125);
	Beep(698, 125);
	sleep(42);
	Beep(622, 125);
	sleep(125);
	Beep(659, 125);
	sleep(167);
	Beep(415, 125);
	Beep(440, 125);
	Beep(523, 125);
	sleep(125);
	Beep(440, 125);
	Beep(523, 125);
	Beep(587, 125);
	sleep(250);
	Beep(784, 125);
	Beep(740, 125);
	Beep(698, 125);
	sleep(42);
	Beep(622, 125);
	sleep(125);
	Beep(659, 125);
	sleep(167);
	Beep(698, 125);
	sleep(125);
	Beep(698, 125);
	Beep(698, 125);
	sleep(625);
	Beep(784, 125);
	Beep(740, 125);
	Beep(698, 125);
	sleep(42);
	Beep(622, 125);
	sleep(125);
	Beep(659, 125);
	sleep(167);
	Beep(415, 125);
	Beep(440, 125);
	Beep(523, 125);
	sleep(125);
	Beep(440, 125);
	Beep(523, 125);
	Beep(587, 125);
	sleep(250);
	Beep(622, 125);
	sleep(250);
	Beep(587, 125);
	sleep(250);
	Beep(523, 125);
	sleep(1125);
	Beep(784, 125);
	Beep(740, 125);
	Beep(698, 125);
	sleep(42);
	Beep(622, 125);
	sleep(125);
	Beep(659, 125);
	sleep(167);
	Beep(415, 125);
	Beep(440, 125);
	Beep(523, 125);
	sleep(125);
	Beep(440, 125);
	Beep(523, 125);
	Beep(587, 125);
	sleep(250);
	Beep(784, 125);
	Beep(740, 125);
	Beep(698, 125);
	sleep(42);
	Beep(622, 125);
	sleep(125);
	Beep(659, 125);
	sleep(167);
	Beep(698, 125);
	sleep(125);
	Beep(698, 125);
	Beep(698, 125);
	sleep(625);
	Beep(784, 125);
	Beep(740, 125);
	Beep(698, 125);
	sleep(42);
	Beep(622, 125);
	sleep(125);
	Beep(659, 125);
	sleep(167);
	Beep(415, 125);
	Beep(440, 125);
	Beep(523, 125);
	sleep(125);
	Beep(440, 125);
	Beep(523, 125);
	Beep(587, 125);
	sleep(250);
	Beep(622, 125);
	sleep(250);
	Beep(587, 125);
	sleep(250);
	Beep(523, 125);
	sleep(625);
}