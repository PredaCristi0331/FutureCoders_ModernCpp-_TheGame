using System.Windows;
using HangmanWpf.Models;
using HangmanWpf.Services;
using HangmanWpf.ViewModels;

namespace HangmanWpf.Views;

public partial class StartWindow : Window
{
    private readonly StartViewModel _vm;
    private readonly IPathsService _paths;
    private readonly IWordProvider _wordProvider;
    private readonly IGameSaveService _gameSaveService;
    private readonly IStatisticsService _statisticsService;

    public StartWindow()
    {
        InitializeComponent();

        _paths = new PathsService();
        var dataStore = new JsonDataStore(_paths);
        var dialogService = new DialogService();
        _gameSaveService = new GameSaveService(_paths);
        _statisticsService = new StatisticsService(_paths);
        _wordProvider = new WordProvider(_paths);

        _vm = new StartViewModel(dataStore, dialogService, _gameSaveService, _statisticsService, _paths);
        _vm.PlayRequested += OnPlayRequested;
        DataContext = _vm;
    }

    private void OnPlayRequested(UserProfile user)
    {
        var gameVm = new GameViewModel(user, _wordProvider, _gameSaveService, _statisticsService, _paths);
        var gameWin = new GameWindow(gameVm);
        gameVm.CancelRequested += () =>
        {
            gameWin.Close();
            Show();
        };
        Hide();
        gameWin.Show();
    }
}
