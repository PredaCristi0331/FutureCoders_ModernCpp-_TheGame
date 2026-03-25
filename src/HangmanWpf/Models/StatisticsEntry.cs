namespace HangmanWpf.Models;

public class StatisticsEntry
{
    public string UserName { get; set; } = string.Empty;
    public string Category { get; set; } = string.Empty;
    public int GamesPlayed { get; set; }
    public int GamesWon { get; set; }
}
