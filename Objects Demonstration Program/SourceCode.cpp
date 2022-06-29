#include <iostream>
#include <string>

// Declare input variables
std::string namein;
int idnumin;
char input;


// Class Test- Houses all the components for displaying and creating objects.
class Test {
protected:
    // Just a demonstration of how protected members work
    bool contine;
public:
    // Data Members
    static int BaseIdNum;
    int idnum;
    int number;
    char letter;
    double floaty;
    std::string name;

    // Constuctors
    // 
    // Main Test constructor- made for declaring an object with a name and/or an ID
    Test(std::string newname, int newidnum) {
        if (newidnum == 0) {
            std::cout << "Constructing a Test object named " << newname << " and a sequential ID number...\n";
            std::cout << "Initialising all data members in " << newname << "...\n";
            // Zero out the variables
            number = 0;
            letter = '0';
            floaty = 0.0;
            name = newname;
            // Increment BaseIdNum by 1 to create a valid ID
            BaseIdNum++;
            // Fill that ID into idnum
            idnum = BaseIdNum;
        }
        else {
            std::cout << "Constructing a Test object with the name " << newname << " and a non-sequential ID Number...\n";
            // Fill newidnum's info into idnum
            idnum = newidnum;
            // Same goes for newname and name
            name = newname;
            // Zero out the variables
            number = 0;
            letter = '0';
            floaty = 0.0;
        }
    }
    // Standard Test constructor- zeroes out all variables and puts the name as 'Unknown'.
    //                            Used to also demonstrate constuctor overloading.
    Test() {
        std::cout << "Constucting an unknown Test object with no name and with a sequential ID number...\n";
        name = "Unknown";
        number = 0;
        letter = '0';
        floaty = 0.0;
        BaseIdNum++;
        idnum = BaseIdNum;
    }

    // Test destructor: Just announces that a Test object is being destroyed.
    ~Test() {
        std::cout << "Destroying the HECK out of a Test object...\n";
    }

    // Functions
    //
    // fnheap()- Function made to create a heap object and return it as a copy.
    Test fnheap() {
        // Inputs name using getline()
        std::cout << "Please enter a name: > ";
        std::cin.ignore();
        getline(std::cin, namein);
        // Asks for ID
        std::cout << "Would you like to enter an ID? [y/n] > ";
        std::cin >> input;
        // if input shows 'y', zero out the input ID variable so it can be ready for the ID input and object creation.
        if (input == 'y') {
            idnumin = 0;
            std::cout << "Ok.\n";
        }
        // else skip the process of asking for ID (and zero out idnumin so it can respond to the main test constuctor).
        else {
            idnumin = 0;
            std::cin.clear();
            std::cin.ignore();
            goto noid;
        }
        std::cout << "Please enter an ID number with a max length of 9 numbers: > ";
        std::cin >> idnumin;
    noid:
        // Create object named 'objheap'
        Test objheap(namein, idnumin);
        // Now ask for the data to be filled in for the object
        std::cout << "Enter a number: > ";
        std::cin >> objheap.number;
        std::cout << "Enter a letter: > ";
        std::cin >> objheap.letter;
        std::cout << "Enter a floating-point number: > ";
        std::cin >> objheap.floaty;
        std::cout << "Exiting fnheap...\n";
        // And then return the object at the end to make the caller object have the correct data.
        return objheap;
    }
    // fnstandard()- Function made to create a heap object and return it as a copy.
    Test fnstandard() {
        // Inputs name using getline()
        std::cout << "Please enter a name: > ";
        std::cin.ignore();
        getline(std::cin, namein);
        // Asks for ID
        std::cout << "Would you like to enter an ID? [y/n] > ";
        std::cin >> input;
        // if input shows 'y', zero out the input ID variable so it can be ready for the ID input and object creation.
        if (input == 'y') {
            idnumin = 0;
            std::cout << "Ok.\n";
        }
        // else skip the process of asking for ID (and zero out idnumin so it can respond to the main test constuctor).
        else {
            std::cin.clear();
            std::cin.ignore();
            goto noid;
        }
        std::cout << "Please enter an ID number with a max length of 9 numbers: > ";
        std::cin >> idnumin;
    noid:
        // Create object named 'objstand'
        Test objstand(namein, idnumin);
        // Now ask for the data to be filled in for the object
        std::cout << "Enter a number: > ";
        std::cin >> objstand.number;
        std::cout << "Enter a letter: > ";
        std::cin >> objstand.letter;
        std::cout << "Enter a floating-point number: > ";
        std::cin >> objstand.floaty;
        std::cout << "Exiting fnstandard...\n";
        // And then return the object at the end to make the caller object have the correct data.
        return objstand;
    }

