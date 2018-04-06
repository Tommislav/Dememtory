#include <iostream>
#include <windows.h>
#include "game.cpp"
#include <stdlib.h> // rand(), srand()

using namespace std;

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

	HANDLE hstdin  = GetStdHandle( STD_INPUT_HANDLE  );
	HANDLE hstdout = GetStdHandle( STD_OUTPUT_HANDLE );
	WORD   index   = 0;

	// Remember how things were when we started
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo( hstdout, &csbi );

	// Tell the user how to stop
	SetConsoleTextAttribute( hstdout, 0xEC );
	std::cout << "Press any key to quit.\n";

	// Draw pretty colors until the user presses any key
	//while (WaitForSingleObject( hstdin, 100 ) == WAIT_TIMEOUT)
	int cnt = 10;
	while(--cnt > 0)
	{
		//SetConsoleTextAttribute( hstdout, colors[ index ] );
		SetConsoleTextAttribute( hstdout, 0x8 );
		std::cout << "H" << std::flush;
		if (++index > sizeof(colors)/sizeof(colors[0]))
			index = 0;
	}
	FlushConsoleInputBuffer( hstdin );

	// Keep users happy
	SetConsoleTextAttribute( hstdout, csbi.wAttributes );
	//return 0;
}


void writeCharAt(HANDLE *handle, SHORT x, SHORT y, char c) {
	COORD pos = {x, y};
	SetConsoleCursorPosition(*handle, pos);
	std::cout << c << std::flush;
}


main() {
	// https://docs.microsoft.com/en-us/windows/console/writeconsole
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO buffer;
	bool success = GetConsoleScreenBufferInfo(hOut, &buffer);
	if (!success) {
		std::cout << "Error fetching console screen buffer" << std::endl;
		return 123;
	}





	// Handle input
	// https://docs.microsoft.com/en-us/windows/console/reading-input-buffer-events
	std::cout << "Press any key: " << std::flush;

	DWORD numRead;
	INPUT_RECORD inputRecBuffer[128];
	if (!SetConsoleMode(hIn, ENABLE_WINDOW_INPUT)) {
		std::cout << "Error setting console mode" << std::endl;
		sleep(5000);
		return 1;
	}

	

	// TODO: We need to write a game loop. To measure performance, and to know how long to sleep, take a look at QueryPerformanceCounter
	// https://msdn.microsoft.com/en-us/library/windows/desktop/ms644904(v=vs.85).aspx
	// and
	// https://msdn.microsoft.com/en-us/library/windows/desktop/dn553408(v=vs.85).aspx
	while (true) {
		// wait for event
		ReadConsoleInput(hIn, inputRecBuffer, 128, &numRead);
		std::cout << numRead << " " << std::flush;
		for (int i=0; i<numRead; i++) {
			switch(inputRecBuffer[i].EventType) {
				case KEY_EVENT:
					KEY_EVENT_RECORD keyEvent = inputRecBuffer[i].Event.KeyEvent;
					std::cout << "Key down: " << keyEvent.bKeyDown << " [" << "x" << "]" << std::flush;	
					break;
			}
		}
		sleep(16);
	}






	//COORD size = GetLargestConsoleWindowSize(hOut);
	COORD size = buffer.dwSize;
	std::cout << "console width: " << size.X << ", height: " << size.Y << std::endl;
	std::cout << "window size: " << buffer.srWindow.Top << ", " << buffer.srWindow.Bottom << std::endl;

	int width = size.X;
	int height = buffer.srWindow.Bottom - buffer.srWindow.Top;
	int top = buffer.srWindow.Top;


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

	if (size.X > 0 && size.Y > 0) {
		sleep(5000);
		for (int n=0; n<20; n++) {
			for (int step=0; step<(height>>1); step++) {

				for (int y=step; y<height-step; y++) {
					if (y == step || y == height-step-1) {
						for (int x=step; x<width-step; x++) {
							writeCharAt(&hOut, x, y, '#');
						}
					}
					writeCharAt(&hOut, step, y, '#');
					writeCharAt(&hOut, width-step-1, y, '#');

				}
				sleep(5);
				std::cout << colors[iCol++];
				iCol %= lCol;
			}
		}
	}
	

	writeCharAt(&hOut, 1, 20, ' ');
	std::cout << "\033[0m";






// for cygwin
// http://www.cplusplus.com/forum/general/18200/
// or
// https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal
	initGame();
	while (getGameIsRunning()) {
		tick();
		sleep(getSleepMs());
	}
	cin.ignore();
	return 0;
}


