// Include libraries
#include <iostream>
#include <string>

// Declare variables
std::string text = "";
long long int letters = 1;
long long int words = 1;
long long int chars = 1;

int main() {
	std::cout << "Welcome!\n\n";

	for (;;) {
		// Ask for text input

		std::cout << "Input 0 to exit at any time. \nPlease input the string of text that you want to see statistics of: > ";
		std::cin.clear();
		getline(std::cin, text);
		if (text == "0") {
			return 0;
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
	std::cout << "Statistics:\nLetters: " << letters << "\nTotal number of characters: " << chars << "\nWords: " << words << std::endl;
	// Zero out variables back to normal
	text = "";
	letters = 1;
	words = 1;
	chars = 1;
	}

}