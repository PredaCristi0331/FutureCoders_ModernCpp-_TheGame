#pragma once
#include <string>

class Card
{
	enum class TypeCard
	{
		GAMECARD,
		FUNDCARD
	};
public:
	Card(int card);

	int GetCardNumber() const;
	
	std::string GetTypeCard() const;

	bool operator==(const Card& card) const;

private:
	int m_Card;
	TypeCard m_typeCard;
};
