#include <string>

using namespace std;

struct GameData {
	string currentState;
	int round;
	int roundStep;
	string deck[8];
	string discovered[8];
	int solved;
	string unsolved = "?????";
	int pickedCard1;
	int pickedCard2;
	
} gameData;

const int MAX_OPTS = 16;
/*
struct Options {
	string labels[MAX_OPTS];
	char input[MAX_OPTS];
	string states[MAX_OPTS];
	int insanities[MAX_OPTS];
	bool completed[MAX_OPTS];
	int len;
};
*/

struct Option {
	string label;
	string state;
	string inputs;
};

struct Event {
	string id;
	string text;
	Option options[16];
	int numOptions;
};



string randomCards[] = {
	"Queen of spades", "King of diamonds", "Ace of hearts", "Jack of clubs",
   	"A picture of Mona Lisa", "A leaping tiger", "A cat with a pipe", "Two lovers holding each other",
	"A firetruck", "A cloud that looks like a ship", "A magnifying glass", "An eye" };




string textStart = "I'm sitting in the kitchen#p1, looking out the window#p1 just waiting for time to pass.#p1 "
"It's a gray autumn afternoon outside. #p1It's been ages since anyone visited me #p1or even talked to me.#p1#n #n " 
"I take out my good old game of #cBmemory#cD, shuffles the cards and puts them on the table. Backface up.#p1#n #n "
"It's in times like this when I really miss my old cat#p1, #cBDusty#cD.";
// play 3 rounds 

Option play = {"Play a game of #>Memory", "play", "mM"};
Option empty = { "", "", "" };

// ---------------------------------
// --- flashback1
string flashback1 = "I'm looking down at the cards on the table. They are very old and worn. They remind me of my wife. "
"She's been dead for several years now, but we always played together. She used to always win, she had such a great memory. "
"I feel old and worn like these cards. Old and worn and lonley."
"Suddenly I feel sick. I don't want to look at these cards anymore!";
// get a glass of water
// get some air outside

Option flashback1a = {"Get a glass of #>Water", "kitchen1", "wW"};
Option flashback1b = {"Go out and get some fresh #>Air", "out1", "aA"};

// --- kitchen1
string kitchen1 = "I'm standing by the fridge. Why did I go here again?"
"Damn, I hate when this happens. Why did I go here... lets see...";
// look in fridge
// look at coffe machine

//Option kitchen1a = {"Open the #>Fridge", "fridge1", "fF"};
//Option kitchen1b = {"Look at the #>Coffee machine", "coffee1", "cC"};


// --- fridge1
string fridge1 = "The fridge is almost empty. I should probably go to the grocary store soon."
"Wait, there is something on the shelf. A key? Why the hell is there a key in the fridge?";
// pick up key
// close door

//Option fridge1a("Pick up the #>Key", "fridge1key", "kK");
//Option fridge1b("#>Close the door", "fridge1close", "cC");


// --- fridge1key
string fridge1key = "It looks like car keys. I put them in my pocket and close the door. "; // -> kitchen1_ending 


// --- fridge1close
string fridge1close = "I don't even want to know why someone has put a key in my fridge."
"I close the door and look around. "; // -> kitchen1_ending 



// --- coffee1
string coffee1 = "I actually feel a little thirsty, a cup of coffee would be nice..."
"But as I try to start it I realise that someone has unplugged the power cord. Good I noticed, no coffee without electricity!";
// connect power cord
// don't connect power cord

//Option coffee1a("#>Connect power cord", "coffee1power", "cC");
//Option coffee1b("#>Don't connect the power cord", "coffe1dont", "dD");


// --- coffee1power
string coffee1power = "I connect the power cord. Now the coffee machine works again! Now I only need a cup, lets see..."
"Oh, look over there! "; // -> textGetWaterEnding

// --- coffee1dont
string coffee1dont = "Now that I think about it I'm not in the mood for coffee. It only messes up my stomach anyways."; // -> kitchen1_ending 

// --- kitchen1_ending
string kitchen1_ending = "On the kitchen table is a partially played game of memory."
"My wife and I always used to play together. It was nice.";
// play








/*
Options start = {
	{ "Play a game of #>Memory", "Continue #>Look out the window", "#>Find the cat" },
	{ 'm', 'l', 'f' },
	{ "play", "window", "cat" },
	{ 0, 0, 1 },
	{ false },
	3 };

Options optPlay = {
	{ "Have another go at that #>Memory" },
	{ 'm' },
	{ "play" },
	{ 0 },
	{ false },
	1 };

Options emptyOpts = { {},{},{},{},{},0};

string textMemoryBase = "I'm looking down at the cards on the table. ";


string textMemoryThirsty = "They are old, just like me. My throat goes dry. I'm feeling sick.";

string textMemoryCat = "Why do I even keep playing? It only brings up bad memories. "
"I should go look for Dusty, he always makes me feel better. Where is he?";

Options memory = {
	{ "Have a glass of #>Water", "#>Quit the game", "Go look for #>Dusty" },
	{ 'w', 'q', 'd' },
	{ "kitchen", "quit", "cat" },
	{ 0, 2, 3 },
	{ false },
	3 };


string memoryRandom[] = {textMemoryWife, textMemoryThirsty, textMemoryCat};
*/



void getEvent(Event* event) {
	if (gameData.currentState == "play" || gameData.currentState =="play2") {
		event->text = "";
		event->numOptions = 0;
		return;
	}



	event->text = textStart;
	event->options[0] = play;
	event->numOptions = 1;
}






string getCardsLayout(string* values, string* deck, int selected1, int selected2) {
	string s = "#s9";

	for (int i=0; i<8; i++) {
		bool wasSelected = i == selected1 || i == selected2;
		string cardVal = wasSelected ? *(deck+i) : *(values+i);
		string cardNum = to_string(i+1);		
		string cheat = *(deck+i);
		if (wasSelected) { s += "#cG"; }
		s += " > Card number " + cardNum + ": " + cardVal + "#cD";
		if (i < 7) { s+= "#n "; }
	}
	s += "#s1#p2 ";
	return s;
}
