module Player;
using namespace game;
import <vector>;
import <iostream>;

game::Player::Player(const std::string& name):m_name(name)
{
	m_cards.push_back(-1);
}

void game::Player::SetId(int id) {
	m_id = id;
}

int game::Player::GetId() {
	return m_id;
}

std::string game::Player::GetName()
{
	return m_name;
}

void game::Player::SetName(std::string name)
{
	m_name = name;
}

void game::Player::SetCards(std::vector<Card> cards)
{
	m_cards = cards;
}

void game::Player::AddCard(Card cardDrawn)
{
	if (m_cards.size() == 1 && m_cards[0] == -1)
		m_cards[0] = cardDrawn;
	else this->m_cards.push_back(cardDrawn);
}

Card game::Player::returnCard(int numberCard)
{
	for (int i = 0; i < m_cards.size(); i++) {
		if (m_cards[i].GetCardNumber() == numberCard)
			return m_cards[i];
	}
	return Card(-1);
}

std::vector<Card>& game::Player::GetCards()
{
	return this->m_cards;
}

const std::vector<Card>& game::Player::GetCards() const
{
	return this->m_cards;
}

void game::Player::ShowCards()
{
	for (int i = 0; i < m_cards.size(); i++) {
		std::cout << m_cards[i].GetCardNumber() << " ";
	}
}

Card game::Player::CardLaidDown(int card)
{
	auto it = std::find_if(m_cards.begin(), m_cards.end(),
		[card](const Card& c) { return c.GetCardNumber() == card; });

	if (it == m_cards.end())
		throw std::runtime_error("Card not found");

	Card result = *it;
	m_cards.erase(it);

	return result;
}

void game::Player::RemoveCard(Card card)
{
	auto it = std::remove_if(m_cards.begin(), m_cards.end(), [&](const Card& c) {
		return c.GetCardNumber() == card.GetCardNumber();
		});
	if (it != m_cards.end()) {
		m_cards.erase(it, m_cards.end());
	}
}

bool game::Player::HasCard(int cardNumber) const
{
	for (const Card& card : m_cards)
	{
		if (card.GetCardNumber() == cardNumber)
			return true;
	}
	return false;
}
