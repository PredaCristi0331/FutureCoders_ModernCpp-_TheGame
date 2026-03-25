using HangmanWpf.Models;

namespace HangmanWpf.Services;

public interface IWordProvider
{
    List<Category> LoadCategories();
    string GetRandomWord(string category);
    List<string> GetCategoryNames();
}
