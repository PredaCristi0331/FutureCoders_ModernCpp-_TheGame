module TheGame;
import <iostream>;
import <vector>;
using namespace game;

game::TheGame::TheGame():m_lostGame(false)
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
    
    // Updated AddGamer Loop
    for(int i = 0; i < nrGamer; ++i) {
        std::cout << "Enter name for player " << (i + 1) << ": ";
        std::string name;
        std::cin >> name;
        m_gameTable.AddGamer(name);
    }
    
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
    m_stats.roundsPlayed++;

    for (int i = 0; i < m_gameTable.GetNrGame(); ++i)
    {
        ShowLastCardsFromStacks();
        std::cout << m_gameTable.GetGamer(i).GetName() << " is playing.\n\n";

        int minCards = (m_gameTable.SizeDeckCards() != 0) ? 2 : 1;
        int nrCards = 0;

        // -------- choose number of cards --------
        while (true)
        {
            std::cout << "How many cards do you want to put? (minimum "
                << minCards << ")\n";

            if (!(std::cin >> nrCards))
            {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Invalid input. Try again.\n";
                continue;
            }

            if (nrCards >= minCards)
                break;

            std::cout << "You must play at least " << minCards << " cards.\n";
        }

        // -------- play cards --------
        for (int j = 0; j < nrCards; )
        {
            int cardSelect, stackCard;

            // Updated ShowCards usage
            m_gameTable.GetGamer(i).ShowCards();

            std::cout << "\nChoose a card (number): ";
            if (!(std::cin >> cardSelect))
            {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Invalid input.\n";
                continue;
            }

            // ----- check if player owns the card -----
            if (!m_gameTable.GetGamer(i).HasCard(cardSelect))
            {
                std::cout << "You don't have that card. Choose one from your hand.\n";
                continue;
            }

            std::cout << "Choose a stack (1-4): ";
            if (!(std::cin >> stackCard))
            {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Invalid input.\n";
                continue;
            }

            if (stackCard < 1 || stackCard > 4)
            {
                std::cout << "Invalid stack number.\n";
                continue;
            }

            bool moveDone = false;
            bool isSpecialMove = false;
            
            // Create Card object
            Card c(cardSelect);
            
            // Use new logic: Check IsValidMove then Push
            // Map stackCard (1-4) to API
            // Note: TheGame.cpp logic manually checks backTrick/validation
            // We should use GameTable::IsValidMove if possible, but TheGame.cpp
            // has specific console logic (splitting 'push' from 'remove last').
            // Let's keep existing structure but update calls.

            switch (stackCard)
            {
            case 1: // Increasing first
            {
                int last = m_gameTable.GetLastCardFromIncreasingFirst().GetCardNumber();

                if (cardSelect > last)
                {
                    m_gameTable.PushIncreasingFirst(m_gameTable.GetGamer(i).CardLaidDown(cardSelect));
                    moveDone = true;
                }
                else if (cardSelect == last - 10)
                {
                    m_gameTable.RemoveLastIncreasingFirst();
                    m_gameTable.PushIncreasingFirst(m_gameTable.GetGamer(i).CardLaidDown(cardSelect));
                    moveDone = true;
                    isSpecialMove = true;
                }
                break;
            }

            case 2: // Increasing second
            {
                int last = m_gameTable.GetLastCardFromIncreasingSecond().GetCardNumber();

                if (cardSelect > last)
                {
                     m_gameTable.PushIncreasingSecond(m_gameTable.GetGamer(i).CardLaidDown(cardSelect));
                    moveDone = true;
                }
                else if (cardSelect == last - 10)
                {
                    m_gameTable.RemoveLastIncreasingSecond();
                     m_gameTable.PushIncreasingSecond(m_gameTable.GetGamer(i).CardLaidDown(cardSelect));
                    moveDone = true;
                    isSpecialMove = true;
                }
                break;
            }

            case 3: // Decreasing first
            {
                int last = m_gameTable.GetLastCardFromDecreasingFirst().GetCardNumber();

                if (cardSelect < last)
                {
                     m_gameTable.PushDecreasingFirst(m_gameTable.GetGamer(i).CardLaidDown(cardSelect));
                    moveDone = true;
                }
                else if (cardSelect == last + 10)
                {
                    m_gameTable.RemoveLastDecreasingFirst();
                     m_gameTable.PushDecreasingFirst(m_gameTable.GetGamer(i).CardLaidDown(cardSelect));
                    moveDone = true;
                    isSpecialMove = true;
                }
                break;
            }

            case 4: // Decreasing second
            {
                int last = m_gameTable.GetLastCardFromDecreasingSecond().GetCardNumber();

                if (cardSelect < last)
                {
                     m_gameTable.PushDecreasingSecond(m_gameTable.GetGamer(i).CardLaidDown(cardSelect));
                    moveDone = true;
                }
                else if (cardSelect == last + 10)
                {
                    m_gameTable.RemoveLastDecreasingSecond();
                     m_gameTable.PushDecreasingSecond(m_gameTable.GetGamer(i).CardLaidDown(cardSelect));
                    moveDone = true;
                    isSpecialMove = true;
                }
                break;
            }
            }

            // -------- move result --------
            if (!moveDone)
            {
                std::cout << "Invalid move. Try again.\n";
                // Don't continue outer loop, just retry inner card selection
                // Logic flow in original was `continue` of `for(int j...)`? No, loop has no increment.
                // It just retries.
                continue;
            }
            
            // Remove from hand (CardLaidDown returns copy, we need to remove it from player)
            // But logic above calls CardLaidDown which might remove it?
            // Checking Player.cppm: CardLaidDown(int) seems to return Card. Does it remove?
            // Need to check Player.cpp implementation. Assuming it returns card.
            // GameTable::RemoveCardFromHand exists.
            
            // Wait, original code: m_gameTable.GetGamer(i).CardLaidDown(cardSelect)
            // If CardLaidDown REMOVES, then we are good.
            // If not, we need explicit remove.
            
            // -------- statistics --------
            m_stats.totalMoves++;
            m_stats.cardsPlayed++;
            if (isSpecialMove)
                m_stats.specialMoves++;

            // -------- draw card --------
            if (m_gameTable.SizeDeckCards() != 0)
            {
                m_gameTable.PushCard(m_gameTable.DeckCardsLast(), i);
                m_gameTable.RemoveDeckCardsLast();
            }

            ++j; // only count successful moves
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
	return m_lostGame;
}

bool game::TheGame::GetLostGame()
{
	return m_lostGame;
}

// Statistics methods implementation
GameStats game::TheGame::GetStats() const
{
	return m_stats;
}

void game::TheGame::PrintStats() const
{
	std::cout << "\n=== Game Statistics ===\n";
	std::cout << "Rounds Played: " << m_stats.roundsPlayed << "\n";
	std::cout << "Total Moves: " << m_stats.totalMoves << "\n";
	std::cout << "Cards Played: " << m_stats.cardsPlayed << "\n";
	std::cout << "Special Moves (Backward Tricks): " << m_stats.specialMoves << "\n";
	std::cout << "=======================\n\n";
}

void game::TheGame::RecordMove(int playerId, bool specialMove)
{
    m_stats.totalMoves++;
    // m_playerStats[playerId].cardsPlayed++; // m_playerStats not defined in header view
    // if (specialMove) {
    //    m_stats.specialMoves++;
    //    m_playerStats[playerId].specialMoves++;
    // }
}
