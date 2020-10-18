#include "interface.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include "lexicalanalyzer.h"
#include "expevaluator.h"
#include <vector>

using namespace std;

void Interface::startInterface() {
    LexicalAnalyzer lexAnalysis;
    expEvaluator expEvaluation;

    cout << "PySUB Interpreter 1.0 - 2020" << endl;
    cout << "Enter program lines or read(<filename>.py) in CLI." << endl;
    cout << "Type 'help' for more information or 'quit' to exit." << endl;

    bool userHasQuit = false;

    while (!userHasQuit) {
        string input;
        cout << ">>> ";
        getline(cin, input);

        // checks if the first 5 characters of the input is "read(" for read command
        string readCheckCommand = input.substr(0, 5);

        // quit
        if (input == "quit" || input == "quit()") {
            userHasQuit = true;
        }

        // clear
        else if (input == "clear" || input == "clear()") {
            if (!programCode.empty()) {
                programCode.clear();
                cout << "Stored code successfully cleared." << endl;
            }
            if (!lexAnalysis.tokenInfo.empty()) {
                lexAnalysis.tokenInfo.clear();
                cout << "Stored token info successfully cleared." << endl;
            }
            if (!expEvaluation.symbolTable.empty()) {
                expEvaluation.symbolTable.clear();
                cout << "Stored symbol table successfully cleared." << endl;
            }
            else {
                cout << "There is currently no code stored in the program." << endl;
            }
        }

        // show
        else if (input == "show" || input == "show()") {
            // for each line in programCode variable, print each line
            // if programCode vector is empty, print that there's nothing stored
            if (!programCode.empty()) {
                for (int i = 0; i < programCode.size(); i++) {
                    cout << "[" << i + 1 << "] " << programCode.at(i) << endl;
                }
            }
            else {
                cout << "No file is currently stored. Store a file using the 'read' command." << endl;
            }
        }

        // show(tokens)
        else if (input == "show(tokens)") {
            if (!programCode.empty()) {
                readProgram(lexAnalysis);
            }
            else {
                cout << "No file is currently stored. Store a file using the 'read' command." << endl;
            }
        }

        // read
        // check if used without parameter
        else if (readCheckCommand == "read") {
            cout << "Please enter a file name parameter [ex. read(filename.py)]" << endl;
        }
        else if (readCheckCommand == "read(") {
            // format fileName to match the format: "filename.py"
            string fileName = input.substr(5);

            // check that the content after '(' was not empty and that the end of the string is ')'
            if (!fileName.empty() && fileName.substr(fileName.size() - 1) == ")") {
                fileName.pop_back();
            }

            // all files with a .py extension MUST be longer than 3 characters
            // ex: "h.py" (4 chars)
            if (fileName.length() > 3) {
                // check if file ends with the .py extension
                string extension = fileName.substr(fileName.size() - 3);

                if (extension != ".py") {
                    cout
                            << "Invalid python file. Make sure the file name you enter ends with '.py'." << endl;
                } else {
                    ifstream fileStream;
                    fileStream.open(fileName);

                    // check for file stream errors
                    if (!fileStream) {
                        cout << "An error was encountered when reading the file. Check that the file exists within "
                                "the interpreter directory." << endl;
                    }
                    else {
                        // the file is all ready to go!
                        // clear current file stored, if any
                        if (!programCode.empty()) {
                            programCode.clear();
                        }
                        // get line by line and add them to vector
                        string line;
                        while (getline(fileStream, line)) {
                            if (!line.empty()) {
                                programCode.push_back(line);
                            }
                        }
                        fileStream.close();
                        cout << "File successfully read. Use the 'show' command to view file content." << endl;
                    }
                }
            }
            else {
                cout << "Invalid file name." << endl;
            }
        }

        // help
        else if (input == "help" || input == "help()") {
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
        } else if (!input.empty()){
            // inline expression evaluator
            lexAnalysis.tokenInfo.clear();
            vector<string> temp;
            temp.push_back(input);
            bool isValid = lexAnalysis.tokenize(temp);

            // proofread code for errors before running it through infix/postfix conversions
            string result = expEvaluation.checkForErrors(lexAnalysis.tokenInfo);
            if (result != "err" && isValid) {
                string postfix = expEvaluation.infixToPostfix(lexAnalysis.tokenInfo);
                lexAnalysis.tokenInfo.clear();
                temp.clear();
                temp.push_back(postfix);
                lexAnalysis.tokenize(temp);
                cout << expEvaluation.postfixEval(lexAnalysis.tokenInfo) << endl;
            }
            // clear token info because it was an inline expression
            // but DON'T clear symbol table here!
            lexAnalysis.tokenInfo.clear();
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
        cout << "The 'clear' command clears out or deletes any lines that are stored in the program." <<
        endl;
    } else if (command == "quit") {
        cout << "The 'quit' command quits the PySub interpreter." << endl;
    } else if (command == "read") {
        cout << "The 'read' command reads a given file with the PySub interpreter [ex. read(<filename>.py)]. The "
                "<filename>.py parameter has to be provided and then the program will be run by the Python "
                "interpreter." << endl;
    } else if (command == "show") {
        cout
                << "The 'show' command shows the lines of the program that are stored in program data structure. Include"
                   " line number when displaying each line of code in the program."
                   << endl;
        cout << "Additionally, using 'show(tokens)' displays the tokens in each PySub program by line." << endl;
    }
    else if (command == "commands") {
        cout << "Here is a list of available commands. Type one to to learn more information about its usage." << endl;
        cout << setw(10) << left << "help" << setw(10) << left << "clear" << setw(10) << left << "quit" << endl;
        cout << setw(10) << left << "read" << setw(10) << left << "show" << endl;
    } else {
        cout << "Unknown command. If you would like to exit the help utility, type 'exit'." << endl;
    }
}

void Interface::readProgram(LexicalAnalyzer &lexAnalysis) {
    if (runLexer(lexAnalysis)) {
        lexAnalysis.display(lexAnalysis.tokenInfo);
    }
}

bool Interface::runLexer(LexicalAnalyzer &lexAnalysis) {
    return lexAnalysis.tokenize(programCode);
}
