export module Card;

import <iostream>;
import <string>;

namespace game {
	export class Card
	{
		enum class TypeCard
		{
			GAMECARD,
			FUNDCARD
		};
	public:
		Card(int card) : m_Card(card) {
			if (card == 1 || card == 100)
				this->m_typeCard = TypeCard::FUNDCARD;
			else this->m_typeCard = TypeCard::GAMECARD;
		}

		int GetCardNumber() const { return m_Card; }
		
		std::string GetTypeCard() const {
			if (m_typeCard == TypeCard::GAMECARD)
				return "GAMECARD";
			else return "FUNDCARD";
		}

		bool operator==(const Card& card) const {
			if (this->GetCardNumber() == card.GetCardNumber())
				return true;
			return false;
		}

	private:
		int m_Card;
		TypeCard m_typeCard;
	};
}
