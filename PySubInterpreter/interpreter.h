#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "lexicalanalyzer.h"

class Interpreter {
public:
	void interpretLine(tokenLineType);
};

#endif // !INTERPRETER_H