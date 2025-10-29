#include "GameTable.h"
#include <list>
#include <algorithm>
#include <iterator>

GameTable::GameTable(int nrGamer):
	m_nrGamer(nrGamer)
{
}

void GameTable::AddGamer()
{
	if (m_nrGamer < 2 || m_nrGamer>5)
	{
		std::cout << "Not valid gamer number. Please select an other gamer number between 2 and 5.";
		return;
	}
	Jucator gamer;
	for (int i = 0; i < m_nrGamer; i++)
		m_Gamers.push_back(gamer);
}

void GameTable::AddInitialCards()
{
	Card cardInitialIncreasing(1);
	Card cardInitialDecreasing(100);
	this->m_increasingFirst.push_back(cardInitialIncreasing);
	this->m_increasingSecond.push_back(cardInitialIncreasing);
	this->m_decreasingFirst.push_back(cardInitialDecreasing);
	this->m_decreasingSecond.push_back(cardInitialDecreasing);
}

void GameTable::MixingDeckCards()
{
	for (int i = 2; i < 100; i++)
	{
		Card c(i);
		this->m_deckCards.push_back(c);
	}
	int i;
	std::cin >> i;
	for (int j = i; j < 90; j++) {
		int k = 2;
		std::swap(m_deckCards[k], m_deckCards[j+5]);
		k++;
	}
}

void GameTable::IssuerCard()
{
	int nrCardsIssuer=-1;
	if (m_nrGamer == 2)
		nrCardsIssuer = 8;
	if (m_nrGamer == 3)
		nrCardsIssuer = 7;
	if (m_nrGamer == 4 || m_nrGamer == 5)
		nrCardsIssuer = 6;
	int k = 0;
	while(k<m_nrGamer) {
		if (m_Gamers[k].GetCards().size() < nrCardsIssuer) {
			m_Gamers[k].AddCard(m_deckCards.back());
			m_deckCards.pop_back();
		}
		else k++;
	}
}
