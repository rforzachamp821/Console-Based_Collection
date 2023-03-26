
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
#include <random>
#include <limits>

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
std::string		sRamTemp[100];
std::string		sCommandRAW;

unsigned long long int nNumofUsrCommands = 0;

long long int	hertz;
long long int	milsecs;
long long int	letters = 1;
long long int	words = 1;
long long int	chars = 1;
long long int	nNumOfFileParseLines = 1;

int				coloption;
int				nLastColour = 7; // 7 because hex number 7 is white
int				inputnum = 0;

short int		ansicol = 0;
short int		nIterationNum = 0;

char			input;

bool			colcheck = false;
bool			bIsRunFromFile = false;
bool			bTempNotesPresent = false;

// Game variables
std::string		player1Name;
std::string		player2Name;

long long int	guessnum;
long long int	rightnum;
long long int	score;

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

/*--------------------PROGRAM PLATFORM CODE BEGIN-----------------------*/
void slowcharfn(bool, std::string);
void VerbosityDisplay(std::string);

class Config {
protected:
	// Function to create a new configuration file
	bool bCreateConfigFile() {
		// Open stream to create config file
		std::ofstream ConfigMake("tag2_config.ini");
		// Format file with default values
		ConfigMake << "ScoreCarDodgeEasy=0\nScoreCarDodgeHard=0\nScoreGuessHard=0\nDefaultColour=7\nVerbosityToggle=0\nSlowCharSpeed=45\nStartupRandomColour=0";

		// Check if ConfigMake failed
		if (ConfigMake.fail()) {
			ConfigMake.close();
			return false;
		}
		else return true;

		return true;
	}

	// Function to update config file with new scores from Config object
	bool bObjectToFileUpdater(Config& HighScoreUpdate) {
		// Open stream to edit config file
		std::ofstream ConfigEdit("tag2_config.ini");
		// Check if anything wrong happened when opening file
		if (ConfigEdit.fail()) {
			std::cout << "Sorry, something wrong happened when writing to file.\nPlease check if config file is existing in the same folder as the program itself, and try again.\n";
			return false;
		}

		// Write to file 
		ConfigEdit << "ScoreCarDodgeEasy=" << HighScoreUpdate.lnHighScoreCarEasy
			<< "\nScoreCarDodgeHard=" << HighScoreUpdate.lnHighScoreCarHard
			<< "\nScoreGuessHard=" << HighScoreUpdate.lnHighScoreGuess
			<< "\nDefaultColour=" << HighScoreUpdate.nDefaultColour
			<< "\nVerbosityToggle=" << HighScoreUpdate.bVerbosityToggle
			<< "\nSlowCharSpeed=" << HighScoreUpdate.nSlowCharSpeed
			<< "\nStartupRandomColour=" << HighScoreUpdate.bStartupRandomColourToggle;
		ConfigEdit.close();
		// Check if ConfigMake failed
		if (ConfigEdit.fail()) {
			std::cout << "Sorry, something unexpected happened when writing to the file.\nMaybe the file got deleted in the middle of the write operation?\n";
			return false;
		}
		else return true;

		return true;
	}
	// Function to update object contents from configuration file
	bool bFileToObjectUpdater(Config& HighScoreUpdate) {
		// Declare variables
		std::string sStrBuf, sIdStr;
		int nIntBuf, nLength;


		// Check if file wasn't found; if not, repeat creating file until it is found
		while (true) {
			// Test to see if file is there
			std::ifstream ConfigTry1("tag2_config.ini");
			if (ConfigTry1.fail()) {
				// Create file with formatting
				VerbosityDisplay("File not found. Creating configuration file...\n");
				bCreateConfigFile();
			}
			else {
				ConfigTry1.close();
				break;
			}
			ConfigTry1.close();
		}
		// Create another new stream
		std::ifstream ConfigIn("tag2_config.ini");

		while (!ConfigIn.eof()) {
			// 1. Take in first line using getline()
			getline(ConfigIn, sStrBuf, '\n');

			// Error handling early
			bool bPresentEqual = true;
			for (int i = 0; i < sStrBuf.length(); i++) {
				if (sStrBuf[i] != '=') {
					bPresentEqual = false;
				}
				else {
					bPresentEqual = true;
					break;
				}
			}
			// Check if there's an equal sign
			if (bPresentEqual == false) {
				VerbosityDisplay("A line has been detected without correct formatting.\nPlease make sure there is an identifier on the left, an equal sign after, and an integer on the right.\n");
				continue;
			}

			// 2. Create stringstream, then use getline() to read to '=' delim
			std::stringstream ssStrBuf;
			ssStrBuf << sStrBuf;
			getline(ssStrBuf, sIdStr, '=');

			// 4. Extract number from ssStrBuf, and put it into RAM buffer
			getline(ssStrBuf, sStrBuf);
			sStrBuf.erase(std::remove_if(sStrBuf.begin(), sStrBuf.end(), [](unsigned char c) { return !std::isdigit(c); }), sStrBuf.end());

			// 5. Use stringstream conversion feature to convert string into integer
			// Get the strbuf sstream ready
			ssStrBuf.str("");
			ssStrBuf.clear();
			// convert
			ssStrBuf << sStrBuf;
			ssStrBuf >> nIntBuf;

			// 6. Use if statement to determine where number in buffer will go in class members
			if (sIdStr == "ScoreCarDodgeEasy") {
				HighScoreUpdate.lnHighScoreCarEasy = nIntBuf;
			}
			else if (sIdStr == "ScoreCarDodgeHard") {
				HighScoreUpdate.lnHighScoreCarHard = nIntBuf;
			}
			else if (sIdStr == "ScoreGuessHard") {
				HighScoreUpdate.lnHighScoreGuess = nIntBuf;
			}
			else if (sIdStr == "DefaultColour") {
				HighScoreUpdate.nDefaultColour = nIntBuf;
			}
			else if (sIdStr == "VerbosityToggle") {
				HighScoreUpdate.bVerbosityToggle = nIntBuf;
			}
			else if (sIdStr == "SlowCharSpeed") {
				HighScoreUpdate.nSlowCharSpeed = nIntBuf;
			}
			else if (sIdStr == "StartupRandomColour") {
				HighScoreUpdate.bStartupRandomColourToggle = nIntBuf;
			}
			else {
				std::cerr << "Sorry, the current listing called <" << sIdStr << "> was not identified. This program will not be using that line.\n";
			}
			// 7. Loop to beginning until eof() posts true
		}
		return true;
	}

public:
	// Config constructor
	Config() {
		// Initialise Object Variables
		lnHighScoreCarEasy, lnHighScoreCarHard, lnHighScoreGuess = 0;
		nDefaultColour = 7;
		bVerbosityToggle = false;
		nSlowCharSpeed = 45;
		bStartupRandomColourToggle = false;

		// Post that a new object has been created
		VerbosityDisplay("Config object created.\n");
	}

	long long int	lnHighScoreGuess = 0;
	long long int	lnHighScoreCarHard = 0;
	long long int	lnHighScoreCarEasy = 0;
	int				nDefaultColour = 7; // 7 (white) is the default colour on startup OOTB
	bool			bVerbosityToggle = false;
	int				nSlowCharSpeed = 45;
	bool			bStartupRandomColourToggle = false;

	// 'token' to access Config::bCreateConfigFile()
	bool bCreateConfigFileGlobal(bool bVerbosity) {
		// Get access of function from public class security, and check if function went well or not
		if (bCreateConfigFile()) {
			// Output a success message only if verbosity is allowed
			if (bVerbosity == true) {
				std::cout << "Creation of config file successful.\n";
			}
			return true;
		}
		else {
			std::cout << "Creation of config file unsuccessful. There may be a bug.\n";
			return false;
		}

		return true;
	}

	// Global public function to access the ObjectToFile Updater (protected member)
	void bObjectToFileUpdaterGlobal(Config& HScoreUpdate) {
		// Start function from if statememt
		if (bObjectToFileUpdater(HScoreUpdate) == true) {
			VerbosityDisplay("Exited function with code 0 (Worked correctly).\n");
		}
		else {
			VerbosityDisplay("Exited function with code 1 (Faliure).\n");
		}
	}

	// Global public function to access the FileToObject Updater (protected member)
	void bFileToObjectUpdaterGlobal(Config& HScoreUpdate) {
		// Start function from if statement
		if (bFileToObjectUpdater(HScoreUpdate) == true) {
			VerbosityDisplay("Exited function with code 0 (Worked correctly).\n");
		}
		else {
			VerbosityDisplay("Exited function with code 1 (Faliure).\n");
		}
	}

	// Function to display Config object contents of choice
	void displayConfigContents(Config& DisplayContents) {
		slowcharfn(true, "Contents of chosen object:");
		// Display Config object contents
		std::cout << "\nScoreCarDodgeEasy=" << DisplayContents.lnHighScoreCarEasy
			<< "\nScoreCarDodgeHard=" << DisplayContents.lnHighScoreCarHard
			<< "\nScoreGuessHard=" << DisplayContents.lnHighScoreGuess 
			<< "\nDefaultColour=" << DisplayContents.nDefaultColour
			<< "\nVerbosityToggle=" << DisplayContents.bVerbosityToggle
			<< "\nSlowCharSpeed=" << DisplayContents.nSlowCharSpeed
			<< "\nStartupRandomColour=" << DisplayContents.bStartupRandomColourToggle << "\n";
		return;
	}

	// Function to display all contents inside configuration file, with no formatting whatsoever
	void displayFileContents() {
		// Open std::ifstream to tag2_config.ini
		std::ifstream DisplayFile("tag2_config.ini");
		// Check if the file was found or if error occured
		if (DisplayFile.fail()) {
			std::cerr << "Configuration file not found or error occured.\nPlease create a configuration file using the 'createconfig' command and try again.\n";
			return;
		}

		// Display file contents, line by line
		std::string sBuffer;
		while (!DisplayFile.eof()) {
			DisplayFile >> sBuffer;
			std::cout << sBuffer << std::endl;
		}

		// Close stream to save memory space
		DisplayFile.close();
		// Get out of function
		return;
	}
};

Config MainConfigObj;

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

// Function to get console cursor position
COORD GetConsoleCursorPosition() {
	HANDLE						console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO	cbsi;

	if (GetConsoleScreenBufferInfo(console, &cbsi)) {
		return cbsi.dwCursorPosition;
	}
	else {
		// The function failed. Call GetLastError() for details.
		COORD invalid = { 0, 0 };
		return invalid;
	}
}

// Mario kart song in beeps function
void mariobeep();

// FileParse feature activator function
bool fileParseFeatureFn(std::string);

