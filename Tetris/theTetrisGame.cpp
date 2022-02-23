#include "theTetrisGame.h"

namespace Tetris {
	TetrisGame::TetrisGame() {}
	TetrisGame::~TetrisGame() {
		if (player1 != nullptr)
			delete player1;
		if (player2 != nullptr)
			delete player2;
	}
	bool TetrisGame::playTheGame() {
		system("CLS");
		steady_clock::time_point start;
		char key = 0;
		int menu = 0;
		shape temp1, temp2;
		bool playerOneOver = false, playerTwoOver = false, shapeOneFinished = true, shapeTwoFinished = true, gameOver = false, endGame = false;
		player1->printTetrisBoard();//print the first player board
		player2->printTetrisBoard();//print the second player board
		do {
			if (gameCont == true) {
				temp1 = player1->getTempShape();
				temp2 = player2->getTempShape();
				player1->printShape(temp1);//print the shape last position before game paused
				player2->printShape(temp2);//print the shape last position before game paused
				shapeOneFinished = false;
				shapeTwoFinished = false;
				gameCont = false;
			}
			if (shapeOneFinished == true) {//generate the next shape
				if (rand() % 20 == 0)
					temp1 = player1->getBomb();
				else
					temp1 = player1->getShape((rand() * rand()) % 7);
				shapeOneFinished = false;
				playerOneOver = player1->newShapeOnBoard(temp1);
			}
			if (shapeTwoFinished == true) {//generate the next shape
				if (rand() % 20 == 0)
					temp2 = player2->getBomb();
				else
					temp2 = player2->getShape((rand() * rand()) % 7);
				shapeTwoFinished = false;
				playerTwoOver = player2->newShapeOnBoard(temp2);
			}
			start = steady_clock::now();
			while (steady_clock::now() - start < milliseconds(500) && key != ESC) {
				if (typeid(*player1) == typeid(computer))//the computer moves
					if (playerOneOver == false && shapeOneFinished == false)
						shapeOneFinished = !(player1->defaultMove(temp1));

				if (typeid(*player2) == typeid(computer))//the computer moves
					if (playerTwoOver == false && shapeTwoFinished != true)
						shapeTwoFinished = !(player2->defaultMove(temp2));

				if (_kbhit())//if a keyboard hit we assemble the move if its valid
				{
					key = _getch();
					if (validPlayerOneKey(key) == true && playerOneOver != true)
						player1->isMoveValid(temp1, key);

					if (validPlayerTwoKey(key) == true && playerTwoOver != true)
						player2->isMoveValid(temp2, key);
				}
				Sleep(50);
			}
			if (playerOneOver == false && key != ESC) {//make a default move 
				if (shapeOneFinished == false)
					shapeOneFinished = !(player1->defaultMove(temp1));//check if the shape hit the tetris matrix used blocks
				if (shapeOneFinished == true)
					if (temp1.getFigure() == 'B')//if the bomb hit something its explode
						player1->bombExplode(temp1);
					else
						player1->scoreRows(true);//check if there is a line/lines completed and if yes it scores them
			}
			if (playerTwoOver == false && key != ESC) {//make a default move 
				if (shapeTwoFinished == false)
					shapeTwoFinished = !(player2->defaultMove(temp2));//check if the shape hit the tetris matrix used blocks
				player1->scoreRows(true);
				if (shapeTwoFinished == true)
					if (temp2.getFigure() == 'B')//if the bomb hit something its explode
						player2->bombExplode(temp2);
					else
						player2->scoreRows(true);//check if there is a line/lines completed and if yes it scores them
			}
			if (playerOneOver == true || playerTwoOver == true)//set the game over flag on if one player lost
				gameOver = true;
			if (key == ESC) {//if some one pressed esc we save the last shape and position and pause the game
				player1->setTempShape(temp1);
				player2->setTempShape(temp2);
			}
		} while (key != ESC && gameOver != true);//exit the loop if the player lost or if the game paused
		if (gameOver == true) {//if one player lost we print the winner and return to the menu
			gotoXY(0, rows + 2);
			cout << "Game Over\n";
			if (playerOneOver == true)
				cout << "Player Two Win\n";
			else
				cout << "Player One Win\n";
			system("pause");
			gameCont = false;
			return false;

		}
		if (key == ESC) {//if some one pressed esc(pause) we stop the game and set the gameCont on
			gotoXY(0, rows + 2);
			cout << "Game Paused\n";
			system("pause");
			gameCont = true;
			return true;
		}
		return true;
	}

	bool TetrisGame::setGameCon(bool gameStatus) {
		gameCont = gameStatus;
		return true;
	}

	bool TetrisGame::getContGame()const {
		return gameCont;
	}
	bool TetrisGame::validPlayerOneKey(char key) {
		if (key == 'a' || key == 'A' || key == 'd' || key == 'D' || key == 's' || key == 'S' || key == 'w' || key == 'W' || key == 'x' || key == 'X')
			return true;
		return false;

	}
	bool TetrisGame::validPlayerTwoKey(char key) {
		if (key == 'j' || key == 'J' || key == 'l' || key == 'L' || key == 'k' || key == 'K' || key == 'i' || key == 'I' || key == 'm' || key == 'M')
			return true;
		return false;
	}


}