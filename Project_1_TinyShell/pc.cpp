#include <iostream>
#include <windows.h>
#include <string>
#include <ctime>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <direct.h>
#include <errno.h>
#include <fstream>
#include <bits/stdc++.h>

using namespace std;

PROCESS_INFORMATION pi[100];
STARTUPINFO si[100];
LPSTR cString[100];
HANDLE hHandless[100];
int status[100];
int n = 0;

void kill(string s)
{
    int id = atoi(s.c_str());

    int a = 1;
    for (int i = 1; i <= n; ++i)
    {
        if (pi[i].dwProcessId == id)
        {
            TerminateProcess(pi[i].hProcess, 0);
            CloseHandle(pi[i].hThread);
            CloseHandle(pi[i].hProcess);
            // cout << pi[i].hProcess << endl;
            printf("Process %s killed\n", cString[i]);
            for (int j = i; j < n; ++j)
            {
                status[j] = status[j + 1];
                pi[j] = pi[j + 1];
                si[j] = si[j + 1];
                cString[j] = cString[j + 1];
            }
            n--;
            a = 0;
            break;
        }
    }
    if (a)
    {
        printf("Can't find process with this id = %d\n", id);
    }
}

void kill_All()
{
    for (int i = 1; i <= n; ++i)
    {
        TerminateProcess(pi[i].hProcess, 0);
        CloseHandle(pi[i].hThread);
        CloseHandle(pi[i].hProcess);

        // cout << pi[i].hProcess << endl;
    }
    printf("All process killed ...\n");
    n = 0;
}

void stop(string s)
{
    int id = atoi(s.c_str());
    int a = 1;
    for (int i = 1; i <= n; ++i)
    {
        if (pi[i].dwProcessId == id)
        {
            a = 0;
            if (status[i] == 1)
            {
                status[i] = 0;
                cout << "Stop " << cString[i] << " success\n";
                SuspendThread(pi[i].hThread);
                break;
            }
            else
            {
                cout << "Process " << cString[i] << " is still stopping" << endl;
                break;
            }
        }
    }
    if (a)
    {
        printf("Can't find process with this id = %d\n", id);
    }
}
void resume(string s)
{
    int id = atoi(s.c_str());
    int a = 1;
    for (int i = 1; i <= n; ++i)
    {
        if (pi[i].dwProcessId == id)
        {
            a = 0;
            if (status[i] == 0)
            {
                status[i] = 1;
                cout << "Process " << cString[i] << " is running again\n";
                ResumeThread(pi[i].hThread);
                break;
            }
            else
            {
                cout << "Process " << cString[i] << " is still running" << endl;
                break;
            }
        }
    }
    if (a)
    {
        printf("Can't find process with this id = %d\n", id);
    }
}

void openInBackOrFore(const string &command, const string &s)
{
    void openProcessInForeGround(const string &s);
    void openProcessInBackGround(const string &s);
    stringstream cc(command);
    //    cout << s << endl;
    string c1, c2;
    cc >> c1;
    cc >> c2;
    if (c2.compare("back") == 0)
    {
        openProcessInBackGround(s);
    }
    else if (c2.compare("fore") == 0)
    {
        openProcessInForeGround(s);
    }
    else
    {
        printf("Illegal command! Add backgound or foreground mode!\n");
    }
}

void openProcessInForeGround(const string &s)
{

    PROCESS_INFORMATION pi;                 // lpStartupInfo    // lpProcessInformation
    STARTUPINFO si = {sizeof(STARTUPINFO)}; // cpp string must be modified to use in c
    LPSTR cString = strdup(s.c_str());
    ZeroMemory(&si, sizeof(si)); // fill this block with zeros
    si.cb = sizeof(si);          // CreateProcess(cString, NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
    if (!CreateProcess(cString,  // No module name (use command line)
                       NULL,     // Command line
                       NULL,     // Process handle not inheritable
                       NULL,     // Thread handle not inheritable
                       FALSE,    // Set handle inheritance to FALSE
                       CREATE_NEW_CONSOLE,
                       NULL, // Use parent's environment block
                       NULL, // Use parent's starting directory
                       &si,  // Pointer to STARTUPINFO structure
                       &pi)  // Pointer to PROCESS_INFORMATION structure
    )
    {
        printf("Changing of directory or opening file not successful!\n");
        return;
    }
    WaitForSingleObject(pi.hProcess, INFINITE); // INFINITE // hProcess: The handle is used to specify the process in all functions that perform operations on the process object.
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
}

