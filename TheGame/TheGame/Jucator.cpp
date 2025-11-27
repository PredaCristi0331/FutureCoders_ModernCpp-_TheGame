export module Jucator;
export import Card;

import <vector>;
import <iostream>;
import <algorithm>;

namespace game {
	export class Jucator
	{
	private:
		std::vector<Card>m_cards;
	public:
		Jucator() {}
		
		void AddCard(Card cardDrawn) {
			this->m_cards.push_back(cardDrawn);
		}

		std::vector<Card> GetCards() const {
			return this->m_cards;
		}

		void ShowCards() {
			for (int i = 0; i < m_cards.size(); i++) {
				std::cout << m_cards[i].GetCardNumber() << " ";
			}
		}

		Card CardLaidDown(int card) {
			int j = -1;
			for(int i=0;i<m_cards.size();i++)
				if (m_cards[i].GetCardNumber() == card) {
					j = i;
					break;
				}
			if (j > -1)
				for (int i = j; i < m_cards.size() - 1; i++)
					m_cards[i] = m_cards[i + 1];
			return m_cards[j];
		}

		void RemoveCard(Card card) {
			auto it = std::remove_if(m_cards.begin(), m_cards.end(), [&](const Card& c) {
				return c.GetCardNumber() == card.GetCardNumber();
			});
			if (it != m_cards.end()) {
				m_cards.erase(it, m_cards.end());
			}
		}
	};
}
