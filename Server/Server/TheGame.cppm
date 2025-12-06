export module TheGame;

export import GameTable;

namespace game {
	// Game statistics structure
	export struct GameStats {
		int totalMoves = 0;
		int cardsPlayed = 0;
		int specialMoves = 0;        // Backward trick moves (-10/+10)
		int roundsPlayed = 0;
	};

	export class TheGame
	{
	private:
		GameTable m_gameTable;
		bool m_lostGame = false;
		GameStats m_stats;
	public:
		TheGame();
		void SetLost();

		void StartTheGame();
		void ShowLastCardsFromStacks();
		void Rund();
		void FinalGame(bool status);
		bool Lost();
		bool GetLostGame();
		
		// Statistics methods
		GameStats GetStats() const;
		void PrintStats() const;
	};
}