#ifndef TEXTENGINE_H
#define TEXTENGINE_H

#include <string>


// sleep
#include <chrono>
#include <thread>

struct termSize {
	int width, height;
};

enum Color { def, red, blue, green, white };

void clearScreen();
termSize getTermSize();
void printAt(std::string str, int x, int y);

void printAt(std::string str, int x, int y, int layer);

void setLayer(int layer);
void clearLayer(int layer);
void setCursorPos(int x, int y);
void setColor(Color col);
void flushScreen();

void sleep(int ms) {
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

#endif
