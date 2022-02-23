#pragma once
#include <windows.h>
#include <process.h>
#include <conio.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <time.h>
#include <string.h>
#define rows 18
#define cols 12
#define playerOne 1
#define playerTwo 41
using namespace std;
using namespace chrono;
namespace Tetris {
	enum { ESC = 27 };
	class Point
	{
	private:
		char figure = '*';
		int x;
		int y;
	public:
		Point(int _x = 0, int _y = 0, char _figure = '*') //constructor
		{
			x = _x;
			y = _y;
			figure = _figure;
		}
		~Point();
		//getters
		char getFigure()const;
		int getX()const;
		int getY()const;
		//setters
		bool setXCord(int _x);
		bool setYCord(int _y);
		bool setPoint(int _x, int _y);
		//adders
		bool addBothCord(int _xVal, int _yVal);
		bool addXCord(int val);
		bool addYCord(int val);
		//Function
		bool printPoint();
	};
	inline void ShowConsoleCursor(bool showFlag)
	{
		HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

		CONSOLE_CURSOR_INFO     cursorInfo;

		GetConsoleCursorInfo(out, &cursorInfo);
		cursorInfo.bVisible = showFlag; // set the cursor visibility
		SetConsoleCursorInfo(out, &cursorInfo);
	}
	inline bool gotoXY(int x, int y) {
		HANDLE hConsoleOutput;
		COORD dwCursorPosition;
		std::cout.flush();
		dwCursorPosition.X = x;
		dwCursorPosition.Y = y;
		hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleCursorPosition(hConsoleOutput, dwCursorPosition);
		return true;
	}
}
