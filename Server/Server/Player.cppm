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
		void AddCard(Card cardDrawn);
		std::vector<Card> GetCards() const;
		void ShowCards();
		Card CardLaidDown(int card);
		void RemoveCard(Card card);
	};
}