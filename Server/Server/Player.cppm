export module Player;
export import Card;
import <string>;

import <vector>;

namespace game {
	export class Player
	{
	private:
		std::string m_name;
		std::vector<Card>m_cards;
	public:
		Player(const std::string& name);
		std::string GetName();
		void AddCard(Card cardDrawn);
		Card returnCard(int numberCard);
		std::vector<Card> GetCards() const;
		void ShowCards();
		Card CardLaidDown(int card);
		void RemoveCard(Card card);
	};
}