// Function to choose random number
int randnum(long long int max, long long int min) {
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(min, max); // distribution in range [min, max]

	long long int randnum = dist(rng);
	nRandcolNum = randnum;
	return randnum;
}

// Function for outputting characters slowly with different colours
void slowcolourrandfn(std::string slowchar) {
	for (int i = 1, a = 0; slowchar[a] != '\0'; i++, a++) {
		if (i == 15) i = 1;
		colour(i);
		sleep(MainConfigObj.nSlowCharSpeed);
		std::cout << slowchar[a];
	}
	colour(nLastColour);
	clearkeebbuf();
	return;
}



// Function for outputting characters slowly
void slowcharfn(bool nLine, std::string slowChar) {
	// Initialise size to the size of slowchar string
	int size = slowChar.size();
	// Output letter by letter, with an interval defined in MainConfigObj.nSlowCharSpeed
	for (int i = 0; i <= size; i++) {
		sleep(MainConfigObj.nSlowCharSpeed);
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
		if (nLastColour < 16) {
			colour(14);
		}

		if (bIsRunFromFile == false) {
			std::cin >> num;
			if (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(INT_MAX, '\n');
				colour(12);
				std::cerr << "Your input was incorrect. Please try again.\n";
				colour(nLastColour);
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
			std::cout << '\n';
			break;
		}
	}
	colour(nLastColour);
	return num;
}

// Function that makes inputting strings one step easier
std::string str(std::string prompt) {
	std::string string = "";

	// Prompt
	std::cout << prompt;
	if (bIsRunFromFile == false) {
		getline(std::cin, string);
	}
	else {
		std::ifstream FileParseStr(sFileParseName);
		// reiterate through the file to get to the correct line number
		long long int line_no = 0;
		// count file line location
		while (line_no <= nNumOfFileParseLines) {
			getline(FileParseStr, spare, '\n');
			line_no++;
		}
		// send data to str
		getline(FileParseStr, string);
		// close stream to save memory space
		FileParseStr.close();
		// increment nNumOfFileParseLines to switch to next file line
		nNumOfFileParseLines++;
		std::cout << '\n';
	}
	
	return string;
}

// Function to properly display messages with verbosity
void VerbosityDisplay(std::string sVerboseMessage) {
	// This function only displays verbosity messages when the VerbosityToggle is set to 'true'.
	//
	// Fetch the VerbosityToggle setting value and exit if false
	if (MainConfigObj.bStartupRandomColourToggle == 0) {
		return;
	}
	// Display the message
	std::cout << "Verbose Message: " << sVerboseMessage;
	return;
}


////////////////////////////////////////////////////////////////////////////////
/* CODE AFTER THIS POINT IS MOSTLY NOT RELATED TO THE PROGRAM'S PLATFORM CODE */
////////////////////////////////////////////////////////////////////////////////


// fail function - used if command not found
void fail() {
	colour(12);
	std::cerr << "Sorry, your input was incorrect. Please try again.\n";
	colour(2);
	std::cerr << "Previous input: ";
	colour(4);
	std::cerr << "'" << sCommandRAW << "'";
	colour(2);
	std::cerr << ".\n";
	colour(7);
	return;
}

// Help function, used to reduce copy pasting
void help() {
	colour(2);
	std::cout << "HELP- Displays all commands with their meaning(s) and usage.\nNOTE: Keep in mind that you can use a command OR a command number to execute the same thing.\n";
	colour(nLastColour);
	std::cout << "\n1. Tutorial- Starts a tutorial on how to use this application.\n2. Exit- Exits the application.\n3. Hello- Outputs 'Hello, World!'.\n"
		<< "4. Cls- Wipes the console text buffer fully (clears the terminal).\n5. Clear- Removes current console text from view using ANSI escape codes (Only works on specific terminals).\n"
		<< "6. Beep- Create a sound for a specific amount of time and hertz.\n7. TextInfo- Output statistics of chosen text.\n8. Sleep- Timer in milliseconds.\n"
		<< "9. Game- Open a list of text-based games to play.\n10. Coltxt- Colours string of chosen text.\n11. Slowchar- Outputs string of text slowly.\n"
		<< "12. Ansi- A terminal editor where you can modify the look and workings of the terminal with ansi escape codes.\n13. Echo- Type this followed by a string of characters to output whatever you want.\n"
		<< "14. Prev- Output your most previous input into the command section of the terminal.\n15. About- Terminal credits, contributors, and licensing/copyright, with an exclusive message.\n16. Colour- Change the terminal text colour.\n"
		<< "17. Randcol- Randomly pick terminal text and background colour.\n18. Cow- Outputs a cow made with characters.\n19. Cowsay- Make a cow say whatever you want.\n20. Einstein- Output one of 50 Albert Einstein's quotes.\n"
		<< "21. Note- Permanently save notes and access them later.\n22. Shutdownsys- Shuts down your computer.\n23. Restartsys- Restarts your computer.\n24. Dundundun- Outputs a 'dundundun' sound.\n25. Saynote- Output notes that were saved using the note command.\n"
		<< "26. Memtest- Simple memory address checker.\n27. Ipinfo- Shows information about your IP.\n28. Cpustress- A simple single core cpu stress tester.\n29. Calculator- Use a list of calculators built into the program.\n"
		<< "30. Converter- Use a list of converters built into the program.\n31. Mario- Output the Super Mario theme song in beeps.\n"
		<< "32. Ansireset- Resets all set ANSI colour codes with the Ansi command.\n33. Explreset- Restarts Explorer.exe to fix problems with the Desktop shell.\n34. Commandnum- Shows the number of commands you have inputted since the start of the terminal execution.\n"
		<< "35. Cry- Make the terminal cry!\n36. Stopwatch- A simple stopwatch that counts in seconds.\n37. Simplecalc- A simple calculator that supports the 4 basic operators (+,-,/,*).\n38. Edison- Outputs a randomly-picked choice of one of 50 Thomas Edison quotes.\n"
		<< "39. FileParse- Begin the FileParse feature which make the terminal run off a chosen file and use its commands.\n40. Pause- pauses the terminal and awaits any keypress to continue.\n41. Nodisp- Don't display the prompt for commands (saves room in terminal window- similar to cmd's '@echo off' command)\n"
		<< "42. Disp- Opposite of Nodisp.\n43. Colreset- Resets all the terminal colours to default (ignoring ANSI colour codes)\n44. Progopen - Opens a program of your choice, straight from the terminal!\n45. Ramtemp - Store notes temporarily and keep them for the duration of this program running.\n"
		<< "46. Saytemp- Outputs your temporary notes that were made by the RamTemp command.\n47. Wisecow- Outputs a cow saying a quote from Albert Einstein or Thomas Edison.\n48. Hacker- Outputs random numbers with green text in the terminal window to make you look like a hacker!\n"
		<< "49. UpdateConfigIn- Updates settings from config file and sends to Config object (used for Config class)\n50. UpdateConfigOut- Upates settings from object and sends to config file (used for Config class)\n51. DispConfig- Display settings from MainConfigObj object.\n"
		<< "52. DispConfigFile- Displays the contents of this program's configuration file.\n53. CreateConfig- Creates a new config file, usually used for overwriting an old one.\n54. CpuBench- A simple single core cpu benching program that benchmarks using floating-point multiplication.\n"
		<< "55. ReverseText- Outputs an inputted string in reverse.\n56. Settings- Modify the default TerminalAppGen2 settings to your liking.\n";
	return;
}

void edison(bool);
void einstein(bool);

// Tutorial function
void tutorial(bool bFromTerminal) {
	cls();
	slowcharfn(true, "Using this terminal is very straight-forward.");
	slowcharfn(true, "On the starting screen of the terminal, you can always type 'help' in the terminal to see all commands and what they do. ");
	sleep(500);
	for (;;) {
		slowcharfn(false, "Type ");
		colour(3);
		slowcharfn(false, "'help'");
		colour(nLastColour);
		slowcharfn(false, " without quotes to see what happens: > ");
		command = "";
		colour(14);
		getline(std::cin, command);
		colour(nLastColour);
		if (command == "help") {
			help();
			colour(47);
			slowcharfn(false, "Congratulations! You now know how to input properly in this terminal.");
			colour(6);
			std::cout << "\n\nPress ENTER to progress through the tutorial...\n";
			colour(nLastColour);
			std::cin.get();
			break;
		}
		else {
			std::cin.clear();
			colour(12);
			std::cerr << "Try again.\n";
			colour(nLastColour);
		}
	}
	// If the user did not enter the tutorial from the actual terminal, create a file to say that the terminal has been completed.
	if (bFromTerminal == false) {
		MainConfigObj.bCreateConfigFileGlobal(false);
	}
	slowcharfn(true, "Some commands also require you to input a number next to a desired option.");
	slowcharfn(true, "Here is a list of 3 options:");
	colour(1);
	std::cout << "[1] Blue\n";
	colour(8);
	std::cout << "[2] Black\n";
	colour(4);
	std::cout << "[3] Red\n";
	colour(nLastColour);
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
			colour(nLastColour);
			std::cerr << "\n";
		}
	}
	for (;;) {
		colour(nLastColour);
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
			colour(nLastColour);
			std::cerr << "\n";
		}
	}
	for (;;) {
		colour(nLastColour);
		slowcharfn(false, "Please input the number next to the option 'Black': > ");
		command = "";
		colour(14);
		std::cin.clear();
		getline(std::cin, command);
		if (command == "2") {
			colour(47);
			std::cout << "Congrats, you've passed the tutorial!";
			colour(nLastColour);
			std::cout << "\nYou are now ready to use the terminal.\n";
			break;
		}
		else {
			std::cin.clear();
			colour(12);
			std::cerr << "Try that again.";
			colour(nLastColour);
			std::cerr << "\n";
		}
	}
	// Return to the next line after this function was called
	return;
}

// Random Number Calculator
void randNumcalc() {
	long long int	min, max;

	slowcharfn(true, "Welcome to the Random Number Picker!");
	// Prompt
	min = num("Please input your lower number boundary: > ");
	max = num("Please input your higher number boundary: > ");

	colour(34052);
	// Calculate and output
	std::cout << "Result: " << randnum(max, min) << "\n";
	colour(nLastColour);
	return;
}

// Fahrenheit to celsius
void fahrenheitToCelsius() {
	long double		fahrenheit;
	long double		celsius;

	fahrenheit = num("Please input your fahrenheit value: > ");

	celsius = (fahrenheit - 32) * 5.0 / 9.0;

	colour(34052);
	std::cout << "Results:\n";
	colour(nLastColour);
	std::cout << "Fahrenheit value: " << fahrenheit << "\nConverted Celsius value: " << celsius << ".\n\n";
	return;
}

