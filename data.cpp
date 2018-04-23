#include <string>

using namespace std;

struct GameData {
	string currentState;
	int flashbackCount;
	int round;
	int roundStep;
	int timesPlayed;
	string deck[8];
	string discovered[8];
	int solved;
	string unsolved = "?????";
	int pickedCard1;
	int pickedCard2;

	int flushDeck;
	bool hasCarKeys;
	bool connectedCoffeeMachine;
	bool hasBoots;
	bool daughterInformed;
	bool coffeeOn;
	bool gameOver;
	string ending;
	
} gameData;

const int MAX_OPTS = 16;

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
	"A firetruck", "A cloud that looks like a ship", "A magnifying glass", "An eye",
   	"A picture of Mona Lisa", "A leaping tiger", "A cat with a pipe", "Two lovers holding each other",
	"A picture of Dusty", "A photo from my wedding", "A human brain", "A bench under a huge oak tree" };



string startScreen = "#cR"
"#s9                               #s1==============#n "
"#s9                                 #s1DEMEMTORY#n "
"#s9                               #s1==============#cD#p3#n #n "
"A text adventure combining #cBDementia#cD and the card game #cBMemory#cD.#p2#n "
"Made for Ludum Dare 41#p2 by #cBTommy Salomonsson#cD.#n #n "
"Instructions:#n #s3Press Space Bar if you ever want to speed up the text (as in #cBPRESS AND HOLD#cD... go ahead and try it right away). Sometimes you just want to skip right to the end.";
Option tut = {"To select an action (white text), type the #>Purple character on your keyboard. That would be a 'P' in this case!", "start", "pP"};



string textStart = "I'm sitting in the kitchen#p1, looking out the window#p1 just waiting for time to pass.#p1 "
"It's a gray autumn afternoon outside. #p1It's been ages since anyone visited me #p1or even talked to me.#p1#n #n " 
"I take out my good old game of #cBmemory#cD, shuffles the cards and puts them on the table. Backface up.#p1#n #n "
"It's in times like this when I really miss my old cat#p1, #cBDusty#cD.";
// play 3 rounds 

Option play = {"Play a game of #>Memory", "play", "mM"};
Option endGame = {"Thanks for playing my game! Press #cPESC#cW to end it", "", ""};

// ---------------------------------
// --- flashback1
string flashback1 = "I'm looking down at the cards on the table. They are very old and worn. They remind me of my wife #p1#cBElise#cD. "
"She's been dead for several years now, but we always played together. She used to always win, she had such a great memory. "
"I feel old and worn like these cards. Old and worn and lonley. "
"Suddenly I feel sick. I don't want to look at these cards anymore!";
// get a glass of water
// get some air outside

Option flashback1a = {"Get a glass of #>Water", "kitchen1", "wW"};
Option flashback1b = {"Go out and get some fresh #>Air", "out1", "aA"};

// --- kitchen1
string kitchen1 = "I'm standing by the fridge. Why did I go here again? "
"Damn, I hate when this happens. Why did I go here... lets see...";
// look in fridge
// look at coffe machine

Option kitchen1a = {"Open the #>Fridge", "fridge1", "fF"};
Option kitchen1b = {"Look at the #>Coffee machine", "coffee1", "cC"};


// --- fridge1
string fridge1 = "The fridge is almost empty. I should probably go to the grocary store soon. "
"Wait, there is something on the shelf. #cBA key#cD? Why the hell is there a key in the fridge?";
// pick up key
// close door

Option fridge1a = {"Pick up the #>Key", "fridge1key", "kK"};
Option fridge1b = {"#>Close the door", "fridge1close", "cC"};


// --- fridge1key
string fridge1key = "It looks like #cBcar keys#cD. I put them in my pocket and close the door. "; // -> kitchen1_ending 


// --- fridge1close
string fridge1close = "I don't even want to know why someone has put a key in my fridge. "
"I close the door and look around. "; // -> kitchen1_ending 



// --- coffee1
string coffee1 = "I actually feel a little thirsty, a cup of coffee would be nice... "
"But as I try to start it I realise that someone has #cBunplugged the power cord#cD. Good I noticed, no coffee without electricity!";
// connect power cord
// don't connect power cord

Option coffee1a = {"#>Connect power cord", "coffee1power", "cC"};
Option coffee1b = {"#>Don't connect the power cord", "coffe1dont", "dD"};


