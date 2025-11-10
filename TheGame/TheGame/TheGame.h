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
	void RundWithDeckCards();
	void RundWithoutDeckCards();
};

