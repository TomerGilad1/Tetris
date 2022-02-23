#include "menu.h"


namespace Tetris {
	void theGameItself() {
		TetrisGame _game;
		int menuOption = 0;
		while (menuOption != ExitProgram) {
			printMenu(_game.getContGame());
			cin >> menuOption;
			if (menuOption == NewHumanVsHumanGame) {
				_game.setGameCon(false);
				newHumanVsHumanGame(_game);
			}
			else if (menuOption == NewHumanVsComputerGame) {
				_game.setGameCon(false);
				newHumanVsComputerGame(_game);
			}
			else if (menuOption == NewComputerVsComputerGame) {
				_game.setGameCon(false);
				newComputerVsComputerGame(_game);
			}
			else if (menuOption == ContinueGame) {
				if (_game.getContGame() == true)
					contGame(_game);
			}
			else if (menuOption == PrintInstructions) {
				printRules();
				system("pause");
			}
		}
	}



	void printMenu(bool contGame) {
		system("CLS");
		cout << endl << "choose action: " << endl << "(1) Start a new game Human vs Human" << endl;
		cout << "(2) Start a new game Human vs Computer" << endl;
		cout << "(3) Start a new game Computer vs Computer" << endl;
		if (contGame == true)
			cout << "(4) Continue a paused game" << endl;
		cout << "(8) Present instructions and keys" << endl;
		cout << "(9) EXIT" << endl;
	}

	bool printRules() {
		system("CLS");
		cout << "The goal is to achieve the highest score" << endl;
		cout << "To pause the game press ESC" << endl;
		cout << "completing 1 line grant 40 points" << endl;
		cout << "completing 2 line grant 120 points" << endl;
		cout << "completing 3 line grant 300 points" << endl;
		cout << "completing 4 line grant 1200 points" << endl;
		cout << "each shift down grant 1 point" << endl << endl << endl;
		//left player
		cout << "Left Player Keys:" << endl;
		cout << "Shift left keys a or A" << endl;
		cout << "Shift right keys d or D" << endl;
		cout << "Rotate clockwise keys s or S" << endl;
		cout << "Rotate counter clock wise keys w or W" << endl;
		cout << "Shift drop down keys x or X" << endl << endl << endl;
		//right player
		cout << "Left Player Keys:" << endl;
		cout << "Shift left keys i or J" << endl;
		cout << "Shift right keys l or L" << endl;
		cout << "Rotate clockwise keys k or K" << endl;
		cout << "Rotate counter clock wise keys i or I" << endl;
		cout << "Shift drop down keys m or M" << endl;
		return true;
	}

	char validPlayerLevel() {
		char level = 'd';
		cout <<"Choose computer level a(best) b(good) c(novice)\n";
		while (level != 'a' && level != 'b' && level != 'c') {
			cin >> level;
			if (level == 'a' || level == 'b' || level == 'c')
				return level;
			system("CLS");
			cout << "Invalid computer level, Choose computer level a(best) b(good) c(novice)\n";
		}
		return true;
	}

	bool newHumanVsHumanGame(TetrisGame& _game) {
		if (_game.getPlayer1() != nullptr)
			delete _game.getPlayer1();
		if (_game.getPlayer2() != nullptr)
			delete _game.getPlayer2();
		_game.setPlayer1(new tetris(playerOne));
		_game.setPlayer2(new tetris(playerTwo));
		return _game.playTheGame();
	}
	bool newHumanVsComputerGame(TetrisGame& _game) {
		if (_game.getPlayer1() != nullptr)
			delete _game.getPlayer1();
		if (_game.getPlayer2() != nullptr)
			delete _game.getPlayer2();
		char computer1Level = validPlayerLevel();
		_game.setPlayer1(new tetris(playerOne));
		_game.setPlayer2(new computer(computer1Level, playerTwo));
		return _game.playTheGame();

	}
	bool newComputerVsComputerGame(TetrisGame& _game) {
		if (_game.getPlayer1() != nullptr)
			delete _game.getPlayer1();
		if (_game.getPlayer2() != nullptr)
			delete _game.getPlayer2();
		char computer1Level = validPlayerLevel();
		char computer2Level = validPlayerLevel();
		computer* player1 = new computer(computer1Level, playerOne);
		computer* player2 = new computer(computer2Level, playerTwo);
		_game.setPlayer1(player1);
		_game.setPlayer2(player2);
		return _game.playTheGame();
		return true;
	}
	bool contGame(TetrisGame& _game) {
		return _game.playTheGame();
	}
}
