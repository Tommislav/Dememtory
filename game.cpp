#include <iostream>
#include <string>
#include "textengine.h"
#include "writer.cpp"
#include "data.cpp"

using namespace std;

string currentState = "";
bool (*stateFunction)(int);

Writer writer;
termSize screenSize;

void setGameState(string);

void initGame() {
	screenSize = getTermSize();
	setGameState("start");
}


bool tick(int millisec) {
	return stateFunction(millisec);
}


bool defaultWrite(int millisec) {
	bool dirty = false;
	if (!wrAtEnd(writer)) {
		dirty = wrPutChar(writer, millisec);
		if (dirty) { 
			flushScreen();
			setCursorPos(writer.x, writer.y);
		 }
		return true;
	}

	return true;
}

void setGameState(string state) {
	currentState = state;
	if (state == "start") {
		wrSetText(writer, textStart, 0, 1);
		stateFunction = &defaultWrite;
	}
}