// Celsius to fahrenheit
void celsiusToFahrenheit() {
	long double		celsius;
	long double		fahrenheit;

	celsius = num("Please input your celsius value: > ");

	fahrenheit = celsius * (9.0 / 5.0) + 32;
	// Output result
	colour(34052);
	std::cout << "Results:\n";
	colour(nLastColour);
	std::cout << "Celsius value: " << celsius << "\nConverted fahrenheit value: " << fahrenheit << ".\n\n";
	return;
}

// Simple Interest Calculator
void simpIntCalc() {
	slowcharfn(true, "Welcome to the Simple Interest Calculator!\n");
	// prompt
	long double		money = num("Please input your principal amount of money: > ");
	long double		percnum = num("Please input the interest rate in percentage: > ");
	long long int	yearnum = num("Please input your time number in years: > ");

	// convert percentage into decimal
	percnum /= 100;

	// formula calculation
	long double endnumtotalinterest = money * percnum * yearnum;
	long double endnumyearinterest = money * percnum;
	long double endnumadded = money * percnum * yearnum + money;
	// Output result
	colour(34052);
	std::cout << "\nResults:\n";
	colour(nLastColour);
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
	for (long long int i = 0; i != time; i++) {
		interest = (endnumtotalinterest * percent * 1) / 100;
		endnumtotalinterest += interest;
	}

	long double		endnumyearinterest = money * (1 + (percent / 100));
	long double		investment = endnumtotalinterest;

	// Output results
	colour(34052);
	std::cout << "\nResults:";
	colour(nLastColour);
	std::cout << "\nCalculated interest for the first year: " << (endnumyearinterest - money) << "\nCalculated interest for all "
		<< time << " years: " << (endnumtotalinterest - money) << "\nTotal money that should be recieved: " << investment << std::endl;
}

// Power of 2 calculator
void powerOf2Calc() {
	int				power = 1;
	long long int	powernum = 1;

	slowcharfn(true, "Welcome to the Power of 2 calculator!");
	while (true) {
		power = num("Please input the power to use for the calculation (max is 62): > ");
		if (power >= 62) {
			std::cout << "Your number was too high. The max number is 62. Please try again.\n";
		}
		else break;
	}
	power++;

	// Work out and output value
	colour(34052);
	std::cout << "\n___Results___\n";
	colour(nLastColour);
	// Increase power by 2 every loop
	for (short int i = 1; i < power; i++) {
		powernum *= 2;
		std::cout << i << ": " << powernum << std::endl;
	}
	std::cout << '\n';
	return;
}

void ratioCalc() {
	// This module will not work if FileParse is enabled due to num() return constraints.
	if (bIsRunFromFile == true) {
		colour(12);
		std::cout << "Sorry, RatioCalc will not work properly with FileParse enabled yet.\nExiting...\n";
		colour(nLastColour);
		return;
	}

	long float *nRatioArray = new long float[128];
	short int nNumofNums = 0;
	long float nAddedRatioNum = 0;
	long float nGivenNum = 0;
	slowcharfn(true, "Welcome to the Ratio Divider Calculator!");
	nGivenNum = num("Please input number to be divided into ratio (0 to exit): > ");
	if (nGivenNum <= 0) {
		return;
	}
	std::cout << "Input anything other than a number to exit at any time.\n";
	for (nNumofNums = 0; nNumofNums <= 127; nNumofNums++) {
		std::cout << "Please input ratio number " << (nNumofNums + 1) << ": > ";
		colour(14);
		std::cin >> nRatioArray[nNumofNums];
		colour(nLastColour);
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
	colour(nLastColour);
	return;
}

// Binary to Denary converter
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
		colour(nLastColour);
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
	colour(nLastColour);
	// End
	return;
}

void denaryToBinary() {
	short int		binary2[64];
	long long int	denary;
	int				i = 0;

	slowcharfn(true, "Welcome to the Denary to Binary converter!");
	// Prompt for denary number
	while (true) {
		denary = num("Please enter your denary number: > ");

		// Negative numbers aren't allowed
		if (denary < 0) {
			std::cout << "Sorry, negative numbers aren't allowed. Please try again.\n";
			continue;
		}
		else break;
		// Output 0 off the spot if denary equals 0
		if (denary == 0) {
			colour(34052);
			std::cout << "Result:\nBinary form of " << denary << ": 0.\n";
			colour(nLastColour);
			return;
		}
		colour(34052);
		std::cout << "Result:\nBinary form of " << denary << ": ";
	}

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
	colour(nLastColour);
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
	std::cout << "Result: " << endnum << "%.\n";
}

// Miles to Kilometres Converter
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
	colour(nLastColour);
	std::cout << "Inputted miles value: " << miles << " miles\nConverted kilometres value: " << kilometres << "km\n";
	return;
}

// Kilometres to Miles Converter
void kilometresToMiles() {
	// variables
	long double	kilometres;
	long double	miles;
	// prompt
	slowcharfn(true, "Welcome to the Kilometres to Miles converter!\n");
	kilometres = num("Please input the number of kilometres: > ");
	if (kilometres < 0) {
		std::cerr << "Negative numbers aren't allowed.\nExiting...\n";
		return;
	}
	// Work out value
	miles = kilometres / 1.609344;
	// Output value
	colour(34052);
	std::cout << "\n___Results___\n";
	colour(nLastColour);
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
			std::cout << "OOPS! Player 2 (" << player2Name << ") has reached 0 CARDS!!!! Player 1 (" << player1Name << ") WON, with " << player1Cards << " cards!!!!";
			break;
		}
		else if (player1Cards == 0) {
			std::cout << "OOPS! Player 1 (" << player1Name << ") has reached 0 CARDS!!!! Player 2 (" << player2Name << ") WON, with " << player2Cards << " cards!!!!";
			break;
		}
			player1Num = randnum(15, 1);
		std::cout << "\nPlayer 1 (" << player1Name << ", " << player1Cards << " cards): \nPress any key to send a card from your stack to the pile (ESC to exit game)...";
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
			std::cout << "Hey, " << player1Name << " just got a SNAP! QUICK, type \"I just snapped Player 2\" without quotes to win your pile: > ";
			std::chrono::time_point<std::chrono::steady_clock> start;
			std::chrono::duration<long double> elapsed_seconds;
			// Start the timer
			start = std::chrono::steady_clock::now();
			getline(std::cin, spare);
			elapsed_seconds = std::chrono::steady_clock::now() - start;
			
			// Check if the person typed anything and the right thing in time
			if (spare != "I just snapped Player 2" || elapsed_seconds.count() > 6.0) {
				std::cout << "Sorry " << player1Name << ", you just lost a great opportunity, as you either typed the wrong thing or didn't type anything in time.\n";
				std::cout << "Player 1 remains with " << player1Cards << " cards.\n\n";
			}
			else {
				std::cout << "SNAP!! Player 1 (" << player1Name << ") gets " << pileCardNum << " cards from the pile!\n";
				player1Cards += pileCardNum;
				pileCardNum = 0;
				std::cout << "Player 1 now has " << player1Cards << " cards!\n\n";
			}

		}
			player2Num = randnum(15, 1);
			if (player2Name == "Computer") {
				std::cout << "Player 2 (" << player2Name << ") Card: " << player2Num << '\n';
			}
			else {
				std::cout << "\nPlayer 2 (" << player2Name << ", " << player2Cards << " cards): \nPress ENTER to send a card from your stack to the pile (ESC to exit game)...";
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
				if (player2Name != "Computer") {
					std::cout << "Hey, " << player2Name << " just got a SNAP! QUICK, type \"I just snapped Player 1\" without quotes to win your pile: > ";
					std::chrono::time_point<std::chrono::steady_clock> start;
					std::chrono::duration<long double> elapsed_seconds;
					// Start the timer
					start = std::chrono::steady_clock::now();
					getline(std::cin, spare);
					elapsed_seconds = std::chrono::steady_clock::now() - start;

					// Check if the person typed anything and the right thing in time
					if (spare != "I just snapped Player 1" || elapsed_seconds.count() > 6.0) {
						std::cout << "Sorry " << player2Name << ", you just lost a great opportunity, as you either typed the wrong thing or didn't type anything in time.\n";
						std::cout << "Player 2 remains with " << player2Cards << " cards.\n\n";
					}
					else {
						std::cout << "SNAP!! Player 2 (" << player2Name << ") gets " << pileCardNum << " cards from the pile!\n";
						player2Cards += pileCardNum;
						pileCardNum = 0;
						std::cout << "Player 2 now has " << player1Cards << " cards!\n\n";
					}
				}
				else {
				std::cout << "SNAP!! Player 2 (" << player2Name << ") gets " << pileCardNum << " cards from the pile!\n";
				player2Cards += pileCardNum;
				std::cout << "Player 2 now has " << player2Cards << " cards!\n\n";
				pileCardNum = 0;
				}
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
	std::cin.get();
	return;
}

