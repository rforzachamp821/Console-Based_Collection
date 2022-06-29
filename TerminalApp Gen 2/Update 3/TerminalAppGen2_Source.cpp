
//////////////////////////////////////////////////////////////////
/* TERMINALAPP GEN2- This is the second generation of the TerminalApp 
series of programs.
Any code in this program is allowed to be reproduced and used for any kind of purpose.
This program is open source. You may take any code without permission.
------------Have fun programming!---------------
*/
/////////////////////////////////////////////////////////////////

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

// Global variables
std::string		command;
std::string		previnput;
std::string		coltxt;
std::string		spare;
std::string		filename;
std::string		text;

long long int	hertz;
long long int	milsecs;
long long int	letters = 1;
long long int	words = 1;
long long int	chars = 1;

int				coloption;
int				lastcolour = 7; // 7 because hex number 7 is white
int				inputnum;

short int		ansicol = 0;
short int		einstein;

char			input;

// Game variables
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

char			mycar1[4][4] = { ' ', '±', '±', ' ', '±', '±', '±', '±', ' ', '±', '±', ' ', '±', '±', '±', '±' };
char			mycar2[4][4] = { ' ', '|', '|', ' ', '0', '*', '*', '0', '|', '*', '*', '|', '0', '_', '_', '0' };
char			mycar3[4][4] = { '/', 'T', 'T', '\\', '|', '_', '_', '|', '|', ' ', ' ', '|', '/', '\\', '/', '\\' };
char			mycar4[4][4] = { '_', '=', '=', '_', '±', '|', '|', '±', '|', '0', '0', '|', '±', ')', '(', '±' };
char			mycar5[4][4] = { '0', '*', '*', '0', '*', '|', '|', '*', '*', '%', '%', '*', '0', '=', '=', '0' };

// Other variables
HANDLE			console = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE			h;

COORD			CursorPosition;
COORD			v4;

CHAR_INFO		v3;
SMALL_RECT		v5;

// Function to put console to sleep so makes programming job easier
void sleep(int ms) { std::this_thread::sleep_for(std::chrono::milliseconds(ms)); }

// Function to initialise text colour so makes programming job easier
void colour(int colnum) { SetConsoleTextAttribute(console, colnum); }

// Function for outputting characters slowly with the same colour
void slowcolourfn(short int colo, std::string slowchar) {
	int size;
	size = 0;
	size = slowchar.size();
	colour(colo);
	for (int i = 0; i <= size; i++) {
		sleep(50);
		std::cout << slowchar[i];
	}
	colour(7);
}

// Function for outputting characters slowly
void slowcharfn(bool nline, std::string slowchar) {
	int size;
	size = 0;
	size = slowchar.size();
	for (int i = 0; i <= size; i++) {
		sleep(50);
		std::cout << slowchar[i];
	}
	if (nline == true) {
		std::cout << std::endl;
	}
}

