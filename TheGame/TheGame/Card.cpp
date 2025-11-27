#include "Card.h"

Card::Card(int card) : m_Card(card) {
	if (card == 1 || card == 100)
		this->m_typeCard = TypeCard::FUNDCARD;
	else this->m_typeCard = TypeCard::GAMECARD;
}

int Card::GetCardNumber() const { return m_Card; }

std::string Card::GetTypeCard() const {
	if (m_typeCard == TypeCard::GAMECARD)
		return "GAMECARD";
	else return "FUNDCARD";
}

bool Card::operator==(const Card& card) const {
	if (this->GetCardNumber() == card.GetCardNumber())
		return true;
	return false;
}
