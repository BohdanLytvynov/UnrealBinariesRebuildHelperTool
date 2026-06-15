// UnrealBinariesRebuildHelperTool.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
//Commom includes both for win32 and linux
#include <iostream>
#include <cstdio>
#include <cstring>
#include <crtdbg.h>
#include <cstdlib>
//Common Macro
#define UT_VAR "UT_PATH"
#define UPRJ_EXT ".uproject"
#define UBT "UnrealBuildTool.exe"
//Error Macro
#define ERR_UPRJ_NOT_FOUND 1
#define ERR_UNABLE_TO_GET_EXE_PATH 2
#define ERR_UNABLE_TO_GET_FOLDER_PATH 3
#define ERR_UABLE_TO_GET_UTIL_PATH 4

//Constants
const char* folders2remove[3] = {"Binaries", "Intermediate", "Saved"};

//Includes and Macro for win32
#ifdef _WIN32

#include <windows.h>
#define MAX_PATH_LEN _MAX_PATH
#define LAST_SLASH '\\'

#elif __linux__//Includes and macro for Linux

#include <unistd.h>
#include <limits.h>
#define MAX_PATH_LEN PATH_MAX
#define LAST_SLASH '/'

#endif // _WIN32

/// <summary>
/// Checks if the c-style string is emty
/// </summary>
/// <param name="str"></param>
/// <returns>true - str is empty, false - str is not empty</returns>
bool CheckEmpty(const char* str)
{
    if (str == nullptr || strlen(str) == 0 || *str == '\0')
        return true;
    return false;
}

/// <summary>
/// Checks if the c-style string is emty and prints custom error msg
/// </summary>
/// <param name="str"></param>
/// <returns>true - str is empty, false - str is not empty</returns>
bool CheckEmpty(const char* str, const char* error)
{
    if (str == nullptr || strlen(str) == 0 || *str == '\0')
    {
        std::cerr << error << std::endl;
        return true;
    }
    return false;
}

/// <summary>
/// Print header
/// </summary>
/// <param name="header"></param>
void PrintHeader(const char* header)
{
    using namespace std;
    cout << "===============================================================" << endl;
    cout << header << endl;
    cout << "===============================================================" << endl;
}

void PrintMsg(const char* msg)
{
    using namespace std;
    if (CheckEmpty(msg))
        return;

    cout << msg << endl;
}

void PrintError(const char* msg)
{
    using namespace std;
    if (CheckEmpty(msg))
        return;

    cerr << msg << endl;
}

void ExecCommand(const char* command)
{
    using namespace std;
    if (CheckEmpty(command))
        return;

    system(command);
}

/// <summary>
/// Get path to the current executing *.exe file
/// </summary>
/// <param name="path">out path to *.exe</param>
void GetCurrentEXEDirectory(char* path)
{
#ifdef _WIN32
    auto c = GetModuleFileNameA(nullptr, path, MAX_PATH_LEN);
    if (c == 0)
    {
        path[0] = '\0';
    }
#elif __linux__
    ssize_t count = readlink("/proc/self/exe", path, MAX_PATH_LEN - 1);
    if (count <= 0)
    {
        path[0] = '\0';
    }
    else
    {
        path[count] = '\0';
    }
    
#endif // _WIN32

}

/// <summary>
/// Cuts off *.exe file name
/// </summary>
/// <param name="path2Exe"></param>
/// <param name="path2Folder"></param>
void GetPath2Folder(const char* path2Exe, char* path2Folder)
{
    if (path2Exe == nullptr || path2Folder == nullptr) return;

#ifdef _WIN32
    strcpy_s(path2Folder, MAX_PATH_LEN, path2Exe);
#else
    std::strncpy(path2Folder, path2Exe, MAX_PATH_LEN - 1);
    path2Folder[MAX_PATH_LEN - 1] = '\0';
#endif

    char* res = strrchr(path2Folder, LAST_SLASH);

    if (res != nullptr)
    {
        *res = '\0';
    }
}

/// <summary>
/// Reads input from the console
/// </summary>
/// <param name="msg">Prompt msg to show</param>
/// <param name="out">Buffer to store the results</param>
/// <param name="outSize">Size of the buffer</param>
void InputStr(const char* msg, char* out, int outSize)
{
    if (out == nullptr || outSize <= 0) return;

    do
    { 
        PrintMsg(msg);
        std::cin.getline(out, outSize);
        if (CheckEmpty(out))
        {
            PrintMsg("Incorrect input! Please try again.");
        }
        else
        {
            break;
        }

    } while (true);
}

