export module Jucator;
export import Card;

import <vector>;

namespace game {
	export class Jucator
	{
	private:
		std::vector<Card>m_cards;
	public:
		Jucator();
		void AddCard(Card cardDrawn);
		std::vector<Card> GetCards();
		void ShowCards();
		Card CardLaidDown(int card);
	};
}