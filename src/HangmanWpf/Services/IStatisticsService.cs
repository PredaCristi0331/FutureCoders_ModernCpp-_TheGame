using HangmanWpf.Models;

namespace HangmanWpf.Services;

public interface IStatisticsService
{
    List<StatisticsEntry> LoadAll();
    StatisticsEntry GetOrCreate(string userName, string category);
    void RecordGame(string userName, string category, bool won);
    void DeleteForUser(string userName);
}
