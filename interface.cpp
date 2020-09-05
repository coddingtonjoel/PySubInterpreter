#include "interface.h"
#include <iostream>
#include <iomanip>

using namespace std;

void Interface::startInterface() {
    cout << "PySUB Interpreter 1.0 - 2020" << endl;
    cout << "Enter program lines or read(<filename>.py) in CLI." << endl;
    cout << "Type 'help' for more information or 'quit' to exit." << endl;

    bool userHasQuit = false;

    while (!userHasQuit) {
        string input;
        cout << ">>> ";
        getline(cin, input);

        // quit
        if (input == "quit" || input == "quit()") {
            userHasQuit = true;
        }

        // clear
        else if (input == "clear" || input == "clear()") {
            for (int i = 0; i < 30; i++) {
                cout << endl;
            }
        }

        // show
        else if (input == "show" || input == "show()") {
            // for each line in programCode variable, print each line
            // if programCode vector is empty, print that there's nothing stored
        }

        // read
        else if (input == "read(<filename>.py)") {
            // for each line in programCode variable, print each line
            // if programCode vector is empty, print that there's nothing stored
        }

        // help
        else if (input == "help") {
            enterHelpUtility();
        } else if (input == "help(help)") {
            getCommandUsage("help");
        } else if (input == "help(show)") {
            getCommandUsage("show");
        } else if (input == "help(clear)") {
            getCommandUsage("clear");
        } else if (input == "help(read)") {
            getCommandUsage("read");
        } else if (input == "help(quit)") {
            getCommandUsage("quit");
        } else {
                cout << "Unknown help parameter. For a list of commands, enter the help utility by typing 'help' and then "
                        "type 'commands'."
                        << endl;
        }
    }
}

void Interface::enterHelpUtility() {
    bool userHasExit = false;

    cout << "Welcome to the help utility!" << endl;
    cout << "* To exit and return to the interpreter, type 'exit'." << endl;
    cout << "* To get a list of commands, type 'commands'." << endl;
    cout << "* To get a description of any command, just type the command at the help> prompt." << endl;

    while (!userHasExit) {
        string input;
        cout << "help> ";
        getline(cin, input);

        if (input == "exit") {
            userHasExit = true;
        }
        else {
            getCommandUsage(input);
        }
    }
}

void Interface::getCommandUsage(const string& command) {
    if (command == "help") {
        cout
                << "The 'help' command enters the help utility where a user can learn more about a command by entering its name. When used with a parameter, the command usage is explained without entering the help utility [ex. help(read)]."
                << endl;
    } else if (command == "clear") {
        cout << "The 'clear' command clears the screen." << endl;
    } else if (command == "quit") {
        cout << "The 'quit' command quits the PySub interpreter." << endl;
    } else if (command == "read") {
        cout << "The 'read' command reads a given file with the PySub interpreter [ex. read(<filename>.py)]. The "
                "<filename>.py parameter has to be provided and then the program will be run by the Python "
                "interpreter." << endl;
    } else if (command == "show") {
        cout
                << "The 'show' command shows the lines of the program that are stored in program data structure. Include"
                   " line number when displaying each line of code in the program." << endl;
    }
    else if (command == "commands") {
        cout << "Here is a list of available commands. Type one to to learn more information about its usage." << endl;
        cout << setw(10) << left << "help" << setw(10) << left << "clear" << setw(10) << left << "quit" << endl;
        cout << setw(10) << left << "read" << setw(10) << left << "show" << endl;
    } else {
        cout << "Unknown command. If you would like to exit the help utility, type 'exit'." << endl;
    }
}