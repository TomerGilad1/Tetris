#pragma once
#include "theTetrisGame.h"

enum Menu
{
NewHumanVsHumanGame=1,
NewHumanVsComputerGame,
NewComputerVsComputerGame,
ContinueGame,
PrintInstructions=8,
ExitProgram
};


namespace Tetris{
void theGameItself();
void printMenu(bool contGame);
bool printRules();
bool newHumanVsHumanGame(TetrisGame& _game);
bool newHumanVsComputerGame(TetrisGame& _game);
bool newComputerVsComputerGame(TetrisGame& _game);
bool contGame(TetrisGame& _game);



}
