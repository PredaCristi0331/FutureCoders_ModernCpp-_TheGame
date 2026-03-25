using System.Collections.ObjectModel;
using System.Windows.Input;
using System.Windows.Threading;
using HangmanWpf.Models;
using HangmanWpf.Services;
using HangmanWpf.ViewModels.Base;

namespace HangmanWpf.ViewModels;

public class GameViewModel : ViewModelBase
{
    private const int MaxMistakes = 6;
    private const int RoundSeconds = 30;
    private const int WinsToWin = 3;

    private readonly IWordProvider _wordProvider;
    private readonly IGameSaveService _gameSaveService;
    private readonly IStatisticsService _statisticsService;
    private readonly IPathsService _paths;

    private readonly DispatcherTimer _timer;

    private UserProfile _user;
    private string _currentWord = string.Empty;
    private string _maskedWord = string.Empty;
    private int _mistakes;
    private int _consecutiveWins;
    private int _timeRemaining;
    private string _selectedCategory = "All";
    private GameState _gameState = GameState.Idle;
    private string _statusMessage = string.Empty;
    private string _hangmanImagePath = string.Empty;
    private string? _currentSaveId;

    public ObservableCollection<LetterKey> Letters { get; } = new();
    public ObservableCollection<string> Categories { get; } = new();
    public ObservableCollection<SavedGame> SavedGames { get; } = new();

    #region Properties

    public UserProfile User
    {
        get => _user;
        set => SetProperty(ref _user, value);
    }

    public string MaskedWord
    {
        get => _maskedWord;
        set => SetProperty(ref _maskedWord, value);
    }

    public int Mistakes
    {
        get => _mistakes;
        set
        {
            SetProperty(ref _mistakes, value);
            UpdateHangmanImage();
        }
    }

    public int ConsecutiveWins
    {
        get => _consecutiveWins;
        set => SetProperty(ref _consecutiveWins, value);
    }

    public int TimeRemaining
    {
        get => _timeRemaining;
        set => SetProperty(ref _timeRemaining, value);
    }

    public string SelectedCategory
    {
        get => _selectedCategory;
        set => SetProperty(ref _selectedCategory, value);
    }

    public GameState GameState
    {
        get => _gameState;
        set
        {
            SetProperty(ref _gameState, value);
            OnPropertyChanged(nameof(IsPlaying));
        }
    }

    public bool IsPlaying => _gameState == GameState.Playing;

    public string StatusMessage
    {
        get => _statusMessage;
        set => SetProperty(ref _statusMessage, value);
    }

    public string HangmanImagePath
    {
        get => _hangmanImagePath;
        set => SetProperty(ref _hangmanImagePath, value);
    }

    public string UserAvatarPath
    {
        get
        {
            var img = _user?.ImagePath ?? string.Empty;
            if (string.IsNullOrWhiteSpace(img))
                img = System.IO.Path.Combine(_paths.AvatarsFolder, "default.jpg");
            else if (!System.IO.Path.IsPathRooted(img))
                img = System.IO.Path.Combine(AppDomain.CurrentDomain.BaseDirectory, img);
            return img;
        }
    }

    #endregion

    #region Commands

    public ICommand NewGameCommand { get; }
    public ICommand SaveGameCommand { get; }
    public ICommand OpenGameCommand { get; }
    public ICommand GuessLetterCommand { get; }
    public ICommand SelectCategoryCommand { get; }
    public ICommand CancelCommand { get; }

    public event Action? CancelRequested;
    public event Action? StatisticsRequested;
    public event Action? AboutRequested;
    public event Func<List<SavedGame>, SavedGame?>? OpenGameDialogRequested;

    #endregion

    public GameViewModel(
        UserProfile user,
        IWordProvider wordProvider,
        IGameSaveService gameSaveService,
        IStatisticsService statisticsService,
        IPathsService paths)
    {
        _user = user;
        _wordProvider = wordProvider;
        _gameSaveService = gameSaveService;
        _statisticsService = statisticsService;
        _paths = paths;

        NewGameCommand = new RelayCommand(StartNewGame);
        SaveGameCommand = new RelayCommand(SaveGame, () => GameState == GameState.Playing);
        OpenGameCommand = new RelayCommand(OpenGame);
        GuessLetterCommand = new RelayCommand<object>(param =>
        {
            if (param is char ch) GuessLetter(ch);
        });
        SelectCategoryCommand = new RelayCommand<object>(param =>
        {
            if (param is string cat) SelectCategory(cat);
        });
        CancelCommand = new RelayCommand(OnCancel);

        _timer = new DispatcherTimer { Interval = TimeSpan.FromSeconds(1) };
        _timer.Tick += OnTimerTick;

        LoadCategories();
        UpdateHangmanImage();
    }

    private void LoadCategories()
    {
        Categories.Clear();
        Categories.Add("All");
        foreach (var name in _wordProvider.GetCategoryNames())
            Categories.Add(name);
    }

    public void StartNewGame()
    {
        _timer.Stop();
        _currentSaveId = null;
        ConsecutiveWins = 0;
        StartRound();
    }

