using System.Text.Json;
using HangmanWpf.Models;

namespace HangmanWpf.Services;

public class WordProvider : IWordProvider
{
    private readonly IPathsService _paths;
    private List<Category>? _cache;
    private static readonly Random _rng = Random.Shared;

    public WordProvider(IPathsService paths)
    {
        _paths = paths;
    }

    public List<Category> LoadCategories()
    {
        if (_cache != null) return _cache;
        try
        {
            var json = File.ReadAllText(_paths.WordsFilePath);
            _cache = JsonSerializer.Deserialize<List<Category>>(json,
                new JsonSerializerOptions { PropertyNameCaseInsensitive = true }) ?? new();
        }
        catch
        {
            _cache = new();
        }
        return _cache;
    }

    public List<string> GetCategoryNames()
        => LoadCategories().Select(c => c.Name).ToList();

    public string GetRandomWord(string category)
    {
        var cats = LoadCategories();
        IEnumerable<string> pool;
        if (string.IsNullOrEmpty(category) || category == "All")
            pool = cats.SelectMany(c => c.Words);
        else
            pool = cats.FirstOrDefault(c => c.Name == category)?.Words ?? Enumerable.Empty<string>();

        var list = pool.ToList();
        if (list.Count == 0) return "HANGMAN";
        return list[_rng.Next(list.Count)].ToUpper();
    }
}