// --- coffee1power
string coffee1power = "I connect the power cord. Now the coffee machine works again! Now I only need a cup, lets see... "
"Oh, look over there! "; // -> textGetWaterEnding

// --- coffee1dont
string coffee1dont = "Now that I think about it I'm not in the mood for coffee. It only messes up my stomach anyways. "; // -> kitchen1_ending 

// --- kitchen1_ending
string kitchen1_ending = "On the kitchen table is a partially played game of memory. "
"Elise and I always used to play together. It was nice.#p2 ";
// play


// --- out1
string out1 = "I stand in the doorway. It is cold and foggy outside and I only have my robe "
"and slippers on so I don't step outside.#n #n "
"The yard is full of yellow leaves. Elise used to love this time of the year for some strange "
"reason, and thanks to that it always fills me with warm memories. She used to love to sit on the bench under "
"the huge oak in the back with #cBDusty#cD in her lap.#n #n "
"Where is Dusty by the way? He never misses a meal, he should be here somewhere!";
Option out1a = { "Lets go to the #>Oak, it's been so long since I last went there", "out2", "oO" };
Option out1b = { "If I open a can of food from the #>Fridge Dusty is sure to come running", "fridge1", "fF"};

// --- out2
string out2 = "I'm standing in the hallway having just #cBput on my boots#cD. I know there was a reason "
"for putting them on, but it seems to have fled my mind. Damn, I hate when this happens!#n "
"Lets see... I'm still in my robe so it must have been something really quick errand.#n "; // -> kitchen1_ending


// -------------- FLASHBACK2
string flashback2 = "I get a strange feeling that the cards have some kind of meaning, but that can't "
"be right. I'm just old and stupid. Old. And Stupid. And lonley. #n #n "
"Where is #cBDusty#cD? Having him purring in my lap while I'm playing memory always gets me in a better "
"mood when I'm feeling down. Oh I need company of someone, even if it is a cat!";
Option flashback2a = {"Sometimes he hides under the #>Bed", "bed1", "bB" };
Option flashback2b = {"Could he be outside? I should look if I need to #>Let him in", "yard1", "lL" };
Option flashback2c = {"Lets fill up his #>Food bowl, he'll come running like a rocket", "kitchen2", "fF" };

// --- bed1
string bed1keys = "I look under the bed. It's dark. And dirty. I stand on all four and try to squint "
"into the darkness when I hear a clinking sound. #cBKeys#cD. They fell from my pocket. Why do I have car keys "
"in my robe pocket? I don't even know who they belong to.#n #n "
"I throw the keys on to the bed and keep looking under the bed. No, he's not there.";

string bed1 = "I look under the bed. It's dark. And dirty. No, he's not there.";

// -- yard1
string yard1 = "It's cold outside, and I'm not wearing any shoes. I call for him, but he does not come. "
"Someone is walking by and staring accusingly at me. A stranger, I don't care.";
Option yard1a = {"#>Keep calling", "flashback2ending", "kK"};
Option yard1b = {"#>Go back inside", "flashback2ending", "gG"};


string yard1boots = "I'm already wearing boots, so I walk out into the yard.#n #n "
"I call out for Dusty, but he's not answering. This is so weird, where can he be?#n #n "
"I go to the bench under the big oak in the back of the yard, where he loved to sit together with "
"Elise. Oh, how I miss her. Where are you Dusty?";
Option yard1c = { "I should not go too #>Far. I'm only in my robe, and it's pretty cold out", "walking1", "fF" };

// --- kitchen2
string kitchen2 = "Cat food. Where do I keep my cat food?#p3 I look through the pantry and "
"all the places I use to store food, but I cannot find it anywhere.#n ";
Option kitchen2a = {"Well, I need to go to the #>Grocery store anyway", "out3", "gG" };

// --- out3
string out3 = "I'm standing on the driveway. My #cBcar#cD is parked here. It's a bit of a walk so I prefer "
"to go by car. I try to open the door... Locked. #n #n #p2";

string out3keys = "In my pocket I can feel the weight of the keys. Perfect.";
string out3boots = "That's just my luck, but it's not far to walk and these boots are of top quality.";
string out3else = "I'm only in my slippers and it's wet and cold. I should get back in. I'm feeling very sad.";

Option out3keysa = {"#>Drive to the store", "badending1", "dD"};
Option out3bootsa = {"Lets #>Walk to the store", "walking1", "wW" };
Option out3elsea = {"Lets get back #>Inside", "flashback2ending", "iI" };