    Test fnunknown() {
        // As no name or ID is required, an object with no arguments is made. This is required for this function.
        Test objunk;
        // Now ask for the data to be filled in for the object
        std::cout << "Enter a number: > ";
        std::cin >> objunk.number;
        std::cout << "Enter a letter: > ";
        std::cin >> objunk.letter;
        std::cout << "Enter a floating-point number: > ";
        std::cin >> objunk.floaty;
        std::cout << "Exiting fnunknown...\n";
        // And then return the object at the end to make the caller object have the correct data.
        return objunk;
    }

};

// Static int variable for Test (this is BaseIdNum, made static so different outer functions can access it without declaring an objet to the class)
int Test::BaseIdNum = 1000;

// Main function- This is where program starts
int main() {
    // Objects are constructed before any other execution in this program
    std::cout << "Constucting Objects... {\n";
    Test obj;
    Test* objheap = new Test;
    Test objstan;
    Test objun;
    std::cout << "}\nConstuction finished.\n";
    int startin = 0;
    std::cout << "Welcome!\n";
    // 'do' loop for easy looping without any sort of goto usage
    do {
        // Ask for what to do first.
        std::cout << "Please choose if you want to initialise: \n1. Heap Test Object\n2. Standard Test Object\n3. Unknown Test Object\n4. Display Heap Test Object\n"
            << "5. Display Standard Test Object\n6. Display Unknown Test Object\n\n[0] Exit\n";
        std::cout << "Input here: > ";
        std::cin >> startin;
        // If statements corresponding for what to do, according to the number pointed at by the user (which is stores in the variable startin).
        // These if statements call the functions required for correctly initialising the objects in the program.
        if (startin == 1) {
            *objheap = obj.fnheap();
        }
        else if (startin == 2) {
            objstan = obj.fnstandard();
        }
        else if (startin == 3) {
            objun = obj.fnunknown();
        }
        else if (startin == 0) {
            std::cout << "Bye.\n";
            break;
        }
        // These are the 'display' if statements, which display the selected object's information, one by one.
        else if (startin == 4) {
            std::cout << "Results:\n\nName: " << objheap->name << "\nID: " << objheap->idnum << "\nNumber entered: " << objheap->number
                << "\nLetter entered: " << objheap->letter << "\nFloating-point number entered: " << objheap->floaty << "\nDone.\n\n";
        }
        else if (startin == 5) {
            std::cout << "Results:\n\nName: " << objstan.name << "\nID: " << objstan.idnum << "\nNumber entered: " << objstan.number
                << "\nLetter entered: " << objstan.letter << "\nFloating-point number entered: " << objstan.floaty << "\nDone.\n\n";
        }
        else if (startin == 6) {
            std::cout << "Results:\n\nName: " << objun.name << "\nID: " << objun.idnum << "\nNumber entered: " << objun.number
                << "\nLetter entered: " << objun.letter << "\nFloating-point number entered: " << objun.floaty << "\nDone.\n\n";
        }
        else {
            std::cout << "Sorry, the input that you placed was incorrect. Please try again.\n";
        }

    } while (true);
    // Delete heap object to save memory and initialise it as nullptr.
    delete objheap;
    objheap = nullptr;
    // Exit program.
    return 0;
}