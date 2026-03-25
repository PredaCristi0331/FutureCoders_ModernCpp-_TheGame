using System.Collections.ObjectModel;
using HangmanWpf.Models;
using HangmanWpf.Services;
using HangmanWpf.ViewModels.Base;

namespace HangmanWpf.ViewModels;

public class StatisticsViewModel : ViewModelBase
{
    public ObservableCollection<StatisticsEntry> Entries { get; } = new();

    public StatisticsViewModel(IStatisticsService statisticsService)
    {
        foreach (var e in statisticsService.LoadAll())
            Entries.Add(e);
    }
}
