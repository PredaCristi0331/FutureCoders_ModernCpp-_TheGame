using Microsoft.Win32;

namespace HangmanWpf.Services;

public class DialogService : IDialogService
{
    public string? OpenImageFileDialog()
    {
        var dlg = new OpenFileDialog
        {
            Title = "Select Avatar Image",
            Filter = "Image Files (*.jpg;*.jpeg;*.png;*.gif)|*.jpg;*.jpeg;*.png;*.gif",
            Multiselect = false
        };
        return dlg.ShowDialog() == true ? dlg.FileName : null;
    }
}
