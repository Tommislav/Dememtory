#include <string>

using namespace std;

const int MAX_OPTS = 16;
struct Options {
	string labels[MAX_OPTS];
	char input[MAX_OPTS];
	string states[MAX_OPTS];
	int insanities[MAX_OPTS];
	bool completed[MAX_OPTS];
	int len;
};

string randomCards[] = {
	"5", "2", "12", "7", "3", "22", "18", "88",
	"Queen of spades", "King of diamonds", "Ace of hearts", "Jack of clubs", "6", "1", "3 of spades", "9",
   	"A picture of Mona Lisa", "A leaping tiger", "A cat with a pipe", "Two lovers holding each other", "A firetruck", "A cloud that looks like a ship", "A magnifying glass", "An eye" };




string textStart = "I'm sitting in the kitchen#p1, looking out the window#p1 just waiting for time to pass.#p1 "
"It's a gray autumn afternoon outside. #p1It's been ages since anyone visited me #p1or even talked to me.#p1#n #n " 
"I take out my good old game of #cBmemory#cD, shuffles the cards and puts them on the table. Backface up.#p1#n #n "
"It's in times like this when I really miss my old cat#p1, #cBDusty#cD.";



Options start = {
	{ "Play a game of #>Memory", "Continue #>Look out the window", "#>Find the cat" },
	{ 'm', 'l', 'f' },
	{ "play", "window", "cat" },
	{ 0, 0, 1 },
	{ FALSE },
	3 };


string textMemoryBase = "I'm looking down at the cards on the table. ";

string textMemoryWife = "They are very old and worn. They remind me of my wife. "
"She's been dead for several years now, but we always played together. She used to always win, she had such a great memory. "
"I feel old and worn like these cards. Old and worn and lonley.";

string textMemoryThirsty = "They are old, just like me. My throat goes dry. I'm feeling sick.";

string textMemoryCat = "Why do I even keep playing? It only brings up bad memories. "
"I should go look for Dusty, he always makes me feel better. Where is he?";

Options memory = {
	{ "Have a glass of #>Water", "#>Quit the game", "Go look for #>Dusty" },
	{ 'w', 'q', 'd' },
	{ "kitchen", "quit", "cat" },
	{ 0, 2, 3 },
	{ FALSE },
	3 };


string memoryRandom[] = {textMemoryWife, textMemoryThirsty, textMemoryCat};

