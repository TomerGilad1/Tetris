#pragma once
#include "Shape.h"

namespace Tetris {
	class tetris {
	protected:
		char board[rows][cols];
		int boardPosGap = 0;
		int score = 0;
		int tetriscnt = 0;
		int shapeCount = 0;
		shape bomb;
	private:
		shape shapesArr[7];
		shape tempShape;


	public:
		/////////////////////////////////initial//////////////////////////////////////////////
		tetris();
		tetris(int posGap);
		virtual ~tetris();
		bool setTempShape(shape& _shape);
		bool initialShapes();
		bool initialBoard();
		bool newShapeOnBoard(shape& _shape);
		//////////////////////////////////moves///////////////////////////////////////
									  //shifting//
		virtual bool defaultMove(shape& _shape);
		bool shiftLeft(shape& _shape);
		bool shiftRight(shape& _shape);
		//shape rotation//
		bool rotateLeft(shape& _shape);
		bool rotateRight(shape& _shape, bool print);

		////////////////////////////////////////////////////////////////////////////////////////
		bool updateBoard(shape& _shape);
		bool tetrisPieceHitBoard(shape& _shape);
		bool setPosGap(int gap);
		bool newPositionValid(shape& _shape);
		bool deleteBoardVals(shape& _shape);

		////////////////////////////////Checks//////////////////////////////
		bool isMoveValid(shape& _shape, char direction);
		bool insideBorders(int _x, int _y);
		////////////////////////////////////////////////Tetris Scoring/////////////////////////
		int tetrisCount(shape& _shape);
		int coverNum(shape& _shape);


		bool scoreRows(bool doTetris);
		bool flatRowsAfterTetris(int* rowsToClear, int tetrisCount);
		bool clearRow(int _row);
		//valid Moves check
		bool scoreSystem(int tetrisCount);
		///////////////////////////////////////////////////////////////////////////
		int getScore()const;
		int getShapeCount()const;
		shape getBomb()const;
		bool bombExplode(shape& _shape);
		shape getTempShape()const;
		shape getShape(int idx)const;
		//prints
		bool printShape(shape& _shape);
		bool printNewShapePos(shape& oldShape, shape& newShapePos);
		bool printBorders();
		bool printTetrisBoard();
		bool printRows(int* rowsToClear, int tetrisCount);
		bool printScore();
		bool clearBoard();
		bool printBoard();
	};
}
