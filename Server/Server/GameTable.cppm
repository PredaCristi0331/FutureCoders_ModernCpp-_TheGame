export module GameTable;

export import Player;
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
		bool GamerCard(int cardNumber, int nrGamer);

		void AddGamer();
		void AddInitialCards();
		void MixingDeckCards();
		void IssuerCard();

		Card GetLastCardFromIncreasingFirst();
		Card GetLastCardFromIncreasingSecond();
		Card GetLastCardFromDecreasingFirst();
		Card GetLastCardFromDecreasingSecond();

		void ShowCardsGamer(int nrGamer);
		std::vector<Card>& GetCardsGamer(int nrGamer);
		const std::vector<Card>& GetCardsGamer(int nrGamer) const;

		void PushIncreasingFirst(Card card);
		void PushIncreasingSecond(Card card);
		void PushDecreasingFirst(Card card);
		void PushDecreasingSecond(Card card);

		Player& GetGamer(int nrGamer);
		const Player& GetGamer(int nrGamer) const;

		void PushCard(Card card, int nrGamer);

		void RemoveDeckCardsLast();
		Card DeckCardsLast();
		int SizeDeckCards();

		void RemoveLastIncreasingFirst();
		void RemoveLastIncreasingSecond();
		void RemoveLastDecreasingFirst();
		void RemoveLastDecreasingSecond();

		void RemoveCardFromHand(int nrGamer, Card card);
		
		// Move validation functions
		bool IsValidMove(Card card, int stackNumber);
		std::vector<int> GetValidMoves(Card card);
		
		bool IsGameWon();

		bool IsMoveAllowed(Card card, int stackNumber);
	private:
		int m_nrGamer;
		std::vector<Player>m_Gamers;
		std::vector<Card>m_deckCards;
		std::vector<Card>m_increasingFirst;
		std::vector<Card>m_increasingSecond;
		std::vector<Card>m_decreasingFirst;
		std::vector<Card>m_decreasingSecond;
	};
	
}