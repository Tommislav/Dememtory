#include <iostream>
#include <unistd.h>
#include <sys/ioctl.h>
#include "game.cpp"
#include "textengine.h"

using namespace std;

typedef unsigned short WORD;

void testPrintColorsCygwin() {
	cout << "\033[1;31mbold red text\033[0m\n";
}


void writeCharAt(int x, int y, char c) {
	//printf("\033[%d;%dH", x+1, y+1);
	//std::cout << c << std::flush;
	// https://stackoverflow.com/questions/27599233/how-to-manipulate-the-terminal-output-buffer-directly
	std::cout << "\e["<< x << ";"<<y<<"H" << c;
}

// textenginge.h
void clearScreen() {
	std::cout << "\033[2J\033[1;1H" << std::flush;
}

// textengine.h
termSize getTermSize() {

	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	termSize t;
	t.width = w.ws_row;
	t.height = w.ws_col;
	return t;
}

// textengine.h
void setCursorPos(int x, int y) {

}

// textengine.h
void printAt(char c, int x, int y, Color col) {
	//printf("\033[%d;%dH", x+1, y+1);
	std::cout << c << std::flush;
}

// textengine.h
void flushScreen() {
	std::cout << std::flush;
}

// TODO Check this out for keyboard input handling!!!
// http://www.cplusplus.com/forum/unices/11910/


main() {
	clearScreen();

	// Turn off console input! Remember to turn it back at the end of program
	//system("stty raw -echo");

	termSize ts = getTermSize();
	std::cout << "console width: " << ts.width << ", height: " << ts.height << std::endl;
	sleep(1000);

	initGame();
	bool lp = true;
	while(lp) {
		int msPassed = 10;
		lp = tick(msPassed);
	}




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
	/*
	int iCol = 0;
	int lCol = 8;

	if (width > 0 && height > 0) {
		//sleep(1000);

		for (int n=0; n<10; n++) {

			for (int step=0; step<(width>>1); step++) {

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



	std::string data = "#s0That was fun! #cR #s1VERY#s0 #cD fun! X-D";
	int cnt = 0;
	int sleepMs = 50;

	while (cnt < data.length()) {

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
				//sleep(500);
			}
			else if (cmd == 'c') {
				switch(val) {
					case 'R': // red
						std::cout << "\033[31m" << std::flush;
						break;
					case 'D':
						std::cout << "\033[0m" << std::flush;
						break;
				}

			}
		}
		std::cout << s << std::flush;

		sleep(sleepMs);
		//char c = getchar();
		//if (c == '') { std::cout << "*" << c << "* " << std::flush; }
	}
	//cin.ignore();
	//clearScreen();
*/
	system("stty cooked");
	return 0;
}


