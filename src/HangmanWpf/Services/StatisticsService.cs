using System.Text.Json;
using HangmanWpf.Models;

namespace HangmanWpf.Services;

public class StatisticsService : IStatisticsService
{
    private readonly IPathsService _paths;
    private static readonly JsonSerializerOptions _opts = new() { WriteIndented = true };

    public StatisticsService(IPathsService paths)
    {
        _paths = paths;
        if (!File.Exists(_paths.StatisticsFilePath))
            File.WriteAllText(_paths.StatisticsFilePath, "[]");
    }

    public List<StatisticsEntry> LoadAll()
    {
        try
        {
            var json = File.ReadAllText(_paths.StatisticsFilePath);
            return JsonSerializer.Deserialize<List<StatisticsEntry>>(json) ?? new();
        }
        catch { return new(); }
    }

    private void SaveAll(List<StatisticsEntry> entries)
    {
        File.WriteAllText(_paths.StatisticsFilePath, JsonSerializer.Serialize(entries, _opts));
    }

    public StatisticsEntry GetOrCreate(string userName, string category)
    {
        var all = LoadAll();
        var entry = all.FirstOrDefault(e => e.UserName == userName && e.Category == category);
        if (entry == null)
        {
            entry = new StatisticsEntry { UserName = userName, Category = category };
            all.Add(entry);
            SaveAll(all);
        }
        return entry;
    }

    public void RecordGame(string userName, string category, bool won)
    {
        var all = LoadAll();
        var entry = all.FirstOrDefault(e => e.UserName == userName && e.Category == category);
        if (entry == null)
        {
            entry = new StatisticsEntry { UserName = userName, Category = category };
            all.Add(entry);
        }
        entry.GamesPlayed++;
        if (won) entry.GamesWon++;
        SaveAll(all);
    }

    public void DeleteForUser(string userName)
    {
        var all = LoadAll().Where(e => e.UserName != userName).ToList();
        SaveAll(all);
    }
}
