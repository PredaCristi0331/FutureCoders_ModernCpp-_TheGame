using HangmanWpf.Models;

namespace HangmanWpf.Services;

public interface IDataStore
{
    List<UserProfile> LoadUsers();
    void SaveUsers(List<UserProfile> users);
}
