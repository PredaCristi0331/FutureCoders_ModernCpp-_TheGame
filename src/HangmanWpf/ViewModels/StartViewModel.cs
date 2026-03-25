using System.Collections.ObjectModel;
using System.Windows.Input;
using HangmanWpf.Models;
using HangmanWpf.Services;
using HangmanWpf.ViewModels.Base;

namespace HangmanWpf.ViewModels;

public class StartViewModel : ViewModelBase
{
    private readonly IDataStore _dataStore;
    private readonly IDialogService _dialogService;
    private readonly IGameSaveService _gameSaveService;
    private readonly IStatisticsService _statisticsService;
    private readonly IPathsService _paths;

    private UserProfile? _selectedUser;
    private string _newUserName = string.Empty;
    private string _newUserImagePath = string.Empty;

    public ObservableCollection<UserProfile> Users { get; } = new();

    public UserProfile? SelectedUser
    {
        get => _selectedUser;
        set
        {
            SetProperty(ref _selectedUser, value);
            OnPropertyChanged(nameof(IsUserSelected));
        }
    }

    public bool IsUserSelected => _selectedUser != null;

    public string NewUserName
    {
        get => _newUserName;
        set => SetProperty(ref _newUserName, value);
    }

    public string NewUserImagePath
    {
        get => _newUserImagePath;
        set => SetProperty(ref _newUserImagePath, value);
    }

    public ICommand PlayCommand { get; }
    public ICommand DeleteUserCommand { get; }
    public ICommand CreateUserCommand { get; }
    public ICommand BrowseImageCommand { get; }

    public event Action<UserProfile>? PlayRequested;

    public StartViewModel(
        IDataStore dataStore,
        IDialogService dialogService,
        IGameSaveService gameSaveService,
        IStatisticsService statisticsService,
        IPathsService paths)
    {
        _dataStore = dataStore;
        _dialogService = dialogService;
        _gameSaveService = gameSaveService;
        _statisticsService = statisticsService;
        _paths = paths;

        PlayCommand = new RelayCommand(OnPlay, () => IsUserSelected);
        DeleteUserCommand = new RelayCommand(OnDeleteUser, () => IsUserSelected);
        CreateUserCommand = new RelayCommand(OnCreateUser, () => !string.IsNullOrWhiteSpace(NewUserName));
        BrowseImageCommand = new RelayCommand(OnBrowseImage);

        LoadUsers();
    }

    private void LoadUsers()
    {
        Users.Clear();
        foreach (var u in _dataStore.LoadUsers())
            Users.Add(u);
    }

    private void OnPlay()
    {
        if (_selectedUser != null)
            PlayRequested?.Invoke(_selectedUser);
    }

    private void OnDeleteUser()
    {
        if (_selectedUser == null) return;
        var users = _dataStore.LoadUsers();
        users.RemoveAll(u => u.Name == _selectedUser.Name);
        _dataStore.SaveUsers(users);
        _gameSaveService.DeleteSavedGamesForUser(_selectedUser.Name);
        _statisticsService.DeleteForUser(_selectedUser.Name);
        SelectedUser = null;
        LoadUsers();
    }

    private void OnCreateUser()
    {
        var name = NewUserName.Trim();
        if (string.IsNullOrWhiteSpace(name)) return;

        var users = _dataStore.LoadUsers();
        if (users.Any(u => u.Name.Equals(name, StringComparison.OrdinalIgnoreCase)))
            return;

        // Make image path relative if possible
        var imgPath = NewUserImagePath;
        if (!string.IsNullOrWhiteSpace(imgPath))
        {
            var baseDir = AppDomain.CurrentDomain.BaseDirectory;
            try
            {
                imgPath = Path.GetRelativePath(baseDir, imgPath);
            }
            catch
            {
                // If GetRelativePath fails (e.g. different drive), keep the original path
            }
        }

        var user = new UserProfile { Name = name, ImagePath = imgPath };
        users.Add(user);
        _dataStore.SaveUsers(users);
        NewUserName = string.Empty;
        NewUserImagePath = string.Empty;
        LoadUsers();
    }

    private void OnBrowseImage()
    {
        var path = _dialogService.OpenImageFileDialog();
        if (path != null)
            NewUserImagePath = path;
    }
}
