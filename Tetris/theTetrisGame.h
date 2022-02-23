#pragma once
#include "computer.h"

namespace Tetris {
	class TetrisGame {
		tetris* player1 = nullptr;
		tetris* player2 = nullptr;
		bool gameCont = false;

	public:
		TetrisGame();
		~TetrisGame();
		bool playTheGame();
		bool setGameCon(bool gameStatus);
		bool getContGame()const;
		bool validPlayerOneKey(char key);
		bool validPlayerTwoKey(char key);
		bool setPlayer1(tetris* _player1) { player1 = _player1; return true; }
		bool setPlayer1(computer* _player1) { player1 = _player1; return true; }
		bool setPlayer2(tetris* _player2) { player2 = _player2; return true; }
		bool setPlayer2(computer* _player2) { player2 = _player2; return true; }
		tetris* getPlayer1() { return player1; }
		tetris* getPlayer2() { return player2; }
	};















}
