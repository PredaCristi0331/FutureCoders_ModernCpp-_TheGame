using System.ComponentModel;

namespace HangmanWpf.Models;

public class LetterKey : INotifyPropertyChanged
{
    private bool _isEnabled = true;

    public char Letter { get; set; }

    public bool IsEnabled
    {
        get => _isEnabled;
        set
        {
            _isEnabled = value;
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(IsEnabled)));
        }
    }

    public event PropertyChangedEventHandler? PropertyChanged;
}
