#include "GameTable.h"
#include "TheGame.h"

GameTable::GameTable()
{
}

GameTable::GameTable(int nrGamer):
	m_nrGamer(nrGamer)
{
}

void GameTable::SetNrGamer(int nrGamer)
{
	m_nrGamer = nrGamer;
}

int GameTable::GetNrGame()
{
	return m_nrGamer;
}

void GameTable::AddGamer()
{
	if (m_nrGamer < 2 || m_nrGamer>5)
	{
		std::cout << "Not valid gamer number. The game finished.";
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
		int k;
		std::cout << "Please enter a number for mixing the cards!\n";
		std::cin >> k;
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

Card GameTable::GetLastCardFromIncreasingFirst()
{
	return m_increasingFirst.back();
}

Card GameTable::GetLastCardFromIncreasingSecond()
{
	return m_increasingSecond.back();
}

Card GameTable::GetLastCardFromDecreasingFirst()
{
	return m_decreasingFirst.back();
}

Card GameTable::GetLastCardFromDecreasingSecond()
{
	return m_decreasingSecond.back();
}

void GameTable::ShowCardsGamer(int nrGamer)
{
	Jucator j = m_Gamers[nrGamer];
	j.ShowCards();
}

std::vector<Card> GameTable::GetCardsGamer(int nrGamer)
{
	return m_Gamers[nrGamer].GetCards();
}

void GameTable::PushIncreasingFirst(Card card)
{
	m_increasingFirst.push_back(card);
}

void GameTable::PushIncreasingSecond(Card card)
{
	m_increasingSecond.push_back(card);
}

void GameTable::PushDecreasingFirst(Card card)
{
	m_decreasingFirst.push_back(card);
}

void GameTable::PushDecreasingSecond(Card card)
{
	m_decreasingSecond.push_back(card);
}

Jucator GameTable::GetGamer(int nrGamer)
{
	return m_Gamers[nrGamer];
}
