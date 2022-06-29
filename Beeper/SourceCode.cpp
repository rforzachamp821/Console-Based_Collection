// Beeper- Take input of hertz and length of time and output it as sound
//
#include <iostream>
#include <windows.h>

// A handle for colours
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

int main() {
	// Declare variables
	// 
	// Char variable saying that sound is being outputted
	char text[40] = "Your sound is being outputted!!!";
	// Hertz variable
	long long int hertz;
	// Time variable
	long long int milsecs;
	std::cout << "Welcome!\nThis is a beeper. You must enter your level of Hertz that you want to output as sound,\nand for how long you want to output it in milliseconds.\n";
	// 'Do' loop so people don't have to click the application every time
	do {
	std::cout << "Input anything other than a number at any time or 0 to exit the application.\n";
	// Enter hertz number
	std::cout << "\nPlease input your Herts value (max that the average teenager can hear is 20000hz): > ";
	std::cin >> hertz;

	// If hertz input is 0, exit program
	if (hertz == 0) {
		// Exit do loop which leads to it exiting the application because 'return 0' is on the other side
		break;
	}
	// Enter milliseconds number
	std::cout << "Enter the number of milliseconds: > ";
	std::cin >> milsecs;

	// If hertz input is 0, exit program
	if (milsecs == 0) {
		// Exit do loop which leads to it exiting the application because 'return 0' is on the other side
		break;
	}

	// Creates colourful text saying that the sound is being outputted
	for (int i = 0, a = 1; i < 40; i++, a++) {
		if (a == 15) {
			a = 1;
		}
		SetConsoleTextAttribute(hConsole, a);
		std::cout << text[i];
	}
	// Set the text colour to white again, which is hex no. 7
	SetConsoleTextAttribute(hConsole, 7);
	// The 'Beep' function located in conio.h allows you to beep for as many herts as you want and for as much time as you want.
	Beep(hertz, milsecs);
	std::cout << "\nDone.\n\n";
} while (true);
	// Exit application
	std::cout << "Bye-bye!\n";
	Sleep(300);
	return 0;
}