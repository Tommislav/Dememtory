#include <iostream>
#include <string>
#include "textengine.h"

using namespace std;

string wrText = "";
int wrPos = 0;
int wrLen = 0;
int wrWait = 30;
int wrCounter = 0;
int wrPause = 0;

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
	if (command == 'p') { // pause
		if (instr == '0') { wrPause = 100; }
		else if (instr == '1') { wrPause = 500; }
		else if (instr == '2') { wrPause = 1000; }
		else if (instr == '3') { wrPause = 1500; }
	}
	if (command == 's') { // speed 
		if (instr == '0') { wrWait = 5; }
		else if (instr == '1') { wrWait = 30; } 
		else if (instr == '2') { wrWait = 50; } 
		else if (instr == '3') { wrWait = 150; } 
	}
	if (command == 'c') { // color
		if (instr == 'R') { setColor(Color::red); }
		else if (instr == 'G') { setColor(Color::green); }
		else if (instr == 'B') { setColor(Color::blue); }
		else if (instr == 'W') { setColor(Color::white); }
		else if (instr == 'D') { setColor(Color::def); }
	}

}



// USE THIS!
bool wrPutChar(int millisec) {
	if (wrAtEnd()) {return false;}

	if (millisec > -1) {

		if (wrPause > 0) {
			wrPause -= millisec;
			if (wrPause > 0) {
				return false;
			}
		}

		wrCounter += millisec;
		if (wrCounter < wrWait) {
			return false;
		}

		wrCounter -= wrWait;
		if (wrCounter < 0) {wrCounter = 0;}

	}

	char c = wrText[wrPos];
	if (c == '^') {
		wrParseCommand('n', ' ');
		wrPos++;
		return wrPutChar(millisec);
	}
	else if (c == '#') {
		wrParseCommand(wrText[wrPos+1], wrText[wrPos+2]);
		wrPos += 3;
		return wrPutChar(millisec);
	}

	print(c);
	wrPos++;
	return true;
}

