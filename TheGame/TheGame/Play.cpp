//#include "Play.h"
//#include "TheGame.h"

module Play;
export import TheGame;
using namespace game;

Play::Play()
{
	TheGame game;
	game.StartTheGame();
	game.Rund();
	if(game.GetLostGame()==false)
		game.FinalGame(false);
}

