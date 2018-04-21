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
