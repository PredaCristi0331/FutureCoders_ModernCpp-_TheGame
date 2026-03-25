namespace HangmanWpf.Models;

public class SavedGame
{
    public string Id { get; set; } = Guid.NewGuid().ToString();
    public string UserName { get; set; } = string.Empty;
    public string Category { get; set; } = string.Empty;
    public string Word { get; set; } = string.Empty;
    public List<char> GuessedLetters { get; set; } = new();
    public int Mistakes { get; set; }
    public int ConsecutiveWins { get; set; }
    public int TimeRemainingSeconds { get; set; }
    public DateTime SavedAt { get; set; } = DateTime.Now;
    public string DisplayName { get; set; } = string.Empty;
}
