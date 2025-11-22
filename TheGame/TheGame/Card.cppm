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
		Card(int card);
		int GetCardNumber();
		std::string GetTypeCard();
		bool operator==(Card card);
		//friend std::ostream& operator<<(std::ostream& os, const Card& card);
	private:
		int m_Card;
		TypeCard m_typeCard;
	};
}