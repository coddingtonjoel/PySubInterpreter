#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "lexicalanalyzer.h"

class Interpreter {
private:
	tokenType block;
	void clearStack();
	bool isStackEmpty();
public:
	int interpretLine(tokenType, int);
};

#endif // !INTERPRETER_H