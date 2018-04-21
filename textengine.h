#ifndef TEXTENGINE_H
#define TEXTENGINE_H

#include <string>


// sleep
#include <chrono>
#include <thread>

struct termSize {
	int width, height;
};
struct coord {
	int x, y;
};

struct timer {
	int counter; // current countdown (towards 0)
	int wait; // how much to reset to (when counter <= 0)
	int skipWait;
	int pause; // pause counter that needs to reach 0 first
	bool isBreak; // if break, don't count down

	timer():counter(0), wait(30), skipWait(5), pause(0), isBreak(false) {}

	bool countDown(int time, bool skip) {
		if (isBreak) { return false; }
		if (!skip && pause > 0) { pause -= time; return false; }
		if (counter > 0) {
			counter -= time;
		}
		if (counter <= 0) {
			counter = skip ? skipWait : wait;
			return true;
		}
		return false;
	}
};


enum Color { def, red, blue, green, white, purple };

void clearScreen();
termSize getTermSize();
void setCursorPos(int x, int y);
coord getCursorPos();
void printAt(std::string str, int x, int y);
void print(std::string);
void print(char c);
void printAt(char c, int x, int y, Color col);

void setColor(Color col);
void setColorAt(Color col, int x, int y);
void flushScreen();

void shakeScreen(int x, int y);

void sleep(int ms) {
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

int intFromChar(char c) {
	char nums[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
	for (int i=0; i<10; i++) {
		if (nums[i] == c) return i;
	}
	return 0;
}

/*
// read string from file
#include <fstream>

std::string readFromFile(std::string fileName) {
	std::ifstream t(fileName);
	t.seekg(0, std::ios::end);
	size_t size = t.tellg();
	std::string buffer(size, ' ');
	t.seekg(0);
	t.read(&buffer[0], size);
	t.close();
	return buffer;
}
*/

#endif
