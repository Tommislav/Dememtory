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

enum Color { def, red, blue, green, white };

void clearScreen();
termSize getTermSize();
void setCursorPos(int x, int y);
coord getCursorPos();
void printAt(std::string str, int x, int y);
void print(std::string);
void print(char c);

void setColor(Color col);
void setColorAt(Color col, int x, int y);
void flushScreen();

void sleep(int ms) {
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

#endif
