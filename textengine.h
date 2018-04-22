#ifndef TEXTENGINE_H
#define TEXTENGINE_H

#include <string>

using namespace std;

// sleep
#include <chrono>
#include <thread>

#include <stdlib.h> // rand/srand
#include <time.h> // time

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
	return -1;
}


void buildDeck(string* templateCards, string* deck, string* discovered, int numSolved, string unsolved) {
	int numCards = 8;
	int numValues = numCards / 2;

	// copy values in order
	for (int i=0; i<numValues; i++) {
		string value = *(templateCards + i);
		int i1 = i * 2;
		int i2 = i * 2 + 1;
		*(deck + i1) = value;
		*(deck + i2) = value;

		if (i >= numSolved) { value = unsolved; }
		*(discovered + i1) = value;
		*(discovered + i2) = value;
	}

	// shuffle both decks in the exact same way
	srand(time(NULL));
	for (int i=0; i<numCards; i++) {
		int rnd = rand() % (numCards);

		string temp1 = *(deck + i);
		string temp2 = *(discovered + i); 

		*(deck + i) = *(deck + rnd);	
		*(deck + rnd) = temp1;	
		*(discovered + i) = *(discovered + rnd);
		*(discovered + rnd) = temp2;
	}
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
