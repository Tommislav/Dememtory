#include <iostream>
#include <string>
#include "textengine.h"
#include "writer.cpp"


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




std::string str1 = "I know I said that if I lived to 100 I'd not regret what happened last night. But I woke up this morning and a century had passed. Sorry. -Geoff Dyer";

std::string str2 = "What scares me?#n Oh, I thought you'd never ask.#n Reality scares me.#n I'm afraid of the possibility that our conversation isn't real.#n I'm afraid of the possibility that all of my friends and relatives, and even my girlfriend aren't real. I'm afraid of the possibility that this world isn't real.#n Or that I'm not real.#n Just your reflection.";


void test(){ 
	clearScreen();
	setCursorPos(0,0);
	print(str2);
	flushScreen();
}

void initGame() {

	wrSetText(str2);

	/*
	sleepMs = 50;
	cnt = 0;
	waitingForInput = false;
	data = readFromFile("data");
	gameIsRunning = true;
	//std::cout << data << std::endl;
	*/
}

bool tick() {

	if (!wrAtEnd()) {
		wrPutChar();
		flushScreen();
		return true;
	}

	return false;

/*
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
	*/
}
