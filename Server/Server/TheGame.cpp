module TheGame;
import <iostream>;
import <vector>;
using namespace game;

game::TheGame::TheGame()
{
}

void game::TheGame::SetLost()
{
	m_lostGame = true;
}


void game::TheGame::StartTheGame()
{
	std::cout << "Please enter the number of the gamers!\n";
	int nrGamer;
	std::cin >> nrGamer;
	m_gameTable.SetNrGamer(nrGamer);
	m_gameTable.AddGamer();
	m_gameTable.AddInitialCards();
	m_gameTable.MixingDeckCards();
	m_gameTable.IssuerCard();
}

void game::TheGame::ShowLastCardsFromStacks()
{
	std::cout << "The last cards of the stacks: \n";
	std::cout << m_gameTable.GetLastCardFromIncreasingFirst().GetCardNumber() << "\n";
	std::cout << m_gameTable.GetLastCardFromIncreasingSecond().GetCardNumber() << "\n";
	std::cout << m_gameTable.GetLastCardFromDecreasingFirst().GetCardNumber() << "\n";
	std::cout << m_gameTable.GetLastCardFromDecreasingSecond().GetCardNumber() << "\n";
}

void game::TheGame::Rund()
{
	for (int i = 0; i < m_gameTable.GetNrGame(); i++) {
		ShowLastCardsFromStacks();
		m_gameTable.ShowCardsGamer(i);
		std::vector<Card>cards = m_gameTable.GetCardsGamer(i);
		if (m_gameTable.SizeDeckCards() != 0)
		{
			std::cout << "Please enter how many cards do you want to put! (It must be minim 2 cards!)\n";
		}
		else std::cout << "Please enter how many cards do you want to put! (It must be minim 1 cards!)\n";
		int nrCards = 0;
		if (m_gameTable.SizeDeckCards() != 0)
		{
			while (nrCards < 2) {
				std::cin >> nrCards;
				if (nrCards < 2) {
					std::cout << "Invalid number of cards. You don't have enough cards.\n";
					bool lost = Lost();
					FinalGame(lost);
					SetLost();
					return;
				}
			}
		}
		else while (nrCards < 1) {
			std::cin >> nrCards;
			if (nrCards < 1) {
				std::cout << "Invalid number of cards. You don't have enough cards.\n";
				bool lost = Lost();
				FinalGame(lost);
				SetLost();
				return;
			}
		}

		for (int i = 0; i < nrCards; i++) {
			std::cout << std::endl << "Please choose a card and enter the number of the card!\n";
			int cardSelect;
			std::cin >> cardSelect;
			std::cout << "Please choose the stack of the cards where you want to put, and enter the number of the stack!\n";
			int stackCard;
			std::cin >> stackCard;

			if (stackCard == 1)
				if (stackCard > m_gameTable.GetLastCardFromIncreasingFirst().GetCardNumber())
				{
					m_gameTable.PushIncreasingFirst(m_gameTable.GetGamer(i).CardLaidDown(cardSelect));
				}
				else {
					if (stackCard == m_gameTable.GetLastCardFromIncreasingFirst().GetCardNumber() - 10) {
						m_gameTable.RemoveLastIncreasingFirst();
						m_gameTable.PushIncreasingFirst(m_gameTable.GetGamer(i).CardLaidDown(cardSelect));
					}
				}

			if (stackCard == 2)
				if (stackCard > m_gameTable.GetLastCardFromIncreasingSecond().GetCardNumber()) {
					m_gameTable.PushIncreasingSecond(m_gameTable.GetGamer(i).CardLaidDown(cardSelect));
				}
				else if (stackCard == m_gameTable.GetLastCardFromIncreasingSecond().GetCardNumber() - 10) {
					m_gameTable.RemoveLastIncreasingSecond();
					m_gameTable.PushIncreasingSecond(m_gameTable.GetGamer(i).CardLaidDown(cardSelect));
				}
			if (stackCard == 3)
				if (stackCard > m_gameTable.GetLastCardFromDecreasingFirst().GetCardNumber())
				{
					m_gameTable.PushDecreasingFirst(m_gameTable.GetGamer(i).CardLaidDown(cardSelect));
				}
				else if (stackCard == m_gameTable.GetLastCardFromDecreasingFirst().GetCardNumber() - 10) {
					m_gameTable.RemoveLastDecreasingFirst();
					m_gameTable.PushDecreasingFirst(m_gameTable.GetGamer(i).CardLaidDown(cardSelect));
				}
			if (stackCard == 4)
				if (stackCard > m_gameTable.GetLastCardFromDecreasingSecond().GetCardNumber()) {
					m_gameTable.PushDecreasingSecond(m_gameTable.GetGamer(i).CardLaidDown(cardSelect));
				}
				else if (stackCard == m_gameTable.GetLastCardFromDecreasingSecond().GetCardNumber() - 10) {
					m_gameTable.RemoveLastDecreasingSecond();
					m_gameTable.PushDecreasingSecond(m_gameTable.GetGamer(i).CardLaidDown(cardSelect));
				}
			if (m_gameTable.SizeDeckCards() != 0)
			{
				m_gameTable.PushCard(m_gameTable.DeckCardsLast(), i);
				m_gameTable.RemoveDeckCardsLast();
			}
		}
	}
}

void game::TheGame::FinalGame(bool status)
{
	std::cout << "The game finished!\n";
	if (status == true)
		std::cout << "Congratulations! You won!";
	else std::cout << "You lost!";
}

bool game::TheGame::Lost()
{
	return false;
}

bool game::TheGame::GetLostGame()
{
	return m_lostGame;
}