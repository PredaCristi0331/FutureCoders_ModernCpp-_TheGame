module GameTable;
using namespace game;
import <iostream>;
import <vector>;
//import <algorithm>;
//import <random>;

game::GameTable::GameTable():m_nrGamer(0)
{
}

game::GameTable::GameTable(int nrGamer):m_nrGamer(nrGamer)
{
}

void game::GameTable::SetNrGamer(int nrGamer)
{
	m_nrGamer = nrGamer;
}

int game::GameTable::GetNrGame()
{
	return m_nrGamer;
}

bool game::GameTable::GamerCard(int cardNumber, int nrGamer)
{
	for (int i = 0; i < m_Gamers[nrGamer].GetCards().size(); i++) {
		if (m_Gamers[nrGamer].returnCard(i) == cardNumber)
			return true;
	}
	return false;
}

void game::GameTable::AddGamer()
{
	if (m_nrGamer < 2 || m_nrGamer>5)
	{
		std::cout << "Not valid gamer number. The game finished.";
		return;
	}
	for (int i = 0; i < m_nrGamer; i++) {
		std::cout << "Name of the "<<i+1<<" gamer:";
		std::string name;
		std::cin >> name;
		Player gamer(name);

		m_Gamers.push_back(gamer);
	}
	std::cout << "The gamers are:\n";
	for (int i = 0; i < m_Gamers.size(); i++)
		std::cout << m_Gamers[i].GetName() << "\n";
}

void game::GameTable::AddInitialCards()
{
	Card cardInitialIncreasing(1);
	Card cardInitialDecreasing(100);
	this->m_increasingFirst.push_back(cardInitialIncreasing);
	this->m_increasingSecond.push_back(cardInitialIncreasing);
	this->m_decreasingFirst.push_back(cardInitialDecreasing);
	this->m_decreasingSecond.push_back(cardInitialDecreasing);
}

void game::GameTable::MixingDeckCards()
{
	for (int i = 2; i < 100; i++)
	{
		Card c(i);
		this->m_deckCards.push_back(c);
	}

	// Fisher-Yates shuffle algorithm
	//import <random>;
	//std::random_device rd;
	//std::mt19937 g(rd());
	//
	//for (int i = static_cast<int>(m_deckCards.size()) - 1; i > 0; i--) {
	//	std::uniform_int_distribution<int> dist(0, i);
	//	int j = dist(g);
	//	
	//	// Swap m_deckCards[i] with m_deckCards[j]
	//	Card temp = m_deckCards[i];
	//	m_deckCards[i] = m_deckCards[j];
	//	m_deckCards[j] = temp;
	//}
}

void game::GameTable::IssuerCard()
{
	int nrCardsIssuer = -1;
	if (m_nrGamer == 2)
		nrCardsIssuer = 8;
	if (m_nrGamer == 3)
		nrCardsIssuer = 7;
	if (m_nrGamer == 4 || m_nrGamer == 5)
		nrCardsIssuer = 6;
	int k = 0;
	while (k < m_nrGamer) {
		if (m_Gamers[k].GetCards().size() < nrCardsIssuer) {
			m_Gamers[k].AddCard(m_deckCards.back());
			m_deckCards.pop_back();
		}
		else k++;
	}
}

Card game::GameTable::GetLastCardFromIncreasingFirst()
{
	return m_increasingFirst.back();
}

Card game::GameTable::GetLastCardFromIncreasingSecond()
{
	return m_increasingSecond.back();
}

Card game::GameTable::GetLastCardFromDecreasingFirst()
{
	return m_decreasingFirst.back();
}

Card game::GameTable::GetLastCardFromDecreasingSecond()
{
	return m_decreasingSecond.back();
}

void game::GameTable::ShowCardsGamer(int nrGamer)
{
	Player j = m_Gamers[nrGamer];
	std::cout << "My cards!\n";
	j.ShowCards();
}

std::vector<Card>& game::GameTable:: GetCardsGamer(int nrGamer)
{
	return m_Gamers[nrGamer].GetCards();
}

const std::vector<Card>& game::GameTable::GetCardsGamer(int nrGamer) const
{
	return m_Gamers[nrGamer].GetCards();
}

