#include <iostream>
#include <string>
#include "textengine.h"
#include "writer.cpp"
#include "data.cpp"

using namespace std;

bool waitingForInput = false;
bool gameIsRunning = false;
string currentState = "";

Writer writer;
termSize screenSize;

void initGame() {
	screenSize = getTermSize();
	wrSetText(writer, textStart, 0, 1);
}

bool defaultWrite(int);

bool tick(int millisec) {
	return defaultWrite(millisec);
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
	}
}
