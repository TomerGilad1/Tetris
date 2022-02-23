#include "Point.h"


namespace Tetris {




	Point::~Point() {}

	//getters

	int Point::getX()const { return x; }
	int Point::getY()const { return y; }
	char Point::getFigure()const { return figure; }
	//setters
	bool Point::setXCord(int _x) { x = _x; return true; }
	bool Point::setYCord(int _y) { y = _y; return true; }
	bool Point::setPoint(int _x, int _y) {
		setXCord(_x);
		setYCord(_y);
		return true;
	}

	//adders
	bool Point::addXCord(int val){
		x += val;
		return true;

	}
	bool Point::addYCord(int val) {
		y += val;
		return true;

	}
	bool Point::addBothCord(int _xVal, int _yVal) {
		addYCord(_yVal);
		addXCord(_xVal);
		return true;

	}

	//print
	bool Point::printPoint() {
		gotoXY(x, y);
		std::cout << figure;
		return true;
	}
}