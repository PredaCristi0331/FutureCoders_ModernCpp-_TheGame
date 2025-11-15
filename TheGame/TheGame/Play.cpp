#include "Play.h"
#include "TheGame.h"

Play::Play()
{
	TheGame game;
	game.StartTheGame();
	game.Rund();
	if(game.GetLostGame()==false)
		game.FinalGame(false);
}

