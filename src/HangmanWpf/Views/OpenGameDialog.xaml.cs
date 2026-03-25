using System.Collections.Generic;
using System.Windows;
using HangmanWpf.Models;

namespace HangmanWpf.Views;

public partial class OpenGameDialog : Window
{
    public SavedGame? SelectedGame { get; private set; }

    public OpenGameDialog(List<SavedGame> savedGames)
    {
        InitializeComponent();
        SavedGamesList.ItemsSource = savedGames;
    }

    private void Open_Click(object sender, RoutedEventArgs e)
    {
        SelectedGame = SavedGamesList.SelectedItem as SavedGame;
        if (SelectedGame == null) return;
        DialogResult = true;
    }

    private void Cancel_Click(object sender, RoutedEventArgs e)
    {
        DialogResult = false;
    }

    private void SavedGamesList_DoubleClick(object sender, System.Windows.Input.MouseButtonEventArgs e)
    {
        SelectedGame = SavedGamesList.SelectedItem as SavedGame;
        if (SelectedGame != null)
            DialogResult = true;
    }
}
