#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "lexicalanalyzer.h"

class Interpreter {
public:
	bool interpretLine(tokenLineType);
};

#endif // !INTERPRETER_H