void openProcessInBackGround(const string &s)
{
    void kill(string s);
    ++n;
    status[n] = 1;
    si[n] = {sizeof(STARTUPINFO)};     // lpStartupInfo // lpProcessInformation
    pi[n];                             // cpp string must be modified to use in c
    ZeroMemory(&si[n], sizeof(si[n])); // fill this block with zeros
    si[n].cb = sizeof(si[n]);
    cString[n] = strdup(s.c_str()); // CreateProcess(cString, NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
    if (!CreateProcess(cString[n],  // No module name (use command line)
                       NULL,        // Command line
                       NULL,        // Process handle not inheritable
                       NULL,        // Thread handle not inheritable
                       FALSE,       // Set handle inheritance to FALSE
                       CREATE_NEW_CONSOLE,
                       NULL,   // Use parent's environment block
                       NULL,   // Use parent's starting directory
                       &si[n], // Pointer to STARTUPINFO structure
                       &pi[n]) // Pointer to PROCESS_INFORMATION structure
    )
    {
        TerminateProcess(pi[n].hProcess, 0);
        CloseHandle(pi[n].hThread);
        CloseHandle(pi[n].hProcess);
        n--;
        printf("Changing of directory or opening file not successful!\n");
        return;
    }
}

void listOfCurrent()
{
    char *buffer;

    // Get the current working directory:

    if ((buffer = _getcwd(NULL, 0)) == NULL)
        perror("_getcwd error");
    else
    {
        //        printf("%s \n", buffer);
    }
    if (_chdir(buffer))
    {
        switch (errno)
        {
        case ENOENT:
            printf("No such file or directory.\n");
            break;
        case EINVAL:
            printf("Invalid buffer.\n");
            break;
        default:
            printf("Unknown error.\n");
            break;
        }
    }
    else
        system("dir");

    free(buffer);
}

void list1()
{
    // Track running process

    printf("\n");
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
    printf("| Numbers            IdProcess                hProcess               Status                      Name   \n");
    for (int i = 1; i <= n; ++i)
    {
        DWORD dwExitCode;
        GetExitCodeProcess(pi[i].hProcess, &dwExitCode);
        if (dwExitCode != 259)
        {
            TerminateProcess(pi[i].hProcess, 0);
            CloseHandle(pi[i].hThread);
            CloseHandle(pi[i].hProcess);
            for (int j = i; j < n; ++j)
            {
                status[j] = status[j + 1];
                pi[j] = pi[j + 1];
                si[j] = si[j + 1];
                cString[j] = cString[j + 1];
            }
            n--;
            i--;
        }
        else
        {
            const char *a = (status[i] == 0) ? "stopping" : "Running ";
            printf("|   %-19d%-26d%-20p%s          %s\n", i, pi[i].dwProcessId, pi[i].hProcess, a, cString[i]);
        }
    }
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
    printf("\n");
}

void runExe(string command)
{
    void openProcessInForeGround(const string &s);
    void openProcessInBackGround(const string &s);
    stringstream cc(command);
    string c1, c2;
    cc >> c1;
    cc >> c2;
    if (c2.compare("back") == 0)
    {
        openProcessInBackGround(c1);
    }
    else if (c2.compare("fore") == 0)
    {
        openProcessInForeGround(c1);
    }
    else
    {
        printf("Illegal command! Add backgound or foreground mode!\n");
    }
}