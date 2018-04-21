
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
CHAR_INFO outputBuffer[BUFF_SIZE] = {};
string debugStr = "";
termSize currSize;
int currX;
int currY;
int currCol;


termSize getTermSize() {
	CONSOLE_SCREEN_BUFFER_INFO buffer;
	bool success = GetConsoleScreenBufferInfo(hOut, &buffer);
	if (!success) {
		std::cout << "Error fetching console screen buffer" << std::endl;
	}
	COORD size = buffer.dwSize;
	int height = buffer.srWindow.Bottom - buffer.srWindow.Top;
	int width = buffer.srWindow.Right - buffer.srWindow.Left;
	if (height > MAX_HEIGHT) { height = MAX_HEIGHT; }
	termSize t;
	t.width = width > MAX_WIDTH ? MAX_WIDTH : width;
	t.height = height;
	return t;
}

void clearScreen() {
	for (int i=0; i<BUFF_SIZE; i++) {
		outputBuffer[i] = {};
	}
}


void setCursorPos(int x, int y) {
	COORD pos = {(SHORT)x, (SHORT)y};
	SetConsoleCursorPosition(hOut, pos);
}

coord getCursorPos(){
	coord c;
	c.x = currX;
	c.y = currY;
	return c;
}

int ColorToInt(Color col) {
	int currCol = 1 | 2 | 4;
	switch(col) {
		case red: currCol = 4 | 8; break;
		case green: currCol = 2 | 8; break;
		case blue: currCol = 1 | 2 | 8; break;
		case white: currCol = 1 | 2 | 4 | 8; break;
		case purple: currCol = 1 | 4 | 8; break;
		default: currCol = 1 | 2 | 4; break;
	}
	return currCol;
}

void print(string str) {
	int i = currY * currSize.width + currX;
	int len = str.length();
	for (int j=0; j<len; j++) {
		outputBuffer[i+j].Char.UnicodeChar = str[j];
		outputBuffer[i+j].Attributes = 8;
	}
}

void printAt(string s, int x, int y) {
}

void printAt(char c, int x, int y, Color col) {
	int i = y * currSize.width + x;	
	outputBuffer[i].Char.UnicodeChar = c;	
	outputBuffer[i].Attributes = ColorToInt(col);
}


void print(char c) {
	int i=currY * currSize.width + currX;
	currX ++;
	if (currX >= currSize.width) {
		currX = 0;
		currY++;
	}
	outputBuffer[i].Char.UnicodeChar = c;
	outputBuffer[i].Attributes = currCol;
}

void debug(string s) {
	debugStr = s;
}

void clearDebug(int y) {
	debugStr = "";
}

void setColor(Color col) {
	setColorAt(col, currX, currY);
}

void setColorAt(Color col, int x, int y) {
	int i= y * currSize.width + x;
	int currCol = ColorToInt(col);
	outputBuffer[i].Attributes = currCol;
}

LARGE_INTEGER freq;

void flushScreen() {
	// http://cecilsunkure.blogspot.se/2011/11/windows-console-game-writing-to-console.html
	LARGE_INTEGER start;
	QueryPerformanceCounter(&start);
	termSize size = getTermSize();
	COORD buffSize = {(SHORT)size.width,(SHORT)size.height};
	COORD buffPos = {0,0};
	SMALL_RECT smallRect = {0,0,(SHORT)size.width,(SHORT)size.height};
	WriteConsoleOutput(hOut, &outputBuffer[0], buffSize, buffPos, &smallRect);

	if (debugStr != "") {
		setCursorPos(0,0);
		cout << debugStr << flush;
	}

	LARGE_INTEGER end;
	QueryPerformanceCounter(&end);

	//std::cout << (1000*(end.QuadPart - start.QuadPart))/freq.QuadPart << " " << std::flush;
}

void shakeScreen(int x, int y) {
	HWND consoleWindow = GetConsoleWindow();
#ifndef cygwin 
// for some reason this crashes when compiling with cygwin g++
	LPRECT currPos = {};
	BOOL success = GetWindowRect(consoleWindow, currPos);
	if (success) {
		x += (int)currPos->left;
		y += (int)currPos->top;
	}
#else
	x += 10;
	y += 10;
#endif

	if (x < 0) x = 0;
	if (y < 0) y = 0;

	SetWindowPos(consoleWindow, 0, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
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

	SetConsoleTitle("LD41 ::::: Dememtory ::::::");
	SMALL_RECT windowSize = {10, 10, 90, 30};
	SetConsoleWindowInfo(hOut, TRUE, &windowSize);


	COORD pos = {0,0};
	SetConsoleCursorPosition(hOut, pos);
	setColor(Color::def);

	currSize = getTermSize();

	LARGE_INTEGER prevTime;
	LARGE_INTEGER currTime;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&prevTime);
	LONGLONG msPassed;

	DWORD numRead;
	INPUT_RECORD inputRecBuffer[128];

	if (!SetConsoleMode(hIn, ENABLE_WINDOW_INPUT)) {
		std::cout << "Error setting console mode" << std::endl;
		sleep(5000);
		return 1;
	}

	initGame();
	bool lp = true;
	while(lp) {

		// CHECK INPUT

		//	clear keydown array in game.cpp
		numKeysDown = 0;

		DWORD numEvents = 0;
		GetNumberOfConsoleInputEvents(hIn, &numEvents);
		if (numEvents > 0) {
			ReadConsoleInput(hIn, inputRecBuffer, 128, &numRead);
			for (int i=0; i<numRead; i++) {
				if (inputRecBuffer[i].EventType == KEY_EVENT) {
					KEY_EVENT_RECORD keyEvent = inputRecBuffer[i].Event.KeyEvent;

					if (keyEvent.uChar.UnicodeChar == 32) {
						// skipButton is in game.cpp
						skipButtonDown = keyEvent.bKeyDown;
					}

					if (keyEvent.bKeyDown || keyEvent.wRepeatCount > 0) {
						if (keyEvent.uChar.UnicodeChar == 27) { return 0; } // ESC
						keysDown[numKeysDown] = keyEvent.uChar.UnicodeChar;
						numKeysDown++;

					}
				}
			}
		}

		// Call game loop
		QueryPerformanceCounter(&currTime);
		msPassed = (1000*(currTime.QuadPart - prevTime.QuadPart)) / freq.QuadPart;
		prevTime = currTime;

		lp = tick(msPassed);
		sleep(1);
	}


	// Handle input
	// https://docs.microsoft.com/en-us/windows/console/reading-input-buffer-events

	// TODO: We need to write a game loop. To measure performance, and to know how long to sleep, take a look at QueryPerformanceCounter
	// https://msdn.microsoft.com/en-us/library/windows/desktop/ms644904(v=vs.85).aspx
	// and
	// https://msdn.microsoft.com/en-us/library/windows/desktop/dn553408(v=vs.85).aspx

	//COORD size = GetLargestConsoleWindowSize(hOut);
	/*
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
*/





// for cygwin
// http://www.cplusplus.com/forum/general/18200/
// or
// https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal
	return 0;
}


