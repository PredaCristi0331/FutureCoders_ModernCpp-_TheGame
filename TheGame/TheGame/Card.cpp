#include "Card.h"
#include <string>

Card::Card(int m_card):
	m_Card(m_card)
{
	if (m_card == 1 || m_card == 100)
		this->m_typeCard = TypeCard::FUNDCARD;
	else this->m_typeCard = TypeCard::GAMECARD;
}

int Card::GetCardNumber()
{
	return m_Card;
}

std::string Card::GetTypeCard()
{
	if (m_typeCard == TypeCard::GAMECARD)
		return "GAMECARD";
	else return "FUNDCARD";
}

bool Card::operator==(Card card)
{
	if (this->GetCardNumber() == card.GetCardNumber())
		return true;
	return false;
}

