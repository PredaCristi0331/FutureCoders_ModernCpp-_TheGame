Link video attached:

https://github.com/user-attachments/assets/28aa4a0a-f35b-40f4-832f-e09f5ebedd74

The Game is a cooperative card game in which participants collaborate to place all 98 cards numbered from 2 to 99 in four distinct piles, respecting strict ordering rules: two piles in ascending order and two in descending order. The collective objective is to use all the cards, in which case the team wins and "beats" the game.

Starting from the concept of the game, it is proposed to implement an application that respects the following fundamental rules:

Structure of the deck of cards

There are two categories of cards:

4 foundation cards (two with the value "1" and two with the value "100"), which constitute the foundations of the four piles and cannot be distributed to the players;

98 cards numbered from 2 to 99, which will be handled exclusively by the players.

Game configuration

A game can be started with a minimum of 2 and a maximum of 5 players.

In the setup phase:

The four foundation cards are placed face up in plain sight.

The deck of 98 cards is shuffled, then a number of cards are dealt to each player, depending on the number of participants:

2 players: 8 cards/player

3 players: 7 cards/player

4–5 players: 6 cards/player

The remaining cards form the draw pile, placed face down.

How the game is played

The game is played in turns, so that each player plays once per round. During their turn:

The player must play at least two cards (one after the draw pile is exhausted), placing them on one or more of the four piles.

Cards are placed one by one, following the ordering rules:

Ascending stacks (starting at "1"): each new card must be larger than the previous one.

Descending stacks (starting at "100"): each new card must be smaller than the previous one.

"Backwards Trick" exception

A controlled deviation from the ordering rules is allowed to increase flexibility:

A card exactly 10 units smaller than the current card can be placed on an ascending stack.

A card exactly 10 units larger than the current card can be placed on a descending stack.

After placing the cards, the player draws from the draw pile a number of cards equal to the number of those played, thus restoring the initial size of his hand. After the draw pile is exhausted, each player is required to play at least one card per turn.

Ending conditions

The game ends in two ways:

Failure: A player cannot place the minimum number of cards allowed in his turn.

Victory: All 98 cards have been placed in the piles validly.

Communication rules

Players can communicate via chat, but without revealing the exact values ​​of the cards in their hand. Communication must be indirect, for example:

"Avoid playing on the first pile."

"I have some good options for the second pile."

User profile

The application will include a user profile containing:

the total number of hours played,

a performance score (1–5), calculated on the ratio of games played, games won, and the number of cards remaining in the players' hands at the end of lost games,

other elements considered relevant.

Basic technical requirements

Client–server architecture: the application must allow the running of at least two client instances and one server instance, using the HTTP protocol and the CROW library.

Login/Register page: users can log in or create an account using a unique username (email/password are optional).

Game page: the main application will work in the console, fully respecting the rules of the game.

Database: data will be managed using the SQLite ORM library.

Advanced components 

Multigaming: support for multiple games simultaneously. Players are automatically distributed into games based on score; a user can wait a maximum of 30 seconds, after which the game starts if there are at least two participants.

Graphical interface (GUI): implementation of a customized graphical interface, different from the original version of the game.

Recommended optional elements

Introducing difficulty levels (easy, medium, hard), selectable at the start of the game or dynamically adjusted.

Implementing rigorous exception handling and performing unit tests using dedicated frameworks (minimum 40 tests or ≥50% code coverage).
