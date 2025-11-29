export module Player;
export import Card;

import <vector>;

namespace game {
	export class Player
	{
	private:
		std::vector<Card>m_cards;
	public:
		Player();
		void AddCard(Card cardDrawn);
		std::vector<Card> GetCards() const;
		void ShowCards();
		Card CardLaidDown(int card);
		void RemoveCard(Card card);
	};
}