// --- walking1
string walking1 = "I lived here for as long as I can remember, but so many new buildings are built. "
"With so many new strangers in them. Not as polite as they used to be back in the days. They are all "
"staring. No one says hello.#n #n "
"Damn it's cold out. Why did I not bring a jacket? In fact... what AM I wearing? Only a robe? "
"Why the hell am I walking outside in just a robe?!?";
Option walking1a {"Turn #>Back", "walking2", "bB"};
Option walking1b {"Keep #>Walking", "walking2", "wW" };

// --- walking2
string walking2 = "I turn around and look the way I just came. Then i look forward again. I don't "
"recognize these houses. Actually. I'm not sure where I am. I walk a little bit further until I reach "
"a crossing in the road. It's really cold now. My teeth start clattering.";
Option walking2a = {"Go #>North", "walking3", "nN" };
Option walking2b = {"Go #>South", "walking3", "sS" };
Option walking2c = {"Go #>West", "walking3", "wW" };
Option walking2d = {"Go #>East", "walking3", "eE" };

// --- walking3
string walking3 = "Someone comes up from behind and grabs me. It's a woman. She's wondering what I'm "
"doing. I'm feeling very ashamed and try to get free of her grip, but I'm too cold and too weak. "
"She says something about calling #cBmy daughter#cD, she does not make any sense. She walks with me somewhere "
"unfamiliar.";
Option walking3a = {"#>Follow", "flashback2ending", "fF" };
Option walking3b = {"#>Struggle to get free", "flashback2ending", "sS" };

// --- badending1
string badending1 = "It takes several tries to start the car. The first time the gear is too high. The "
"second time I forget to release the hand break. But at last I'm out on the road.#n #n #p3"
"I was a really good driver when I was young, and I still enjoy a good ride with the wind blowing in my "
"hair. I gear up and wind down the side window.#n #n #p3"
"In the passenger seat sits #cBElise#cD. She is so young and so beautiful, and her smile is to die for.#p3 "
"She's smiling now.#p3 The blond hair flowing in the wind.#p2 I press the gas pedal a bit more.#n #n "
"#s3I'm feeling so happy.#s2#n #n #n "
"#cRGAME OVER! (you reached ending: 'BAD_ENDING_CAR')#cD#s1";

// --- flashback2ending
string flashback2ending = "I'm sitting at the kitchen table again. I'm cold and has a feeling of shame, "
"but I cannot remember why. In front of me are those memory cards. Did I walk away, or have I been "
"sitting here the whole time?";


// ---------------- flashback3
string flashback3 = "I'm stiff. And bored. I need to stand up.";
Option flashback3a = {"I need a cup of #>Coffee", "coffee2", "cC" };

// --- coffee2
string coffee2 = "It feels like I wake up from a dream. I have been standing pushing the coffee maching "
"power button on and off. On and off. On and off.#n #n "
"I snap out of it and turn it on.";

string coffee2connected = "The little power button lights up. I get back to the table";
string coffee2disconnected = "The power button does not light up. That's strange! I feel tempted to press "
"off and on again, but resist and walk out to the hallway instead.";

Option coffee2disconnecteda = {"I need to get my #>Coffe some other way it seems", "out4", "cC"};

// --- badending2
string badending2 = "I suddenly smell smoke. Who can it be that is smoking? I know Elise used "
"to sneak out and have a smoke every now and again. I remember how she used to smell when sneaking in "
"after having had one of those smokes under the oak. She only laughed when I told her to stop. That "
"beautiful smile of hers.#n #n "
"I should really... do... Argh, I can't think with this alarm constantly ringing. And I'm tired.#n #n "
"Tired of being alone. Tired of feeling ashamed. Tired of this loud alarm. And actually just plain tired.#n #n "
"I should go to bed. Tomorrow I'm sure Dusty will come back home.#n #n #n #s2#cR"
"GAME OVER! (You reached ending: 'BAD_ENDING_SMOKE')#cD#s1";

// --- out4
string out4 = "I'm outside. I'm walking on the sidewalk. I'm freezing my butt off!#n #n "
"The houses look strange. I haven't walked that long, have I? Or maybe I have. I'm not sure. "
"And why am I still in this bloody robe? It's getting really, really cold!#n #n "
"Suddenly I see a police car driving along the street. I could ask them for directions, but I'm "
"actually not sure where I'm heading, and I feel ashamed enough walking in this robe.#n #n "
"To the side, just a few steps from the sidewalk is the edge of the forest.";
Option out4a = {"Shold I call the #>Police over", "flashback3ending", "pP" };
Option out4b = {"or should I step out of #>Sight, and let them pass?", "badending3", "sS" };

