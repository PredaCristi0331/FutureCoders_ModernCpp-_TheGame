module GameTable;
using namespace game;
import <iostream>;
import <vector>;
import <algorithm>;
import <random>;

game::GameTable::GameTable():m_nrGamer(0)
{
}

game::GameTable::GameTable(int nrGamer):m_nrGamer(nrGamer)
{
}

game::GameTable::GameTable(GameTable&& other) noexcept
	: m_nrGamer(other.m_nrGamer),
	  m_Gamers(std::move(other.m_Gamers)),
	  m_deckCards(std::move(other.m_deckCards)),
	  m_increasingFirst(std::move(other.m_increasingFirst)),
	  m_increasingSecond(std::move(other.m_increasingSecond)),
	  m_decreasingFirst(std::move(other.m_decreasingFirst)),
	  m_decreasingSecond(std::move(other.m_decreasingSecond))
{
	other.m_nrGamer = 0;
}

game::GameTable& game::GameTable::operator=(GameTable&& other) noexcept
{
	if (this != &other) {
		m_nrGamer = other.m_nrGamer;
		m_Gamers = std::move(other.m_Gamers);
		m_deckCards = std::move(other.m_deckCards);
		m_increasingFirst = std::move(other.m_increasingFirst);
		m_increasingSecond = std::move(other.m_increasingSecond);
		m_decreasingFirst = std::move(other.m_decreasingFirst);
		m_decreasingSecond = std::move(other.m_decreasingSecond);
		other.m_nrGamer = 0;
	}
	return *this;
}

void game::GameTable::SetNrGamer(int nrGamer)
{
	m_nrGamer = nrGamer;
}

int game::GameTable::GetNrGame() const
{
	return m_nrGamer;
}

void game::GameTable::AddGamer(const std::string& name)
{
	if (m_Gamers.size() >= 5) return;
	
    Player gamer(name);
	m_Gamers.push_back(gamer);
    m_nrGamer = static_cast<int>(m_Gamers.size());
}

int game::GameTable::GetGamerIndexByName(const std::string& name) const
{
    for(size_t i=0; i<m_Gamers.size(); ++i) {
        if(m_Gamers[i].GetName() == name) return static_cast<int>(i);
    }
    return -1;
}

void game::GameTable::AddInitialCards()
{
    m_increasingFirst.clear();
    m_increasingSecond.clear();
    m_decreasingFirst.clear();
    m_decreasingSecond.clear();

	Card cardInitialIncreasing(1);
	Card cardInitialDecreasing(100);
	this->m_increasingFirst.push_back(cardInitialIncreasing);
	this->m_increasingSecond.push_back(cardInitialIncreasing);
	this->m_decreasingFirst.push_back(cardInitialDecreasing);
	this->m_decreasingSecond.push_back(cardInitialDecreasing);
}

void game::GameTable::MixingDeckCards()
{
    m_deckCards.clear();
	for (int i = 2; i < 100; i++)
	{
		Card c(i);
		this->m_deckCards.push_back(c);
	}

	std::random_device rd;
	std::mt19937 g(rd());
    std::shuffle(m_deckCards.begin(), m_deckCards.end(), g);
}

void game::GameTable::IssuerCard()
{
	int nrCardsIssuer = -1;
	if (m_nrGamer == 2)
		nrCardsIssuer = 8;
	else if (m_nrGamer == 3)
		nrCardsIssuer = 7;
	else if (m_nrGamer >= 4)
		nrCardsIssuer = 6;
    
    if (nrCardsIssuer == -1) return;

    for(int k=0; k<m_nrGamer; ++k) {
        while(m_Gamers[k].GetCards().size() < nrCardsIssuer && !m_deckCards.empty()) {
             m_Gamers[k].AddCard(m_deckCards.back());
             m_deckCards.pop_back();
        }
    }
}

Card game::GameTable::GetLastCardFromIncreasingFirst() const
{
    if(m_increasingFirst.empty()) return Card(1);
	return m_increasingFirst.back();
}

Card game::GameTable::GetLastCardFromIncreasingSecond() const
{
     if(m_increasingSecond.empty()) return Card(1);
	return m_increasingSecond.back();
}

Card game::GameTable::GetLastCardFromDecreasingFirst() const
{
     if(m_decreasingFirst.empty()) return Card(100);
	return m_decreasingFirst.back();
}

Card game::GameTable::GetLastCardFromDecreasingSecond() const
{
     if(m_decreasingSecond.empty()) return Card(100);
	return m_decreasingSecond.back();
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
    if(nrGamer >= 0 && nrGamer < m_Gamers.size())
	    m_Gamers[nrGamer].AddCard(card);
}

void game::GameTable::RemoveDeckCardsLast()
{
    if(!m_deckCards.empty())
	    m_deckCards.pop_back();
}

Card game::GameTable::DeckCardsLast()
{
    if(m_deckCards.empty()) return Card(0);
	return m_deckCards.back();
}

int game::GameTable::SizeDeckCards()
{
	return static_cast<int>(m_deckCards.size());
}

void game::GameTable::RemoveCardFromHand(int nrGamer, Card card)
{
    if(nrGamer >= 0 && nrGamer < m_Gamers.size())
	    m_Gamers[nrGamer].RemoveCard(card);
}

bool game::GameTable::IsValidMove(Card card, int stackNumber)
{
	int cardValue = card.GetCardNumber();
	
	switch (stackNumber) {
	case 1:
		if (m_increasingFirst.empty()) return false;
		return cardValue > m_increasingFirst.back().GetCardNumber() ||
			   cardValue == m_increasingFirst.back().GetCardNumber() - 10;
	case 2:
		if (m_increasingSecond.empty()) return false;
		return cardValue > m_increasingSecond.back().GetCardNumber() ||
			   cardValue == m_increasingSecond.back().GetCardNumber() - 10;
	case 3:
		if (m_decreasingFirst.empty()) return false;
		return cardValue < m_decreasingFirst.back().GetCardNumber() ||
			   cardValue == m_decreasingFirst.back().GetCardNumber() + 10;
	case 4:
		if (m_decreasingSecond.empty()) return false;
		return cardValue < m_decreasingSecond.back().GetCardNumber() ||
			   cardValue == m_decreasingSecond.back().GetCardNumber() + 10;
	default:
		return false;
	}
}

bool game::GameTable::IsGameWon()
{
	if (m_deckCards.size() > 0) return false;
	for (const auto& gamer : m_Gamers) {
		if (gamer.GetCards().size() > 0) return false;
	}
	return true;
}

bool game::GameTable::IsGameLost(int currentPlayerIndex)
{
    if(currentPlayerIndex < 0 || currentPlayerIndex >= m_Gamers.size()) return true;
    
    const auto& hand = m_Gamers[currentPlayerIndex].GetCards();
    if(hand.empty()) return false;
    
    for(const auto& card : hand) {
        for(int stack=1; stack<=4; ++stack) {
            if(IsValidMove(card, stack)) return false;
        }
    }
    
    return true;
}
