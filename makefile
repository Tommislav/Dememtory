hellomake: main_win.cpp
	g++ main_win.cpp -DtempDev -o textadventure.exe
	cmd.exe /C start cmd.exe /C textadventure.exe	