// Clear screen function (Flushes screen buffer)
void cls() {

	CONSOLE_SCREEN_BUFFER_INFO v6;
	if ((h = (HANDLE)GetStdHandle(0xFFFFFFF5), (unsigned int)GetConsoleScreenBufferInfo(h, &v6)))
	{
		v5.Right = v6.dwSize.X;
		v5.Bottom = v6.dwSize.Y;
		v3.Char.UnicodeChar = 32;
		v4.Y = -v6.dwSize.Y;
		v3.Attributes = v6.wAttributes;
		v4.X = 0;
		*(DWORD*)&v5.Left = 0;
		ScrollConsoleScreenBufferW(h, &v5, 0, v4, &v3);
		v6.dwCursorPosition = { 0 };
		HANDLE v1 = GetStdHandle(0xFFFFFFF5);
		SetConsoleCursorPosition(v1, v6.dwCursorPosition);
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

// Function to choose random number
int randnum(long long int max, long long int min) {
	if (max == 0) max = 1;
	srand((unsigned)time(0));
	int randnum = rand() % (max - min + 1) + min;
	return randnum;
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
		<< "14. Prev- Output your most previous input into the command section of the terminal.\n15. About- Terminal credits and contributors, with an exclusive message.\n16. Colourtxt- Change the terminal text colour.\n"
		<< "17. Randcol- Randomly pick terminal text and background colour.\n18. Cow- Outputs a cow made with characters.\n19. Cowsay- Make a cow say whatever you want.\n20. Einstein- Output one of 50 Albert Einstein's quotes.\n"
		<< "21. Note- Permanently save notes and access them later.\n22. Shutdownsys- Shuts down your computer.\n23. Restartsys- Restarts your computer.\n24. Dundundun- Outputs a 'dundundun' sound.\n25. Saynote- Output notes that were saved using the note command.\n"
		<< "26. Memtest- Simple memory address checker.\n27. Ipinfo- Shows information about your IP.\n28. Cpustress- A simple single core cpu stress tester.\n";
	return;
}

// Tutorial function
void tutorial(bool FromTerminal) {
	cls();
	slowcharfn(true, "Using this terminal is very straight-forward.");
	slowcharfn(true, "On the starting screen of the terminal, you can always type 'help' in the terminal to see all commands and what they do.");
	sleep(500);
	for (;;) {
		slowcharfn(false, "Type 'help' without quotes to see what happens: > ");
		command = "";
		colour(14);
		std::cin.clear();
		getline(std::cin, command);
		colour(7);
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
			std::cin.ignore();
			std::cout << "Try again.\n";
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
	colour(7);
	sleep(500);
	for (;;) {
		slowcharfn(false, "Please input the number next to the option 'Blue': > ");
		command = "";
		colour(14);
		std::cin.clear();
		getline(std::cin, command);
		colour(7);
		if (command == "1") {
			colour(10);
			std::cout << "Correct!\n\n";
			break;
		}
		else {
			// std::cin.clear used to flush text input buffer (obviously for error handling)
			std::cin.clear();
			colour(12);
			std::cout << "Try that again.";
			colour(7);
			std::cout << "\n";
		}
	}
	for (;;) {
		colour(7);
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
			std::cout << "Try that again.";
			colour(7);
			std::cout << "\n";
		}
	}
	for (;;) {
		colour(7);
		slowcharfn(false, "Please input the number next to the option 'Black': > ");
		command = "";
		colour(14);
		std::cin.clear();
		getline(std::cin, command);
		if (command == "2") {
			colour(47);
			std::cout << "Congrats, you've passed the tutorial!";
			colour(7);
			std::cout << "\nYou are now ready to use the terminal.\n\n";
			break;
		}
		else {
			std::cin.clear();
			colour(12);
			std::cout << "Try that again.";
			colour(7);
			std::cout << "\n";
		}
	}
	// Return to the next line after this function was called
	return;
}

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
		std::cout << "Input number here : > ";
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
				std::ofstream scoreguesshardout("scoreguesshard.confi");
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

void gotoxy(int x, int y) {
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}

void setcursor(bool visible, DWORD size) {
	if (size == 0) {
		size = 20;
	}
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console, &lpCursor);
}

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

void genEnemy(int ind) {
	enemy_x[ind] = 17 + rand() % (33);
}

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

void eraseEnemy(int ind) {
	if (enemyF[ind] == 1) {
		gotoxy(enemy_x[ind], enemy_y[ind]); std::cout << "    ";
		gotoxy(enemy_x[ind], enemy_y[ind] + 1); std::cout << "    ";
		gotoxy(enemy_x[ind], enemy_y[ind] + 2); std::cout << "    ";
		gotoxy(enemy_x[ind], enemy_y[ind] + 3); std::cout << "    ";
	}
}

void resetEnemy(int ind) {
	eraseEnemy(ind);
	enemy_y[ind] = 1;
	genEnemy(ind);
}

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

void eraseCar() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			gotoxy(j + carPos, i + 22); std::cout << " ";
		}
	}
}

int collision() {
	if (enemy_y[0] + 4 >= 22) {
		if (enemy_x[0] + 4 - carPos >= 0 && enemy_x[0] + 4 - carPos < 9) {
			return 1;
		}
	}
	return 0;
}

void gameoverhard() {
	cls();

	if (highscorecarhard < scorecar) {
		highscorecarhard = scorecar;
		std::cout << "\t\tNew high score!!!\n\n";
	}
	std::ofstream scorecarhardout("TerminalApp_Temp\\scorecardodgehard.confi");
	scorecarhardout << "";
	scorecarhardout << highscorecarhard;
	scorecarhardout.close();

	std::cout << std::endl;
	std::cout << "\t\t--------------------------" << std::endl;
	std::cout << "\t\t-------- Game Over -------" << std::endl;
	std::cout << "\t\t--------------------------" << std::endl;
	std::cout << "\t\tYou crashed!\n\t\tYou racked up " << scorecar << " points in this round.\n\nPress ENTER to go back to menu... ";
	scorecar = 0;
	std::cin.clear();
	std::cin.get();
	return;
}

