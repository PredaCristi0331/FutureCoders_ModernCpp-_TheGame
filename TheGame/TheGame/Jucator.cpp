#include "Jucator.h"

Jucator::Jucator()
{
}

void Jucator::AddCard(Card cardDrawn)
{
	this->m_cards.push_back(cardDrawn);
}

std::vector<Card> Jucator::GetCards()
{
	return this->m_cards;
}

Card Jucator::CardLaidDown(Card card)
{
	int j = -1;
	for(int i=0;i<m_cards.size();i++)
		if (m_cards[i] == card) {
			j = i;
			break;
		}
	if (j > -1)
		for (int i = j; i < m_cards.size() - 1; i++)
			m_cards[i] = m_cards[i + 1];
	return card;
}
