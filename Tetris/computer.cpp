#include "computer.h"

namespace Tetris {

	computer::computer(char _level) {
		level = _level;
	}
	computer::computer(char _level,int _BoardPosGap) {
		level = _level;
		boardPosGap = _BoardPosGap;
	}
	bool computer::defaultMove(shape& _shape) {
		if (level == 'b') {
			if ((_shape.getFigure() == 'B' && chooseNextShape != false) || (shapeCount % 40 == 0 && shapeCount > 0)) {//randomly choose spot every 40 shapes
				this->right = rand() % (cols/2);
				this->left = rand() % (cols/2);
				chooseNextShape = false;
				shapeCount++;
			}
		}
		if (level == 'c') {
			if ((_shape.getFigure() == 'B' && chooseNextShape != false) || (shapeCount % 30 == 0 && shapeCount > 0)) {//randomly choose spot every 30 shapes
				holdShape = _shape;
				this->right = rand() % (cols/2);
				this->left = rand() % (cols/2);
				chooseNextShape = false;
				shapeCount++;
			}
		}
		if (chooseNextShape == true) {
			shapeCount++;
			bool done = true;
			int shapeId = 0, idx = 0;
			int right[4] = { 0,0,0,0 };
			int left[4] = { 0,0,0,0 };
			shape result;
			shape arr[4] = { _shape, _shape, _shape, _shape };
			shape** optionArr = new shape * [4];
			allRotationOptions(arr);//set all the rotation cords
			leftRightOpt(arr, right, left);
			for (int i = 0; i < 4; i++) {//memory allocation for the possible moves
				optionArr[i] = new shape[(right[i] + left[i]) + 1];
				optionArr[i][left[i]] = arr[i];
			}
			giveMeTheMoves(optionArr, right, left);//set the array of possible moves by the order which the default position is the middle in the array[left]
			pushThemDown(optionArr, right, left);//save the last possible position on board
		    if (_shape.getFigure() == 'O') {
				int x = 5;
				printBoard();
			}
			holdShape = giveMeBestMove(optionArr, right, left, shapeId, idx);
			evalutateShape(_shape, holdShape);
			pushShapeUp(holdShape);
			for (int i = 0; i < 4; i++) //memory allocation for the possible moves
				delete[] optionArr[i];
			chooseNextShape = false;
			this->tempSwapped = false;
		}
		if (this->right == 0 && this->left == 0) {//make a move down
			if (tempSwapped == false) {
				printNewShapePos(_shape, holdShape);
				_shape = holdShape;
				this->tempSwapped = true;
			}
			chooseNextShape = !(tetris::defaultMove(_shape));
			if (chooseNextShape == false) {
				score += 1;
				printScore();
			}
			if (chooseNextShape == true)
				return false;
			return true;
		}
		else if (this->right > 0) {//make a shift right move
			this->right -= 1;
			shiftRight(_shape);
			if (this->right == 0) {
				printNewShapePos(_shape, holdShape);
				_shape = holdShape;
			}
			return true;
		}
		else if (this->left > 0) {//make a shift left move
			this->left -= 1;
			shiftLeft(_shape);
			if (this->left == 0) {
				printNewShapePos(_shape, holdShape);
				_shape = holdShape;
			}
			return true;
		}
		return true;
	}
	bool computer::pushShapeUp(shape& _shape) {
		bool noteDone = true;
		while (noteDone == true) {
			_shape.addYvals(-1, -1, -1, -1);
			noteDone =newPositionValid(_shape);
		}
		_shape.addYvals(1, 1, 1, 1);
		return true;
	}
	bool computer::evalutateShape(shape old_shape, shape new_shape) {
		int moves = (new_shape.getPoints()[1].getX() - old_shape.getPoints()[1].getX());
		this->right = 0;
		this->left = 0;
		if (moves > 0)
			this->right = moves;
		if (moves < 0)
			this->left -= moves;
		return true;
	}
	shape computer::giveMeBestMove(shape** optionArr, int* right, int* left, int& shapeId, int& idx) {
		printBoard();
		bool saveNew = false;
		int shapeIdx[4] = { 0,0,0,0 };
		int maxTet[4] = { 0, 0, 0, 0 };
		int arrayBestLineDrop[4] = { 0,0,0,0 };
		int bestShapeFromArray[4] = { 0,0,0,0 };
		int blankSpots[4] = { 0,0,0,0 };
		int current_blank = 0, best_blank = 0;
		int rowLevel = 0, bestRowLevel = 0;
		int shapId = 0;
		int currMax = 0;
		int best;
		int current;
		shape bestShape = optionArr[0][0];
		char c = optionArr[0][0].getFigure();
		int t = 4;
		if (c != 'B') {
			if (c == 'O')
				t = 1;
			for (int i = 0; i < t; i++) {
				int size = left[i] + right[i] + 1;
				best = current = tetrisCount(optionArr[i][0]);
				for (int j = 0; j < size; j++) {
					current = tetrisCount(optionArr[i][j]);
					if (current > best) {
						best = current;
						shapeIdx[i] = j;
						maxTet[i] = current;
					}
				}
			}
			for (int i = 0; i < 4; i++) {
				if (maxTet[i] > currMax) {
					currMax = maxTet[i];
					shapId = i;
				}
			}
			if (currMax > 0) {
				shapeId = shapId;
				idx = shapeIdx[shapId];
				return optionArr[shapId][shapeIdx[shapId]];
			}

		}
		if (optionArr[0][0].getFigure() == 'B')
			return bestBombSpot(optionArr, right, left, shapeId, idx);
		if (optionArr[0][0].getFigure() == 'Z')
			return bestZShape(optionArr, right, left, shapeId, idx);
		if (optionArr[0][0].getFigure() == 'S')
			return bestSShape(optionArr, right, left, shapeId, idx);
		if (optionArr[0][0].getFigure() == 'T')
			return bestTShape(optionArr, right, left, shapeId, idx);
		if (optionArr[0][0].getFigure() == 'J')
			return bestJShape(optionArr, right, left, shapeId, idx);
		if (optionArr[0][0].getFigure() == 'L')
			return bestLShape(optionArr, right, left, shapeId, idx);
		if (optionArr[0][0].getFigure() == 'I')
			return bestIShape(optionArr, right, left, shapeId, idx);
		return bestOShape(optionArr, right, left, shapeId, idx);
	}
	int computer::coverNum(shape _shape, int* pntIdx, int size) {
		int count = 0;
		for (int i = 0; i < size; i++) {
			int x = _shape.getPoints()[pntIdx[i]].getX() - 1;
			int y = _shape.getPoints()[pntIdx[i]].getY();
			if (y == rows)
				count++;
			if (y < rows) {//will make sure we won't try to acess unallocated memory so the program wont crash
				if (board[y][x] == '*')
					count++;
			}
		}
		return count;
	}
	shape computer::bestBombSpot(shape** optionArr, int* right, int* left, int& shapeId, int& idx) {
		int spots[cols] = { 0,0,0,0,0,0,0,0,0,0,0,0};
		int max = 19;
		int index = 0;
		for (int i = 0; i < cols; i++) {
			spots[i] = shapeRowLevel(optionArr[0][i]);
			if (spots[i] < max) {
				max = spots[i];
				index = i;
			}
		}
		return optionArr[0][index];
	} 
	shape computer::bestZShape(shape** optionArr, int* right, int* left, int& shapeId, int& idx) {
		int shapesIdx[4][4] = { {0,2,3},{1,3},{0,2,3},{1,3} };//the shape points that neeed to be checked below
		int sizes[4] = { 3,2 };//the number of positions to test
		bool saveNew = false;
		int shapeIdx[4] = { 0,0,0,0 };
		int maxTet[4] = { 0,0,0,0 };
		int arrayBestLineDrop[4] = { 0,0,0,0 };
		int bestShapeFromArray[4] = { 0,0,0,0 };
		int blankSpots[4] = { 0,0,0,0 };
		int rowLevelByIdx[4] = { 0, 0,0,0 };
		int current_blank = 0, best_blank = 0;
		int rowLevel = 0, bestRowLevel = 0;
		int shapId = 0;
		int currMax = 0;
		int best;
		int current;
		for (int i = 0; i < 4; i++) {
			int size = left[i] + right[i] + 1;
			int* arr = new int[size];
			rowLevel = 0;
			bestRowLevel = 0;
			best = 0;
			current = 0;
			best_blank = 0;
			for (int j = 0; j < size; j++) {
				current = coverNum(optionArr[i][j], shapesIdx[i], sizes[i]);
				current_blank = blankSpotsBelow(optionArr[i][j]);
				rowLevel = shapeRowLevel(optionArr[i][j]);
				if (current > best)//if the new location cover more we save it
					saveNew = true;
				else if (current == best && current_blank < best_blank)//if the new location has less blank spots below we will save it
					saveNew = true;
				else if (current == best && current_blank == best_blank && rowLevel > bestRowLevel)////if the new location has better  Y level(bigger Y) we will save it
					saveNew = true;
				if (saveNew == true) {//if we found a better location for the current shape we save it
					rowLevelByIdx[i] = rowLevel;
					bestRowLevel = rowLevel;
					best = current;
					best_blank = current_blank;
					blankSpots[i] = best_blank;
					bestShapeFromArray[i] = j;
					arrayBestLineDrop[i] = best;
				}
				saveNew = false;
			}
		}

		int idxPriority[4] = { 0,1,0,1 };//the best shape value from the left to the right
		return bestShapeAfterEvaluation(idxPriority, rowLevelByIdx, arrayBestLineDrop, optionArr, bestShapeFromArray, sizes, 4);//this will return the best shape
	}
	shape computer::bestSShape(shape** optionArr, int* right, int* left, int& shapeId, int& idx) {
		int shapesIdx[4][4] = { {0,2,3},{0,2},{0,2,3},{0,2} };
		int sizes[2] = { 3,2 };
		bool saveNew = false;
		int shapeIdx[2] = { 0,0 };
		int maxTet[2] = { 0,0 };
		int arrayBestLineDrop[2] = { 0,0 };
		int bestShapeFromArray[2] = { 0,0 };
		int blankSpots[2] = { 0,0 };
		int rowLevelByIdx[2] = { 0,0 };
		int current_blank = 0, best_blank = 0;
		int rowLevel = 0, bestRowLevel = 0;
		int shapId = 0;
		int currMax = 0;
		int best;
		int current;
		for (int i = 0; i < 2; i++) {
			int size = left[i] + right[i] + 1;
			int* arr = new int[size];
			rowLevel = 0;
			bestRowLevel = 0;
			best = 0;
			current = 0;
			best_blank = 0;
			for (int j = 0; j < size; j++) {
				current = coverNum(optionArr[i][j], shapesIdx[i], sizes[i]);
				current_blank = blankSpotsBelow(optionArr[i][j]);
				rowLevel = shapeRowLevel(optionArr[i][j]);
				if (current > best)//if the new location cover more we save it
					saveNew = true;
				else if (current == best && current_blank < best_blank)//if the new location has less blank spots below we will save it
					saveNew = true;
				else if (current == best && current_blank == best_blank && rowLevel > bestRowLevel)////if the new location has better  Y level(bigger Y) we will save it
					saveNew = true;
				if (saveNew == true) {//if we found a better location for the current shape we save it
					rowLevelByIdx[i] = rowLevel;
					bestRowLevel = rowLevel;
					best = current;
					best_blank = current_blank;
					blankSpots[i] = best_blank;
					bestShapeFromArray[i] = j;
					arrayBestLineDrop[i] = best;
				}
				saveNew = false;
			}
		}

		int idxPriority[4] = { 0,1,0,1 };//the best shape value from the left to the right
		return bestShapeAfterEvaluation(idxPriority, rowLevelByIdx, arrayBestLineDrop, optionArr, bestShapeFromArray, sizes, 4);//this will return the best shape
	}
	shape computer::bestOShape(shape** optionArr, int* right, int* left, int& shapeId, int& idx) {
		int shapesIdx[4][4] = { {1,3}};
		int sizes[4] = { 2 };
		bool saveNew = false;
		int shapeIdx[4] = { 0,0,0,0 };
		int maxTet[4] = { 0,0,0,0 };
		int arrayBestLineDrop[4] = { 0,0,0,0 };
		int bestShapeFromArray[4] = { 0,0,0,0 };
		int blankSpots[4] = { 0,0,0,0 };
		int current_blank = 0, best_blank = 0;
		int rowLevel = 0, bestRowLevel = 0;
		int shapId = 0;
		int currMax = 0;
		int best;
		int current;
		printBoard();
		for (int i = 0; i < 1; i++) {
			int size = left[i] + right[i] + 1;
			int* arr = new int[size];
			best = 0;
			current = 0;
			for (int j = 0; j < size; j++) {
				current = coverNum(optionArr[i][j], shapesIdx[i], sizes[i]);
				current_blank = blankSpotsBelow(optionArr[i][j]);
				rowLevel = shapeRowLevel(optionArr[i][j]);
				if (current > best)//if the new location cover more we save it
					saveNew = true;
				else if (current == best && current_blank < best_blank)//if the new location has less blank spots below we will save it
					saveNew = true;
				else if (current == best && current_blank == best_blank && bestRowLevel < rowLevel)//if the new location has better  Y level(bigger Y) we will save it
					saveNew = true;
				if (saveNew == true) {//if we found a better location for the current shape we save it
					bestRowLevel = rowLevel;
					best = current;
					best_blank = current_blank;
					blankSpots[i] = best_blank;
					bestShapeFromArray[i] = j;
					arrayBestLineDrop[i] = best;
				}
				saveNew = false;
			}
		}
		return optionArr[0][bestShapeFromArray[0]];//since its O there is no rotation so we check only the default shape
	}
	shape computer::bestTShape(shape** optionArr, int* right, int* left, int& shapeId, int& idx) {
		int shapesIdx[4][4] = { {0,1,3},{2,3},{3,2,0},{2,0 } };
		int sizes[4] = { 3,2,3,2 };
		bool saveNew = false;
		int rowLevelByIdx[4] = { 0,0,0,0 };
		int shapeIdx[4] = { 0,0,0,0 };
		int maxTet[4] = { 0,0,0,0 };
		int arrayBestLineDrop[4] = { 0,0,0,0 };
		int bestShapeFromArray[4] = { 0,0,0,0 };
		int blankSpots[4] = { 0,0,0,0 };
		int current_blank = 0, best_blank = 0;
		int rowLevel = 0, bestRowLevel = 0;
		int shapId = 0;
		int currMax = 0;
		int best;
		int current;
		for (int i = 0; i < 4; i++) {
			int size = left[i] + right[i] + 1;
			int* arr = new int[size];
			rowLevel = 0;
			bestRowLevel = 0;
			best = 0;
			current = 0;
			best_blank = 0;
			for (int j = 0; j < size; j++) {
				current = coverNum(optionArr[i][j], shapesIdx[i], sizes[i]);
				current_blank = blankSpotsBelow(optionArr[i][j]);
				rowLevel = shapeRowLevel(optionArr[i][j]);
				if (current > best)
					saveNew = true;
				else if (current == best && current_blank < best_blank)
					saveNew = true;
				else if (current == best && current_blank == best_blank && rowLevel > bestRowLevel)
					saveNew = true;
				else if (current == best && current_blank == best_blank && rowLevel > bestRowLevel && i == 1)
					if (insideBorders(optionArr[i][j].getPoints()[2].getX() + 1, optionArr[i][j].getPoints()->getY()))
						if (board[optionArr[i][j].getPoints()[2].getX(), optionArr[i][j].getPoints()->getY() - 1] == "*")
							saveNew = true;
				else if (current == best && current_blank == best_blank && rowLevel > bestRowLevel && i == 3)
					if (insideBorders(optionArr[i][j].getPoints()[2].getX() - 1, optionArr[i][j].getPoints()->getY()))
						if (board[optionArr[i][j].getPoints()[2].getX() - 2, optionArr[i][j].getPoints()->getY() - 1] == "*")
							saveNew = true;
				if (saveNew == true) {
					rowLevelByIdx[i] = rowLevel;
					bestRowLevel = rowLevel;
					best = current;
					best_blank = current_blank;
					blankSpots[i] = best_blank;
					bestShapeFromArray[i] = j;
					arrayBestLineDrop[i] = best;
				}
				saveNew = false;
			}
		}
		int idxPriority[4] = { 2,1,3,0 };
		return bestShapeAfterEvaluation(idxPriority, rowLevelByIdx, arrayBestLineDrop, optionArr, bestShapeFromArray, sizes, 4);
	}
	shape computer::bestJShape(shape** optionArr, int* right, int* left, int& shapeId, int& idx) {
		int shapesIdx[4][4] = { {1,2,3},{3,0},{0,2,3},{0,1} };
		int sizes[4] = { 3,2,3,2 };
		bool saveNew = false;
		int shapeIdx[4] = { 0,0,0,0 };
		int maxTet[4] = { 0,0,0,0 };
		int arrayBestLineDrop[4] = { 0,0,0,0 };
		int bestShapeFromArray[4] = { 0,0,0,0 };
		int blankSpots[4] = { 0,0,0,0 };
		int current_blank = 0, best_blank = 0;
		int rowLevel = 0, bestRowLevel = 0;
		int rowLevelByIdx[4] = { 0, 0, 0, 0 };
		int shapId = 0;
		int currMax = 0;
		int best;
		int current;
		for (int i = 0; i < 4; i++) {
			int size = left[i] + right[i] + 1;
			int* arr = new int[size];
			rowLevel = 0;
			bestRowLevel = 0;
			best = 0;
			current = 0;
			best_blank = 0;
			for (int j = 0; j < size; j++) {
				current = coverNum(optionArr[i][j], shapesIdx[i], sizes[i]);
				current_blank = blankSpotsBelow(optionArr[i][j]);
				rowLevel = shapeRowLevel(optionArr[i][j]);
				if (current > best)
					saveNew = true;
				else if (current == best && current_blank < best_blank)
					saveNew = true;
				else if (current == best && current_blank == best_blank && rowLevel > bestRowLevel)
					saveNew = true;
				if (saveNew == true) {
					rowLevelByIdx[i] = rowLevel;
					bestRowLevel = rowLevel;
					best = current;
					best_blank = current_blank;
					blankSpots[i] = best_blank;
					bestShapeFromArray[i] = j;
					arrayBestLineDrop[i] = best;
				}
				saveNew = false;
			}
		}
		int idxPriority[4] = { 1,2,0,3 };
		return bestShapeAfterEvaluation(idxPriority, rowLevelByIdx, arrayBestLineDrop, optionArr, bestShapeFromArray, sizes, 4);
	}
	shape computer::bestLShape(shape** optionArr, int* right, int* left, int& shapeId, int& idx) {
		int shapesIdx[4][4] = { {1,2,3},{0,1},{0,2,3},{0,3} };
		int sizes[4] = { 3,2,3,2 };
		bool alternative = false;
		bool saveNew = false;
		int shapeIdx[4] = { 0,0,0,0 };
		int maxTet[4] = { 0,0,0,0 };
		int arrayBestLineDrop[4] = { 0,0,0,0 };
		int bestShapeFromArray[4] = { 0,0,0,0 };
		int blankSpots[4] = { 0,0,0,0 };
		int rowLevelByIdx[4] = { 0,0,0,0 };
		int current_blank = 0, best_blank = 0;
		int rowLevel = 0, bestRowLevel = 0;
		int shapId = 0;
		int currMax = 0;
		int best;
		int current;
		for (int i = 0; i < 4; i++) {//check
			int size = left[i] + right[i] + 1;
			int* arr = new int[size];
			rowLevel = 0;
			bestRowLevel = 0;
			best = 0;
			current = 0;
			best_blank = 0;
			for (int j = 0; j < size; j++) {
				current = coverNum(optionArr[i][j], shapesIdx[i], sizes[i]);
				current_blank = blankSpotsBelow(optionArr[i][j]);
				rowLevel = shapeRowLevel(optionArr[i][j]);
				if (current > best)
					saveNew = true;
				else if (current == best && current_blank < best_blank)
					saveNew = true;
				else if (current == best && current_blank == best_blank && rowLevel > bestRowLevel)
					saveNew = true;
				if (saveNew == true) {
					rowLevelByIdx[i] = rowLevel;
					bestRowLevel = rowLevel;
					best = current;
					best_blank = current_blank;
					blankSpots[i] = best_blank;
					bestShapeFromArray[i] = j;
					arrayBestLineDrop[i] = best;
				}
				saveNew = false;
			}
		}
		int idxPriority[4] = { 3,2,0,1 };
		return bestShapeAfterEvaluation(idxPriority, rowLevelByIdx, arrayBestLineDrop,optionArr, bestShapeFromArray, sizes, 4);
	}
	//this is the algorithem that find the current best shape, its follow the priority of the shapes we set at "idxPriority" first it try to find if there is a perfect fit
	// if there isnt he check if there is a shape with -1 covers and if it didnt find again he check -2 covers
	shape computer::bestShapeAfterEvaluation(int* idxPriority, int* rowLevelByIdx, int* arrayBestLineDrop, shape** optionArr, int* bestShapeFromArray, int* sizes, int size) {
		for (int i = 0; i < 2; i++) {
			if (arrayBestLineDrop[idxPriority[0]] == sizes[idxPriority[0]] - i) {
				if (arrayBestLineDrop[idxPriority[1]] == sizes[idxPriority[1]] - i && rowLevelByIdx[idxPriority[0]] < rowLevelByIdx[idxPriority[1]]) {
					if (arrayBestLineDrop[idxPriority[2]] == sizes[idxPriority[2]] - i && rowLevelByIdx[idxPriority[1]] < rowLevelByIdx[idxPriority[2]]) {
						if (arrayBestLineDrop[idxPriority[3]] == sizes[idxPriority[3]] - i && rowLevelByIdx[idxPriority[2]] < rowLevelByIdx[idxPriority[3]])
							return optionArr[idxPriority[3]][bestShapeFromArray[idxPriority[3]]];
						return optionArr[idxPriority[2]][bestShapeFromArray[idxPriority[2]]];
					}
					return optionArr[idxPriority[1]][bestShapeFromArray[idxPriority[1]]];
				}
			}
			if (arrayBestLineDrop[idxPriority[0]] == sizes[idxPriority[0]] - i) {
				if (arrayBestLineDrop[idxPriority[2]] == sizes[idxPriority[2]] - i && rowLevelByIdx[idxPriority[0]] < rowLevelByIdx[idxPriority[2]]) {
					if (arrayBestLineDrop[idxPriority[3]] == sizes[idxPriority[3]] - i && rowLevelByIdx[idxPriority[2]] < rowLevelByIdx[idxPriority[3]]) {
						return optionArr[idxPriority[3]][bestShapeFromArray[idxPriority[3]]];
					}
					return optionArr[idxPriority[2]][bestShapeFromArray[idxPriority[2]]];
				}
			}
			if (arrayBestLineDrop[idxPriority[0]] == sizes[idxPriority[0]] - i) {
				if (arrayBestLineDrop[idxPriority[3]] == sizes[idxPriority[3]] - i && rowLevelByIdx[idxPriority[0]] < rowLevelByIdx[idxPriority[3]]) {
					return optionArr[idxPriority[3]][bestShapeFromArray[idxPriority[3]]];
				}
			}
			if (arrayBestLineDrop[idxPriority[1]] == sizes[idxPriority[1]] - i) {
				if (arrayBestLineDrop[idxPriority[2]] == sizes[idxPriority[2]] - i && rowLevelByIdx[idxPriority[1]] < rowLevelByIdx[idxPriority[2]]) {
					if (arrayBestLineDrop[idxPriority[3]] == sizes[idxPriority[3]] - i && rowLevelByIdx[idxPriority[2]] < rowLevelByIdx[idxPriority[3]])
						return optionArr[idxPriority[3]][bestShapeFromArray[idxPriority[3]]];
					return optionArr[idxPriority[2]][bestShapeFromArray[idxPriority[2]]];
				}
			}

			if (arrayBestLineDrop[idxPriority[1]] == sizes[idxPriority[1]] - i) {
				if (arrayBestLineDrop[idxPriority[3]] == sizes[idxPriority[3]] - i && rowLevelByIdx[idxPriority[2]] < rowLevelByIdx[idxPriority[3]]) {
					return optionArr[idxPriority[3]][bestShapeFromArray[idxPriority[3]]];
				}
			}

			if (arrayBestLineDrop[idxPriority[2]] == sizes[idxPriority[2]] - i) {
				if (arrayBestLineDrop[idxPriority[3]] == sizes[idxPriority[3]] - i && rowLevelByIdx[idxPriority[2]] < rowLevelByIdx[idxPriority[3]]) {
					return optionArr[idxPriority[3]][bestShapeFromArray[idxPriority[3]]];
				}
			}

			if (arrayBestLineDrop[idxPriority[0]] == sizes[idxPriority[0]] - i)
				return optionArr[idxPriority[0]][bestShapeFromArray[idxPriority[0]]];
			if (arrayBestLineDrop[idxPriority[1]] == sizes[idxPriority[1]] - i)
				return optionArr[idxPriority[1]][bestShapeFromArray[idxPriority[1]]];
			if (arrayBestLineDrop[idxPriority[2]] == sizes[idxPriority[2]] - i)
				return optionArr[idxPriority[2]][bestShapeFromArray[idxPriority[2]]];
			if (arrayBestLineDrop[idxPriority[3]] == sizes[idxPriority[3]] - i)
				return optionArr[idxPriority[3]][bestShapeFromArray[idxPriority[3]]];
		}
			return optionArr[idxPriority[0]][bestShapeFromArray[idxPriority[0]]];
	}
	shape computer::bestIShape(shape** optionArr, int* right, int* left, int& shapeId, int& idx) {
		bool saveNew = false;
		int shapeIdx[2] = { 0,0 };
		int maxTet[2] = { 0, 0 };
		int arrayBestLineDrop[2] = { 0,0 };
		int bestShapeFromArray[2] = { 0,0 };
		int blankSpots[2] = { 0,0 };
		int rowLevelByIdx[2] = { 0,0 };
		int current_blank = 0, best_blank = 0;
		int rowLevel = 0, bestRowLevel = 0;
		int shapId = 0;
		int currMax = 0;
		int best;
		int current;
		for (int i = 0; i < 2; i++) {
			int size = left[i] + right[i] + 1;
			int* arr = new int[size];
			best = 0;
			current = 0;
			best_blank = blankSpotsBelow(optionArr[i][0]);
			bestRowLevel = shapeRowLevel(optionArr[i][0]);
			bestShapeFromArray[i] = 0;
			for (int j = 0; j < size; j++) {
				rowLevel = shapeRowLevel(optionArr[i][j]);
				if (current > best)
					saveNew = true;
				else if (current == best && current_blank < best_blank)
					saveNew = true;
				else if (current == best && current_blank == best_blank && rowLevel > bestRowLevel)
					saveNew = true;
				if (saveNew == true) {
					rowLevelByIdx[i] = rowLevel;
					bestRowLevel = rowLevel;
					best = current;
					best_blank = current_blank;
					blankSpots[i] = best_blank;
					bestShapeFromArray[i] = j;
					arrayBestLineDrop[i] = best;
				}
				saveNew = false;
			}
		}
		if (arrayBestLineDrop[0] == 4 && rowLevelByIdx[0] > rowLevelByIdx[1])
			return optionArr[0][bestShapeFromArray[0]];
		else
			return optionArr[1][bestShapeFromArray[1]];
	}
	int computer::shapeRowLevel(shape _shape) {
		int best = 0;
		int current = 0;
		for (int i = 0; i < 4; i++) {
			current = _shape.getPoints()[i].getY();
			if (current > best)
				best = current;
		}
		return best;
	}
	int computer::blankSpotsBelow(shape _shape) {
		int count = 0;
		placeOnBoard(_shape);
		for (int i = 0; i < 4; i++) {
			int x = _shape.getPoints()[i].getX() - 1;
			int y = _shape.getPoints()[i].getY();
			if (y < rows) {//will make sure we won't try to acess unallocated memory so the program wont crash
				if (board[y][x] == ' ')
					count++;
			}
		}
		removeFromBoard(_shape);
		return count;

	}
	bool computer::placeOnBoard(shape& _shape) {
		for (int i = 0; i < 4; i++) {
			int x = _shape.getPoints()[i].getX() - 1;
			int y = _shape.getPoints()[i].getY() - 1;
			board[y][x] = '*';
		}
		return true;
	}
	bool computer::removeFromBoard(shape& _shape) {
		for (int i = 0; i < 4; i++) {
			int x = _shape.getPoints()[i].getX() - 1;
			int y = _shape.getPoints()[i].getY() - 1;
			board[y][x] = ' ';
		}
		return true;
	}
	//push the array of shapes till they hit something on board, it will be used for the evaluation of the shapes
	bool computer::pushThemDown(shape** optionArr, int* right, int* left) {
		int count = 0;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < left[i]; j++) {
				while (tetrisPieceHitBoard(optionArr[i][j]) == false)
					optionArr[i][j].addYvals(1, 1, 1, 1);
				optionArr[i][j].addYvals(-1, -1, -1, -1);
			}
			while (tetrisPieceHitBoard(optionArr[i][right[i]]) == false)
				optionArr[i][right[i]].addYvals(1, 1, 1, 1);
			optionArr[i][right[i]].addYvals(-1, -1, -1, -1);
			for (int j = left[i]; j < left[i] + right[i] + 1; j++) {
				while (tetrisPieceHitBoard(optionArr[i][j]) == false)
					optionArr[i][j].addYvals(1, 1, 1, 1);
				optionArr[i][j].addYvals(-1, -1, -1, -1);
			}
		}
		return true;
	}
	bool computer::giveMeTheMoves(shape** optionArr, int* right, int* left) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < left[i]; j++) {
				optionArr[i][j] = optionArr[i][left[i]]; // save the default middle place
				optionArr[i][j].addXvals(-(j + 1), -(j + 1), -(j + 1), -(j + 1));//save the new positions by array specific order
			}
			for (int j = 0; j < right[i]; j++) {
				optionArr[i][j + left[i] + 1] = optionArr[i][left[i]];//save the default middle place
				optionArr[i][j + left[i] + 1].addXvals(j + 1, j + 1, j + 1, j + 1);////save the new positions by array specific order
			}
		}
		return true;
	}
	bool computer::leftRightOpt(shape* arr, int* right, int* left) {
		bool done = true;
		shape tempShape;
		for (int i = 0; i < 4; i++) {//will tell us how much space we have to rotate left and right
			tempShape = arr[i];
			while (done != false)
			{
				arr[i].addXvals(1, 1, 1, 1);
				done = newPositionValid(arr[i]);
				if (done == true)
					right[i] += 1;

			}
			done = true;
			arr[i] = tempShape;
			while (done != false) {
				arr[i].addXvals(-1, -1, -1, -1);
				done = newPositionValid(arr[i]);
				if (done == true)
					left[i] += 1;
			}
			arr[i] = tempShape;
			done = true;
		}
		return true;
	}
	bool computer::allRotationOptions(shape* arr) {//create all shape variations and update the array to hold them
		for (int i = 1; i < 4; i++)
			tillRotated(arr[i], i);
		return true;
	}
	bool computer::tillRotated(shape& arr, int rotateNum) {
		for (int i = 0; i < rotateNum; i++) {//rotate the shape
			int count = 0;
			while (rotateRight(arr, false) == false && count != 4) {
				arr.addYvals(1, 1, 1, 1);
				count++;
			}
			liftUp(arr);//push the shape up till she get to the top of the board for the first move
		}
		return true;
	}
	bool computer::liftUp(shape& shape) {//push the shape up till she get to the top of the board for the first move
		int liftY = 0;
		for (int i = 0; i < 4; i++) {
			if (shape.getPoints()[i].getY() == 1)
				return false;
			if (liftY > shape.getPoints()[i].getY())
				liftY = shape.getPoints()[i].getY();
		}
		shape.addYvals(-liftY, -liftY, -liftY, -liftY);
		return true;
	}
}