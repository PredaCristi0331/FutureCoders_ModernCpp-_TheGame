export module TheGame;

export import GameTable;

namespace game {
	export class TheGame
	{
	private:
		GameTable m_gameTable;
		bool m_lostGame = false;
	public:
		TheGame();
		void SetLost();

		void StartTheGame();
		void ShowLastCardsFromStacks();
		void Rund();
		void FinalGame(bool status);
		bool Lost();
		bool GetLostGame();
	};
}