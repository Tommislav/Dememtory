#include <iostream>
#include <windows.h>
#include "game.cpp"
#include <stdlib.h> // rand(), srand()
#include "textengine.h"

using namespace std;

HANDLE hOut;
HANDLE hIn;
const int MAX_WIDTH = 400;
const int MAX_HEIGHT = 40;
const int BUFF_SIZE = MAX_WIDTH * MAX_HEIGHT;
char textBuffer[BUFF_SIZE] = {};
termSize currSize;
int currX;
int currY;


void writeCharAt(SHORT x, SHORT y, char c) {
	COORD pos = {x, y};
	SetConsoleCursorPosition(hOut, pos);
	std::cout << c << std::flush;
}

void printAt(SHORT x, SHORT y, std::string s) {
	COORD pos = {x, y};
	SetConsoleCursorPosition(hOut, pos);
	std::cout << s << std::flush;
}

termSize getTermSize() {
	CONSOLE_SCREEN_BUFFER_INFO buffer;
	bool success = GetConsoleScreenBufferInfo(hOut, &buffer);
	if (!success) {
		std::cout << "Error fetching console screen buffer" << std::endl;
	}
	COORD size = buffer.dwSize;
	//std::cout << "console width: " << size.X << ", height: " << size.Y << std::endl;
	//std::cout << "window size: " << buffer.srWindow.Top << ", " << buffer.srWindow.Bottom << std::endl;
	int height = buffer.srWindow.Bottom - buffer.srWindow.Top;
	if (height > MAX_HEIGHT) { height = MAX_HEIGHT; }
	termSize t;
	t.width = size.X > MAX_WIDTH ? MAX_WIDTH : size.X;
	t.height = height;
	return t;
}

void clearScreen() {
	textBuffer[BUFF_SIZE] = {};
}

void printAt(string str, int x, int y) {
	setCursorPos(x,y);
	print(str);
}


void setCursorPos(int x, int y) {
	currX = x;
	currY = y;
}

coord getCursorPos(){
	coord c;
	c.x = currX;
	c.y = currY;
	return c;
}

void print(string str) {
	int i = currY * currSize.width + currX;
	int len = str.length();
	for (int j=0; j<len; j++) {
		textBuffer[i+j] = str[j];
	}
}

void print(char c) {
	int i=currY * currSize.width + currX;
	currX ++;
	if (currX >= currSize.width) {
		currX = 0;
		currY++;
	}
	textBuffer[i] = c;
}

void setColor(Color col) {
}

LARGE_INTEGER freq;

void flushScreen() {
	LARGE_INTEGER start;
	QueryPerformanceCounter(&start);
	termSize size = getTermSize();
	for (SHORT y=0; y<size.height; y++) {
		COORD pos = {0, y};
		SetConsoleCursorPosition(hOut, pos);
		char line[BUFF_SIZE] = {0};
		for (int x=0; x<size.width; x++) {
			int i = y * size.width + x;
			line[x] = textBuffer[i] == 0 ? ' ' : textBuffer[i];
		}

		// write line
		DWORD charsWritten = 0;
		bool success = WriteConsole(hOut, &line, size.width, &charsWritten, NULL);
		if (!success) {
			pos = {0,0};
			SetConsoleCursorPosition(hOut, pos);
			std::cout << "ERROR WRITING TO OUTPUT BUFFER: " << GetLastError() << std::endl;
		}
	}

	COORD pos = {0, 0};
	SetConsoleCursorPosition(hOut, pos);
	LARGE_INTEGER end;
	QueryPerformanceCounter(&end);

	std::cout << (1000*(end.QuadPart - start.QuadPart))/freq.QuadPart << " " << std::flush;
}



