#include <iostream>
#include <string>
#include "textengine.h"

using namespace std;

struct Writer {
	string text;
	int pos;
	int len;
	int wait;
	int counter;
	int pause;
	int startX;
	int startY;
	int x;
	int y;
	int w;
	int h;
	int layer;
	Color col;
	Writer() : pos(0), len(0), counter(0), pause(0), startX(0), startY(0), x(0), y(0), w(0), h(0), layer(0), wait(30), col(Color::def) {}
};


bool wrAtEnd(Writer &writer) {
	return writer.pos == writer.len;
}

void wrParseCommand(Writer &writer, char command, char instr) {
	if (command == 'n') { // newline
		writer.x = 0;
		writer.y ++;
		}
	if (command == 'p') { // pause
		if (instr == '0') { writer.pause = 100; }
		else if (instr == '1') { writer.pause = 500; }
		else if (instr == '2') { writer.pause = 1000; }
		else if (instr == '3') { writer.pause = 1500; }
	}
	if (command == 's') { // speed 
		if (instr == '0') { writer.wait = 5; }
		else if (instr == '1') { writer.wait = 30; } 
		else if (instr == '2') { writer.wait = 50; } 
		else if (instr == '3') { writer.wait = 150; } 
	}
	if (command == 'c') { // color
		if (instr == 'R') { writer.col = Color::red; }
		else if (instr == 'G') { writer.col = Color::green; }
		else if (instr == 'B') { writer.col = Color::blue; }
		else if (instr == 'W') { writer.col = Color::white; }
		else if (instr == 'P') { writer.col = Color::purple; }
		else if (instr == 'D') { writer.col = Color::def; }
	}
/*
	if (command == 'c') { // color
		if (instr == 'R') { setColor(Color::red); }
		else if (instr == 'G') { setColor(Color::green); }
		else if (instr == 'B') { setColor(Color::blue); }
		else if (instr == 'W') { setColor(Color::white); }
		else if (instr == 'D') { setColor(Color::def); }
	}
*/
}



bool wrPutChar(Writer &writer, int millisec) {
	if (wrAtEnd(writer)) {return false;}
	if (millisec > -1) {
		if (writer.pause > 0) {
			writer.pause -= millisec;
			if (writer.pause > 0) {
				return false;
			}
		}

		writer.counter += millisec;
		if (writer.counter < writer.wait) {
			return false;
		}

		writer.counter -= writer.wait;
		if (writer.counter < 0) { writer.counter = 0; }
	}

	char c = writer.text[writer.pos];
	if (c == '^') {
		wrParseCommand(writer, 'n', ' ');
		writer.pos++;
		return wrPutChar(writer, millisec);
	}
	else if (c == '#') {
		wrParseCommand(writer, writer.text[writer.pos+1], writer.text[writer.pos+2]);
		writer.pos += 3;
		return wrPutChar(writer, millisec);
	}

	printAt(c, writer.x, writer.y, writer.col);
	writer.pos ++;
	writer.x ++;
	return true;
}

void wrResize(Writer &writer) {
	termSize size = getTermSize();
	writer.w = size.width;
	writer.h = size.height;
	int lastSpace = 0;
	int lastSpaceX = writer.startX;
	int x = writer.startX;
	for (int i=0; i<writer.len; i++) {

		if (writer.text[i] == '#') {
			if (writer.text[i+1] == 'n') { x = 0; }
			i+=2;
			continue;
		}

		if (writer.text[i] == '^') { writer.text[i] = ' '; }
		if (writer.text[i] == ' ') { 
			lastSpace = i;
			lastSpaceX = x;
		 }
		
		x++;
		if (x >= size.width) {
			writer.text[lastSpace] = '^';
			x -= lastSpaceX;
		}
	}
	// re-write all resized text
	int oldPos = writer.pos;
	writer.pos = 0;
	for (int i=0; i<oldPos; i++) {
		wrPutChar(writer, -1);
	}
}

void wrSetText(Writer &writer, string text, int startX, int startY) {
	writer.text = text;
	writer.pos = 0;
	writer.len = text.size();
	writer.startX = writer.x = startX;
	writer.startY = writer.y = startY;
	wrResize(writer);
}

