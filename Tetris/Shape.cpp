#include "Shape.h"
namespace Tetris {


//constructorfs
	shape::shape() {
		figure = 'x';
		Point* arr = new Point[4];
		arr[0].setPoint(0, 0);
		arr[1].setPoint(0, 0);
		arr[2].setPoint(0, 0);
		arr[3].setPoint(0, 0);
	}
	shape::~shape() {}
	//getters
	char shape::getFigure()const { return figure; }
	Point* shape::getPoints() { return arr; }
	//setters
	bool shape::setFigure(char _figure) { figure = _figure; return true; }
	bool shape::setShape(int pnt1X, int pnt1Y, int pnt2X, int pnt2Y, int pnt3X, int pnt3Y, int pnt4X, int pnt4Y, char _figure) {
		arr[0].setPoint(pnt1X, pnt1Y);
		arr[1].setPoint(pnt2X, pnt2Y);
		arr[2].setPoint(pnt3X, pnt3Y);
		arr[3].setPoint(pnt4X, pnt4Y);
		figure = _figure;
		return true;
	}

	bool shape::addXvals(int x1, int x2, int x3, int x4) {
		arr[0].addXCord(x1);
		arr[1].addXCord(x2);
		arr[2].addXCord(x3);
		arr[3].addXCord(x4);
		return true;

	}
	bool shape::addYvals(int y1, int y2, int y3, int y4) {
		arr[0].addYCord(y1);
		arr[1].addYCord(y2);
		arr[2].addYCord(y3);
		arr[3].addYCord(y4);
		return true;
	}

	bool shape::rotateRight() {
		int x, y;
		Point p = arr[1];
		for (int i = 0; i < 4; i++) {
			x = arr[i].getY() - p.getY();
			y = arr[i].getX() - p.getX();
			arr[i].setXCord(p.getX() - x);
			arr[i].setYCord(p.getY() + y);
		}
		return true;
	}
	bool shape::rotateLeft() {
		int x, y;
		Point p = arr[1];
		for (int i = 0; i < 4; i++) {
			x = arr[i].getY() - p.getY();
			y = arr[i].getX() - p.getX();
			arr[i].setXCord(p.getX() + x);
			arr[i].setYCord(p.getY() - y);
		}
		return true;
	}

	//print
	bool shape::printShape() {
		for (int i = 0; i < 4; i++)
			arr[i].printPoint();
		return true;
	}


}

