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

std::string str2 = "What scares me?#n #p2Oh, I thought you'd never ask.#n Reality scares me.#n #p2#f0I'm afraid of the possibility that our conversation isn't real.#n I'm afraid of the possibility that all of my friends and relatives, and even my girlfriend aren't real#p2#f3......#p2#f1 I'm afraid of the possibility that this world isn't real.#n Or that I'm not real.#n Just your reflection.#p3#p3#p3 ";

std::string str3 = "Line 1#nLine2#nLine3";


std::string ld41 = "#n  LUDUM DARE 41 #p2#s0....................................................................#p2#n "
" #s2#cBTOOLS:#cD   #s1 #p1C++, Custom engine. No libraries.#p1#n "
" #cBIDE:#cD      #p1VIM. Probably Visual Studio for debugging.#p0#n "
" #cBGRAPHICS:#cD #p1Nothing! #p1Text only baby! #p1That plays straight in your CMD!#p0#n #n "
" #p2For so long I've been wanting to get down and dirty in C++, so now - with WAY too#n "
" little preparations - I've decided to write my first complete C++ project ever.#n "
" #s2WHAT COULD POSSIBLY GO WRONG#s1?!? #p1And lets do it without any libraries as well!#n "
" #cR#s3Handmade Hero #cDstyle! #p1#s1Whish me luck #p2#s2because #p1 ";

std::string imin = 
" IIIIIIIIII MMMMMMMM               MMMMMMMM     IIIIIIIIII NNNNNNNN        NNNNNNNN#n " 
" I::::::::I M:::::::M             M:::::::M     I::::::::I N:::::::N       N::::::N#n "
" I::::::::I M::::::::M           M::::::::M     I::::::::I N::::::::N      N::::::N#n "
" II::::::II M:::::::::M         M:::::::::M     II::::::II N:::::::::N     N::::::N#n "
"   I::::I   M::::::::::M       M::::::::::M       I::::I   N::::::::::N    N::::::N#n "
"   I::::I   M:::::::::::M     M:::::::::::M       I::::I   N:::::::::::N   N::::::N#n "
"   I::::I   M:::::::M::::M   M::::M:::::::M       I::::I   N:::::::N::::N  N::::::N#n "
"   I::::I   M::::::M M::::M M::::M M::::::M       I::::I   N::::::N N::::N N::::::N#n "
"   I::::I   M::::::M  M::::M::::M  M::::::M       I::::I   N::::::N  N::::N:::::::N#n "
"   I::::I   M::::::M   M:::::::M   M::::::M       I::::I   N::::::N   N:::::::::::N#n "
"   I::::I   M::::::M    M:::::M    M::::::M       I::::I   N::::::N    N::::::::::N#n "
"   I::::I   M::::::M     MMMMM     M::::::M       I::::I   N::::::N     N:::::::::N#n "
" II::::::II M::::::M               M::::::M     II::::::II N::::::N      N::::::::N#n "
" I::::::::I M::::::M               M::::::M     I::::::::I N::::::N       N:::::::N#n "
" I::::::::I M::::::M               M::::::M     I::::::::I N::::::N        N::::::N#n "
" IIIIIIIIII MMMMMMMM               MMMMMMMM     IIIIIIIIII NNNNNNNN         NNNNNNN";



bool colorFlash = false;
int tickWait = 0;
int colCycle = 0;

void initGame() {
	setCursorPos(0, 0);
	setColor(Color::def);
	wrSetText(ld41);
}

bool tick(int millisec) {

	bool dirty = false;
	if (!wrAtEnd() && !colorFlash) {
		dirty = wrPutChar(millisec);
		if (dirty) { flushScreen(); }
		return true;
	}

	if (!colorFlash) {
		wrSetText(imin);
		setCursorPos(0, 11);
		setColor(Color::green);
		while(!wrAtEnd()) {
			wrPutChar(-1);
		}
		flushScreen();
		colorFlash = true;
		return true;
	}

	tickWait -= millisec;	
	if (tickWait > 0) {return true; }
	tickWait = 20;

	// color flashing
	++colCycle %= 2;
	for (int x=0; x<83; x++) {
		for (int y=11; y<27; y++) {
			Color c = colCycle == 0 ? Color::red : Color::blue;
			setColorAt(c, x, y);
		}
	}
	flushScreen();	



	return true;
}
