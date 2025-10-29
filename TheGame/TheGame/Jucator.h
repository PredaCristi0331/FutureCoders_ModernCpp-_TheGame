#pragma once
#include <iostream>
#include <vector>
#include "Card.h"

class Jucator
{
private:
	std::vector<Card>m_cards;
public:
	Jucator();
	void AddCard(Card cardDrawn);
	std::vector<Card> GetCards();
	Card CardLaidDown(Card card);
};

