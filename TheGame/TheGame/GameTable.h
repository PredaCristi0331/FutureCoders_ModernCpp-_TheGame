#pragma once
#include <iostream>
#include "Jucator.h"
#include "Card.h"
#include <vector>
#include <list>
class GameTable
{
private:
	int m_nrGamer;
	std::vector<Jucator>m_Gamers;
	std::vector<Card>m_deckCards;
	std::vector<Card>m_increasingFirst;
	std::vector<Card>m_increasingSecond;
	std::vector<Card>m_decreasingFirst;
	std::vector<Card>m_decreasingSecond;
public:
	GameTable(int nrGamer);
	void AddGamer();
	void AddInitialCards();
	void MixingDeckCards();
	void IssuerCard();
};

