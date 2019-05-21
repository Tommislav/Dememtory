#include <iostream>
#include <unistd.h> // usleep
#include <ncurses.h>
#include <sys/ioctl.h>
#include "game.cpp"
#include "textengine.h"

using namespace std;

Color currentColor;
int currentAttr = -1;

void writeCharAt(int x, int y, char c) {
	mvprintw(y, x, &c);
}

// textenginge.h
void clearScreen() {
	clear();
}

// textengine.h
termSize getTermSize() {

	int x=0;
	int y=0;
	getmaxyx(stdscr, y, x);

	termSize t;
	t.width = x;
	t.height = y;
	return t;
}

// textengine.h
void setCursorPos(int x, int y) {
	move(y, x);
}

void setColor(Color col) {
	int attr = currentAttr;
	switch(col) {
		case Color::def:
			attr = 1;
			break;
		case Color::red:
			attr = 2;
			break;
		case Color::blue:
			attr = 3;
			break;
		case Color::green:
			attr = 4;
			break;
		case Color::white:
			attr = 5;
			break;
		case Color::purple:
			attr = 6;
			break;
		default:
			attr = -1;
	}

	if (currentAttr == attr) { return; }

	if (currentAttr != -1) {
		attroff(COLOR_PAIR(currentAttr));
		currentAttr = -1;
	}
	currentAttr = attr;
	if (currentAttr != -1) {
		attron(COLOR_PAIR(currentAttr));
	}
}

// textengine.h
void printAt(char c, int x, int y, Color col) {
	setColor(col);
	mvaddch(y, x, c);
}


// textengine.h
void flushScreen() {
	refresh();
}

// TODO Check this out for keyboard input handling!!!
// http://www.cplusplus.com/forum/unices/11910/


bool tryInitNcurses() {
	initscr();
	noecho();
	keypad(stdscr, true);
	curs_set(0);
	nodelay(stdscr, true);
	
	if (!has_colors()) {
		std::cout << "Cannot start game, console does not have color support! Exitin..." << std::endl;
		return false;
	}
	
	start_color();

	// Define custom colors, the built-ins are
	// not close enough to what I want
	int TSCOL_BLUE = 20;
	int TSCOL_BLACK = 21;
	int TSCOL_PURPLE = 22;
	int TSCOL_DEFAULT = 23;
	int TSCOL_WHITE = 24;
	int TSCOL_GREEN = 25;

	// color range 0-999
	init_color(TSCOL_BLUE, 0, 999, 999);
	init_color(TSCOL_BLACK, 0,0,0);
	init_color(TSCOL_PURPLE, 999, 0, 999);
	init_color(TSCOL_DEFAULT, 750, 750, 750);
	init_color(TSCOL_GREEN, 0, 999, 0);
	init_color(TSCOL_WHITE, 999, 999, 999);


	init_pair(1, TSCOL_DEFAULT, TSCOL_BLACK);
	init_pair(2, COLOR_RED, TSCOL_BLACK);
	init_pair(3, TSCOL_BLUE, TSCOL_BLACK);
	init_pair(4, COLOR_GREEN, TSCOL_BLACK);
	init_pair(5, TSCOL_WHITE, TSCOL_BLACK);
	init_pair(6, TSCOL_PURPLE, TSCOL_BLACK);

	// force background to be black!
	bkgd(COLOR_PAIR(1));



	return true;
}


void cleanupNcurses() {
	endwin();
}


void fetchKeyboardInput() {
	numKeysDown = 0;
	fastForwardButtonDown = false;
	int c;
	while((c = getch()) != ERR) {
		keysDown[numKeysDown++] = (char)c;
		if ((char)c == ' ') { skipToEndButtonPressed = true; }
	}
}


main() {
	
	if (!tryInitNcurses()) {
		cleanupNcurses();
		return 0;
	}

	
	clearScreen();

	initGame();
	bool lp = true;
	while(lp) {
		int msPassed = 10;
		usleep(10000);
		fetchKeyboardInput();
		lp = tick(msPassed);
	}

	cleanupNcurses();
	return 0;
}


