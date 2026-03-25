using HangmanWpf.Models;

namespace HangmanWpf.Services;

public interface IGameSaveService
{
    List<SavedGame> LoadSavedGames(string userName);
    void SaveGame(SavedGame game);
    void DeleteSavedGamesForUser(string userName);
}
