#pragma once

export module GameTable;

export import Player;
export import Card;
import <vector>;
import <string>;
import <algorithm>;
import <random>;

namespace game {
	export class GameTable
	{
	public:
		GameTable();
		GameTable(int nrGamer);
		
        // Setup methods
		void SetNrGamer(int nrGamer);
		int GetNrGame() const; // Renamed accessor for clarity
		
        // Player management
		void AddGamer(const std::string& name); // Modified to take name directly
		Player& GetGamer(int index);
		const Player& GetGamer(int index) const;
        int GetGamerIndexByName(const std::string& name) const;
        int GetNrGamers() const { return m_nrGamer; }

        // Game initialization
		void AddInitialCards();
		void MixingDeckCards();
		void IssuerCard(); // Deal cards

        // Deck management
		void PushCard(Card card, int gamerIndex);
		void RemoveDeckCardsLast();
		Card DeckCardsLast();
		int SizeDeckCards();

        // Piles accessors
		Card GetLastCardFromIncreasingFirst() const;
		Card GetLastCardFromIncreasingSecond() const;
		Card GetLastCardFromDecreasingFirst() const;
		Card GetLastCardFromDecreasingSecond() const;

        // Piles State for Serialization
        const std::vector<Card>& GetIncreasingFirst() const { return m_increasingFirst; }
        const std::vector<Card>& GetIncreasingSecond() const { return m_increasingSecond; }
        const std::vector<Card>& GetDecreasingFirst() const { return m_decreasingFirst; }
        const std::vector<Card>& GetDecreasingSecond() const { return m_decreasingSecond; }

        // Piles modification
		void PushIncreasingFirst(Card card);
		void PushIncreasingSecond(Card card);
		void PushDecreasingFirst(Card card);
		void PushDecreasingSecond(Card card);

        // Backwards trick helpers (remove last card)
		void RemoveLastIncreasingFirst();
		void RemoveLastIncreasingSecond();
		void RemoveLastDecreasingFirst();
		void RemoveLastDecreasingSecond();

        // Player actions
		void RemoveCardFromHand(int gamerIndex, Card card);
		
		// Logic & Validation
		bool IsValidMove(Card card, int stackNumber);
		bool IsMoveAllowed(Card card, int stackNumber); // Redundant? overlapping with IsValidMove
		bool IsGameWon();
        bool IsGameLost(int currentPlayerIndex); // Check if current player has moves

	private:
		int m_nrGamer;
		std::vector<Player> m_Gamers;
		std::vector<Card> m_deckCards;
		
        // Piles
        std::vector<Card> m_increasingFirst;
		std::vector<Card> m_increasingSecond;
		std::vector<Card> m_decreasingFirst;
		std::vector<Card> m_decreasingSecond;
	};
}