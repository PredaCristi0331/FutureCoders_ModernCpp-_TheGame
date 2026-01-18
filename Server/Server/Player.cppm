export module Player;
export import Card;
import <string>;

import <vector>;

namespace game {
	export class Player
	{
	private:
		std::string m_name;
		std::vector<Card>m_cards;
	public:
		Player(const std::string& name);
		Player(const Player& other) = default;
		Player& operator=(const Player& other) = default;
		Player(Player&& other) noexcept;
		Player& operator=(Player&& other) noexcept;
		~Player() = default;
		
		std::string GetName() const;
		void SetName(std::string name);
		void AddCard(Card cardDrawn);
		std::vector<Card>& GetCards();
		const std::vector<Card>& GetCards() const;
		void RemoveCard(Card card);
		bool HasCard(int cardNumber) const;
	};
}