    private void StartRound()
    {
        _currentWord = _wordProvider.GetRandomWord(_selectedCategory);
        Mistakes = 0;
        TimeRemaining = RoundSeconds;
        GameState = GameState.Playing;
        StatusMessage = string.Empty;
        UpdateMaskedWord();
        ResetLetters();
        _timer.Start();
    }

    private void UpdateMaskedWord()
    {
        var guessed = Letters.Where(l => !l.IsEnabled).Select(l => l.Letter).ToHashSet();
        MaskedWord = string.Join(" ", _currentWord.Select(c =>
            c == ' ' ? ' ' : (guessed.Contains(c) ? c : '_')));
    }

    private void ResetLetters()
    {
        Letters.Clear();
        for (char c = 'A'; c <= 'Z'; c++)
            Letters.Add(new LetterKey { Letter = c, IsEnabled = true });
    }

    private void GuessLetter(char letter)
    {
        if (GameState != GameState.Playing) return;
        var ch = char.ToUpper(letter);
        var key = Letters.FirstOrDefault(l => l.Letter == ch);
        if (key == null || !key.IsEnabled) return;

        key.IsEnabled = false;

        if (!_currentWord.Contains(ch))
            Mistakes++;

        UpdateMaskedWord();
        CheckWinLose();
    }

    private void CheckWinLose()
    {
        // Won current word?
        if (!MaskedWord.Contains('_'))
        {
            ConsecutiveWins++;
            if (ConsecutiveWins >= WinsToWin)
            {
                _timer.Stop();
                GameState = GameState.Won;
                StatusMessage = $"🎉 You won! Congratulations, {_user.Name}!";
                _statisticsService.RecordGame(_user.Name, _selectedCategory, true);
                ConsecutiveWins = 0;
            }
            else
            {
                StatusMessage = $"✅ Word guessed! Level {ConsecutiveWins}/{WinsToWin}. Next round...";
                _timer.Stop();
                Task.Delay(1500).ContinueWith(_ =>
                    System.Windows.Application.Current?.Dispatcher.Invoke(StartRound));
            }
            return;
        }

        if (Mistakes >= MaxMistakes)
        {
            _timer.Stop();
            GameState = GameState.Lost;
            StatusMessage = $"💀 Game over! The word was: {_currentWord}";
            _statisticsService.RecordGame(_user.Name, _selectedCategory, false);
            ConsecutiveWins = 0;
        }
    }

    private void OnTimerTick(object? sender, EventArgs e)
    {
        TimeRemaining--;
        if (TimeRemaining <= 0)
        {
            _timer.Stop();
            GameState = GameState.Lost;
            StatusMessage = $"⏰ Time's up! The word was: {_currentWord}";
            _statisticsService.RecordGame(_user.Name, _selectedCategory, false);
            ConsecutiveWins = 0;
        }
    }

    private void UpdateHangmanImage()
    {
        HangmanImagePath = System.IO.Path.Combine(_paths.HangmanPartsFolder, $"{_mistakes}.png");
    }

    private void SaveGame()
    {
        var save = new SavedGame
        {
            Id = _currentSaveId ?? Guid.NewGuid().ToString(),
            UserName = _user.Name,
            Category = _selectedCategory,
            Word = _currentWord,
            GuessedLetters = Letters.Where(l => !l.IsEnabled).Select(l => l.Letter).ToList(),
            Mistakes = _mistakes,
            ConsecutiveWins = _consecutiveWins,
            TimeRemainingSeconds = _timeRemaining,
            SavedAt = DateTime.Now,
            DisplayName = $"{_selectedCategory} – {DateTime.Now.ToString("g", System.Globalization.CultureInfo.InvariantCulture)}"
        };
        _currentSaveId = save.Id;
        _gameSaveService.SaveGame(save);
        StatusMessage = "Game saved!";
    }

    private void OpenGame()
    {
        _timer.Stop();
        var saved = _gameSaveService.LoadSavedGames(_user.Name);
        if (saved.Count == 0)
        {
            StatusMessage = "No saved games found.";
            return;
        }

        var chosen = OpenGameDialogRequested?.Invoke(saved);
        if (chosen == null) return;

        RestoreFromSave(chosen);
    }

    public void RestoreFromSave(SavedGame save)
    {
        _timer.Stop();
        _currentSaveId = save.Id;
        _currentWord = save.Word;
        SelectedCategory = save.Category;
        ConsecutiveWins = save.ConsecutiveWins;
        TimeRemaining = save.TimeRemainingSeconds;
        Mistakes = save.Mistakes;
        GameState = GameState.Playing;
        StatusMessage = "Game loaded!";

        ResetLetters();
        foreach (var ch in save.GuessedLetters)
        {
            var key = Letters.FirstOrDefault(l => l.Letter == ch);
            if (key != null) key.IsEnabled = false;
        }

        UpdateMaskedWord();
        UpdateHangmanImage();
        _timer.Start();
    }

    private void SelectCategory(string cat)
    {
        SelectedCategory = cat;
        ConsecutiveWins = 0;
        StartRound();
    }

    private void OnCancel()
    {
        _timer.Stop();
        GameState = GameState.Idle;
        CancelRequested?.Invoke();
    }

    public void RequestStatistics() => StatisticsRequested?.Invoke();
    public void RequestAbout() => AboutRequested?.Invoke();

    public void Cleanup() => _timer.Stop();
}
