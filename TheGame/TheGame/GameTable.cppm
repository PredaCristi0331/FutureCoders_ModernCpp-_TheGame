export module GameTable;
export import Jucator;
export import Card;
import <iostream>;
import <vector>;

namespace game {
	export class GameTable
	{
	public:
		GameTable();
		GameTable(int nrGamer);
		void SetNrGamer(int nrGamer);
		int GetNrGame();

		void AddGamer();
		void AddInitialCards();
		void MixingDeckCards();
		void IssuerCard();

		Card GetLastCardFromIncreasingFirst();
		Card GetLastCardFromIncreasingSecond();
		Card GetLastCardFromDecreasingFirst();
		Card GetLastCardFromDecreasingSecond();

		void ShowCardsGamer(int nrGamer);
		std::vector<Card> GetCardsGamer(int nrGamer);

		void PushIncreasingFirst(Card card);
		void PushIncreasingSecond(Card card);
		void PushDecreasingFirst(Card card);
		void PushDecreasingSecond(Card card);

		Jucator GetGamer(int nrGamer);

		void PushCard(Card card, int nrGamer);

		void RemoveDeckCardsLast();
		Card DeckCardsLast();
		int SizeDeckCards();

		void RemoveLastIncreasingFirst();
		void RemoveLastIncreasingSecond();
		void RemoveLastDecreasingFirst();
		void RemoveLastDecreasingSecond();
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