#include "Filesystem.hpp"


#if defined(WIN32) || defined(_WIN32)
    #include <windows.h>
    #include <initguid.h>
    #include <KnownFolders.h>
    #include <ShlObj.h>
    #include <wchar.h>
#elif __APPLE__
    //Include needed header files
#elif __linux__ || __unix__
    #include <unistd.h>
    #include <sys/types.h>
    #include <pwd.h>
#endif


std::filesystem::path Grog::Filesystem::GetUserDirectoryPath() {
#if defined(WIN32) || defined(_WIN32)
    std::filesystem::path path;
    PWSTR ppszPath = nullptr;
    if (SUCCEEDED(SHGetKnownFolderPath(&FOLDERID_Profile, 0, NULL, &ppszPath)))
        path = std::filesystem::path{ ppszPath };
    CoTaskMemFree(ppszPath);
    return path;
#elif __APPLE__
    //TODO: implementation
#elif __linux__ || __unix__
    const char* homedir = getenv("HOME");
    if (homedir == nullptr)
        homedir = getpwuid(getuid())->pw_dir;
    return std::filesystem::path{ homedir };
#endif
}

std::filesystem::path Grog::Filesystem::GetUserDocumentsDirectoryPath() {
#if defined(WIN32) || defined(_WIN32)
    std::filesystem::path path;
    PWSTR ppszPath = nullptr;
    if (SUCCEEDED(SHGetKnownFolderPath(&FOLDERID_Documents, 0, NULL, &ppszPath)))
        path = std::filesystem::path{ ppszPath };
    CoTaskMemFree(ppszPath);
    return path;
#elif __APPLE__
    //TODO: implementation
#elif __linux__ || __unix__
    return GetUserDirectoryPath() / "Documents";
#endif
}

std::filesystem::path Grog::Filesystem::GetUserConfigDirectoryPath() {
#if defined(WIN32) || defined(_WIN32)
    std::filesystem::path path;
    PWSTR ppszPath = nullptr;
    if (SUCCEEDED(SHGetKnownFolderPath(&FOLDERID_LocalAppData, 0, NULL, &ppszPath)))
        path = std::filesystem::path{ ppszPath };
    CoTaskMemFree(ppszPath);
    return path;
#elif __APPLE__
    //TODO: implementation
#elif __linux__ || __unix__
    const char* dir = getenv("XDG_CONFIG_HOME");
    if (dir == nullptr)
        return GetUserDirectoryPath() / ".config";
    return std::filesystem::path{ dir };
#endif
}

std::filesystem::path Grog::Filesystem::GetSystemConfigDirectoryPath() {
#if defined(WIN32) || defined(_WIN32)
    std::filesystem::path path;
    PWSTR ppszPath = nullptr;
    if (SUCCEEDED(SHGetKnownFolderPath(&FOLDERID_ProgramData, 0, NULL, &ppszPath)))
        path = std::filesystem::path{ ppszPath };
    CoTaskMemFree(ppszPath);
    return path;
#elif __APPLE__
    //TODO: implementation
#elif __linux__ || __unix__
    return std::filesystem::path{ "/etc" };
#endif
}

std::filesystem::path Grog::Filesystem::GetUserDataDirectoryPath() {
#if defined(WIN32) || defined(_WIN32)
    std::filesystem::path path;
    PWSTR ppszPath = nullptr;
    if (SUCCEEDED(SHGetKnownFolderPath(&FOLDERID_LocalAppData, 0, NULL, &ppszPath)))
        path = std::filesystem::path{ ppszPath };
    CoTaskMemFree(ppszPath);
    return path;
#elif __APPLE__
    //TODO: implementation
#elif __linux__ || __unix__
    const char* dir = getenv("XDG_DATA_HOME");
    if (dir == nullptr)
        return GetUserDirectoryPath() / ".local/share";
    return std::filesystem::path{ dir };
#endif
}

std::filesystem::path Grog::Filesystem::GetSystemDataDirectoryPath() {
#if defined(WIN32) || defined(_WIN32)
    std::filesystem::path path;
    PWSTR ppszPath = nullptr;
    if (SUCCEEDED(SHGetKnownFolderPath(&FOLDERID_ProgramData, 0, NULL, &ppszPath)))
        path = std::filesystem::path{ ppszPath };
    CoTaskMemFree(ppszPath);
    return path;
#elif __APPLE__
    //TODO: implementation
#elif __linux__ || __unix__
    return std::filesystem::path{ "/etc" };
#endif
}

std::filesystem::path Grog::Filesystem::GetTempDirectoryPath() {
    return std::filesystem::temp_directory_path();
}