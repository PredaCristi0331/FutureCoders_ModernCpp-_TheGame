using System.Windows;
using HangmanWpf.ViewModels;

namespace HangmanWpf.Views;

public partial class StatisticsWindow : Window
{
    public StatisticsWindow(StatisticsViewModel vm)
    {
        InitializeComponent();
        DataContext = vm;
    }

    private void Close_Click(object sender, RoutedEventArgs e) => Close();
}
