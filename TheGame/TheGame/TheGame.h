#pragma once
#include <iostream>
#include "GameTable.h"
class TheGame
{
private:
	GameTable m_gameTable;
	bool m_lostGame = false;
public:
	TheGame();
	void SetLost();
	
	void StartTheGame();
	void ShowLastCardsFromStacks();
	void Rund();
	void FinalGame(bool status);
	bool Lost();
	bool GetLostGame();
};

