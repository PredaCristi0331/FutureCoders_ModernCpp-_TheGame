using System.Text.Json;
using HangmanWpf.Models;

namespace HangmanWpf.Services;

public class GameSaveService : IGameSaveService
{
    private readonly IPathsService _paths;
    private static readonly JsonSerializerOptions _opts = new() { WriteIndented = true };

    public GameSaveService(IPathsService paths)
    {
        _paths = paths;
        var dir = Path.GetDirectoryName(_paths.SavedGamesFilePath)!;
        Directory.CreateDirectory(dir);
        if (!File.Exists(_paths.SavedGamesFilePath))
            File.WriteAllText(_paths.SavedGamesFilePath, "[]");
    }

    private List<SavedGame> LoadAll()
    {
        try
        {
            var json = File.ReadAllText(_paths.SavedGamesFilePath);
            return JsonSerializer.Deserialize<List<SavedGame>>(json) ?? new();
        }
        catch { return new(); }
    }

    private void SaveAll(List<SavedGame> games)
    {
        File.WriteAllText(_paths.SavedGamesFilePath, JsonSerializer.Serialize(games, _opts));
    }

    public List<SavedGame> LoadSavedGames(string userName)
        => LoadAll().Where(g => g.UserName == userName).ToList();

    public void SaveGame(SavedGame game)
    {
        var all = LoadAll();
        var existing = all.FirstOrDefault(g => g.Id == game.Id);
        if (existing != null)
            all.Remove(existing);
        all.Add(game);
        SaveAll(all);
    }

    public void DeleteSavedGamesForUser(string userName)
    {
        var all = LoadAll().Where(g => g.UserName != userName).ToList();
        SaveAll(all);
    }
}
