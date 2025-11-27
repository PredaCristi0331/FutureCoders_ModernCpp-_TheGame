export module GameTable;
export import Jucator;
export import Card;
import <iostream>;
import <vector>;
import <algorithm>;
import <random>;

namespace game {
	export class GameTable
	{
	public:
		GameTable() : m_nrGamer(0) {}
		
		GameTable(int nrGamer) : m_nrGamer(nrGamer) {}
		
		void SetNrGamer(int nrGamer) { m_nrGamer = nrGamer; }
		
		int GetNrGame() { return m_nrGamer; }

		void AddGamer() {
			if (m_nrGamer < 2 || m_nrGamer>5)
			{
				std::cout << "Not valid gamer number. The game finished.";
				return;
			}
			Jucator gamer;
			for (int i = 0; i < m_nrGamer; i++)
				m_Gamers.push_back(gamer);
		}

		void AddInitialCards() {
			Card cardInitialIncreasing(1);
			Card cardInitialDecreasing(100);
			this->m_increasingFirst.push_back(cardInitialIncreasing);
			this->m_increasingSecond.push_back(cardInitialIncreasing);
			this->m_decreasingFirst.push_back(cardInitialDecreasing);
			this->m_decreasingSecond.push_back(cardInitialDecreasing);
		}

		void MixingDeckCards() {
			for (int i = 2; i < 100; i++)
			{
				Card c(i);
				this->m_deckCards.push_back(c);
			}
			
			std::random_device rd;
			std::mt19937 g(rd());
			std::shuffle(m_deckCards.begin(), m_deckCards.end(), g);
		}

		void IssuerCard() {
			int nrCardsIssuer=-1;
			if (m_nrGamer == 2)
				nrCardsIssuer = 8;
			if (m_nrGamer == 3)
				nrCardsIssuer = 7;
			if (m_nrGamer == 4 || m_nrGamer == 5)
				nrCardsIssuer = 6;
			int k = 0;
			while(k<m_nrGamer) {
				if (m_Gamers[k].GetCards().size() < nrCardsIssuer) {
					m_Gamers[k].AddCard(m_deckCards.back());
					m_deckCards.pop_back();
				}
				else k++;
			}
		}

		Card GetLastCardFromIncreasingFirst() { return m_increasingFirst.back(); }
		Card GetLastCardFromIncreasingSecond() { return m_increasingSecond.back(); }
		Card GetLastCardFromDecreasingFirst() { return m_decreasingFirst.back(); }
		Card GetLastCardFromDecreasingSecond() { return m_decreasingSecond.back(); }

		void ShowCardsGamer(int nrGamer) {
			Jucator j = m_Gamers[nrGamer];
			j.ShowCards();
		}

		std::vector<Card> GetCardsGamer(int nrGamer) { return m_Gamers[nrGamer].GetCards(); }

		void PushIncreasingFirst(Card card) { m_increasingFirst.push_back(card); }
		void PushIncreasingSecond(Card card) { m_increasingSecond.push_back(card); }
		void PushDecreasingFirst(Card card) { m_decreasingFirst.push_back(card); }
		void PushDecreasingSecond(Card card) { m_decreasingSecond.push_back(card); }

		Jucator GetGamer(int nrGamer) { return m_Gamers[nrGamer]; }

		void PushCard(Card card, int nrGamer) { m_Gamers[nrGamer].AddCard(card); }

		void RemoveDeckCardsLast() { m_deckCards.pop_back(); }
		Card DeckCardsLast() { return m_deckCards.back(); }
		int SizeDeckCards() { return static_cast<int>(m_deckCards.size()); }

		void RemoveLastIncreasingFirst() { m_increasingFirst.pop_back(); }
		void RemoveLastIncreasingSecond() { m_increasingSecond.pop_back(); }
		void RemoveLastDecreasingFirst() { m_decreasingFirst.pop_back(); }
		void RemoveLastDecreasingSecond() { m_decreasingSecond.pop_back(); }
		
		void RemoveCardFromHand(int nrGamer, Card card) { m_Gamers[nrGamer].RemoveCard(card); }
		
		bool IsGameWon() {
			if (m_deckCards.size() > 0) return false;
			for (const auto& gamer : m_Gamers) {
				if (gamer.GetCards().size() > 0) return false;
			}
			return true;
		}

	private:
		int m_nrGamer;
		std::vector<Jucator>m_Gamers;
		std::vector<Card>m_deckCards;
		std::vector<Card>m_increasingFirst;
		std::vector<Card>m_increasingSecond;
		std::vector<Card>m_decreasingFirst;
		std::vector<Card>m_decreasingSecond;
	};
}
