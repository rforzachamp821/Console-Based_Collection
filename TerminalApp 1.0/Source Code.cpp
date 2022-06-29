#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <conio.h>
#include <string>
#include <windows.h>

using namespace std; // Makes it much easier; Sorry for those who are triggered.

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
string command, note, note2, note3, note4, note5;
short int exitprot = 0, gameprot, snote = 0;
char oper; // alot of variables lol

void system(std::string const& s) {
    std::system(s.c_str());
}

void guessgame()
{
    int number;
    int guess;
    char play;
    SetConsoleTextAttribute(hConsole, 30);
    system("cls");
    short int tries = 0;
    cout << "################### RYAN GAMES INC #####################################################################\n";
    cout << "Welcome to the Number Guessing Game! You have 10 tries to guess a number between 1 and 100. \n";
    cout << "If you get the answer wrong, the game will tell you if your answer was higher or lower than the actual one.\n";
    cout << "\n";
    cout << "Good luck!\n";
    cout << "Press any key to begin the game...\n";
    cout << "########################################################################################################\n";
    _getch();
qipd:
    number = (rand() % 100) + 1;
    system("cls");
    cout << "\n";
    cout << "Let the games begin!\n";
        Sleep(1000);
    b:
        system("cls");
        cout << "\n\n";
        cout << "Input your guess number here: > ";
        cin >> guess;
        if (guess > 100)
        {
            cout << "That guess was higher than 100. Please retry.";
            goto b;
        }
        if (guess == number) goto win;
        if (guess > number) {
            cout << "Wrong! Your guess was higher than the actual number.\nYou have used up " << tries << " tries out of 10.\n";
            tries++;
            _getch();
            goto b;
        }
        if (guess < number) {
            cout << "Wrong! Your guess was lower than the actual number.\nYou have used up " << tries << " tries out of 10.\n";
            tries++;
            _getch();
            goto b;
        }
        if (tries == 10) goto lose;

win:
        cout << "Congrats! You guessed the number in " << tries << " tries!\n";
        cout << "Would you like to play again? > ";
        cin >> play;
        while (cin.fail())
        {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cout << "Please retry.\n";
            goto win;
        }
        goto playcheck;

    lose:
        system("cls");
        cout << "Oh no! You have used up all of your 10 tries.\nYou lost! Would you like to play again?";
        cin >> play;
        while (cin.fail())
        {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cout << "Please retry.\n";
            goto win;
        }
        goto playcheck;

    playcheck:
        if (play == 'y' || play == 'Y')
        {
            goto qipd;
        }
        if (play == 'n' || play == 'N')
        {
            cout << "Ok, exiting...\n";
            SetConsoleTextAttribute(hConsole, 1);
            return;
        }
}

char* concatenateString(const char szSrc1[], const char szSrc2[])
{
    // allocate an array of sufficient length
    int nTargetSize = strlen(szSrc1) + strlen(szSrc2) + 1;
    char* pszTarget = new char[nTargetSize];

    // first copy the first string into the target
    int nT;
    for (nT = 0; szSrc1[nT] != '\0'; nT++)
    {
        pszTarget[nT] = szSrc1[nT];
    }

    // now copy the contents of the second string onto the end of the first
    for (int nS = 0; szSrc2[nS] != '\0'; nT++, nS++)
    {
        pszTarget[nT] = szSrc2[nS];
    }

    // add the terminator to szTarget
    pszTarget[nT] = '\0';

    // return results to caller
    return pszTarget;
}

