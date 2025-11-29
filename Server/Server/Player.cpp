module Player;
using namespace game;
import <vector>;
import <iostream>;

game::Player::Player()
{
	m_cards.push_back(-1);
}

void game::Player::AddCard(Card cardDrawn)
{
	if (m_cards.size() == 1 && m_cards[0] == -1)
		m_cards[0] = cardDrawn;
	else this->m_cards.push_back(cardDrawn);
}

std::vector<Card> game::Player::GetCards() const
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
	int j = -1;
	for (int i = 0; i < m_cards.size(); i++)
		if (m_cards[i].GetCardNumber() == card) {
			j = i;
			break;
		}
	if (j > -1)
		for (int i = j; i < m_cards.size() - 1; i++)
			m_cards[i] = m_cards[i + 1];
	return m_cards[j];
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
