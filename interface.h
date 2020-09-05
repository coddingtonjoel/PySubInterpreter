#ifndef PYSUBINTERPRETER_INTERFACE_H
#define PYSUBINTERPRETER_INTERFACE_H

#include <vector>
#include <string>

using namespace std;


class Interface {
private:
    typedef vector<string> programType;
    programType programCode;

    void enterHelpUtility();
    void getCommandUsage(const string&);
public:
    void startInterface();
};


#endif
