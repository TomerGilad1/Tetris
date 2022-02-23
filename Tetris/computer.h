#pragma once
#include "Tetris.h"
namespace Tetris {
	class computer :virtual public tetris {
		private:
		char level='a';
		int left = 0;
		int right = 0;
		int down = 0;
		bool chooseNextShape = true;
		shape holdShape;
		bool tempSwapped = false;
	public:
		computer(char _level = 'a'); 
		computer(char _level, int _BoardPosGap);
		~computer() {}
		virtual bool defaultMove(shape& _shape);
		bool pushShapeUp(shape& _shape);
		bool allRotationOptions(shape* arr);
		bool leftRightOpt(shape* arr, int* right, int* left);
		bool giveMeTheMoves(shape** arr, int* right, int* left);
		bool pushThemDown(shape** optionArr, int* right, int* left);
		shape giveMeBestMove(shape** optionArr, int* right, int* left, int& shapeId, int& idx);
		int blankSpotsBelow(shape _shape);
		bool evalutateShape(shape old_shape, shape new_shape);
		bool tillRotated(shape& arr, int rotateNum);
		bool liftUp(shape& shape);
		int coverNum(shape _shape, int* pntIdx, int size);
		//find the best bomb position
		shape bestBombSpot(shape** optionArr, int* right, int* left, int& shapeId, int& idx);
		//find the best O shape position
		shape bestOShape(shape** optionArr, int* right, int* left, int& shapeId, int& idx);
		//find the best Z shape position
		shape bestZShape(shape** optionArr, int* right, int* left, int& shapeId, int& idx);
		//find the best S shape position
		shape bestSShape(shape** optionArr, int* right, int* left, int& shapeId, int& idx);
		//find the best T shape position
		shape bestTShape(shape** optionArr, int* right, int* left, int& shapeId, int& idx);
		//find the best J shape position
		shape bestJShape(shape** optionArr, int* right, int* left, int& shapeId, int& idx);
		//find the best L shape position
		shape bestLShape(shape** optionArr, int* right, int* left, int& shapeId, int& idx);
		//find the best I shape position
		shape bestIShape(shape** optionArr, int* right, int* left, int& shapeId, int& idx);
		//get the lowest Y the shape go
		int shapeRowLevel(shape _shape);
		//place the shape on the board
		bool placeOnBoard(shape& _shape);
		//remove the shape from the board
		bool removeFromBoard(shape& _shape);
		//return the shape that fit the best 
		shape bestShapeAfterEvaluation(int* idxPriority, int* rowLevelByIdx, int* arrayBestLineDrop, shape** optionArr,int* bestShapeFromArray,int* sizes, int size);
	};

}