// Snap game
void snapGameInterface() {
		char	inputchar;

		while (true) {
			colour(173);
			cls();
			std::cout << "################################### SNAP GAME - MENU ######################################\n\n\t\t\t\t[1] Play\n\t\t\t\t[2] How to Play\n\t\t\t\t[3] Exit\n\n######################################################################################\n";
			std::cout << "\nPlease input your desired number here: > ";
			std::cin >> inputnum;
			std::cin.ignore();
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
				std::cin.ignore();
				if (inputnum == 3) break;
				switch (inputnum) {
				case 1:
					while (true) {
					player2Name = str("Please input Player 2's name: > ");
					if (player2Name == "Computer") {
						std::cout << "Sorry, you can't use Computer as your name. Please try again.\n";
					}
					else break;
					}
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
				colour(nLastColour);
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
void guessGameHardInterface() {
	colour(30);
	cls();
	std::cout << "################################# GUESS THE NUMBER - HARD ##############################################\n";
	std::cout << "Welcome to Guess The Number!";
	do {
		std::cout << "\n[1] Play\n[2] Tutorial\n[3] High Score\n[4] Exit\n";
		std::cout << "Input number here: > ";
		std::cin >> inputnum;
		std::cin.ignore();
		if (inputnum == 1) {
			break;
		}
		else if (inputnum == 2) {
			cls();
			std::cout << "################################# GUESS THE NUMBER - HARD ######################################\n";
			slowcharfn(true, "Welcome to Guess The Number - Hard! \nYou have 5 tries to guess a number between 2 randomly picked numbers. \n");
			slowcharfn(true, "These 2 numbers are between 1 and 100. The game will log your highest score.");
			slowcharfn(true, "If you get the answer wrong, the game will tell you if your answer was higher or lower than the correct number.\n");
			slowcharfn(true, "Good luck!\n");
			std::cout << "Press ENTER to continue...\n";
			std::cout << "################################################################################################\n";
			std::cin.get();
			cls();
			std::cout << "################################# GUESS THE NUMBER - HARD ######################################\n";
		}
		else if (inputnum == 3) {
			colour(34052);
			std::cout << "Current high score: " << MainConfigObj.lnHighScoreGuess << ".";
			colour(30);
			std::cout << "\n";
		}
		else {
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			colour(nLastColour);
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
				colour(nLastColour);
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
				if (MainConfigObj.lnHighScoreGuess < score) {
					MainConfigObj.lnHighScoreGuess = score;
					std::cout << "New high score!!!\n\n";
				}
				// Update Files
				MainConfigObj.bObjectToFileUpdaterGlobal(MainConfigObj);
				std::cout << "Oh no! You have used up all of your 5 tries.\nYour score was " << score << ", and the correct number was " << rightnum << ".\nYou lost! Would you like to play again? [y/n] > ";
				std::cin >> input;
				if (input == 'y') {
					cls();
					break;
				}
				else {
					std::cin.clear();
					std::cin.ignore(INT_MAX, '\n');
					colour(nLastColour);
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
	enemy_x[ind] = 17 + randnum(INT_MAX, 0) % (33);
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
void drawCarEasy() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			gotoxy(j + carPos, i + 22);
			if (CarType == 1) std::cout << mycar1[i][j];
			else if (CarType == 2) std::cout << mycar2[i][j];
			else if (CarType == 3) std::cout << mycar3[i][j];
			else if (CarType == 4) std::cout << mycar4[i][j];
		}
	}
}

// Hard car game draw car function
void drawCarHard() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			gotoxy(j + carPos, i + 22);
			if (CarType == 1) std::cout << mycar1[i][j];
			else if (CarType == 2) std::cout << mycar4[i][j];
			else if (CarType == 3) std::cout << mycar5[i][j];
			else if (CarType == 4) std::cout << mycar3[i][j];
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

	if (MainConfigObj.lnHighScoreCarHard < scorecar) {
		MainConfigObj.lnHighScoreCarHard = scorecar;
		colourtxt("New high score!!!\n\n");
	}
	// Update Files
	MainConfigObj.bObjectToFileUpdaterGlobal(MainConfigObj);
	// Gameover screen
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

	if (MainConfigObj.lnHighScoreCarEasy < scorecar) {
		MainConfigObj.lnHighScoreCarEasy = scorecar;
		std::cout << "New high score!!!\n\n";
	}
	// Update Files
	MainConfigObj.bObjectToFileUpdaterGlobal(MainConfigObj);
	// Gameover
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
void carInstructions() {
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
void carGameEasyMain() {
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
					// Update score if exited match was a high score
					if (scorecar > MainConfigObj.lnHighScoreCarEasy) {
						MainConfigObj.lnHighScoreCarEasy = scorecar;
						// Update high scores
						MainConfigObj.bObjectToFileUpdaterGlobal(MainConfigObj);
					}
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

		drawCarEasy();
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
	return;
}

// Change car function (easy)
void changecarEasy() {
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
	gotoxy(19, 2);
	std::cout << " /TT\\";
	gotoxy(19, 3);
	std::cout << " |__|";
	gotoxy(19, 4);
	std::cout << " |  |";
	gotoxy(19, 5);
	std::cout << " /\\/\\";
	gotoxy(18, 7);
	std::cout << "Rocket Car";
	gotoxy(30, 2);
	std::cout << " _==_";
	gotoxy(30, 3);
	std::cout << " ±||±";
	gotoxy(30, 4);
	std::cout << " |00|";
	gotoxy(30, 5);
	std::cout << " ±)(±";
	gotoxy(30, 7);
	std::cout << "F1 Car";
	gotoxy(0, 9);
	std::cout << "[1] Standard\n[2] Slicer\n[3] Rocket Car\n[4] F1 Car\n";
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
		else if (inputnum == 4) {
			CarType = 4;
			std::cout << "Option 4 set!\n";
		}
		else if (inputnum == 0) {
			return;
		}
		else {
			std::cin.clear();
			std::cin.ignore();
			std::cout << "Please retry.\n";
		}
		if (inputnum == 1 || inputnum == 2 || inputnum == 3 || inputnum == 4) {
			sleep(1000);
			break;
		}
	} while (true);
	return;
}


void cardodgeeasy() {
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
			carGameEasyMain();
			break;
		case 2:
			changecarEasy();
			break;
		case 3:
			colour(32778);
			std::cout << "Current high score: " << MainConfigObj.lnHighScoreCarEasy;
			if (MainConfigObj.lnHighScoreCarEasy <= 50) {
				std::cout << " (Below Average).";
			}
			else if (MainConfigObj.lnHighScoreCarEasy <= 100) {
				std::cout << " (Average).";
			}
			else if (MainConfigObj.lnHighScoreCarEasy <= 200) {
				std::cout << " (Above Average).";
			}
			else if (MainConfigObj.lnHighScoreCarEasy <= 325) {
				std::cout << " (Insane).";
			}
			else if (MainConfigObj.lnHighScoreCarEasy <= 525) {
				std::cout << " (G.O.A.T Level).";
			}
			else if (MainConfigObj.lnHighScoreCarEasy > 525) {
				std::cout << " (GOD Level).";
			}
			colour(47);
			std::cout << "\nPress ENTER to continue...";
			std::cin.ignore();
			std::cin.get();
			break;
		case 4:
			carInstructions();
			break;
		case 5:
			CarType = 1;
			scorecar = 0;
			colour(nLastColour);
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
					// Update high score if exited round did hit high score
					if (scorecar > MainConfigObj.lnHighScoreCarHard) {
						MainConfigObj.lnHighScoreCarHard = scorecar;
						// Update high scores
						MainConfigObj.bObjectToFileUpdaterGlobal(MainConfigObj);
					}
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

		drawCarHard();
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
	gotoxy(29, 2);
	std::cout << " /TT\\";
	gotoxy(29, 3);
	std::cout << " |__|";
	gotoxy(29, 4);
	std::cout << " |  |";
	gotoxy(29, 5);
	std::cout << " /\\/\\";
	gotoxy(28, 7);
	std::cout << "Rocket Car";
	gotoxy(0, 9);
	std::cout << "[1] Standard\n[2] F1 Car\n[3] Army Car\n[4] Rocket Car\n";
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
		else if (inputnum == 4) {
			CarType = 4;
			std::cout << "Option 4 set!\n";
		}
		else if (inputnum == 0) {
			return;
		}
		else {
			std::cin.clear();
			std::cin.ignore();
			std::cout << "Please retry.\n";
		}
		if (inputnum == 1 || inputnum == 2 || inputnum == 3 || inputnum == 4) {
			sleep(1000);
			break;
		}
	} while (true);
	return;
}

// Car dodge hard function/game
void cardodgehard() {
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
			std::cout << "Current high score: " << MainConfigObj.lnHighScoreCarHard;
			// Check score skill level
			if (MainConfigObj.lnHighScoreCarHard <= 9) {
				std::cout << " (Below Average).";
			}
			else if (MainConfigObj.lnHighScoreCarHard <= 37) {
				std::cout << " (Average).";
			}
			else if (MainConfigObj.lnHighScoreCarHard <= 70) {
				std::cout << " (Above Average).";
			}
			else if (MainConfigObj.lnHighScoreCarHard <= 95) {
				std::cout << " (Insane).";
			}
			else if (MainConfigObj.lnHighScoreCarHard <= 150) {
				std::cout << " (G.O.A.T Level).";
			}
			else if (MainConfigObj.lnHighScoreCarHard > 150) {
				std::cout << " (GOD Level).";
			}
			colour(176);
			std::cout << "\nPress ENTER to continue...";
			std::cin.ignore();
			std::cin.get();
			break;
		case 4:
			carInstructions();
			break;
		case 5:
			CarType = 1;
			scorecar = 0;
			colour(nLastColour);
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
	std::cout << "################################# GUESS THE NUMBER - EASY ##############################################\n";
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
			std::cout << "################################# GUESS THE NUMBER - EASY ##############################################\n";
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
			colour(nLastColour);
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
				colour(nLastColour);
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
					colour(nLastColour);
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
					colour(nLastColour);
					cls();
					return;
				}
			}
		} while (true);
	}


}