string comresult(string a) //This is the command function. All commands and their functions are in here.
{
    if (a == "concatenate" || a == "Concatenate")
    {
        // Prompt user
        cout << "This program accepts two strings from the keyboard and outputs them concatenated together." << endl;

        // input two strings
        cout << "Enter first string: > ";
        char szString1[256];
        cin.getline(szString1, 256);

        cout << "Enter second string: > ";
        char szString2[256];
        cin.getline(szString2, 256);

        // now concatenate one onto the end of the other
        cout << "Concatenating second string onto first...\n";
        char* pszT = concatenateString(szString1, szString2);

        // display result
        cout << "Result: '" << pszT << "'\n";

        // return the memory to the heap
        delete[] pszT;
        pszT = nullptr;
        goto sd;
    }
    if (a == "colour" || a == "Colour" || a == "Colour " || a == "colour ")
    {
        short int col;
    asdg:
        cout << " 0- Black\n" << " 1- Blue\n" << " 2- Green\n" << " 3- Aqua\n" << " 4- Red\n" << " 5- Purple\n" << " 6- Yellow\n" << " 7- White\n" << " 8- Gray\n" << " 9. Light Blue\n" << "10. Light Green\n" << "11. Light Aqua\n" << "12. Light Red\n" << "13. Light Purple\n" << "14. Light Yellow\n" << "15. Bright White\n" << "Please input the number beside the colour that you want, or input any letter to exit. > ";
        cin >> col;
        while (cin.fail())
        {
            cin.clear();
            cin.ignore();
            goto sd;
        }
        if (col > 15)
        {
        cout << "Sorry, that input was not understood. Please try again.\n";
        goto asdg;
        }
       SetConsoleTextAttribute(hConsole,col);
       cout << "\nCommand Executed.\n";
       goto sd;
    }
    if (a == "memtest" || a == "Memtest" || a == "Memtest " || a == "memtest ")
    {
    qiw:
        long long int memtest;
        cout << "Welcome to MemTest. ";
        cout << "Please enter a number: > ";
        cin >> memtest;
        while (cin.fail())
        {
            cin.clear();
            cin.ignore();
            cout << "Please try again, with entering a number this time." << endl;
            goto qiw;
        }
        long long int* ip;
        ip = &memtest;
        cout << "Results:\n";
        cout << "Value of number inputted is : ";
        cout << memtest << endl;
        cout << "Value of memory address of number inputted is : ";
        cout << ip << endl;
        cout << "Value of pointer variable to number inputted is : ";
        cout << *ip << endl << endl;
        cout << "Execution complete.\n";
        goto sd;
    }
    if (a == "hello" || a == "Hello" || a == "Hello " || a == "hello ")
    {
        cout << "Hello, World!" << endl;
        goto sd;
    }
    if (a == "game" || a == "Game" || a == "Game " || a == "game ")
    {
    aie:
        cout << "Welcome to the Game Launchpad. Exit by inputting 0. Please choose a number:\n";
        cout << "1. Guessing Game\n ";
        cout << "Input here: > ";
        cin >> gameprot;
        while (cin.fail())
        {
            cin.clear();
            cin.ignore();
            cout << "Please try again with only a number." << endl;
            goto aie;
        }
        switch (gameprot)
        {
        case 1:
            gameprot = 1;
            cout << "Ok, travelling to \n";
            guessgame();
            break;
        case 0:
            cout << "Exiting...\n";
            gameprot = 0;
            break;
        default:
            cout << "Please try again with only the numbers mentioned." << endl;
            goto aie;
        }
        goto sd;
    }
    if (a == "wbrowse" || a == "Wbrowse")
    {
        cout << "Please input what website you would like to enter: > ";
        string web;
        getline(cin, web);
        system("start " + web);
        goto sd;
    }
    if (a == "openlst" || a == "Openlst" || a == "openlst " || a == "Openlst ")
    {
        cout << "Welcome to the Application Opener List. Please choose the app you would like to open:" << endl;
        cout << "1. Notepad\n2. WordPad\n3. Command Prompt\n4. File Explorer\n5. Paint\n6. Microsoft Edge \n0. Exit\n";
        short int openlst;
    vbsd:
        cout << "Please input your desired number: > ";
        cin >> openlst;
        while (cin.fail())
        {
            cin.clear();
            cin.ignore();
            cout << "Overload. Please try again." << endl;
            goto vbsd;
        }
        switch (openlst)
        {
        case 1:
            system("start notepad");
            break;
        case 2:
            system("start write");
            break;
        case 3:
            system("start cmd");
            break;
        case 4:
            system("start explorer");
            break;
        case 5:
            system("start mspaint");
            break;
        case 6:
            system("start msedge");
            break;

        case 0:
            goto sd;
        }
        goto vbsd;
    }
    if (a == "calc" || a == "Calc" || a == "calc " || a == "Calc ")
    {
        long long int no1, no2;
        cout << "Welcome to the custom terminal calculator. Input any letter to exit." << endl << "Allowed operations are: x X * - + / " << endl;
        for (;;)
        {
            cout << "Number: > ";
            cin >> no1;
            while (cin.fail())
            {
                cin.clear();
                cin.ignore();
                cout << "Exiting..." << endl;
                goto sd;
            }
            cout << "Operation (Type 'e' to exit): > ";
            cin >> oper;
            while (cin.fail())
            {
                cin.clear();
                cin.ignore();
                cout << "Exiting..." << endl;
                goto sd;
            }
            cout << "Number: > ";
            cin >> no2;
            while (cin.fail())
            {
                cin.clear();
                cin.ignore();
                cout << "Exiting..." << endl;
                goto sd;
            }

            switch (oper)
            {
            case '*':
                cout << "The answer is " << no1 * no2 << "." << endl;
                break;
            case '+':
                cout << "The answer is " << no1 + no2 << "." << endl;
                break;
            case '/':
                if (no2 == 0)
                {
                    cout << "Cannot divide by 0." << endl;
                    goto sd;
                }
                cout << "The answer is " << no1 / no2 << "." << endl;
                break;
            case '-':
                cout << "The answer is " << no1 - no2 << "." << endl;
                break;
            case 'x':
                cout << "The answer is " << no1 * no2 << "." << endl;
                break;
            case 'X':
                cout << "The answer is " << no1 * no2 << "." << endl;
                break;

            default:
                cout << "I did not understand that. Sorry!" << endl;
            }
        }
    }
    if (a == "saynote" || a == "Saynote" || a == "saynote " || a == "Saynote ")
    {
        if (snote == 0)
        {
            cout << "You have not saved any temporary notes. Please create a note by typing [note] so you can display one." << endl;
            goto sd;
        }
        cout << "Your notes:" << endl;
        cout << '\n' << note << endl << note2 << endl << note3 << endl << note4 << endl << note5 << endl << '\n';
        cout << "Operation complete." << endl;
        goto sd;
    }
    if (a == "note" || a == "Note" || a == "Note " || a == "note ")
    {
        short int i = 0;
        i++;
        cout << "Please set your notes. You can set a maximum of 5 notes at a time. Press 0 at any time to stop placing notes." << endl;
        cout << "They do not get saved permanently, and you will lose these notes when you leave this terminal." << endl;
            cout << "Note " << i << " : > ";
            getline(cin, note);
            if (note == "0")
            {
                note = "";
                goto sd;
            }
            snote++;
            i++;
            cout << "Note " << i << " : > ";
            getline(cin, note2);
            if (note2 == "0")
            {
                note2 = "";
                goto sd;
            }
            i++;
            cout << "Note " << i << " : > ";
            getline(cin, note3);
            if (note3 == "0")
            {
                note3 = "";
                goto sd;
            }
            i++;
            cout << "Note " << i << " : > ";
            getline(cin, note4);
            if (note4 == "0")
            {
                note4 = "";
                goto sd;
            }
            i++;
            cout << "Note " << i << " : > ";
            getline(cin, note5);
            if (note5 == "0")
            {
                note5 = "";
                goto sd;
            }
        goto sd;
    }
    if (a == "einstein" || a == "Einstein" || a == "einstein " || a == "Einstein ")
    {
        short int einstein;
        einstein = (rand() % 50) + 1;
        cout << '\n';
        switch (einstein)
        {
        case 1:
            cout << "Two things are infinite: the universe and human stupidity; and I'm not sure about the universe.";
            break;
        case 2:
            cout << "There are only two ways to live your life. One is as though nothing is a miracle. The other is as though everything is a miracle." ;
            break;
        case 3:
            cout << "I am enough of an artist to draw freely upon my imagination. Imagination is more important than knowledge. Knowledge is limited. Imagination encircles the world." ;
            break;
        case 4:
            cout << "If you can't explain it to a six year old, you don't understand it yourself." ;
            break;
        case 5:
            cout << "If you want your children to be intelligent, read them fairy tales. If you want them to be more intelligent, read them more fairy tales." ;
            break;
        case 6:
            cout << "Logic will get you from A to Z; imagination will get you everywhere." ;
            break;
        case 7:
            cout << "Life is like riding a bicycle. To keep your balance, you must keep moving." ;
            break;
        case 8:
            cout << "Anyone who has never made a mistake has never tried anything new." ;
            break;
        case 9:
            cout << "I speak to everyone in the same way, whether he is the garbage man or the president of the university." ;
            break;
        case 10:
            cout << "Never memorize something that you can look up." ;
            break;
        case 11:
            cout << "When you are courting a nice girl an hour seems like a second. When you sit on a red-hot cinder a second seems like an hour. That's relativity." ;
            break;
        case 12:
            cout << "A clever person solves a problem. A wise person avoids it." ;
            break;
        case 13:
            cout << "Science without religion is lame, religion without science is blind." ;
            break;
        case 14:
            cout << "Reality is merely an illusion, albeit a very persistent one." ;
            break;
        case 15:
            cout << "Any fool can know. The point is to understand." ;
            break;
        case 16:
            cout << "If we knew what it was we were doing, it would not be called research, would it?" ;
            break;
        case 17:
            cout << "I have no special talents. I am only passionately curious." ;
            break;
        case 18:
            cout << "A human being is a part of the whole called by us universe, a part limited in time and space. He experiences himself, his thoughts and feeling as something separated from the rest, a kind of optical delusion of his consciousness. This delusion is a kind of prison for us, restricting us to our personal desires and to affection for a few persons nearest to us. Our task must be to free ourselves from this prison by widening our circle of compassion to embrace all living creatures and the whole of nature in its beauty." ;
            break;
        case 19:
            cout << "Try not to become a man of success. Rather become a man of value." ;
            break;
        case 20:
            cout << "The important thing is not to stop questioning. Curiosity has its own reason for existence. One cannot help but be in awe when he contemplates the mysteries of eternity, of life, of the marvelous structure of reality. It is enough if one tries merely to comprehend a little of this mystery each day" ;
            break;
        case 21:
            cout << "The world as we have created it is a process of our thinking. It cannot be changed without changing our thinking." ;
            break;
        case 22:
            cout << "Once you can accept the universe as matter expanding into nothing that is something, wearing stripes with plaid comes easy." ;
            break;
        case 23:
            cout << "If I were not a physicist, I would probably be a musician. I often think in music. I live my daydreams in music. I see my life in terms of music." ;
            break;
        case 24:
            cout << "The most beautiful experience we can have is the mysterious. It is the fundamental emotion that stands at the cradle of true art and true science." ;
            break;
        case 25:
            cout << "I know not with what weapons World War III will be fought, but World War IV will be fought with sticks and stones." ;
            break;
        case 26:
            cout << "You never fail until you stop trying." ;
            break;
        case 27:
            cout << "Great spirits have always encountered violent opposition from mediocre minds." ;
            break;
        case 28:
            cout << "The measure of intelligence is the ability to change." ;
            break;
        case 29:
            cout << "Gravitation is not responsible for people falling in love." ;
            break;
        case 30:
            cout << "It is not that I'm so smart. But I stay with the questions much longer." ;
            break;
        case 31:
            cout << "Creativity is intelligence having fun." ;
            break;
        case 32:
            cout << "The world is a dangerous place to live, not because of the people who are evil, but because of the people who don't do anything about it." ;
            break;
        case 33:
            cout << "If A is a success in life, then A equals x plus y plus z. Work is x; y is play; and z is keeping your mouth shut." ;
            break;
        case 34:
            cout << "Black holes are where God divided by zero." ;
            break;
        case 35:
            cout << "Everything must be made as simple as possible. But not simpler." ;
            break;
        case 36:
            cout << "The best way to cheer yourself is to cheer somebody else up." ;
            break;
        case 37:
            cout << "When you trip over love, it is easy to get up. But when you fall in love, it is impossible to stand again." ;
            break;
        case 38:
            cout << "Whoever is careless with the truth in small matters cannot be trusted with important matters." ;
            break;
        case 39:
            cout << "What is right is not always popular and what is popular is not always right." ;
            break;
        case 40:
            cout << "Peace cannot be kept by force; it can only be achieved by understanding." ;
            break;
        case 41:
            cout << "In the middle of difficulty lies opportunity." ;
            break;
        case 42:
            cout << "A question that sometimes drives me hazy: am I or are the others crazy?" ;
            break;
        case 43:
            cout << "Imagination is everything. It is the preview of life's coming attractions.";
            break;
        case 44:
            cout << "The pursuit of truth and beauty is a sphere of activity in which we are permitted to remain children all our lives.";
            break;
        case 45:
            cout << "We dance for laughter, we dance for tears, we dance for madness, we dance for fears, we dance for hopes, we dance for screams, we are the dancers, we create the dreams.";
            break;
        case 46:
            cout << "The woman who follows the crowd will usually go no further than the crowd. The woman who walks alone is likely to find herself in places no one has ever been before." ;
            break;
        case 47:
            cout << "Time is an illusion." ;
            break;
        case 48:
            cout << "It would be possible to describe everything scientifically, but it would make no sense; it would be without meaning, as if you described a Beethoven symphony as a variation of wave pressure." ;
            break;
        case 49:
            cout << "I must be willing to give up what I am in order to become what I will be." ;
            break;
        case 50:
            cout << "Love is a better master than duty." ;
            break;
        }
        cout << " -Albert Einstein" << endl;
        cout << '\n';
        goto sd;
    }
    if (a == "cow" || a == "Cow" || a == "cow " || a == "Cow ")
    {
        cout << "  ^__^" << endl;
        cout << "  (OO)\_______     " << endl;
        cout << "  (__)\       )_/\\/\\" << endl;
        cout << "      ||----W |" << endl;
        cout << "      ||     ||" << endl;
        cout << '\n';
        cout << "Cow :)" << endl;
        goto sd;

    }
    if (a == "cowsay" || a == "Cowsay" || a == "cowsay " || a == "Cowsay ")
    {
        string po;
        cout << "Please input what you want Alfred the Cow to say: > ";
        getline(cin, po);
        cout << '\n';
        cout << '\n';
        cout << "<" << po << ">" << endl;
        cout << "    |" << endl;
        cout << "    |  ^__^" << endl;
        cout << "    -- (OO)\_______     " << endl;
        cout << "       (__)\       )_/\\/\\ " << endl;
        cout << "           ||----W |" << endl;
        cout << "           ||     ||" << endl;
        cout << '\n';
        cout << "Alfred is a good boy!" << endl;
        goto sd;

    }

    if (a == "cry" || a == "Cry" || a == "cry " || a == "Cry ")
    {
        system("color 16");
        for (int i = 0; i < 128; i++)
        {
            cout << "WAAAAAAAAAAAAAAHAHAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA :((((((((((((((((((((((((((((((((((((((((" << endl;
        }
        cout << "Press ENTER to continue..." << endl;
        cin.get();
        system("color 07");
        goto sd;

    }
    if (a == "")
    {
        goto sd;
    }
    if (a == "restartsys" || a == "Restartsys" || a == "restartsys " || a == "Restartsys ")
    {
        string shut;
    bn:
        cout << "Are you sure you want to restart your PC??? [y/n] >";
        cin >> shut;
        if (shut == "y")
        {
            cout << "Ok, restarting..." << endl;
            Sleep(500);
            system("shutdown -r -f -t 00");
        }
        else if (shut == "n")
        {
            cout << "Aborting..." << endl;
            goto sd;
        }
        else
        {
            cout << "Sorry, I did not understand that input. Please try again.";
            goto bn;
        }

        goto sd;
    }
    if (a == "shutdownsys" || a == "Shutdownsys" || a == "Shutdownsys " || a == "shutdownsys ")
    {
        string shut;
    bdn:
        cout << "Are you sure you want to shutdown your PC??? [y/n] >";
        getline (cin, shut);
        if (shut == "y")
        {
            cout << "Ok, shutting down..." << endl;
            Sleep(500);
            system("shutdown -s -f -t 00");
        }
        else if (shut == "n")
        {
            cout << "Aborting..." << endl;
            goto sd;
        }
        else
        {
            cout << "Sorry, I did not understand that input. Please try again." << endl;
            goto bdn;
        }

        goto sd;
    }
    if (a == "sleep" || a == "Sleep " || a == "sleep " || a == "Sleep")
    {
        long long int as;
        qpi:
        cout << "Please input your number of seconds to time. Maximum is 2 billion seconds. Input 0 to exit. >";

        as = 1;
        cin >> as;
        if (as == 0)
        {
            cout << "Exiting..." << endl;
            goto sd;
        }
        while(cin.fail())
        {
            cin.clear();
            cin.ignore();
            cout << "Sorry, i do not accept characters. Please try again." << endl;
            goto qpi;
        }
        long long int ad = as;
        as = as * 1000;
        cout << "Ok, timing " << ad << " seconds..." << endl;
        Sleep(as);
        cout << "Timer finished. Press ENTER to continue." << endl;
        cin.ignore(10, '\n');
        cin.get();
        goto sd;
    }
    if (a == "coloursys" || a == "Coloursys" || a == "Coloursys " || a == "coloursys ")
    {
        short int col;
    as:
        cout << "1- Blue" << endl << "2- Red" << endl << "3- Green" << endl << "4- White" << endl << "5- Purple" << endl << "6- Yellow" << endl << "7- Aqua" << endl << "8- Gray" << endl << "Please input the number beside the colour that you want, or input 0 to exit. >";
        cin >> col;
        switch (col)
        {
        case 1:
            system("color 1");
            cout << "Command executed." << endl;
            break;
        case 2:
            system("color 4");
            cout << "Command executed." << endl;
            break;
        case 3:
            system("color a");
            cout << "Command executed." << endl;
            break;
        case 4:
            system("color F");
            cout << "Command executed." << endl;
            break;
        case 5:
            system("color 5");
            cout << "Command executed." << endl;
            break;
        case 6:
            system("color 6");
            cout << "Command executed." << endl;
            break;
        case 7:
            system("color 3");
            cout << "Command executed." << endl;
            break;
        case 8:
            system("color 8");
            cout << "Command executed." << endl;
            break;

        case 0:
            cout << "Leaving." << endl;
            break;
        default:
            cout << "I did not understand that. Please retry." << endl;
            goto as;
        }
        goto sd;
    }
    if (a == "cls" || a == "clear" || a == "Clear" || a == "Cls")
    {
    system("cls");
        goto sd;
    }
    if (a == "help" || a == "Help" || a == "Help " || a == "help ")
    {
        cout << "HELP: Shows all commands. " << endl;
        cout << "1. help" << endl;
        cout << "2. helpcommand" << endl;
        cout << "3. echo" << endl;
        cout << "4. cls" << endl;
        cout << "5. color" << endl;
        cout << "6. exit" << endl;
        cout << "7. sleep" << endl;
        cout << "8. restartsys" << endl;
        cout << "9. shutdownsys" << endl;
        cout << "10. cry" << endl;
        cout << "11. einstein" << endl;
        cout << "12. cow" << endl;
        cout << "13. cowsay" << endl;
        cout << "14. note" << endl;
        cout << "15. notesay" << endl;
        cout << "16. openlst\n";
        cout << "17. calc\n";
        cout << "18. hello\n";
        cout << "19. game\n";
        cout << "20. memtest\n";
        cout << "21. clear\n";
        cout << "22. wbrowse\n";
        cout << "23. concatenate\n";

        goto sd;
    }
    if (a == "Helpcommand" || a == "helpcommand" || a == "Helpcommand " || a == "helpcommand ")
    {
        cout << "VERY IMPORTANT NOTE: DO NOT USE CAPITAL LETTERS WHEN INPUTTING ANY COMMANDS. THEY WILL NOT WORK." << endl;
        cout << "" << '\n';
        cout << "1. Help- Shows all commands. Does not show their meaning and usage." << endl;
        cout << "2. Helpcommand- Shows all commands, but with their meaning and usage." << endl;
        cout << "3. Echo- Allows you to echo any input you type into the terminal, but without spaces. Anything after a space or whitespace will not work." << endl;
        cout << "4. Cls- Clears the terminal screen." << endl;
        cout << "5. Color- Changes the color of terminal text. Does not change background color." << endl;
        cout << "6. Exit- Exits the terminal." << endl;
        cout << "7. Sleep- Basically a timer, but in seconds." << endl;
        cout << "8. Restartsys- Restarts the computer with no warning." << endl;
        cout << "9. Shutdownsys- Shuts down the computer with no warning." << endl;
        cout << "10. Cry- Makes the computer cry with a blue background and light yellow text." << endl;
        cout << "11. Einstein- Outputs a wise quote made by Albert Einstein. Picks from a random set of specific ones." << endl;
        cout << "12. Cow- displays a cow." << endl;
        cout << "13. Cowsay- makes a cow say something that you input." << endl;
        cout << "14. Note- Sets temporary notes. Notes get erased when terminal is closed. Sets up to 5 notes at one time." << endl;
        cout << "15. Saynote- Displays temporary notes set with the [note] command. " << endl;
        cout << "16. Openlst- Opens a program listed in a launcher.\n";
        cout << "17. Calc- A simple calculator that does not accept floating-point numbers (risk of inaccuracy).\n";
        cout << "18. Hello- Outputs 'Hello World'.\n";
        cout << "19. Game- Access terminal games. Sends you to a list of game syou can choose from.\n";
        cout << "20. Memtest- Tests memory accessibility and pointer accessibility within stack memory.\n";
        cout << "21. Clear- Clears the terminal screen\n";
        cout << "22. Wbrowse- Enter a website adress and it it will be opened.\n";
        cout << "23. Concatenate- Using pointer variables, the program will take two stings and output them concatenated together.\n";
        cout << "More commands will be added soon! :)" << endl;
        goto sd;
    }
    if (a == "echo" || a == "Echo" || a == "echo " || a == "Echo ")
    {
        cout << "Input what you want me to output. >";
        string echovar;
        getline(cin, echovar);
        cout << echovar << endl;
        cout << endl;
        goto sd;
    }

    if (a == "exit" || a == "Exit" || a == "Exit " || a == "exit ")
    {
        exitprot = 1;
        goto sd;
    }
    cout << "'" << command << "'" << " was not recognised as a function, operable program or command." << endl;
    cout << "You had probably put a space or uppercase letter in your command preference. This terminal is space and case sensitive." << endl;
    cout << "Type 'Help' to show a list of commands." << endl;

    sd: // SUPER IMPORTANT GOTO LABEL- This is to not trigger the unknown command set above.
    if (a == "")
    {
        command = "w";
        a = "w";
        goto sd;
    }
    return (a);
}

