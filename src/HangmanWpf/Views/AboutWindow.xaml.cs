using System.Windows;
using HangmanWpf.ViewModels;

namespace HangmanWpf.Views;

public partial class AboutWindow : Window
{
    public AboutWindow(AboutViewModel vm)
    {
        InitializeComponent();
        DataContext = vm;
    }

    private void Ok_Click(object sender, RoutedEventArgs e) => Close();
}