/// <summary>
/// Gets the env Variable according to its name
/// </summary>
/// <param name="varName">Name of the Env Var</param>
/// <param name="out">Buffer to store the results</param>
/// <param name="outSize">Size of Buffer</param>
void GetEnvVar(const char* varName, char* out, size_t outSize)
{
    if (CheckEmpty(varName))
    {
        out[0] = '\0';
        return;
    }
#ifdef _WIN32

    if (out == nullptr || outSize == 0) return;

    char* buf = nullptr;
    size_t sz = 0;

    if (_dupenv_s(&buf, &sz, varName) == 0 && buf != nullptr)
    {
        strcpy_s(out, outSize, buf);
        free(buf);//Use it cause _dupenv_s allocates memory using malloc
    }
    else
    {  
        out[0] = '\0';//Empty string
    }
    
#elif __linux__

    const char* envVal = std::getenv(varName);
    if (envVal != nullptr)
    {
        std::strncpy(out, envVal, outSize - 1);//No null terminator at the end of the string
        out[outSize - 1] = '\0'; //Null terminator at the end
    }
    else
    {
        out[0] = '\0';
    }

#endif // _WIN32
}

/// <summary>
/// Sets all the characters in the string to 0
/// </summary>
/// <param name="str"></param>
/// <param name="size"></param>
void ClearString(char* str, size_t size)
{
    if (size == 0) return;

    memset(str, 0, size);
}

void WaitForKey()
{
    std::cout << "Press Enter to continue . . .";
    std::cin.get();
}

int main()
{
    using namespace std;
#ifndef NDEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif 
    PrintHeader("\tUnreal Binaries Rebuild Helper Tool");
    PrintMsg("\t[1/4] Calculating path to the Unreal Utilities...");
    //1 Get PATH to the current UnrealBuildTool.exe
    char path2UnrealUtilities[MAX_PATH_LEN] = {0};
    GetEnvVar(UT_VAR, path2UnrealUtilities, sizeof(path2UnrealUtilities));
    //Path was not found
    if (CheckEmpty(path2UnrealUtilities))
    {
        PrintError("Unable to get ENV VAR <UT_PATH>!");
        return ERR_UABLE_TO_GET_UTIL_PATH;
    }
    printf("Path to Unreal Utilities: %s", path2UnrealUtilities);
    char uprjName[512] = {0};
    //2 Get project name
    InputStr("\nPlease enter the name of your Unreal Engine 4.27 project: ", uprjName, sizeof(uprjName));
    PrintMsg("\t[2/4] Calculating path to the *.uproject file...");
    char path2Exe[MAX_PATH_LEN] = {0};
    //Get Current exe dir
    GetCurrentEXEDirectory(path2Exe);
    if (CheckEmpty(path2Exe))
    {
        PrintError("Unable to get the path to *.exe file!");
        return ERR_UNABLE_TO_GET_EXE_PATH;
    }
    char path2Folder[MAX_PATH_LEN] = {0};
    //Get path to the folder with the unreal project
    GetPath2Folder(path2Exe, path2Folder);
    if (CheckEmpty(path2Folder))
    {
        PrintError("Unable to calculate path to the folder with *.uproject file!");
        return ERR_UNABLE_TO_GET_FOLDER_PATH;
    }

    printf("Directory path: %s\n", path2Folder);

    char path2Uprj[MAX_PATH_LEN] = {0};
    snprintf(path2Uprj, MAX_PATH_LEN, "%s%c%s%s", path2Folder, LAST_SLASH, uprjName, UPRJ_EXT);
    printf("Path to unreal project: %s\n", path2Uprj);
    PrintMsg("\t[3/4] Removing old compiled files...");

    char commandBuffer[4096] = {0};
    char pathForDeletion[MAX_PATH_LEN] = {0};
    size_t totalFolders = sizeof(folders2remove) / sizeof(folders2remove[0]);//C-style calc
    for (size_t i = 0; i < totalFolders; ++i)
    {
        //Build path for removal
        snprintf(pathForDeletion, sizeof(pathForDeletion), "%s%c%s", path2Folder, LAST_SLASH, folders2remove[i]);
#ifdef _WIN32
        snprintf(commandBuffer, sizeof(commandBuffer), "%s \"%s\"", "rmdir /s /q", pathForDeletion);
#elif __linux__
        snprintf(commandBuffer, sizeof(commandBuffer), "%s \"%s\"", "rm -rf", pathForDeletion);
#endif //_WIN32
        printf("[Info] Deleting % s\n", folders2remove[i]);
        ExecCommand(commandBuffer);
        ClearString(commandBuffer, sizeof(commandBuffer));
        ClearString(pathForDeletion, sizeof(pathForDeletion));
    }
    PrintMsg("Old files removed.");
    PrintMsg("\t[4/4] Rebuilding files...");
    char path2UBT[MAX_PATH_LEN] = {0};

    snprintf(path2UBT, sizeof(path2UBT), "%s%c%s", path2UnrealUtilities, LAST_SLASH, UBT);
    snprintf(commandBuffer, sizeof(commandBuffer), "\"\"%s\" -projectfiles -project=\"%s%c%s%s\" -game -rocket -progress\"", path2UBT, path2Folder, LAST_SLASH, uprjName, UPRJ_EXT);
#ifndef NDEBUG
    printf("[DEBUG] Command: %s\n", commandBuffer);
#endif // !NDEBUG

    ExecCommand(commandBuffer);
    ClearString(commandBuffer, sizeof(commandBuffer));
    ClearString(path2UBT, sizeof(path2UBT));

    WaitForKey();

    return 0;
}
