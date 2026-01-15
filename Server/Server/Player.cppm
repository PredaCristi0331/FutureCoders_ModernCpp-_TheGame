export module Player;
export import Card;
import <string>;

import <vector>;

namespace game {
	export class Player
	{
	private:
		int m_id = 0;
		std::string m_name;
		std::vector<Card>m_cards;
	public:
		Player(const std::string& name);
		void SetId(int id);
		int GetId();
		std::string GetName() const;
		void SetName(std::string name);
		void SetCards(std::vector<Card>cards);
		void AddCard(Card cardDrawn);
		Card returnCard(int numberCard);
		std::vector<Card>& GetCards();
		const std::vector<Card>& GetCards() const;
		void ShowCards();
		Card CardLaidDown(int card);
		void RemoveCard(Card card);
		bool HasCard(int cardNumber) const;
	};
}