int main() {

	// https://docs.microsoft.com/en-us/windows/console/writeconsole
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	hIn = GetStdHandle(STD_INPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO buffer;
	bool success = GetConsoleScreenBufferInfo(hOut, &buffer);
	if (!success) {
		std::cout << "Error fetching console screen buffer" << std::endl;
		return 123;
	}

	currSize = getTermSize();

	COORD pos = {0,0};
	SetConsoleCursorPosition(hOut, pos);

	LARGE_INTEGER prevTime;
	LARGE_INTEGER currTime;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&prevTime);
	LONGLONG msPassed;

	initGame();
	bool lp = true;
	while(lp) {
		QueryPerformanceCounter(&currTime);
		msPassed = (1000*(currTime.QuadPart - prevTime.QuadPart)) / freq.QuadPart;
		prevTime = currTime;

		lp = tick(msPassed);
		sleep(1);
	}


	// Handle input
	// https://docs.microsoft.com/en-us/windows/console/reading-input-buffer-events
	std::cout << "Press any key (escape to continue): " << std::flush;

	DWORD numRead;
	INPUT_RECORD inputRecBuffer[128];
	if (!SetConsoleMode(hIn, ENABLE_WINDOW_INPUT)) {
		std::cout << "Error setting console mode" << std::endl;
		sleep(5000);
		return 1;
	}




	

	// TODO: We need to write a game loop. To measure performance, and to know how long to sleep, take a look at QueryPerformanceCounter
	// https://msdn.microsoft.com/en-us/library/windows/desktop/ms644904(v=vs.85).aspx
	// and
	// https://msdn.microsoft.com/en-us/library/windows/desktop/dn553408(v=vs.85).aspx
	bool keyIsDown = false;
	bool keyWasDown = false;
	bool loop = true;
	while (loop) {
		// wait for event
		ReadConsoleInput(hIn, inputRecBuffer, 128, &numRead);
		//std::cout << numRead << " " << std::flush;
		for (int i=0; i<numRead; i++) {
			switch(inputRecBuffer[i].EventType) {
				case KEY_EVENT:
					KEY_EVENT_RECORD keyEvent = inputRecBuffer[i].Event.KeyEvent;

					keyIsDown = keyEvent.bKeyDown;
					if (keyIsDown != keyWasDown) {
						if (keyIsDown) {
							printAt(5, 5, "KEY IS DOWN! ");
							writeCharAt(5, 6, ' ');
							std::cout << "ascii: " << keyEvent.uChar.AsciiChar << std::flush;
							writeCharAt(5, 7, ' ');
							std::cout << "unicode: " << keyEvent.uChar.UnicodeChar << std::flush;

							if (keyEvent.uChar.UnicodeChar == 27) {
								loop = false;
							}
						}
						else {
							printAt(5, 5, "KEY IS UP!          ");
							printAt(5, 6, "                    ");
							printAt(5, 7, "                    ");
						}
					}
					keyWasDown = keyIsDown;


					//std::cout << "Key down: " << keyEvent.bKeyDown << " [" << "x" << "]" << std::flush;	
					break;
			}
		}
		sleep(16);
	}






	//COORD size = GetLargestConsoleWindowSize(hOut);
	COORD size = buffer.dwSize;
	std::cout << "console width: " << size.X << ", height: " << size.Y << std::endl;
	std::cout << "window size: " << buffer.srWindow.Top << ", " << buffer.srWindow.Bottom << std::endl;

	int width = size.X;
	int height = buffer.srWindow.Bottom - buffer.srWindow.Top;
	int top = buffer.srWindow.Top;


	string colors[] = {
		"\033[31m",
		"\033[32m",
		"\033[33m",
		"\033[34m",
		"\033[35m",
		"\033[36m",
		"\033[37m",
		"\033[0m"
	};
	int iCol = 0;
	int lCol = 8;

	if (size.X > 0 && size.Y > 0) {
		sleep(5000);
		for (int n=0; n<20; n++) {
			for (int step=0; step<(height>>1); step++) {

				for (int y=step; y<height-step; y++) {
					if (y == step || y == height-step-1) {
						for (int x=step; x<width-step; x++) {
							writeCharAt(x, y, '#');
						}
					}
					writeCharAt(step, y, '#');
					writeCharAt(width-step-1, y, '#');

				}
				sleep(5);
				std::cout << colors[iCol++];
				iCol %= lCol;
			}
		}
	}
	

	writeCharAt(1, 20, ' ');
	std::cout << "\033[0m";






// for cygwin
// http://www.cplusplus.com/forum/general/18200/
// or
// https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal
	return 0;
}


