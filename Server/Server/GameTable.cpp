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

	/*std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(m_deckCards.begin(), m_deckCards.end(), g);*/
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

bool game::GameTable::IsGameWon()
{
	if (m_deckCards.size() > 0) return false;
	for (const auto& gamer : m_Gamers) {
		if (gamer.GetCards().size() > 0) return false;
	}
	return true;
}
