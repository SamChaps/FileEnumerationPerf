#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <winternl.h>

struct FileInfo
{
    std::wstring name;
    FILETIME creationTime;
    FILETIME lastWriteTime;
    DWORD attributes;
};

std::vector<FileInfo> EnumerateFiles(const std::wstring& directory)
{
    std::vector<FileInfo> files;
    std::stack<std::wstring> directories;
    directories.push(directory);

    while (!directories.empty())
    {
        std::wstring currentDir = directories.top();
        directories.pop();

        WIN32_FIND_DATA findFileData;
        HANDLE hFind = FindFirstFileEx((currentDir + L"\\*").c_str(), FindExInfoBasic, &findFileData, FindExSearchNameMatch, NULL, 0);

        if (hFind == INVALID_HANDLE_VALUE)
        {
            std::wcerr << L"FindFirstFileEx failed (" << GetLastError() << L")\n";
            continue;
        }

        do
        {
            const std::wstring fileOrDir = findFileData.cFileName;
            if (fileOrDir != L"." && fileOrDir != L"..")
            {
                FileInfo fileInfo;
                fileInfo.name = currentDir + L"\\" + fileOrDir;
                fileInfo.creationTime = findFileData.ftCreationTime;
                fileInfo.lastWriteTime = findFileData.ftLastWriteTime;
                fileInfo.attributes = findFileData.dwFileAttributes;
                files.push_back(fileInfo);

                if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    directories.push(fileInfo.name);
                }
            }
        } while (FindNextFile(hFind, &findFileData) != 0);

        if (GetLastError() != ERROR_NO_MORE_FILES)
        {
            std::wcerr << L"FindNextFile failed (" << GetLastError() << L")\n";
        }

        FindClose(hFind);
    }

    return files;
}

int main()
{
    // TODO: Enter a valid path to a directory with files.
    std::wstring directory = L"";
    std::vector<FileInfo> files;

    LARGE_INTEGER frequency;
    LARGE_INTEGER start;
    LARGE_INTEGER end;

    // Get the frequency of the performance counter
    QueryPerformanceFrequency(&frequency);

    // Start timing
    QueryPerformanceCounter(&start);

    files = EnumerateFiles(directory);

    // End timing
    QueryPerformanceCounter(&end);

    // Calculate the elapsed time in milliseconds
    double elapsedTime = static_cast<double>(end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;
    std::wcout << L"Enumeration took: " << elapsedTime << L" ms" << std::endl;
    std::wcout << L"Total files: " << files.size() << std::endl;

    return 0;
}