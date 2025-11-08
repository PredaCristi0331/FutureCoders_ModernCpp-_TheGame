#include "TheGame.h"

TheGame::TheGame()
{
}

void TheGame::StartTheGame()
{
	std::cout << "Please enter the number of the gamers!\n";
	int nrGamer;
	std::cin >> nrGamer;
	GameTable gameTable(nrGamer);
	gameTable.AddGamer();
	gameTable.AddInitialCards();
	gameTable.MixingDeckCards();
	gameTable.IssuerCard();
}

void TheGame::Rund()
{

}