// Commands function- this is where all commands will go
void commands() {
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
		colour(nLastColour);
		std::cin.clear();
		std::cin.ignore(INT_MAX, '\n');
		if (input == 'y') tutorial(true);
		return;
	}
	// hello
	else if (command == "hello" || command == "3") {
		colour(randnum(15, 1));
		std::cout << "Hello, World!\n";
		colour(nLastColour);
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
		if (nLastColour == 7) colour(3);
		slowcharfn(false, "Input text that you want to colour: > ");
		colour(14);
		std::cin.clear();
		coltxt = str("");
		colour(7);
		// This function colours the text
		colourtxt(coltxt);
		colour(nLastColour);
		std::cout << "\n";
		return;
	}
	// slowchar
	else if (command == "slowchar" || command == "11") {
		std::cout << "Input what you want to output as 'slow characters': > ";
		colour(14);
		spare = str("");
		colour(nLastColour);
		// A boolean is passed along with the function so it knows if you need a newline or not
		slowcharfn(false, spare);
		std::cout << std::endl;
		return;
	}
	// ansi
	else if (command == "ansi" || command == "12") {
		int nAnsiCount = -1;
		colour(2);
		slowcharfn(true, "Welcome to the ANSI Editor.");
		colour(nLastColour);
		while (true) {
			nAnsiCount++;
			std::cout << "Please input your ANSI escape code without any square brackets or \\x1b (type 0 to exit, 1 for list of ansi codes): > ";
			spare = str("");
			if (spare == "0" || spare == "exit") {
				std::cout << "Do you want to save your changes? [y/n] > ";
				std::cin.clear();
				getline(std::cin, spare);
				if (spare == "y") {
					if (nAnsiCount != 0) {
					ansicol = 1;
					}
				}
				else ansicol = 0;
				return;
			}
			if (spare == "1") {
				std::cout << "Graphics/Colours:\n\n0m - Reset all\n1m- Bold mode (Reset: 22m)\n2m- Dim/faint mode (Reset: 22m)\n3m- Italic mode (Reset: 23m)\n"
					<< "4m- Underline mode (Reset: 24m)\n5m- Blinking mode (Reset: 25m)\n7m- Inverse/Reverse mode (Reset: 27m)\n8m- Hidden/Invisible mode (Reset: 28m)\n"
					<< "9m- Strikethrough mode (Reset: 29m)\n";
				nAnsiCount--;
				continue;
			}
			if (spare == "0m") {
				ansicol = 0;
			}
			// This is passed along with the value that the user inputted
			std::cout << "\x1b[" << spare << std::endl;
		}
	}
	// beep
	else if (command == "beep" || command == "6") {
		// Variables
		char text[40] = "Your sound is being outputted!!!";
		long long int hertz;
		long long int milsecs;

		std::cout << "Input 0 to exit.\n\nWelcome to Beep!\nThe maximum an average person can hear is 20,000 hertz (please don't try it).\n";

		// Enter hertz number
		hertz = num("\nPlease input your hertz value: > ");

		// If hertz input is 0, exit program
		if (hertz <= 0) {
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			return;
		}
		// Enter milliseconds number
		milsecs = num("\nEnter the amount of time in milliseconds: > ");

		// If millisecond input is 0, exit program
		if (milsecs <= 0) {
			std::cin.clear();
			std::cin.ignore();
			return;
		}

		// Creates colourful text saying that the sound is being outputted
		colourtxt(text);
		// Set the text colour to what it was before
		colour(nLastColour);
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
		std::cout << "Input 0 to exit.\nThis is a sleep function that times in ";
		colour(34052);
		std::cout << "milliseconds.";
		colour(nLastColour);
		// Prompt
		sleeptime = num("\nPlease input time in milliseconds: > ");

		// Exit if 0 is inputted
		if (sleeptime <= 0) {
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			return;
		}
		colourtxt("Timing...\n");
		sleep(sleeptime);
		std::cout << "\nTimer finished.\n";
		// Clear keyboard buffer
		clearkeebbuf();
		std::cin.clear();
		std::cin.ignore(INT_MAX, '\n');
		return;
	}
	// echo
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
		colour(nLastColour);
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
		colour(nLastColour);
		// switch case for inputted number
		switch (inputnum) {
		case 1:
			guessgameeasy();
			break;
		case 2:
			guessGameHardInterface();
			break;
		case 3:
			cardodgeeasy();
			break;
		case 4:
			cardodgehard();
			break;
		case 5:
			snapGameInterface();
			break;
		default:
			std::cin.clear();
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			break;
		}
		colour(nLastColour);
		return;
	}
	// prev
	else if (command == "prev" || command == "14") {
		colour(randnum(15, 1));
		std::cout << "Previous input: ";
		colour(4);
		std::cout << "'" << previnput << "'";
		colour(nLastColour);
		std::cout << ".\n";
		return;
	}
	// about
	else if (command == "about" || command == "abt" || command == "15") {
		std::cout << "Please press '1' or '2' if you want:\n[1] Credits and message\n[2] TerminalAppGen2 License\n";
		input = _getch();
		if (input == '1')
			goto credits;
		else if (input == '2')
			goto licence;
		else return;

		// Credits and message
	credits:
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
		std::cout << "\nA short message from the maker:\n";
		colour(15);
		slowcharfn(true, "I hope you all like this program so far. I really enjoy making this program with my friends and family,\nand recieveing feedback that is helpful to the development of it. I will hopefully learn more\nwith regards to programming while developing apps to make your experience with my programs a \nlot nicer.\nThanks guys! :)\n");
		colour(nLastColour);

		// license
	licence:
		slowcharfn(false, "This program uses the MIT Open-Source Licence. Here's some more info:\n\n");
		// MIT Licence
		std::cout << "Copyright (c) 2023, Ryan Zorkot\n\nPermission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:\n\nThe above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.\n\nTHE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n";
		sleep(500);
		slowcharfn(true, "\nBasically, you can do whatever you want as long as you include the original copyright and license notice in any copy of this program. Simple, right?");
		return;
	}
	// colourtxt
	else if (command[0] == 'c' && command[1] == 'o' && command[2] == 'l' && command[3] == 'o' && command[4] == 'u' && command[5] == 'r' || command == "16") {
	if (colcheck == true) {
		fail();
		colcheck = false;
		return;
	}
		if (command[6] != ' ' || command == "16") {
			coloption = 0;
			std::cout << "Text colour options:\n [1] Black\n [2] Blue\n [3] Green\n [4] Aqua\n [5] Red\n [6] Purple\n [7] Yellow\n [8] White\n [9] Grey\n[10] Light Blue\n"
				<< "[11] Light Green\n[12] Light Aqua\n[13] Light Red\n[14] Light Purple\n[15] Light Yellow\n[16] Bright White\n";
			do {
				std::cout << "Please input your option here (0 to exit): > ";
				colour(14);
				coloption = num("");
				colour(nLastColour);

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
			nLastColour = coloption;
			// ansicol set to 0 to show that this is not an ansi colour
			ansicol = 0;
			std::cin.ignore();
			return;
		}
		else {
			short int coloption2;
			// Calculate colour
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
			// Colour option set was too high
			if (coloption > 16 || coloption < 1) {
				std::cin.clear();
				std::cerr << "Please input a number from the list of colours and try again.\nYou can view the list by typing the 'colour' command alone.\n";
				return;
			}
			coloption--;
			colour(coloption);
			nLastColour = coloption;
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
		colour(nLastColour);
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
		colour(nLastColour);
		std::cout << " file\nin this ";
		colour(11);
		std::cout << "Program's";
		colour(nLastColour);
		std::cout << " folder.\n\nType ";
		colour(12);
		std::cout << "\"^exit\"";
		colour(nLastColour);
		std::cout << " without quotes to exit.\n\n";
		std::ofstream filee("Notes.txt");
		// loop
		do {
			// Ask for notes input
			std::cout << "Note ";
			colour(6);
			std::cout << innum;
			colour(nLastColour);
			std::cout << ": ";
			colour(14);
			getline(std::cin, text);
			colour(nLastColour);
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
		std::cerr << "Are you sure you want to restart this computer? [y/n] > ";
		colour(14);
		std::cin >> input;
		colour(nLastColour);

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
		std::cerr << "Are you sure you want to shut down this computer? [y/n] > ";
		colour(14);
		std::cin >> input;
		colour(nLastColour);

		while (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			return;
		}
		if (input == 'y') {
			// cmd shutdown command
			system("shutdown /s /t 0");
		}
		else {
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			return;
		}
	}
	// randcol
	else if (command == "randcol" || command == "17") {
		nLastColour = randnum(255, 0);
		colour(nLastColour);
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
		std::cout << '\n';
		return;
	}
	// saynote
	else if (command == "saynote" || command == "snote" || command == "25") {
		std::ifstream fileein("Notes.txt");
		// check if fileein stream is working
		if (fileein.fail()) {
			colour(12);
			std::cerr << "The Notes.txt file is unavailable for access.\n";
			colour(nLastColour);
			std::cerr << "Please make sure you have written any notes or you haven't deleted the ";
			colour(11);
			std::cerr << "Notes.txt";
			colour(nLastColour);
			std::cerr << " file.\n";
			fileein.close();
			return;
		}
		// output notes here
		colour(11);
		std::cout << "\nPermanent Notes:\n\n";
		colour(nLastColour);
		spare = "";
		for (int i = 1; std::getline(fileein, spare); i++) {
			std::cout << i << ": " << spare << std::endl;
		}
		// output that there are no more notes to show
		colour(12);
		std::cerr << "\nThere are no more notes to show.\n";
		colour(nLastColour);
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
			colour(nLastColour);
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
				colour(nLastColour);
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

			inputnum = num("\nPlease input your option: > ");

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

			inputnum = num("\nPlease input your option: > ");

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
		colour(nLastColour);
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
	colour(nLastColour);
	return;
	}
	// commandnum
	else if (command == "commandnum" || command == "cnum" || command == "34") {
	colour(randnum(15, 1));
	std::cout << "You have inputted ";
	colour(9);
	std::cout << nNumofUsrCommands;
	colour(nRandcolNum);
	std::cout << " commands since the start of this terminal's execution.\n";
	colour(nLastColour);
	}
	// cry
	else if (command == "cry" || command == "35") {
		// Here's a small cry storyline
		colour(34052);
		std::cout << "Cry- A short story by Ryan (the developer):\n\n";
		colour(nLastColour);
		sleep(1000);
		colourtxt("*Story starts*\n\n");
		sleep(500);
		colour(11);
		std::cout << "Computer: ";
		colour(nLastColour);
		sleep(500);
		slowcharfn(true, "Aight, I'm hungry. I want a cookie!");
		std::cout << "          ";
		slowcharfn(true, "But where could I find one? ");
		sleep(750);
		colour(11);
		std::cout << "Web Server: ";
		colour(nLastColour);
		sleep(500);
		slowcharfn(true, "*begins talking to other computers and supplying them cookies*");
		sleep(750);
		slowcharfn(false, "(The computer now knows where to get cookies. The computer goes to the web server and says:)");
		sleep(750);
		colour(11);
		std::cout << "\nComputer: ";
		colour(nLastColour);
		sleep(500);
		slowcharfn(true, "Hey web server, could I please have a cookie-");
		sleep(500);
		colour(11);
		std::cout << "Web Server: ";
		colour(nLastColour);
		sleep(500);
		slowcharfn(false, "Sorry, your request could not be processed. \n");
		std::cout << "            ";
		slowcharfn(false, "Error info: \"We're out of cookies, dude. Come back later.\"");
		sleep(750);
		colour(11);
		std::cout << "\nComputer: \n";
		colour(nLastColour);
		sleep(500);
		for (short int i = 1; i < 16; i++) {
			colour(i);
			std::cout << "WAAAAAAAAAAAAAAAAAAAAHAHAHHAHAHAAAAAAAAAAAAAAAAAAHA\n";
			sleep(50);
		}
		sleep(500);
		colourtxt("\n*Story done*\n");
		return;
	}
	// stopwatch
	else if (command == "stopwatch" || command == "stopw" || command == "36") {
	std::chrono::time_point<std::chrono::steady_clock> start;
	std::chrono::duration<long double> elapsed_seconds;

	std::cout << "This is a simple but very accurate stopwatch which measures time to a ten-thousandth of a second.\nPress any key while the stopwatch is running to exit it.\n\n";
	colour(4);
	slowcharfn(true, "Press 's' to start stopwatch...");
	input = _getch();
	colour(11);
	std::cout << "Timing...\n\n";
	// Start the timer
	start = std::chrono::steady_clock::now();
	std::cout << "Time: \n";
	colour(nLastColour);
	// Loop until key is pressed
	while (!_kbhit()) {
		elapsed_seconds = std::chrono::steady_clock::now() - start;
		std::cout << "\r               \r" << elapsed_seconds.count() << " sec";
		// implemented a sleep function so cpu resources are in check (10ms rest)
		sleep(10);
	}
	clearkeebbuf();
	// Output exact time at the end
	elapsed_seconds = std::chrono::steady_clock::now() - start;
	std::cout << "\nTotal accurate time elapsed: ";
	colour(11);
	std::cout << elapsed_seconds.count();
	colour(nLastColour);
	std::cout << " seconds." << std::endl;
	return;
	}
	// simplecalc
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
	else if (std::cin.fail()) {
		std::cout << "Sorry, an error occured. Possibly input was incorrect?\n";
		std::cin.clear();
		std::cin.ignore(INT_MAX, '\n');
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
	colour(nLastColour);
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
		std::cout << "Welcome to the FileParse Runner!\nPlease input the exact filepath of the file you want to run from (0 to exit): > ";
		colour(14);
		getline(std::cin, sFileParse);
		colour(nLastColour);
		if (sFileParse == "0") return;

		// Test if the file is legit
		std::ifstream FileParseTest(sFileParse);
		if (FileParseTest.fail() == true) {
			colour(4);
			std::cerr << "Sorry, an error occured while opening the file.\nYou probably put the wrong filename into the program. Please try again.\n\n";
			colour(nLastColour);
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
		std::cout << ">\n\n";
	}
	else {
		std::cout << "\nFile execution Failed!\nAn error occured during execution of file. File used: <";
		colour(11);
		std::cout << sFileParse;
		colour(nLastColour);
		std::cout << ">\n\n";
	}
	return;
	}
	// pause
	else if (command == "pause" || command == "40") {
	std::cout << "Press any key to exit PAUSE command...";
	_getch();
	std::cout << '\n';
	return;
	}
	// colreset
	else if (command == "colreset" || command == "43") {
	// Reset colours to default
	colour(DEFAULT_COL);
	std::cout << "Colours reset.\n";
	return;
	}
	// progopen
	else if (command == "progopen" || command == "open" || command == "44") {
	std::string sFileDir = "start ";
	// Prompt block
	colour(randnum(15, 1));
	slowcharfn(true, "Welcome to the Program Opener!");
	colour(nLastColour);
	std::cout << "\nHere's a list of options for programs to open (0 to exit):\n";
	std::cout << "[1] File Explorer\n[2] Control Panel\n[3] MSPaint\n[4] Device Manager\n[5] Disk Management\n[6] Microsoft Edge\n[7] Administrative Tools\n[8] Services\n[9] Task Manager\n[10] Other (Type a Directory)\n";
	inputnum = num("\nPlease input your choice here: > ");
	std::cin.ignore();

	// check if exit
	if (inputnum == 0) {
		std::cout << "Exiting...\n";
		return;
	}

	// Contains main instructions (switch)
	switch (inputnum) {
	case 1:
		system("start explorer.exe");
		break;
	case 2:
		system("start control");
		break;
	case 3:
		system("start mspaint");
		break;
	case 4:
		system("start devmgmt.msc");
		break;
	case 5:
		system("start diskmgmt.msc");
		break;
	case 6:
		system("start microsoft-edge:");
		break;
	case 7:
		system("start control admintools");
		break;
	case 8:
		system("start services.msc");
		break;
	case 9:
		system("start taskmgr");
		break;
	case 10:
		colour(nLastColour);
		std::cout << "\nPlease enter a file directory to execute: > ";
		getline(std::cin, spare);

		// Append 'start' to the spare variable
		sFileDir += spare; // std::string supports + and += operators for concatenation

		// Send the sFileDir var to system()
		system(sFileDir.c_str());

		// Exit
		colour(randnum(15, 1));
		std::cout << "Execution complete.\n";
		colour(nLastColour);
		return;
	default:
		std::cout << "Sorry, your choice was not understood. Please try again later.\n";
		return;
	}

	colour(10);
	std::cout << "Program successfully opened!\n";
	colour(nLastColour);
	return;
	}
	// ramtemp
	else if (command == "ramtemp" || command == "tempnote" || command == "45") {
	nIterationNum = 0;
	colour(randnum(15, 1));
	slowcharfn(true, "Welcome to the RamTemp Storer!");
	colour(nLastColour);

	// Use colours to make it look good
	std::cout << "\nThis allows you to store notes temporarily, for the duration of this program running.\n"
		<< "You can access your notes using the command '";
	colour(11);
	std::cout << "saytemp";
	colour(nLastColour);
	std::cout << "' when you are done.\nYou can store up to ";
	colour(10);
	std::cout << "100 notes";
	colour(nLastColour);
	std::cout << " at a time.\n\nType \"";
	colour(11);
	std::cout << "^exit";
	colour(nLastColour);
	std::cout << "\" to exit RamTemp at any time.\n\n";

	while (true) {
		// Check if max notes have been hit
		if (nIterationNum == 100) {
			std::cout << "Sorry, you have hit the max number of notes.\nPlease retry later to reset the counter and begin overwriting previous notes.\n";
			return;
		}
		std::cout << "Note ";
		colour(randnum(15, 1));
		std::cout << (nIterationNum + 1);
		colour(nLastColour);
		std::cout << ": ";
		
		// Take in only one (1) note at a time
		colour(14);
		sRamTemp[nIterationNum] = str("");
		colour(nLastColour);

		// '^exit' means exit RamTemp
		if (sRamTemp[nIterationNum] == "^exit") {
			std::cout << "Exiting...\n";
			nIterationNum--;
			return;
		}
		// Iterate counter
		nIterationNum++;
		// Change notes bool indicator
		bTempNotesPresent = true;
	}
	return;
	}
	// saytemp
	else if (command == "saytemp" || command == "temp" || command == "46") {
	// Check if notes are present
	if (bTempNotesPresent == false) {
		colour(4);
		std::cerr << "You can't display any temporary notes because you don't have any. \nPlease type in the 'RamTemp' command to make notes later.\n";
		colour(nLastColour);
		std::cout << "Exiting...\n";
		return;
	}

	colour(11);
	std::cout << "\nYour RAMTemp notes:\n\n";
	colour(nLastColour);
	// Iterate through notes until nIterationNum value is reached
	short int i = 0;
	while (i <= nIterationNum) {
		std::cout << "Note ";
		colour(11);
		std::cout << (i + 1);
		colour(nLastColour);
		std::cout << ": ";
		// Display Note
		std::cout << sRamTemp[i] << '\n';
		// Increment i
		i++;
	}
	colour(10);
	std::cout << "\nAll notes have been displayed above.\n";
	return;
	}
	else if (command == "wisecow" || command == "47") {
	short int rand;
	// Pick a random number
	rand = randnum(2, 1);

	colour(randnum(15, 1));
	slowcharfn(true, "Henry the very wise cow says:\n");
	colour(nLastColour);

	CONSOLE_SCREEN_BUFFER_INFO ConsBufInf;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ConsBufInf);

	for (int i = 0; i <= (ConsBufInf.dwSize.X - 1); i++) { std::cout << '*'; }

	// 2 is for an Einstein quote, 1 is for Edison
	if (randnum(2, 1) == 2) {
		einstein(true);
	}
	else {
		edison(true);
	}

	for (int i = 0; i <= (ConsBufInf.dwSize.X - 1); i++) { std::cout << '_'; }

	std::cout << "\n";
	std::cout << "    |\n";
	std::cout << "    |  ^__^\n";
	std::cout << "    -- (OO)\_______     \n";
	std::cout << "       (__)\       )_/\\/\\ \n";
	std::cout << "           ||----W |\n";
	std::cout << "           ||     ||\n";
	std::cout << "\nNot bad, Henry. Come back later, Henry the Cow needs his hay.\n";

	return;
	}
	// hacker
	else if (command == "hacker" || command == "hack" || command == "48") {
	int nLineNum = 0;
	colour(randnum(15, 1));
	slowcharfn(false, "Welcome to Hacker!");
	colour(nLastColour);
	std::cout << "\n\nThis prints out green numbers to make the terminal look like you are hacking.\nPress any key in the middle of the run to exit.\n\n";

	nLineNum = num("Please input how many lines of hacking-looking code you want to get outputted (0 to exit): > ");
	std::cin.ignore(INT_MAX, '\n');

	// Input 0 to exit
	if (nLineNum == 0) {
		colour(4);
		std::cout << "Exiting...\n";
		colour(nLastColour);
		return;
	}
	// Negative numbers aren't allowed
	if (nLineNum < 0) {
		colour(4);
		std::cout << "Negative numbers aren't allowed.\nExiting...\n";
		colour(nLastColour);
		return;
	}

	CONSOLE_SCREEN_BUFFER_INFO ConsBufInf;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ConsBufInf);

	// Random number spammer
	colour(10);
	int i = 0;
	int j = 0;
	// You can leave by pressing a key, as indicated by _kbhit()
	while (i <= nLineNum && !_kbhit()) {
		j = 0;
		while (j <= ConsBufInf.dwSize.X) {
			// Spam from range 999999 [max], 0 [min]
			std::cout << randnum(999999, 0);
			j++;
		}
		std::cout << '\n';
		i++;
	}
	std::cout << '\n';
	clearkeebbuf();
	return;
	}
	// UpdateConfigIn
	else if (command == "updateConfigin" || command == "uhsin" || command == "49") {
		Config UHSIn;
		// Update Files
		UHSIn.bFileToObjectUpdaterGlobal(MainConfigObj);
		return;
	}
	// UpdateConfigOut
	else if (command == "updateConfigout" || command == "uhsout" || command == "50") {
		Config UHSOut;
		// Update Objects
		UHSOut.bObjectToFileUpdaterGlobal(MainConfigObj);
		return;
	}
	// DispConfig
	else if (command == "dispConfig" || command == "dhscores" || command == "51") {
		// Read from MainConfigObj object members and output
		MainConfigObj.displayConfigContents(MainConfigObj);
		std::cout << "\n\nRead finished.\n";
		return;
	}
	// dispconfigfile
	else if (command == "dispconfigfile" || command == "dispconf" || command == "52") {
		slowcharfn(true, "\nFile Contents:\n");
		// Read from configuration file and output
		MainConfigObj.displayFileContents();
		std::cout << "\n\nRead finished.\n";
		return;
	}
	// createconfig
	else if (command == "createconfig" || command == "53") {
		colour(4);
		std::cout << "Warning! Creating a configuration file when there already is one will result in the previous\nconfiguration file being completely overwritten with the default values.\n\n";
		colour(nLastColour);
		std::cout << "Are you sure you want to continue? [y/n]: > ";
		std::cin >> input;
		std::cin.clear();
		std::cin.ignore(INT_MAX, '\n');
		// check if yes or no
		if (input != 'y') return;

		// create config file using Config::bCreateConfigFile() through an access token in public label
		MainConfigObj.bCreateConfigFileGlobal(true);
		return;
	}
	// cpubench
	else if (command == "cpubench" || command == "54") {
		unsigned long long int nCpuBench;
		std::chrono::time_point<std::chrono::steady_clock> start, end;
		std::chrono::duration<double> elapsed_seconds; // Save the duration of the operation here

		colour(randnum(15, 1));
		slowcharfn(true, "\nWelcome to the CPU Benchmark test.");
		colour(nLastColour);
		std::cout << "This is a simple, single core benchmark test that does floating-point multiplication.\nYou can press any key in the middle of the test to stop.\n\nThe default number of reiterations is 1 million, and maximum is 12 trillion.";
		std::cout << "\nPlease type in the number of reiterations to do (input 0 for default value, or a character to exit): > ";
		std::cin >> nCpuBench;

		// Abort if something hapened in std::cin stream
		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			colour(12);
			std::cout << "Exiting...\n";
			colour(nLastColour);
			return;
		}
		else {
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
		}

		// Set default if input is 0
		if (nCpuBench == 0) {
			nCpuBench = 1000000;
		}
		// Leave if the user killed std::cin OR if number is too high
		if (nCpuBench > 12000000000000) {
			colour(12);
			std::cout << "Your number seemed to be too high or is a negative, or some other random error occured. \nTry again later.\n";
			colour(nLastColour);
			return;
		}

		// benchmark part
		float num = 1.5;
		colourtxt("Benchmark has started...\n");
		// Start the timer
		start = std::chrono::steady_clock::now();
		// loop
		for (unsigned long long int i = 0; i <= nCpuBench; i++) {
			// increment variable
			num *= num;
			// kbhit is used to see if a key was pressed
			if (_kbhit()) {
				colour(12);
				std::cerr << "Stress test terminated. Exiting...\n";
				colour(nLastColour);
				clearkeebbuf();
				return;
			}
		}
		// Stop the timer and save to 'end' object
		end = std::chrono::steady_clock::now();
		elapsed_seconds = end - start;

		std::cout << "Benchmark complete. Your computer's single core took " << elapsed_seconds.count() << " seconds to finish the test.\nCompare that with your friend's scores!\n";
		return;
	}
	// reversetext
	else if (command == "reversetext" || command == "revtext" || command == "55") {
		// Declare variables
		std::string sRevText;

		colour(randnum(15, 1));
		slowcharfn(true, "Welcome to ReverseText!");
		colour(nLastColour);
		// Prompt
		sRevText = str("Please input string that you want to be outputted in reverse (0 to exit): > ");
		// exit if 0
		if (sRevText == "0") {
			colour(4);
			std::cout << "Exiting...\n";
			colour(nLastColour);
			return;
		}

		// Main part
		colour(10);
		std::cout << "___Output:___\n";
		colour(nLastColour);
		// std::reverse reverses the string
		std::reverse(sRevText.begin(), sRevText.end());
		std::cout << sRevText << "\n\n";
		return;
	}
	// settings
	else if (command == "settings" || command == "preferences" || command == "56") {
		colour(randnum(15, 1));
		slowcharfn(true, "Welcome to the Settings page!");
		colour(nLastColour);
		while (true) {
			std::cout << "Please select the setting that you want to modify:\n";
			std::cout << "[1] Default Colour\n[2] ConfigFile Verbosity Messages\n[3] SlowChar Speed\n[4] Use Random Colour on Startup\n\n";
			inputnum = num("Enter number here (type 0 to exit): > ");
			std::cin.ignore(INT_MAX, '\n');
	
			// if 0, exit
			if (inputnum == 0) {
				colour(12);
				std::cout << "Exiting...\n";
				colour(nLastColour);
				return;
			}

			// switch statement to scroll between setting choices
			switch (inputnum) {
			case 1:
				// DefaultColour
				std::cout << "DefaultColour: Sets the default colour to use on TerminalAppGen2 startup.\n";
				colour(11);
				std::cout << "NOTE: You can get the list of colour numbers using the 'colour' command if you need them.\n";
				colour(nLastColour);
				std::cout << "This is an integer value.\n\nDefault value: 7\n";
				std::cout << "Current saved value: " << MainConfigObj.nDefaultColour << "\n";
				inputnum = num("Please input new DefaultColour value (negative number to abort): > ");
				// abort if inputted number is a negative number
				if (inputnum < 0) {
					colour(12);
					std::cout << "Aborting...\n";
					colour(nLastColour);
					break;
				}
				// Colours over number 255 aren't allowed
				if (inputnum > 255) {
					colour(12);
					std::cerr << "Sorry, but numbers over 255 aren't allowed for the SlowCharSpeed setting. Please try again.\n";
					colour(nLastColour);
				}
	
				// Set new value in object
				MainConfigObj.nDefaultColour = inputnum;
				// Update config file
				MainConfigObj.bObjectToFileUpdaterGlobal(MainConfigObj);
				// Finished
				colour(10);
				std::cout << "Done. Restart TerminalAppGen2 to see your changes!\n\n";
				colour(nLastColour);
				break;
		case 2:
				// VerbosityToggle
				std::cout << "VerbosityToggle: Toggles the option for the output of some extra verbosity messages.\nThis is a boolean value.\n\nDefault value: 0\n";
				std::cout << "Current saved value: " << MainConfigObj.bVerbosityToggle << '\n';
				while (true) {
					inputnum = num("Please input new VerbosityToggle value (negative number to abort): > ");
					// exit while loop if inputted num is negative
					if (inputnum < 0) break;
					if (inputnum != 1 && inputnum != 0) {
						colour(12);
						std::cout << "This is a boolean value that can only take in an input of 1 (true) or 0 (false). Please try again.\n";
							colour(nLastColour);
					}
					else break;
				}

				// abort if inputted number is a negative number
				if (inputnum < 0) {
					colour(12);
					std::cout << "Aborting...\n";
					colour(nLastColour);
					break;
				}
	
				// Set new value in object
				MainConfigObj.bVerbosityToggle = inputnum;
				// Update config file
				MainConfigObj.bObjectToFileUpdaterGlobal(MainConfigObj);
				// Finished
				colour(10);
				std::cout << "Done. Restart TerminalAppGen2 to see your changes!\n\n";
				colour(nLastColour);
				break;
				case 3:
				// SlowCharSpeed
				std::cout << "SlowCharSpeed: Sets the speed of letters that appear slowly in text that appears slowly, in milliseconds.\nThis is an integer value.\n\nDefault value: 45\n";
				std::cout << "Current saved value: " << MainConfigObj.nSlowCharSpeed << '\n';
				while (true) {
					inputnum = num("Please input new SlowCharSpeed value (0 to disable entirely, negative number to abort): > ");
					// exit while loop if number is negative
					if (inputnum < 0) break;
					else break;
				}
				// abort if inputted number is a negative number
				if (inputnum < 0) {
					colour(12);
					std::cout << "Aborting...\n";
					colour(nLastColour);
					break;
				}
				// Set new value in object
				MainConfigObj.nSlowCharSpeed = inputnum;
				// Update config file
				MainConfigObj.bObjectToFileUpdaterGlobal(MainConfigObj);
				// Finished
				colour(10);
				std::cout << "Done. You do not need to restart for this setting to take effect.\n\n";
				colour(nLastColour);
				break;
			case 4:
				// StartupRandomColour
				std::cout << "StartupRandomColour: A toggle for those who want a random colour option when starting up TerminalAppGen2. \n";  
				colour(11);
				std::cout << "NOTE: This overrides DefaultColour.";
				colour(nLastColour);
				std::cout << "\nThis is a boolean value.\n\nDefault value: 0\n";
				std::cout << "Current saved value: " << MainConfigObj.bStartupRandomColourToggle << '\n';
				while (true) {
					inputnum = num("Please input new StartupRandomColour value (negative number to abort): > ");
					// exit while loop if inputted num is negative
					if (inputnum < 0) break;
					if (inputnum != 1 && inputnum != 0) {
						colour(12);
						std::cout << "This is a boolean value that can only take in an input of 1 (true) or 0 (false). Please try again.\n";
						colour(nLastColour);
					}
					else break;
				}
				// abort if inputted number is a negative number
				if (inputnum < 0) {
					colour(12);
					std::cout << "Aborting...\n";
					colour(nLastColour);
					break;
				}
	
				// Set new value in object
				MainConfigObj.bStartupRandomColourToggle = inputnum;
				// Update config file
				MainConfigObj.bObjectToFileUpdaterGlobal(MainConfigObj);
				// Finished
				colour(10);
				std::cout << "Done. Restart TerminalAppGen2 to see your changes!\n\n";
				colour(nLastColour);
				break;
			default:
				colour(12);
				std::cout << "It seems as if your setting choice was not found in the list of settings displayed.\nPlease try again.\n";
				colour(nLastColour);
		}
		}

		std::cin.ignore(INT_MAX, '\n');
		return;
	}
	// edison
	else if (command == "edison" || command == "38") {
	// edison command is in a separate function to be compatible with the wisecow command
	edison(false);
	return;
	}
	// einstein
	else if (command == "einstein" || command == "20") {
		// einstein command is in a separate function to be compatible with the wisecow command
		einstein(false);
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
	std::cout << "Loading...\n";

	// Initialise sRamTemp array members to ""
	for (int i = 0; i <= 99; i++) {
		sRamTemp[i] = "";
	}
	
	// Check if config file exists by creating checking stream early (before Config::bFileToObjectUpdaterGlobal())
	std::ifstream IsNew("tag2_config.ini");

	// Start the Config file-to-object updater to update scores at startup
	MainConfigObj.bFileToObjectUpdaterGlobal(MainConfigObj);

	// Set default colour option from settings
	nLastColour = MainConfigObj.nDefaultColour;

	// If settings wants a random colour on startup, overwrite nLastColour from previous step
	if (MainConfigObj.bStartupRandomColourToggle == true) {
		nLastColour = randnum(255, 0);
		colour(nLastColour);
		// Display a verbosity message about the new colour number
		spare = "Colour number has been set to " + nLastColour;
		VerbosityDisplay(spare);
	}

	// Activate the FileParse feature/method if requested to do so (through command-line arguments/program itself)
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

	std::cout << "Ready!\n";
	cls();

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
		std::cin.ignore(INT_MAX, '\n');
		if (input == 'y') {
			tutorial(false);
		}
		else {
			MainConfigObj.bCreateConfigFileGlobal(false);
			std::cout << "Ok, get ready!\n";
		}
	}
	IsNew.close();
	sleep(100);
	colour(randnum(15, 1));
	std::cout << "Press ENTER to begin...\n";
	// Protect from std::cin.get() conflicting with an upcoming getline() function call 
	// while still doing the same 'press enter to begin' task
	std::cin.ignore(INT_MAX, '\n');

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
	while (true) {
		if (command == "nodisp" || command == "41") { 
			if (nodisp == true) {
				colour(randnum(15, 1));
				std::cout << "NODISP is already on.\n";
			} else std::cout << "NODISP activated!\n";
			nodisp = true; 
		} else if (command == "disp" || command == "42") { 
			if (nodisp == false) {
				colour(randnum(15, 1));
				std::cout << "DISP is already on.\n";
			} else std::cout << "DISP activated!\n";
			nodisp = false; 
		}

		colour(nLastColour);
		if (nodisp == false) {
			std::cout << "Command: > ";
		}
		if (nLastColour < 16) {
			colour(14);
		}

		// Request command input
		command[6] = ' ';
		previnput = sCommandRAW;
		getline(std::cin, command);
		if (command == "") continue;

		colour(nLastColour);

		// If command doesn't equal echo, convert all characters to lowercase, else convert only the echo parts of the command.
		sCommandRAW = command;
		if (command[0] != 'e' && command[1] != 'c' && command[2] != 'h' && command[3] != 'o') {
			for (int i = 0; i < command.length(); i++) { command[i] = tolower(command[i]); }
		}
		else for (int i = 0; i < 5; i++) { command[i] = tolower(command[i]); }

		// Colour command check
		if (command != "colour" && command[6] != ' ') colcheck = true; else colcheck = false;
		// If command equals exit, return 0
		if (command == "exit" || command == "2" || command == "goodbye") {
			// Check if there are notes present so user doesn't lose them on exit
			if (bTempNotesPresent == true) {
				std::cerr << "There are still saved temporary notes in this program. If you exit, those notes will be lost.\nProceed to exit? [y/n] > ";
				colour(14);
				std::cin >> input;
				std::cin.ignore();
				if (input != 'y') {
					colour(12);
					std::cout << "Aborted.\n";
					colour(nLastColour);
					continue;
				}
			}
			colour(randnum(15, 1));
			slowcharfn(false, "Bye-bye!");
			sleep(100);
			return 0;
		}

		// increment the nNumberOfUsrCommands variable to make CommandNum command work
		nNumofUsrCommands++;

		// Only call the command function if command has something in it, else go back to the input statement above
		commands();
	}
	return 0;
}

