#include <iostream>
#include <string>
#include "textengine.h"
#include "writer.cpp"
#include "data.cpp"

#include <stdlib.h> // rand/srand
#include <time.h> // time

using namespace std;


struct GameData {
	int insanity;	
	int numCardsSolved;
	int solvedCards[8];
	int lastPickedCard;
} gameData;



string currentState = "";
bool (*stateFunction)(int);
Options currentOption;
char listenForInput[MAX_OPTS];
string scenes[MAX_OPTS];
int listenForInputLen;
char keysDown[128];
int numKeysDown = 0;
bool skipButtonDown;
int stateTransition;
string transitionToState;
char lastStateInput;

Writer writer;
Writer optWriter;
termSize screenSize;



void setGameState(string);

void initGame() {
	screenSize = getTermSize();
	setGameState("play");
}


void setOptions(Options &opt) {
	listenForInputLen = 0;
	string s = "#cW";
	int cnt = 0;

	if (currentState == "play" || currentState == "play2") {
		for (int i=0; i<8; i++) {
			string num = to_string(++cnt);

			if (currentState == "play2") {
				int lastPicked = intFromChar(lastStateInput);
				if (i == lastPicked-1) {
					s += num + ". Card number " + num + ": #cG" + randomCards[gameData.lastPickedCard] + "#cD#n ";
					continue;
				}
			}
			listenForInput[listenForInputLen] = num[0];
			scenes[listenForInputLen] = currentState == "play2" ? "result" : "play2";
			listenForInputLen++;
			s += num + ". Card number #>"+num+ ": ????#n ";
		}	
	}

	for (int i=0; i<opt.len; i++) {
		if (opt.insanities[i] <= gameData.insanity) {
			s += to_string(cnt++) + ". " + opt.labels[i] + "#n ";
			listenForInput[listenForInputLen] = opt.input[i];
			scenes[listenForInputLen] = opt.states[i];
			listenForInputLen++;
		}
	}
	s += "> ";
	int y = writer.y;
	wrSetText(optWriter, s, 0, y+2);
}

bool keyIsDown(const char &c) {
	if (numKeysDown == 0) { return false; }
	for (int i=0; i<numKeysDown; i++) {
		if (keysDown[i] == c) { return true; }
	}
	return false;
}


char checkInput() {
	for (int i=0; i<listenForInputLen; i++) {
		if (keyIsDown(listenForInput[i])) {
			stateTransition = 400;
			transitionToState = scenes[i];
			lastStateInput = listenForInput[i];
			return listenForInput[i];
		}
	}
	return 0;
}



bool tick(int millisec) {
	if (stateTransition > 0) {
		stateTransition -= millisec;
		if (stateTransition <= 0) { setGameState(transitionToState); }
		return true;
	}

	// writerSkip is in writer.cpp
	// skipButtonDown is set from platform layer
	// yuck, this should be done beter...
	writerSkip = skipButtonDown;
	return stateFunction(millisec);
}


bool defaultWrite(int millisec) {
	bool dirty = false;
	if (!wrAtEnd(writer)) {
		dirty = wrPutChar(writer, millisec);
		if (wrAtEnd(writer)) {
				setOptions(currentOption);
		}
	}
	else if (!wrAtEnd(optWriter)) {
		dirty = wrPutChar(optWriter, millisec);
	}
	if (dirty) { 
		flushScreen();
		Writer* wr = wrAtEnd(writer) ? &optWriter : &writer;
		setCursorPos(wr->x, wr->y);
	}

	if (wrAtEnd(optWriter)) {
		char validInput = checkInput();
		if (validInput != 0) {
			printAt(validInput, optWriter.x, optWriter.y, Color::purple);
			flushScreen();
			setCursorPos(optWriter.x + 1, optWriter.y);
		}
	}
	return true;
}


void setGameState(string state) {
	currentState = state;
	clearScreen();
	if (state == "start") {
		wrSetText(writer, textStart, 0, 1);
		stateFunction = &defaultWrite;
		currentOption = start;
	}
	else if (state == "play") {
		wrSetText(writer, textMemoryBase + textMemoryWife, 0, 1);
		stateFunction = &defaultWrite;
		currentOption = memory;
		gameData.lastPickedCard = -1;
	}
	else if (state == "play2") {
		srand(time(NULL));
		int c1 = rand() % 8;
		gameData.lastPickedCard = c1;
		string txt = "First card is #cG" + randomCards[c1] + "#cD, pick one more!";
		wrSetText(writer, txt, 0, 1);
		currentOption = memory;
	}
	else if (state == "result") {
		int c2 = rand() % 8;
		string txt;
		if (c2 == gameData.lastPickedCard) {
			"The second card is also #cG" + randomCards[c2] + "#cD!";
			gameData.numCardsSolved += 2;
		}
		else {
			"The second card is #cG" + randomCards[c2] + "#cD";
		}
	}
}
