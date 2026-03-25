using System.Text.Json;
using HangmanWpf.Models;

namespace HangmanWpf.Services;

public class JsonDataStore : IDataStore
{
    private readonly IPathsService _paths;
    private static readonly JsonSerializerOptions _opts = new() { WriteIndented = true };

    public JsonDataStore(IPathsService paths)
    {
        _paths = paths;
        EnsureFiles();
    }

    private void EnsureFiles()
    {
        if (!File.Exists(_paths.UsersFilePath))
            File.WriteAllText(_paths.UsersFilePath, "[]");
        if (!File.Exists(_paths.StatisticsFilePath))
            File.WriteAllText(_paths.StatisticsFilePath, "[]");
    }

    public List<UserProfile> LoadUsers()
    {
        try
        {
            var json = File.ReadAllText(_paths.UsersFilePath);
            return JsonSerializer.Deserialize<List<UserProfile>>(json) ?? new();
        }
        catch
        {
            return new();
        }
    }

    public void SaveUsers(List<UserProfile> users)
    {
        var json = JsonSerializer.Serialize(users, _opts);
        File.WriteAllText(_paths.UsersFilePath, json);
    }
}