bool fileParseFeatureFn(std::string sFileName) {
	bool bSuccess = true;
	bool nodisp = false;
	long long int nMainNumOfFileLines = 0;
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
		// Count to file number
		while (nMainNumOfFileLines <= nNumOfFileParseLines) {
			getline(fileParseReadIn, spare);
			nMainNumOfFileLines++;
		}
		// Begin read from file to command buffer
		previnput = sCommandRAW;
		command[6] = ' ';
		getline(fileParseReadIn, command);

		// A few commands that are required to be external from the commands() function
		if (command == "") continue;
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
			if (nLastColour < 16) {
				colour(14);
			}
			std::cout << command << '\n';
		}
		colour(nLastColour);


		// If command doesn't equal echo, convert all characters to lowercase, else convert only the echo parts of the command.
		sCommandRAW = command;
		if (command[0] != 'e' && command[1] != 'c' && command[2] != 'h' && command[3] != 'o') {
			for (int i = 0; i < command.length(); i++) { command[i] = tolower(command[i]); }
		}
		else for (int i = 0; i < 5; i++) { command[i] = tolower(command[i]); }

		// Colour command check
		if (command != "colour" && command[6] != ' ') colcheck = true; else colcheck = false;
		// Exit command
		if (command == "exit" || command == "2" || command == "goodbye") {
			colour(randnum(15, 1));
			slowcharfn(false, "Bye-bye!");
			sleep(100);
			// Reset bIsRunFromFile
			bIsRunFromFile = false;
			nNumOfFileParseLines = 1;
			return true;
		}

		// Only call the command function if command has something in it, else go back to the input statement above
		commands();

		nNumOfFileParseLines++;
		nMainNumOfFileLines++;
	}

	return bSuccess;
}

