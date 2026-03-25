using System.Windows;
using System.Windows.Input;
using HangmanWpf.Models;
using HangmanWpf.Services;
using HangmanWpf.ViewModels;

namespace HangmanWpf.Views;

public partial class GameWindow : Window
{
    private readonly GameViewModel _vm;

    public GameWindow(GameViewModel vm)
    {
        InitializeComponent();
        _vm = vm;
        DataContext = vm;

        _vm.StatisticsRequested += OnStatisticsRequested;
        _vm.AboutRequested += OnAboutRequested;
        _vm.OpenGameDialogRequested += OnOpenGameDialogRequested;

        Closed += (_, _) => _vm.Cleanup();

        // Wire keyboard letter shortcuts
        KeyDown += OnKeyDown;
    }

    private void OnKeyDown(object sender, KeyEventArgs e)
    {
        if (_vm.GameState != Models.GameState.Playing) return;
        var keyStr = e.Key.ToString();
        if (keyStr.Length == 1 && char.IsLetter(keyStr[0]))
        {
            _vm.GuessLetterCommand.Execute((object)char.ToUpper(keyStr[0]));
        }
    }

    private void Statistics_Click(object sender, RoutedEventArgs e)
        => _vm.RequestStatistics();

    private void About_Click(object sender, RoutedEventArgs e)
        => _vm.RequestAbout();

    private void OnStatisticsRequested()
    {
        var statsVm = new StatisticsViewModel(new StatisticsService(new PathsService()));
        var statsWin = new StatisticsWindow(statsVm);
        statsWin.Owner = this;
        statsWin.ShowDialog();
    }

    private void OnAboutRequested()
    {
        var aboutWin = new AboutWindow(new AboutViewModel());
        aboutWin.Owner = this;
        aboutWin.ShowDialog();
    }

    private SavedGame? OnOpenGameDialogRequested(List<SavedGame> savedGames)
    {
        var dlg = new OpenGameDialog(savedGames);
        dlg.Owner = this;
        if (dlg.ShowDialog() == true)
            return dlg.SelectedGame;
        return null;
    }
}
