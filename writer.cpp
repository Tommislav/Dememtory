#include <iostream>
#include <string>
#include "textengine.h"

using namespace std;

string wrText = "";
int wrPos = 0;
int wrLen = 0;
int wrPause = 30;
int wrCounter = 0;

void wrSetText(string text) {
	wrText = text;
	wrPos = 0;
	wrLen = text.size();
}

bool wrAtEnd() {
	return wrPos == wrLen;
}

void wrWriteResized() {
	// re-write all in current textbuffer to fit in new size?? todo
}

void wrParseCommand(char command, char instr) {
	if (command == 'n') { // newline
		coord c = getCursorPos();
		setCursorPos(0, c.y + 1);
	}
}



// USE THIS!
bool wrPutChar(int millisec) {
	if (wrAtEnd()) {return true;}

	wrCounter += millisec;
	if (wrCounter < wrPause) {
		return false;
	}

	wrCounter -= wrPause;
	if (wrCounter < 0) {wrCounter = 0;}


	char c = wrText[wrPos];
	if (c == '^') {
		wrParseCommand('n', ' ');
	}
	else if (c == '#') {
		wrParseCommand(wrText[wrPos+1], wrText[wrPos+2]);
		wrPos += 3;
		return wrPutChar(millisec);
	}

	print(c);
	flushScreen();
	wrPos++;
	return wrAtEnd();
}

