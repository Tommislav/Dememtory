#include <iostream>
#include <unistd.h>
#include <sys/ioctl.h>
#include "game.cpp"

using namespace std;

typedef unsigned short WORD;

int add(int a, int b) {
  return a + b;
}


char const* one() {
  return "one";
}

char const* two() {
  return "two";
}

void sendParamAsCopy(int param) {
	cout << "parameter as copy address: " << &param << endl;
}
void sendParamAsRef(int &param) {
	cout << "parameter as ref address: " << &param << endl;
}

void sendParamAsPtr(int *param) {
	cout << "parameter as ptr address: " << param << "  (val: "<< *param << ")" << endl;
}

void sendConstParam(const int param) {
	cout << "const param value is " << param << endl;
}

int test() {
  printf("wow\n"); 
  printf("hello world\n");
  std::cout << "test" << 123 << "hej" << endl;
  cout << "int: " << (sizeof(int)*8) << endl;

  int a = add(1,2);
  cout << a << endl;

  char const* s = "hej";
  cout << s << endl;

  cout << one() << endl;


  // function pointer "f", zero arguments, method have return value char const*
  char const* (*f)() = &one;
  f();


  int v = 1;
  int *pv = &v;
  cout << "value: " << (*pv) << ", addr: " << pv << endl;

  cout << "base address of v " << &v << endl;
  sendParamAsCopy(v);
  sendParamAsRef(v);
  sendParamAsPtr(&v);
  sendConstParam(1);
  
  
  string str1 = "hej";
  string str2 = str1 + std::to_string(v); // req. c++ 11
  cout << str2 << endl;

	testFuncInOtherFile();
  
  return 0;
}


void testPrintColorsCygwin() {
	cout << "\033[1;31mbold red text\033[0m\n";
}


void testPrintColorsWinCmd() {
	const WORD colors[] =
		{
		0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
		0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6
		};

	WORD   index   = 0;


	// Draw pretty colors until the user presses any key
	//while (WaitForSingleObject( hstdin, 100 ) == WAIT_TIMEOUT)
	int cnt = 10;
	while(--cnt > 0)
	{
		//SetConsoleTextAttribute( hstdout, colors[ index ] );
		//SetConsoleTextAttribute( hstdout, 0x8 );
		std::cout << "H" << std::flush;
		if (++index > sizeof(colors)/sizeof(colors[0]))
			index = 0;
	}
	// Keep users happy
	//SetConsoleTextAttribute( hstdout, csbi.wAttributes );
	//return 0;
}


void writeCharAt(int x, int y, char c) {
	printf("\033[%d;%dH", x+1, y+1);
	std::cout << c << std::flush;
}

void clearScreen() {
	std::cout << "\033[2J\033[1;1H" << std::flush;
	//std::system("clear");
}

// Things I want my engine to handle
// #################################
// get width/height
// print text at pos X,Y
// print sequence of text char by char
// change speed
// change color
// keep button pressed to speed up!
// handle input
// clear screen
// print chars to layers (buffers)
// animations. boxes. misc utils.

// TODO Check this out for keyboard input handling!!!
// http://www.cplusplus.com/forum/unices/11910/


main() {
	clearScreen();

	// Turn off console input! Remember to turn it back at the end of program
	system("stty raw -echo");

	// Handle input
	// https://docs.microsoft.com/en-us/windows/console/reading-input-buffer-events

	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	int width = w.ws_row;
	int height = w.ws_col;

	std::cout << "console width: " << width << ", height: " << height << std::endl;

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
	int iCol = 0;
	int lCol = 8;

	if (width > 0 && height > 0) {
		sleep(1000);

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

	system("stty cooked");
	return 0;
}


