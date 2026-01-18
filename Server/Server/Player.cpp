module Player;
using namespace game;
import <vector>;
import <iostream>;

game::Player::Player(const std::string& name):m_name(name)
{
	m_cards.push_back(-1);
}

game::Player::Player(Player&& other) noexcept
	: m_name(std::move(other.m_name)), m_cards(std::move(other.m_cards))
{
}

game::Player& game::Player::operator=(Player&& other) noexcept
{
	if (this != &other) {
		m_name = std::move(other.m_name);
		m_cards = std::move(other.m_cards);
	}
	return *this;
}

std::string game::Player::GetName() const
{
	return m_name;
}

void game::Player::SetName(std::string name)
{
	m_name = name;
}

void game::Player::AddCard(Card cardDrawn)
{
	if (m_cards.size() == 1 && m_cards[0] == -1)
		m_cards[0] = cardDrawn;
	else this->m_cards.push_back(cardDrawn);
}

std::vector<Card>& game::Player::GetCards()
{
	return this->m_cards;
}

const std::vector<Card>& game::Player::GetCards() const
{
	return this->m_cards;
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