//--- badending3
string badending3 = "I quickly step in behind the trees. I don't think the police saw me.#n #n "
"It's so cold! My hands are shaking. Then I hear a familiar sound, and my heart skips a beat.#n #n "
"A cat meows. It's Dusty! Where have you been all this time?!?#n #n "
"He only looks at me, and starts walking. His black fur gleaming.#n #n "
"He is leading me home!#n #n #n #s2#cR"
"GAME OVER! (You reached ending: 'BAD_ENDING_FOREST')#cD#s1";

// --- flashback3ending
string flashback3ending = "I'm sitting at my kitchen table. In front of me is a partially completed "
"game of memory cards. I'm very cold, and my robe is even a bit wet. I wonder why.#n #n "
"I should try and finish this game before going to bed. If Elise had been here we would already "
"have solved it. She is out somewhere right now though. I need to solve this one myself.";


// --- flashback4
string flashback4 = "Suddenly there's a noise from the hallway, and Elise is back. "
"She steps in to the kitchen with her beautiful smile. ";

string flashback4daughter = "So I hear you are out walking the neighbourhood in only your robe? Ms Watson "
"called me right after she left you here. This cannot go on, we're placing you in a home tomorrow. "
"I'll sleep over tonight so you don't wander away again. Oh, and one more thing! ";

string flashback4cont = "She brings me a cage with a black cat in it. It's Dusty! She found him!#n "
"'I thought you needed someone to keep you company, and you always loved cats', she says. I'm so happy! Elise is home and Dusty "
"is finally back! #n #n "
"We spend the rest of the evening playing memory together. Dusty is lying in my lap, purring. "
"Her memory is still much better than mine. #s3I'm so happy!#n #n #s2#cR"
"GAME OVER! ";

string flashback4endpartial = "(You reached ending: 'GOOD_ENDING_PARTIAL)#s1#cD ";
string flashback4endfull = "(You reached ending: 'GOOD_ENDING_FULL)#s1#cD ";

void setEventText(Event* e, string s) {
	e->text = s;
	e->numOptions = 0;
}
void addEventOpt(Event* e, Option o) {
	e->options[e->numOptions] = o;
	e->numOptions++;
}