void game::GameTable::PushIncreasingFirst(Card card)
{
	m_increasingFirst.push_back(card);
}

void game::GameTable::PushIncreasingSecond(Card card)
{
	m_increasingSecond.push_back(card);
}

void game::GameTable::PushDecreasingFirst(Card card)
{
	m_decreasingFirst.push_back(card);
}

void game::GameTable::PushDecreasingSecond(Card card)
{
	m_decreasingSecond.push_back(card);
}

Player& game::GameTable::GetGamer(int nrGamer)
{
	return m_Gamers[nrGamer];
}

const Player& game::GameTable::GetGamer(int nrGamer) const
{
	return m_Gamers[nrGamer];
}

void game::GameTable::PushCard(Card card, int nrGamer)
{
	m_Gamers[nrGamer].AddCard(card);
}

void game::GameTable::RemoveDeckCardsLast()
{
	m_deckCards.pop_back();
}

Card game::GameTable::DeckCardsLast()
{
	return m_deckCards.back();
}

int game::GameTable::SizeDeckCards()
{
	return static_cast<int>(m_deckCards.size());
}

void game::GameTable::RemoveLastIncreasingFirst()
{
	m_increasingFirst.pop_back();
}

void game::GameTable::RemoveLastIncreasingSecond()
{
	m_increasingSecond.pop_back();
}

void game::GameTable::RemoveLastDecreasingFirst()
{
	m_decreasingFirst.pop_back();
}

void game::GameTable::RemoveLastDecreasingSecond()
{
	m_decreasingSecond.pop_back();
}

void game::GameTable::RemoveCardFromHand(int nrGamer, Card card)
{
	m_Gamers[nrGamer].RemoveCard(card);
}

// Move validation functions
bool game::GameTable::IsValidMove(Card card, int stackNumber)
{
	int cardValue = card.GetCardNumber();
	
	switch (stackNumber) {
	case 1: // Increasing First
		if (m_increasingFirst.empty()) return false;
		return cardValue > m_increasingFirst.back().GetCardNumber() ||
			   cardValue == m_increasingFirst.back().GetCardNumber() - 10;
	case 2: // Increasing Second
		if (m_increasingSecond.empty()) return false;
		return cardValue > m_increasingSecond.back().GetCardNumber() ||
			   cardValue == m_increasingSecond.back().GetCardNumber() - 10;
	case 3: // Decreasing First
		if (m_decreasingFirst.empty()) return false;
		return cardValue < m_decreasingFirst.back().GetCardNumber() ||
			   cardValue == m_decreasingFirst.back().GetCardNumber() + 10;
	case 4: // Decreasing Second
		if (m_decreasingSecond.empty()) return false;
		return cardValue < m_decreasingSecond.back().GetCardNumber() ||
			   cardValue == m_decreasingSecond.back().GetCardNumber() + 10;
	default:
		return false;
	}
}

std::vector<int> game::GameTable::GetValidMoves(Card card)
{
	std::vector<int> validStacks;
	
	for (int i = 1; i <= 4; i++) {
		if (IsValidMove(card, i)) {
			validStacks.push_back(i);
		}
	}
	
	return validStacks;
}

bool game::GameTable::IsGameWon()
{
	if (m_deckCards.size() > 0) return false;
	for (const auto& gamer : m_Gamers) {
		if (gamer.GetCards().size() > 0) return false;
	}
	return true;
}

bool game::GameTable::IsMoveAllowed(Card card, int stackNumber)
{
	const Card* top = nullptr;
	switch (stackNumber) {
	case 1: if (!m_increasingFirst.empty()) top = &m_increasingFirst.back(); break;
	case 2: if (!m_increasingSecond.empty()) top = &m_increasingSecond.back(); break;
	case 3: if (!m_decreasingFirst.empty()) top = &m_decreasingFirst.back(); break;
	case 4: if (!m_decreasingSecond.empty()) top = &m_decreasingSecond.back(); break;
	}
	if (!top) return true; // dacă stiva e goală
	int diff = card.GetCardNumber() - top->GetCardNumber();
	if (stackNumber <= 2) return diff > 0 || diff == -10; // increasing
	return diff < 0 || diff == 10; // decreasing
}
