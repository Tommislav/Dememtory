#include <string>

// sleep
#include <chrono>
#include <thread>

struct termSize {
	int width, int height;
}

void clearScreen();
termSize getTermSize();
void printAt(std::string str, int x, int y);

void printAt(std::string str, int x, int y, int layer);

void sleep(int ms) {
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
