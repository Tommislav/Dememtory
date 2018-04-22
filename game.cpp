#include <iostream>
//#include <string>
#include "textengine.h"
#include "writer.cpp"
#include "data.cpp"

#include <stdlib.h> // rand/srand
#include <time.h> // time

using namespace std;


bool (*stateFunction)(int);
Event currentEvent;
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
	buildDeck(&randomCards[8], &gameData.deck[0], &gameData.discovered[0], gameData.solved, gameData.unsolved);
	gameData.pickedCard1 = -1;
	gameData.pickedCard2 = -1;
	setGameState("start");
}


string updateAvailableCardsForInput(string nextState) {
	string opt = "#s9";
	opt += gameData.currentState == "play" ? " > Which card to flip? " : " > I can flip one more: ";
	int cnt = 0;
	for (int i=0; i<8; i++) {
		bool isSolved = gameData.discovered[i] != gameData.unsolved;
		bool wasPicked = i == gameData.pickedCard1 || i == gameData.pickedCard2;
		bool selectable = !isSolved && !wasPicked;
		if (selectable) {
			string strNum = to_string(i+1);
			if (cnt > 0) { opt += ", "; }
			opt += "#>" + strNum;
			listenForInput[listenForInputLen] = strNum[0];	
			scenes[listenForInputLen] = nextState;
			listenForInputLen++;
			cnt++;
		}
	}
	if (cnt == 0) { return ""; }
	opt += "#s1 #n ";
	return opt;
}

void setOptions() {
	listenForInputLen = 0;
	string s = "#cW";

	if (gameData.currentState == "play" || gameData.currentState == "play2") {
		s += updateAvailableCardsForInput(gameData.currentState == "play" ? "play2" : "result");
	}

	for (int i=0; i<currentEvent.numOptions; i++) {
		s += " > " + currentEvent.options[i].label + "#n ";

		string input = currentEvent.options[i].inputs;
		for (int j=0; j<input.length(); j++) {
			listenForInput[listenForInputLen] = input[j];
			scenes[listenForInputLen] = currentEvent.options[i].state;
			listenForInputLen++;
		}
	}
	s += ">>> ";
	int y = writer.y;
	wrSetText(optWriter, s, 0, y+2);
	optWriter.tim.wait = -1;
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

			if (gameData.currentState == "play") { gameData.pickedCard1 = intFromChar(lastStateInput) - 1; }
			if (gameData.currentState == "play2") { gameData.pickedCard2 = intFromChar(lastStateInput) - 1; }

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
				setOptions();
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
		if (checkInput() != 0) {
			printAt(lastStateInput, optWriter.x, optWriter.y, Color::purple);
			flushScreen();
			setCursorPos(optWriter.x + 1, optWriter.y);
		}
	}
	return true;
}


void setGameState(string state) {
	gameData.roundStep++;
	gameData.currentState = state;
	clearScreen();

	getEvent(&currentEvent);

	if (state == "play" || state == "play2" || state == "result") {
		if (state == "play") {
			gameData.pickedCard1 = -1;
			gameData.pickedCard2 = -1;
		}

		string cardsLayout = getCardsLayout(
				&gameData.discovered[0], 
				&gameData.deck[0], 
				gameData.pickedCard1, 
				gameData.pickedCard2);
		
		stateFunction = &defaultWrite;

		if (state == "result") {

			string card1 = gameData.pickedCard1 < 0 ? "ERR":gameData.deck[gameData.pickedCard1];
			string card2 = gameData.pickedCard2 < 0 ? "ERR":gameData.deck[gameData.pickedCard2];

			bool match = card1 == card2;
			string matchText = "That is a match!#p3 ";
			string failText = "That is not right... I slowly flip the cards back again.#p2 ";
			cardsLayout +="#n " + (match ? matchText : failText);
			if (match) { 
				gameData.discovered[gameData.pickedCard1] = card1;
				gameData.discovered[gameData.pickedCard2] = card2;
				gameData.solved++; 
			}

			gameData.round++; // START NEW ROUND
			gameData.roundStep = 0;
		}

		wrSetText(writer, cardsLayout, 0, 1);
	}
	else {
		wrSetText(writer, currentEvent.text, 0, 1);
		stateFunction = &defaultWrite;
	}
}
