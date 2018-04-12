#include <iostream>
#include <string>
#include "textengine.h"


// read string from file
#include <fstream>



int sleepMs = 50;
int cnt = 0;
bool waitingForInput = false;
bool gameIsRunning = false;
std::string data = "";

int getSleepMs() { return sleepMs; }
bool getWaitingForInput() { return waitingForInput; }
bool getGameIsRunning() { return gameIsRunning; }

void testFuncInOtherFile() {
	std::cout << "test from game.cpp" << std::endl;
}


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

void initGame() {
	sleepMs = 50;
	cnt = 0;
	waitingForInput = false;
	data = readFromFile("data");
	gameIsRunning = true;
	//std::cout << data << std::endl;
}







void tick() {
	char s = data[cnt++];
	if (s == '#') {
		// COMMAND
		char cmd = data[cnt++];
		char val = data[cnt++];
		s = data[cnt++];

		if (cmd == 's') {
			switch(val) {
				case '0':
					sleepMs = 50;
					break;
				case '1':
					sleepMs = 800;
					break;
			}
		}
		else if (cmd == 'p') {
			sleep(500);
		}
		else if (cmd == 'c') {
			switch(val) {
				case 'R': // red
					std::cout << "\033[31m";
					break;
				case 'D':
					    std::cout << "\033[0m";
					    break;
			}

		}
	}
	std::cout << s << std::flush;
	gameIsRunning = cnt < data.length();
}
