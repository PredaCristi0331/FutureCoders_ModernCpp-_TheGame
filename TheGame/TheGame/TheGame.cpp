#include "TheGame.h"

TheGame::TheGame()
{
}

void TheGame::StartTheGame()
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

void TheGame::Rund()
{
	for (int i = 0; i < m_gameTable.GetNrGame(); i++) {
		std::cout << "The last cards of the stacks: \n";
		std::cout << m_gameTable.GetLastCardFromIncreasingFirst().GetCardNumber() << "\n";
		std::cout << m_gameTable.GetLastCardFromIncreasingSecond().GetCardNumber() << "\n";
		std::cout << m_gameTable.GetLastCardFromDecreasingFirst().GetCardNumber() << "\n";
		std::cout << m_gameTable.GetLastCardFromDecreasingSecond().GetCardNumber() << "\n";
		m_gameTable.ShowCardsGamer(i);
		std::vector<Card>cards = m_gameTable.GetCardsGamer(i);
		std::cout << std::endl << "Please choose a card, and enter the number of the card!\n";
		int nrCard1, nrCard2;
		std::cin >> nrCard1;
		std::cout << "Please choose the stack of the cards where you want to put, and enter the number of the stack!\n";
		int stack1, stack2;
		std::cin >> stack1;
		std::cout << "Please choose another card, and enter the number of the card!\n";
		std::cin >> nrCard2;
		std::cout << "Please choose the stack of the cards where you want to put, and enter the number of the stack!\n";
		std::cin >> stack2;
		if (stack1 == 1) m_gameTable.PushIncreasingFirst(m_gameTable.GetGamer(i).CardLaidDown(nrCard1));
		if (stack1 == 2) m_gameTable.PushIncreasingSecond(m_gameTable.GetGamer(i).CardLaidDown(nrCard1));
		if (stack1 == 3) m_gameTable.PushDecreasingFirst(m_gameTable.GetGamer(i).CardLaidDown(nrCard1));
		if (stack1 == 4) m_gameTable.PushDecreasingSecond(m_gameTable.GetGamer(i).CardLaidDown(nrCard1));

		if (stack2 == 1) m_gameTable.PushIncreasingFirst(m_gameTable.GetGamer(i).CardLaidDown(nrCard2));
		if (stack2 == 2) m_gameTable.PushIncreasingSecond(m_gameTable.GetGamer(i).CardLaidDown(nrCard2));
		if (stack2 == 3) m_gameTable.PushDecreasingFirst(m_gameTable.GetGamer(i).CardLaidDown(nrCard2));
		if (stack2 == 4) m_gameTable.PushDecreasingSecond(m_gameTable.GetGamer(i).CardLaidDown(nrCard2));
	}
}
