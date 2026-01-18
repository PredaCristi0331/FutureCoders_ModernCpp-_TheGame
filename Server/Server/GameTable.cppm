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
		GameTable(const GameTable& other) = default;
		GameTable& operator=(const GameTable& other) = default;
		GameTable(GameTable&& other) noexcept;
		GameTable& operator=(GameTable&& other) noexcept;
		~GameTable() = default;
		
		void SetNrGamer(int nrGamer);
		int GetNrGame() const;
		
		void AddGamer(const std::string& name);
		Player& GetGamer(int index);
		const Player& GetGamer(int index) const;
        int GetGamerIndexByName(const std::string& name) const;
        int GetNrGamers() const { return m_nrGamer; }

		void AddInitialCards();
		void MixingDeckCards();
		void IssuerCard();

		void PushCard(Card card, int gamerIndex);
		void RemoveDeckCardsLast();
		Card DeckCardsLast();
		int SizeDeckCards();

		Card GetLastCardFromIncreasingFirst() const;
		Card GetLastCardFromIncreasingSecond() const;
		Card GetLastCardFromDecreasingFirst() const;
		Card GetLastCardFromDecreasingSecond() const;

        const std::vector<Card>& GetIncreasingFirst() const { return m_increasingFirst; }
        const std::vector<Card>& GetIncreasingSecond() const { return m_increasingSecond; }
        const std::vector<Card>& GetDecreasingFirst() const { return m_decreasingFirst; }
        const std::vector<Card>& GetDecreasingSecond() const { return m_decreasingSecond; }

		void PushIncreasingFirst(Card card);
		void PushIncreasingSecond(Card card);
		void PushDecreasingFirst(Card card);
		void PushDecreasingSecond(Card card);

		void RemoveCardFromHand(int gamerIndex, Card card);
		
		bool IsValidMove(Card card, int stackNumber);
		bool IsGameWon();
        bool IsGameLost(int currentPlayerIndex);

	private:
		int m_nrGamer;
		std::vector<Player> m_Gamers;
		std::vector<Card> m_deckCards;
		
        std::vector<Card> m_increasingFirst;
		std::vector<Card> m_increasingSecond;
		std::vector<Card> m_decreasingFirst;
		std::vector<Card> m_decreasingSecond;
	};
}