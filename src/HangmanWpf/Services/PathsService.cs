namespace HangmanWpf.Services;

public class PathsService : IPathsService
{
    private readonly string _dataFolder;

    public PathsService()
    {
        // Data folder lives next to the executable (or in the project root during development)
        var exe = AppDomain.CurrentDomain.BaseDirectory;
        _dataFolder = Path.Combine(exe, "Data");
        Directory.CreateDirectory(_dataFolder);

        var savedFolder = Path.Combine(_dataFolder, "Saves");
        Directory.CreateDirectory(savedFolder);
    }

    public string UsersFilePath => Path.Combine(_dataFolder, "users.json");
    public string StatisticsFilePath => Path.Combine(_dataFolder, "statistics.json");
    public string WordsFilePath => Path.Combine(_dataFolder, "words.json");
    public string SavedGamesFilePath => Path.Combine(_dataFolder, "Saves", "savedgames.json");
    public string AssetsFolder => Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Assets");
    public string AvatarsFolder => Path.Combine(AssetsFolder, "Avatars");
    public string HangmanPartsFolder => Path.Combine(AssetsFolder, "HangmanParts");
}
