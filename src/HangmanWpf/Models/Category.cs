namespace HangmanWpf.Models;

public class Category
{
    public string Name { get; set; } = string.Empty;
    public List<string> Words { get; set; } = new();
}