void getEvent(Event* e) {
	string state = gameData.currentState;

	if (state == "startScreen") {
		setEventText(e, startScreen);
		addEventOpt(e, tut);
		return;
	}

	if (state == "start") {
		setEventText(e, textStart);
		addEventOpt(e, play);
		return;
	}

	if (gameData.round == 3 && gameData.flashbackCount == 0) {
		gameData.currentState = "flashback1";
		gameData.flashbackCount = 1;
		setEventText(e, flashback1);	
		addEventOpt(e, flashback1a);
		addEventOpt(e, flashback1b);
		return;
	}
	if (state == "kitchen1") {
		setEventText(e, kitchen1);
		addEventOpt(e, kitchen1a);
		addEventOpt(e, kitchen1b);
		return;
	}
	if (state == "fridge1") {
		setEventText(e, fridge1);
		addEventOpt(e, fridge1a);
		addEventOpt(e, fridge1b);
		return;
	}
	if (state == "fridge1key") {
		gameData.hasCarKeys = true;
		setEventText(e, fridge1key + kitchen1_ending);
		addEventOpt(e, play);
		return;
	}
	if (state == "fridge1close") {
		setEventText(e, fridge1close + kitchen1_ending);
		addEventOpt(e, play);
		return;
	}
	if (state == "coffee1") {
		setEventText(e, coffee1);
		addEventOpt(e, coffee1a);
		addEventOpt(e, coffee1b);
		return;
	}
	if (state == "coffee1power") {
		gameData.connectedCoffeeMachine = true;
		setEventText(e, coffee1power + kitchen1_ending);
		addEventOpt(e, play);
		return;
	}
	if (state == "coffee1dont") {
		setEventText(e, coffee1dont + kitchen1_ending);
		addEventOpt(e, play);
	}
	if (state == "out1") {
		setEventText(e, out1);
		addEventOpt(e, out1a);
		addEventOpt(e, out1b);
	}
	if (state == "out2") {
		gameData.hasBoots = true;
		setEventText(e, out2 + kitchen1_ending);
		addEventOpt(e, play);
	}


	if (gameData.flashbackCount == 1 && gameData.solved >= 3) {
		gameData.currentState = "flashback2";
		gameData.flashbackCount = 2;
		setEventText(e, flashback2);
		addEventOpt(e, flashback2a);
		addEventOpt(e, flashback2b);
		addEventOpt(e, flashback2c);
	}
	if (state == "bed1") {
		if (gameData.hasCarKeys) {
			setEventText(e, bed1keys);
			gameData.hasCarKeys = false;
		}
		else {
			setEventText(e, bed1);
		}
		addEventOpt(e, flashback2b);
		addEventOpt(e, flashback2c);
	}
	if (state == "yard1") {
		if (gameData.hasBoots) {
			setEventText(e, yard1boots);
			addEventOpt(e, yard1c);
		}
		else {
			setEventText(e, yard1);
			addEventOpt(e, yard1a);
			addEventOpt(e, yard1b);
		}
	}
	if (state == "kitchen2") {
		setEventText(e, kitchen2);
		addEventOpt(e, kitchen2a);
	}
	if (state == "out3") {
		if (gameData.hasCarKeys) {
			setEventText(e, out3 + out3keys);
			addEventOpt(e, out3keysa);
		}
		else if (gameData.hasBoots) {
			setEventText(e, out3 + out3boots);
			addEventOpt(e, out3bootsa);
		}
		else {
			setEventText(e, out3 + out3else);
			addEventOpt(e, out3elsea);
		}
	}
	if (state == "walking1") {
		setEventText(e, walking1);
		addEventOpt(e, walking1a);
		addEventOpt(e, walking1b);
	}
	if (state == "walking2") {
		setEventText(e, walking2);
		addEventOpt(e, walking2a);
		addEventOpt(e, walking2b);
		addEventOpt(e, walking2c);
		addEventOpt(e, walking2d);
	}
	if (state == "walking3") {
		gameData.daughterInformed = true;
		setEventText(e, walking3);
		addEventOpt(e, walking3a);
		addEventOpt(e, walking3b);
	}
	if (state == "badending1") {
		gameData.gameOver = true;
		setEventText(e, badending1);
		addEventOpt(e, endGame);
	}
	if (state == "flashback2ending") {
		setEventText(e, flashback2ending);
		addEventOpt(e, play);
		gameData.flashbackCount = 2;
		gameData.flushDeck = 1;
	}

	if (gameData.flashbackCount == 2 && gameData.timesPlayed == 1) {
		gameData.flashbackCount = 3;
		gameData.currentState = "flashback3";
		setEventText(e, flashback3);
		addEventOpt(e, flashback3a);
		return;
	}
	if (state == "coffee2") {
		gameData.flushDeck = 2;
		if (gameData.connectedCoffeeMachine) {
			setEventText(e, coffee2 + coffee2connected);
			addEventOpt(e, play);
			gameData.coffeeOn = true;
		}
		else {
			setEventText(e, coffee2 + coffee2disconnected);
			addEventOpt(e, coffee2disconnecteda);
		}
		return;
	}
	if (gameData.flashbackCount == 3 && gameData.solved >= 1 && gameData.coffeeOn) {
		gameData.currentState = "badending2";
		setEventText(e, badending2);
		addEventOpt(e, endGame);
		gameData.gameOver = true;
		return;
	}
	if (state == "out4") {
		setEventText(e, out4);
		addEventOpt(e, out4a);
		addEventOpt(e, out4b);
	}
	if (state == "badending3") {
		setEventText(e, badending3);
		addEventOpt(e, endGame);
		gameData.gameOver = true;
	}
	if (state == "flashback3ending") {
		setEventText(e, flashback3ending);
		addEventOpt(e, play);
		gameData.flushDeck = 2;
	}
	if (gameData.flashbackCount == 3 && gameData.solved == 4) {
		gameData.currentState = "flashback4";
		if (gameData.daughterInformed) {
			setEventText(e, flashback4 + flashback4daughter + flashback4cont + flashback4endfull);
			gameData.gameOver = true;
		}
		else {
			setEventText(e, flashback4 + flashback4cont + flashback4endpartial);
			gameData.gameOver = true;
		}
		addEventOpt(e, endGame);
	}



	if (gameData.currentState == "play" || gameData.currentState =="play2") {
		setEventText(e, "");
		return;
	}
	if (gameData.currentState == "result") {
		setEventText(e, ""); 
		addEventOpt(e, play);
		return;
	}
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