int main(int nNumberofArgs, char* pszArgs[]) // Reqired for command input, function triggers and exiting program.
{
    srand((unsigned)time(0));
    system("mode con: cols=1080 lines=1920");
    cout << "Welcome to my first terminal!" << endl;
    cout << "Please press any key to begin." << endl;
    _getch();
    cout << "Type [help] for all commands." << endl;
    cout << "Type [helpcommand] to show all command meaning and usage." << endl;
a:
    command = "w";
    cout << "Command: >";
    cin.clear();

    getline(cin, command);
    comresult(command);
 
    if (exitprot == 1) // for exiting the program
    {
        if (snote == 1) // to alert user that there are notes saved
        {
            vbc:
            char oqe;
        cout << "You have notes saved. If you leave, you will lose these notes. Are you sure you want to exit now??? [y/n] >";
        cin >> oqe;
        switch (oqe)
        {
        case 'y':
            cout << "Leaving..." << endl;
            return 0;
            break;
        case 'n':
            cout << "Aborting..." << endl;
            exitprot = 0;
            snote = 1;
            goto a;
            break;
        default:
            cout << "That input was not understood. Please try again." << endl;
            goto vbc;
            break;
        }
        }
        cout << "Stopping." << endl;

        return 0; // terminates execution
    }
    goto a; // important, or the program would stop after one command has been input

    // End of Program
}