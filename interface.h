#ifndef PYSUBINTERPRETER_INTERFACE_H
#define PYSUBINTERPRETER_INTERFACE_H

#include <vector>
#include <string>
#include "lexicalanalyzer.h"

typedef std::vector<std::string> programType;

class Interface {
private:
    programType programCode;
    void enterHelpUtility();
    void getCommandUsage(const std::string&);
    void readProgram(LexicalAnalyzer &lexAnalysis);
    bool runLexer(LexicalAnalyzer &lexAnalysis);
public:
    void startInterface();
};


#endif