void edison(bool bWiseCowActivated) {
	short int		edison;

	// Randomly pick number
	edison = randnum(50, 1);
	std::cout << "\n\"";

	switch (edison) {
	case 1:
		std::cout << "Our greatest weakness lies in giving up. The most certain way to succeed is always to try just one more time.";
		break;
	case 2:
		std::cout << "I have not failed. I've just found 10,000 ways that won't work.";
		break;
	case 3:
		std::cout << "Having a vision for what you want is not enough... Vision without execution is hallucination.";
		break;
	case 4:
		std::cout << "Opportunity is missed by most people because it is dressed in overalls and looks like work.";
		break;
	case 5:
		std::cout << "There is a better way for everything. Find it.";
		break;
	case 6:
		std::cout << "The successful person makes a habit of doing what the failing person doesn't like to do.";
		break;
	case 7:
		std::cout << "Success is 90% perspiration and 10% inspiration.";
		break;
	case 8:
		std::cout << "I never view mistakes as failures. They are simply opportunities to find out what doesn't work.";
		break;
	case 9:
		std::cout << "Tomorrow is my exam but I don't care because a single sheet of paper can't decide my future.";
		break;
	case 10:
		std::cout << "Never go to sleep without a request to your subconscious.";
		break;
	case 11:
		std::cout << "Your worth consists in what you are and not in what you have.";
		break;
	case 12:
		std::cout << "The greatest invention in the world is the mind of a child.";
		break;
	case 13:
		std::cout << "Each time you fail, you have eliminated another wrong option.";
		break;
	case 14:
		std::cout << "People are not remembered by how few times they fail, but by how often they succeed. Every wrong step is another step forward.";
		break;
	case 15:
		std::cout << "The value of an idea lies in the using of it.";
		break;
	case 16:
		std::cout << "Time is really the only capital that any human being has, and the only thing he can't afford to lose.";
		break;
	case 17:
		std::cout << "Restlessness is discontent and discontent is the first necessity of progress. Show me a thoroughly satisfied man and I will show you a failure.";
		break;
	case 18:
		std::cout << "What you are will show in what you do.";
		break;
	case 19:
		std::cout << "Do the thing and the power will come.";
		break;
	case 20:
		std::cout << "I find out what the world needs. Then I go ahead and try to invent it.";
		break;
	case 21:
		std::cout << "Every failure is a lesson learned about your strategy.";
		break;
	case 22:
		std::cout << "Before you reject an idea, find at least five good things about it.";
		break;
	case 23:
		std::cout << "Absorb ideas from every source.";
		break;
	case 24:
		std::cout << "A good intention, with a bad approach, often leads to a poor result.";
		break;
	case 25:
		std::cout << "There seems to be no limit to which some men will go to avoid the labor of thinking. Thinking is hard work.";
		break;
	case 26:
		std::cout << "When you have exhausted all possibilities, remember this - you haven't.";
		break;
	case 27:
		std::cout << "Many of life's failures are people who did not realize how close they were to success when they gave up.";
		break;
	case 28:
		std::cout << "I owe my success to the fact that I never had a clock in my workroom.";
		break;
	case 29:
		std::cout << "I have far more respect for the person with a single idea who gets there than for the person with a thousand ideas who does nothing.";
		break;
	case 30:
		std::cout << "If we did all the things we are capable of doing, we would literally astound ourselves.";
		break;
	case 31:
		std::cout << "Continued innovation is the best way to beat the competition.";
		break;
	case 32:
		std::cout << "Good fortune is what happens when opportunity meets with planning.";
		break;
	case 33:
		std::cout << "I didn't fail 1000 times. The light bulb was an invention with 1000 steps.";
		break;
	case 34:
		std::cout << "You can't realize your dreams unless you have one to begin with.";
		break;
	case 35:
		std::cout << "To get a great idea, come up with lots of them.";
		break;
	case 36:
		std::cout << "The trouble with most people is that they quit before they start.";
		break;
	case 37:
		std::cout << "The doctor of the future will no longer treat the human frame with drugs, but rather will cure and prevent disease with nutrition.";
		break;
	case 38:
		std::cout << "A teacher sent the following note home with a six-year-old boy 'He is too stupid to learn.' That boy was Thomas A. Edison.";
		break;
	case 39:
		std::cout << "I never perfected an invention that I did not think about in terms of the service it might give others... I find out what the world needs, then I proceed to invent.";
		break;
	case 40:
		std::cout << "Fools call wise men fools. A wise man never calls any man a fool.";
		break;
	case 41:
		std::cout << "I failed my way to success.";
		break;
	case 42:
		std::cout << "Most of my ideas belonged to other people who never bothered to develop them.";
		break;
	case 43:
		std::cout << "The strength of the constitution lies in the will of the people to defend it.";
		break;
	case 44:
		std::cout << "I'd put my money on the sun and solar energy. What a source of power! I hope we don't have to wait until oil and coal run out before we tackle that.";
		break;
	case 45:
		std::cout << "Non-violence leads to the highest ethics, which is the goal of all evolution. Until we stop harming all other living beings, we are still savages.";
		break;
	case 46:
		std::cout << "The most necessary task of civilization is to teach people how to think. It should be the primary purpose of our public schools. The mind of a child is naturally active, it develops through exercise. Give a child plenty of exercise, for body and brain. The trouble with our way of educating is that it does not give elasticity to the mind. It casts the brain into a mold. It insists that the child must accept. It does not encourage original thought or reasoning, and it lays more stress on memory than observation.";
		break;
	case 47:
		std::cout << "To do much clear thinking a person must arrange for regular periods of solitude when they can concentrate and indulge the imagination without distraction.";
		break;
	case 48:
		std::cout << "The world owes nothing to any man, but every man owes something to the world.";
		break;
	case 49:
		std::cout << "It's obvious that we don't know one millionth of one percent about anything.";
		break;
	case 50:
		std::cout << "To invent, you need a good imagination and a pile of junk.";
		break;
	default:
		std::cerr << "Oops, sorry! An error occured with this command/module.\n";
		return;
	}
	// Append 'Thomas Edison' to the end
	std::cout << "\"";
	colour(randnum(15, 1));
	std::cout << "\n-Thomas Edison\n";
	if (bWiseCowActivated == false) std::cout << '\n';
	colour(nLastColour);
	return;
}

void einstein(bool bWiseCowActivated) {
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
	if (bWiseCowActivated == false) std::cout << '\n';
	colour(nLastColour);
	return;
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