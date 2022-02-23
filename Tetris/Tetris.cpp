#include "computer.h"

namespace Tetris {

	/////////////////////////////////initial///////////////////////////////////////////////
	tetris::tetris(int posGap) {
		shape* shapesArr = new shape[7];
		initialShapes();
		initialBoard();
		boardPosGap = posGap;
	}
	tetris::tetris() {
		shape* shapesArr = new shape[7];
		initialShapes();
		initialBoard();
	}
	tetris::~tetris() {}
	bool tetris::setTempShape(shape& _shape) { tempShape = _shape; return true; }
	bool tetris::initialShapes() {
		shapesArr[0].setShape(5, 1, 6, 1, 7, 1, 8, 1, 'I'); // I ////
		shapesArr[1].setShape(6, 1, 5, 1, 5, 2, 4, 2, 'S'); // S ////
		shapesArr[2].setShape(6, 1, 7, 1, 7, 2, 8, 2, 'Z'); // Z ////
		shapesArr[3].setShape(7, 1, 7, 2, 6, 2, 5, 2, 'L'); // L ////
		shapesArr[4].setShape(6, 2, 7, 2, 7, 1, 8, 2, 'T'); // T ////
		shapesArr[5].setShape(6, 1, 6, 2, 7, 2, 8, 2, 'J'); // J ////
		shapesArr[6].setShape(6, 1, 6, 2, 7, 1, 7, 2, 'O'); // O ////
		bomb.setShape(cols / 2, 1, cols / 2, 1, cols / 2, 1, cols / 2, 1, 'B');
		return true;
	}
	bool tetris::initialBoard() {
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++) {
				board[i][j] = ' ';
			}
		return true;
	}
	bool tetris::newShapeOnBoard(shape& _shape) {
		bool flag = tetrisPieceHitBoard(_shape);
		printShape(_shape);
		return flag;
	}
	//////////////////////////////////moves///////////////////////////////////////
							      //shifting//
	bool tetris::defaultMove(shape& _shape) {//Default move is basicly shifting down
		shape newShape = _shape;
		newShape.addYvals(1, 1, 1, 1);
		if (tetrisPieceHitBoard(newShape) == true) {
			updateBoard(_shape);
			printBoard();
			return false;
		}
		else {
			printNewShapePos(_shape, newShape);
			_shape = newShape;
			printScore();
			return true;
		}
	}
	bool tetris::shiftLeft(shape& _shape) {//shifting the shape left if its a valid move
		shape newShape = _shape;
		newShape.addXvals(-1, -1, -1, -1);
		if (newPositionValid(newShape) == false) {
			return false;
		}
		else {
			printNewShapePos(_shape, newShape);
			_shape = newShape;
			return true;
		}
	}
	bool tetris::shiftRight(shape& _shape) {//shifting the shape right if its a valid move
		deleteBoardVals(_shape);
		shape newShape = _shape;
		newShape.addXvals(1, 1, 1, 1);
		if (newPositionValid(newShape) == false) {
			return false;
		}
		else {
			printNewShapePos(_shape, newShape);
			_shape = newShape;
			return true;
		}
	}
                        //shape rotation////
	bool tetris::rotateRight(shape& _shape,bool print) {//rotate the shape Clock wise if its a valid move
		shape oldShape = _shape;
		_shape.rotateRight();
		if (newPositionValid(_shape) == false || _shape.getFigure() == 'B' || _shape.getFigure() == 'O') {
			_shape = oldShape;
			return false;
		}
		else {
			if(print==true)
			printNewShapePos(oldShape, _shape);
			return true;
		}
	}
	bool tetris::rotateLeft(shape& _shape) {//rotate the shape counterClock wise if its a valid move
		shape oldShape = _shape;
		_shape.rotateLeft();
		if (newPositionValid(_shape) == false) {
			_shape = oldShape;
			return false;
		}
		else {
			printNewShapePos(oldShape, _shape);
			return true;
		}
	}
	
	////////////////////////////////////////////////////////////////////////////////////////
	bool tetris::updateBoard(shape& _shape) {
		for (int i = 0; i < 4; i++) {
			int x = _shape.getPoints()[i].getX() - 1;
			int y = _shape.getPoints()[i].getY() - 1;
			board[y][x] = '*';
		}
		return true;
	}
	bool tetris::tetrisPieceHitBoard(shape& _shape) {
		for (int i = 0; i < 4; i++) {
			int x = _shape.getPoints()[i].getX() - 1;
			int y = _shape.getPoints()[i].getY() - 1;
			if (x + 1 > cols || y + 1 > rows || x < 0 || y < 0)
				return true;
			if (board[y][x] == '*')
				return true;
		}
		return false;
	}
	bool tetris::setPosGap(int gap) {
		boardPosGap = gap;
		return true;
	}
	bool tetris::newPositionValid(shape& _shape) {
		int x, y;
		for (int i = 0; i < 4; i++) {
			x = _shape.getPoints()[i].getX();
			y = _shape.getPoints()[i].getY();
			if (insideBorders(x, y) == false)
				return false;
			if (board[y - 1][x - 1] == '*')
				return false;

		}
		return true;
	}
	bool tetris::deleteBoardVals(shape& _shape) {
		int x, y;
		for (int i = 0; i < 4; i++) {
			x = _shape.getPoints()[i].getX() - 1;
			y = _shape.getPoints()[i].getY() - 1;
			board[y][x] = ' ';
		}
		return true;
	}
	////////////////////////////////player key hit//////////////////////////////
	bool tetris::isMoveValid(shape& _shape, char direction) {
		if (direction == 'a' || direction == 'A' || direction == 'j' || direction == 'J')
			return shiftLeft(_shape);
		if (direction == 'd' || direction == 'D' || direction == 'l' || direction == 'L')
			return shiftRight(_shape);
		if (direction == 's' || direction == 'S' || direction == 'k' || direction == 'K')
			return rotateLeft(_shape);
		if (direction == 'w' || direction == 'W' || direction == 'i' || direction == 'I')
			return rotateRight(_shape,true);
		if (direction == 'x' || direction == 'X' || direction == 'm' || direction == 'M')
			if (defaultMove(_shape) == true) {
				score += 1;
				return true;
			}
		return false;
	}
	//shifting//
	int tetris::tetrisCount(shape& _shape) {
		int count = 0;
		int tetrisCount = 0;
		for (int i = 0; i < 4; i++) {
			int x = _shape.getPoints()[i].getX() -1;
			int y = _shape.getPoints()[i].getY()- 1;
			board[y][x] = '*';
		}
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				if (board[i][j] == '*')
					count++;
			}
			if (count == cols) {
				tetrisCount++;
				tetriscnt++;
			}
			count = 0;
		}
		for (int i = 0; i < 4; i++) {
			int x = _shape.getPoints()[i].getX() - 1;
			int y = _shape.getPoints()[i].getY() - 1;
			board[y][x] = ' ';
		}
		return tetrisCount;
	}
	int tetris::coverNum(shape& _shape) {
		int count = 0;
		for (int i = 0; i < 4; i++) {
			int x = _shape.getPoints()[i].getX() - 1;
			int y = _shape.getPoints()[i].getY();
			if (y == rows)
				count++;
			if (y < rows) {//will make sure we won't try to acess unallocated memory so the program wont crash
				if (board[y][x] == '*')
					count++;
			}
		}
		return count;
	}
	bool tetris::scoreRows(bool doTetris) {
		int count = 0;
		int rowsToClear[4];
		int tetrisCount = 0;
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				if (board[i][j] == '*')
					count++;
			}
			if (count == cols) {
				rowsToClear[tetrisCount] = i;
				tetrisCount++;
			}
			count = 0;
		}
		if (doTetris == false && tetrisCount > 0)
			return true;
		if (tetrisCount > 0 && doTetris == true) {
			flatRowsAfterTetris(rowsToClear,tetrisCount);
			scoreSystem(tetrisCount);
			scoreRows(doTetris);
			printBoard();
			return true;
		}
		return false;
	}
	bool tetris::flatRowsAfterTetris(int* rowsToClear, int tetrisCount) {
		int count = 0;
		printRows(rowsToClear,tetrisCount);
		for (int k = 0; k < tetrisCount; k++) {
 			for (int i = rowsToClear[k]; i > 0; i--) {
				for (int j = 0; j < 12; j++) {
					board[i][j] = board[i - 1][j];
				}
			}
		}
		return true;
	}
	bool tetris::printRows(int* rowsToClear, int tetrisCount) {//make the effect of completed lines blinking
		steady_clock::time_point start;
		start = steady_clock::now();
		const char a = 254;
		string row = { char(a), char(a), char(a), char(a), char(a), char(a), char(a), char(a), char(a), char(a), char(a), char(a) };
		do {
			for (int k = 0; k < tetrisCount; k++) {
				gotoXY(boardPosGap, rowsToClear[k]+1);
				cout << "           ";
				gotoXY(boardPosGap, rowsToClear[k]+1);
				cout << row;
			}
		}while(steady_clock::now() - start < milliseconds(300));
		return true;
	}
	bool tetris::clearRow(int _row) {
		for (int i = 0; i < cols; i++)
			board[_row][i] = ' ';
		return true;
	}
	bool tetris::scoreSystem(int tetrisCount)
	{
		if (tetrisCount == 1) {
			score += 40;
			return true;
		}
		if (tetrisCount == 2) {
			score += 100;
			return true;
		}
		if (tetrisCount == 3) {
			score += 300;
			return true;
		}
		if (tetrisCount == 4) {
			score += 1200;
			return true;
		}
		return false;
	}
	bool tetris::insideBorders(int _x, int _y) {
		if (_x < 1 || _x > cols || _y > rows || _y < 1)
			return false;
		return true;
	}


	
	//////////////////////////// GETTERS////////////////////////////////////
	
	shape tetris::getBomb()const {
		return bomb;
	}
	bool tetris::bombExplode(shape& _shape) {//this function clear the valid board positions with the radius of 5
		int x = _shape.getPoints()[0].getX();
		int y = _shape.getPoints()[0].getY();
		int currX = 0;
		int currY = 0;
		for (int i = 0; i < 5; i++) {
			currX = x + i;
			for (int row = 0; row < 5; row++) {
				currY = y + row;
				if (insideBorders(currX, currY) == true) {
					board[currY - 1][currX - 1] = ' ';
				}
				currY = y - row;
				if (insideBorders(currX, currY) == true) {
					board[currY - 1][currX - 1] = ' ';
				}
			}
		}
		for (int i = 1; i < 5; i++) {
			currX = x - i;
			for (int row = 0; row < 5; row++) {
				currY = y + row;
				if (insideBorders(currX, currY) == true) {
					board[currY - 1][currX - 1] = ' ';
				}
				currY = y - row;
				if (insideBorders(currX, currY) == true) {
					board[currY - 1][currX - 1] = ' ';
				}

			}
		}
		printBoard();
		return true;
	}
	int tetris::getScore()const { return score; }
	int tetris::getShapeCount()const {
		return shapeCount; }
	shape tetris::getTempShape()const { return tempShape; }
	shape tetris::getShape(int idx)const { return shapesArr[idx]; }
	///////////////////////prints
	bool tetris::printShape(shape& _shape) {
		for (int i = 0; i < 4; i++) {
			int x = _shape.getPoints()[i].getX() - 1;
			int y = _shape.getPoints()[i].getY();
			gotoXY(boardPosGap + x, y);
			cout << '*';
		}
		return true;
	}
	bool tetris::printNewShapePos(shape& oldShape, shape& newShapePos) {//update the shape positions in the game board
		for (int i = 0; i < 4; i++) {
			int x = boardPosGap+oldShape.getPoints()[i].getX()-1;
			int y = oldShape.getPoints()[i].getY();
			gotoXY(x, y);
			cout << ' ';
		}
		for (int i = 0; i < 4; i++) {
			int x = boardPosGap+newShapePos.getPoints()[i].getX() - 1;
			int y = newShapePos.getPoints()[i].getY();
			gotoXY(x, y);
			cout << '*';
		}
		return true;
	}
	bool tetris::printBorders() {
		for (int i = 0; i <= rows + 1; ++i) {
			gotoXY(boardPosGap-1, i);
			cout << char(254);
		}
		for (int i = 0; i <= rows + 1; ++i) {
			gotoXY(boardPosGap+cols, i);
			cout << char(254);
		}
		for (int j = 0; j < cols + 1; j++) {
			gotoXY(boardPosGap+j, 0);
			cout << char(254);
		}
		for (int j = 0; j < cols + 1; j++) {
			gotoXY(boardPosGap+j, rows + 1);
			cout << char(254);
		}
		printScore();
		return true;
	}
	bool tetris::printBoard() {
		clearBoard();
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++) {
				gotoXY(boardPosGap+j, i + 1);
				cout << board[i][j];
			}
		printScore();
		return true;
	}
	bool tetris::printScore() {
		gotoXY(boardPosGap + cols + 5, 3);
		cout << "Score:";
		gotoXY(boardPosGap + cols + 5, 4);
		cout << score;
		return true;
	}
	bool tetris::clearBoard() {
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++) {
				gotoXY(boardPosGap + j, i + 1);
				cout<<' ';
			}
		return true;
	}
	bool tetris::printTetrisBoard() {
		printBorders();
		printBoard();
		return true;
	}

}


