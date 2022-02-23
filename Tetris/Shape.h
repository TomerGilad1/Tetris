#pragma once
#include "Point.h"
namespace Tetris {
	class shape {
	private:
		int left = 0;
		int right = 0;
		char figure;
		Point arr[4];
	public:
		//constructor
		shape();
		//destructor
		~shape();
		//getters
		char getFigure()const;
		Point* getPoints();
		//setters
		bool setFigure(char _figure);
		bool setShape(int pnt1X, int pnt1Y, int pnt2X, int pnt2Y, int pnt3X, int pnt3Y, int pnt4X, int pnt4Y, char _figure);
		//adders
		bool addXvals(int x1, int x2, int x3, int x4);
		bool addYvals(int y1, int y2, int y3, int y4);
		//rotations
		bool rotateRight();
		bool rotateLeft();
		//print
		bool printShape();

	};

}