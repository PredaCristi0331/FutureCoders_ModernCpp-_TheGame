#pragma once
#include <iostream>
#include "GameTable.h"
class TheGame
{
private:
	GameTable m_gameTable;
public:
	TheGame();
	void StartTheGame();
	void ShowLastCardsFromStacks();
	void Rund();
	void FinalGame(bool status);
	bool Lost();
};

