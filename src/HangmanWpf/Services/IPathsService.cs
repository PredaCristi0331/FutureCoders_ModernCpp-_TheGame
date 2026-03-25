namespace HangmanWpf.Services;

public interface IPathsService
{
    string UsersFilePath { get; }
    string StatisticsFilePath { get; }
    string WordsFilePath { get; }
    string SavedGamesFilePath { get; }
    string AssetsFolder { get; }
    string AvatarsFolder { get; }
    string HangmanPartsFolder { get; }
}