void gameovereasy() {
	cls();

	if (highscorecareasy < scorecar) {
		highscorecareasy = scorecar;
		std::cout << "\t\tNew high score!!!\n\n";
	}
	std::ofstream scorecareasyout("TerminalApp_Temp\\scorecardodgeeasy.confi");
	scorecareasyout << "";
	scorecareasyout << highscorecareasy;
	scorecareasyout.close();

	std::cout << std::endl;
	std::cout << "\t\t--------------------------" << std::endl;
	std::cout << "\t\t-------- Game Over -------" << std::endl;
	std::cout << "\t\t--------------------------" << std::endl;
	std::cout << "\t\tYou crashed!\n\t\tYou racked up " << scorecar << " points in this round.\n\nPress ENTER to go back to menu... ";
	scorecar = 0;
	std::cin.clear();
	std::cin.get();
	return;
}

void updateScore() {
	gotoxy(WIN_WIDTH + 7, 5);
	std::cout << "Points: " << scorecar << std::endl;
}

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
	std::cin.get();
}

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

	gotoxy(WIN_WIDTH + 7, 2); std::cout << "Car Game";
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

		sleep(50);
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
		std::cout << "Please input your option: > ";
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
		std::cin >> input;

		if (input == '1') {
			play();
		}
		else if (input == '2') changecar();
		else if (input == '3') {
			colour(32778);
			std::cout << "Current high score: " << highscorecareasy << ".";
			colour(47);
			std::cout << "\nPress ENTER to continue...";
			std::cin.ignore();
			std::cin.get();
		}
		else if (input == '4') instructions();
		else if (input == '5') {
			CarType = 1;
			colour(lastcolour);
			cls();
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			return;
		}
		else {
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

	gotoxy(WIN_WIDTH + 7, 2); std::cout << "Car Game";
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
	std::cout << " 0**0";
	gotoxy(18, 3);
	std::cout << " *||*";
	gotoxy(18, 4);
	std::cout << " *%%*";
	gotoxy(18, 5);
	std::cout << " 0==0";
	gotoxy(18, 7);
	std::cout << "Army Car";
	gotoxy(0, 9);
	std::cout << "[1] Standard\n[2] F1 Car\n[3] Army Car\n";
	do {
		std::cout << "Please input your option: > ";
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

void cardodgehard() {

	std::ifstream scorecarhardin("TerminalApp_Temp\\scorecardodgehard.confi");

	if (scorecarhardin.fail()) {
		std::ofstream scorecarhard("TerminalApp_Temp\\scorecardodgehard.confi");
		scorecarhard << "0";
		scorecarhard.close();
	}
	scorecarhardin >> highscorecarhard;
	scorecarhardin.close();

	srand((unsigned)time(nullptr));

	do {
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
		std::cin >> input;

		if (input == '1') {
			playhard();
		}
		else if (input == '2') changecarhard();
		else if (input == '3') {
			colour(32780);
			std::cout << "Current high score: " << highscorecarhard << ".";
			colour(47);
			std::cout << "\nPress ENTER to continue...";
			std::cin.ignore();
			std::cin.get();
		}
		else if (input == '4') instructions();
		else if (input == '5') {
			CarType = 1;
			colour(lastcolour);
			cls();
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			return;
		}
		else {
			Beep(500, 250);
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
		}

	} while (true);

	return;
}

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
	// help
	if (command == "help" || command == "0") {
		help();
	}
	// tutorial
	else if (command == "tutorial" || command == "1") {
		tutorial(true);
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
	}
	// coltxt
	else if (command == "coltxt" || command == "10") {
		if (lastcolour == 7) colour(3);
		slowcharfn(false, "Input text that you want to colour: > ");
		colour(14);
		std::cin.clear();
		getline(std::cin, coltxt);
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
		getline(std::cin, spare);
		colour(lastcolour);
		// A boolean is passed along with the function so it knows if you need a newline or not
		slowcharfn(false, spare);
		std::cout << std::endl;
		return;
	}
	// ansi
	else if (command == "ansi" || command == "12") {
		slowcharfn(true, "Welcome to the ANSI Editor.");
		colour(2);
		do {
			std::cout << "Please input your ANSI escape code without any square brackets or \\x1b (type 0 to exit, 1 for list of ansi codes): > ";
			getline(std::cin, spare);
			if (spare == "0") {
				std::cout << "Do you want to save your changes? [y/n] > ";
				std::cin.clear();
				getline(std::cin, spare);
				if (spare == "y") {
					ansicol = 1;
				}
				return;
			}
			if (spare == "1") {
				std::cout << "Graphics/Colours:\n\n0m - Reset all\n1m- Bold mode (Reset: 22m)\n2m- Dim/faint mode (Reset: 22m)\n3m- Italic mode (Reset: 23m)\n" 
							<< "4m- Underline mode (Reset: 24m)\n5m- Blinking mode (Reset: 25m)\n7m- Inverse/Reverse mode (Reset: 27m)\n8m- Hidden/Invisible mode (Reset: 28m)\n"
							<< "9m- Strikethrough mode (Reset: 29m)\n";
				spare = "a";
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

		std::cout << "This is a beeper. You must enter your hertz value that you want to output as sound,\nand for how long you want to output it in milliseconds.\nInput anything other than a number at any time or 0 to exit.\n";
		// Do loop
		do {
			// Enter hertz number
			std::cout << "\nPlease input your hertz value (max that a teen can hear is 20000hz): > ";
			colour(14);
			std::cin >> hertz;
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
			std::cout << "Enter the number of milliseconds: > ";
			colour(14);
			std::cin >> milsecs;
			colour(lastcolour);

			// Clear input buffer if input failed
			while (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(INT_MAX, '\n');
				return;
			}

			// If hertz input is 0, exit program
			if (milsecs == 0) {
				std::cin.clear();
				std::cin.ignore();
				return;
			}

			// Creates colourful text saying that the sound is being outputted
			colourtxt(text);
			// Set the text colour to white again
			colour(7);
			// The 'Beep' function located in conio.h.
			Beep(hertz, milsecs);
			std::cout << "\nDone.\n\n";
		} while (true);
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
			std::cin >> sleeptime;
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
			sleep(sleeptime);
			return;
		// echo
	} else if (command[0] == 'e' && command[1] == 'c' && command[2] == 'h' && command[3] == 'o' || command == "13") {
		if (command == "13" || command[4] != ' ') {
			std::cout << "Input what you want me to output: > ";
			getline(std::cin, spare);
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
			getline(std::cin, text);
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
	std::cout << "Welcome to the Game Selector! Please choose your game (0 to exit):";
	// Here's an example of a numbered list in this program
	std::cout << "\n[1] Guess the number! (Easy)";
	std::cout << "\n[2] Guess the number! (Hard)";
	std::cout << "\n[3] Car Dodge (Easy)";
	std::cout << "\n[4] Car Dodge (Hard)";
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
	std::cout << "Previous input: ";
	colour(4);
	std::cout << "'" << previnput << "'";
	colour(lastcolour);
	std::cout << ".\n";
	return;
	}
	// about
	else if (command == "about" || command == "15") {
	std::cerr << "Enter password to access this command: > ";
	getline(std::cin, spare);
	if (spare != "(1)@32#845$*&~`") {
		std::cerr << "Incorrect password.\n";
		return;
	}
	cls();
	std::cout << "TerminalApp Gen 2-\n";
	colour(randnum(14, 1));
	slowcharfn(true, "The terminal PACKED with colours!");
	colour(7);
	slowcolourfn(randnum(14, 1), "Creator: ");
	std::cout << "Ryan Zorkot\n";
	sleep(500);
	slowcolourfn(randnum(14, 1), "Designer: ");
	std::cout << "Ryan Zorkot and Belal Abdelghany\n";
	sleep(500);
	slowcolourfn(70, "Massive thank-you to those who contributed to the project and gave feedback, such as my dad, school friends,\n");
	slowcolourfn(70, "and my dad's friends.\n");
	sleep(500);
	slowcolourfn(3, "This program was designed and tested on 2 different laptops, which are the Surface Book 3 and Dell Precision M4600.\n");
	std::cout << "A short message from the maker:\n";
	colour(7);
	return;
	}
	// colourtxt
	else if (command == "colourtxt" || command == "16") {
	coloption = 0;
	std::cout << "Text colour options:\n [1] Black\n [2] Blue\n [3] Green\n [4] Aqua\n [5] Red\n [6] Purple\n [7] Yellow\n [8] White\n [9] Grey\n[10] Light Blue\n"
		<< "[11] Light Green\n[12] Light Aqua\n[13] Light Red\n[14] Light Purple\n[15] Light Yellow\n[16] Bright White\n";
	do {
	std::cout << "Please input your option here: > ";
	colour(14);
	std::cin >> coloption;
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
	} while(true);

	coloption--;
	colour(coloption);
	lastcolour = coloption;
	// ansicol set to 0 to show that this is not an ansi colour
	ansicol = 0;
	std::cin.ignore();
	return;
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
	else if (command == "cowsay" || command == "19") {
	int numofchars;
	std::cout << "Please input what you want Henry the Cow to say: > ";
	colour(14);
	getline(std::cin, spare);
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
	else if (command == "restartsys" || command == "23") {
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
	else if (command == "shutdownsys" || command == "22") {
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
		Beep(1568, 250);
		Beep(1568, 250);
		Beep(1568, 250);
		Beep(1245, 1250);
		Beep(1397, 250);
		Beep(1397, 250);
		Beep(1397, 250);
		Beep(1175, 1250);
		return;
	}
	// saynote
	else if (command == "saynote" || command == "25") {
	std::ifstream filee("TerminalApp_Temp\\Notes.txt");
	if (filee.fail()) {
		colour(12);
		std::cerr << "The Notes.txt file is unavailable for access.\n";
		colour(lastcolour);
		std::cerr << "Please make sure you have written any notes or you haven't deleted the ";
		colour(11);
		std::cerr << "Notes.txt";
		colour(lastcolour);
		std::cerr << " file.\n";
		filee.close();
		return;
	}
	// output notes here
	std::cout << "\nNotes:\n\n";
	spare = "";
	for (int i = 1; std::getline(filee, spare); i++) {
		std::cout << i << ": " << spare << std::endl;
	}
	// output that there are no more notes to show
	colour(12);
	std::cerr << "\nThere are no more notes to show.\n";
	colour(lastcolour);
	}
	// memtest
	else if (command == "memtest" || command == "26") {
	// variable 1
	long long int* memtest = new long long int;
	slowcharfn(true, "Welcome to MemTest.");
	do {
	std::cout << "Please enter a number: > ";
	colour(14);
	std::cin >> *memtest;
	colour(lastcolour);
	if (std::cin.fail()) {
		std::cin.clear();
		std::cin.ignore(INT_MAX, '\n');
		std::cerr << "Try that again.\n";
	}
	else break;
	} while (true);
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
	std::cin.clear();
	std::cin.ignore(INT_MAX, '\n');
	return;
	}
	// ipinfo
	else if (command == "ipinfo" || command == "27") {
	system("ipconfig /all");
	return;
	}
	// cpustress
	else if (command == "cpustress" || command == "28") {
	std::cout << "Welcome to the CPU Stress test.\nThis is a simple single core stress test.\nPress any key in the middle of the stress test to exit.\n\nPress ENTER to continue...";
	std::cin.clear();
	std::cin.get();

	// stress test part
	while (true) {
		// kbhit is used to see if a key was pressed
		if (_kbhit()) {
			std::cout << "Stress test terminated. Exiting...\n";
			std::cin.ignore();
			return;
		}
	}
	}
	// einstein
	else if (command == "einstein" || command == "20") {
			einstein = randnum(50, 0);
			std::cout << '\n';
			switch (einstein)
			{
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
			}
			std::cout << " -Albert Einstein\n";
			std::cout << '\n';
			return;
	}
	// incorrect input part
	else {
		previnput = command;
		colour(12);
		std::cerr << "Sorry, your input was incorrect. Please try again (this terminal is case sensitive by the way).\n";
		colour(2);
		std::cerr << "Previous input: ";
		colour(4);
		std::cerr << "'" << previnput << "'";
		colour(2);
		std::cerr << "'.\n";
		colour(7);
		return;
	}
	return;
}

// Main
int main() {
	// Create fstream stream
	std::ifstream IsNew("TerminalApp_Temp\\IsNew.confi");
	colour(randnum(14, 1));
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
			IsOld << "Has completed enough of the tutorial.";
			IsOld.close();
			std::cout << "Ok, get ready!\n";
		}
	}
	IsNew.close();
	sleep(100);
	colour(randnum(14, 1));
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
	do {
		command = "";
		if (ansicol == 0) {
			if (lastcolour == 7) colour(7); else colour(lastcolour);
		}
		std::cout << "Command: > ";
		if (lastcolour < 16) {
			colour(14);
		}
		// Ask for command input
		getline(std::cin, command);
		colour(lastcolour);
		// If command doesn't equal echo, convert all characters to lowercase, else convert only the echo parts of the command.
		if (command[0] != 'e' && command[1] != 'c' && command[2] != 'h' && command[3] != 'o') {
			for (int i = 0; i < command.length(); i++) { command[i] = tolower(command[i]); }
		}
		else for (int i = 0; i < 5; i++) { command[i] = tolower(command[i]); }

		// If command equals exit, return 0
		if (command == "exit" || command == "2") {
			colour(3);
			slowcharfn(false, "Bye-bye!");
			sleep(100);
			return 0;
		}
		// Only call the command function if command has something in it, else go back to the input statement above
		if (command != "") commands();
		previnput = command;
	} while (true);
	return 0;
}