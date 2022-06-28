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
#include "Analyse.h"
#include "Command.h"
using std::to_string;
using namespace std;

PROCESS_INFORMATION pi[100];
STARTUPINFO si[100];
LPSTR cString[100];
HANDLE hHandle[100];
int status[100];
int n = 0;
HANDLE hForeProcess;

void fore(string s);
void back(string s);
void runExe(string path, string mode);
void runBat(string path);

// catch Ctrl+C signal
void signHandler(int signum)
{
    if (hForeProcess != NULL)
    {
        TerminateProcess(hForeProcess, 0);
        hForeProcess = NULL;
    }
    exit(1);
}

// Process Kill
void kill(string s)
{
    unsigned long id = std::stoul(s);
    bool isNotExist = 1;
    for (int i = 0; i < n; i++)
    {
        if (pi[i].dwProcessId == id)
        {
            TerminateProcess(pi[i].hProcess, 0);
            CloseHandle(pi[i].hThread);
            CloseHandle(pi[i].hProcess);
            cout << "Process " << cString[i] << " killed successfully\n\n";
            for (int j = i; j < n; j++)
            {
                status[j] = status[j + 1];
                pi[j] = pi[j + 1];
                si[j] = si[j + 1];
                cString[j] = cString[j + 1];
            }
            n--;
            isNotExist = 0;
            break;
        }
    }
    if (isNotExist)
    {
        cout << "No process with id " << id << "\n\n";
    }
}

void killAll()
{
    if (n == 0)
    {
        cout << "There are no process working at the moment.\n\n";
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            TerminateProcess(pi[i].hProcess, 0);
            CloseHandle(pi[i].hThread);
            CloseHandle(pi[i].hProcess);
        }
        cout << "All process killed sucessfully.\n";
        cout << "Number of process killed: " << n << "\n\n";
        n = 0;
    }
}

// Stop process
void stop(string s)
{
    unsigned long id = std::stoul(s);
    int isNotExist = 1;
    for (int i = 0; i < n; i++)
    {
        if (pi[i].dwProcessId == id)
        {
            isNotExist = 0;
            if (status[i] == 1)
            {
                status[i] = 0;
                cout << "Process " << id << " stopped.\n\n";
                SuspendThread(pi[i].hThread);
                break;
            }
            else
            {
                cout << "Process " << cString[i] << " is already stopped\n\n";
                break;
            }
        }
    }
    if (isNotExist)
    {
        cout << "No process with id " << id << endl
             << endl;
    }
}

// Resume process
void resume(string s)
{
    unsigned long id = std::stoul(s);
    int isNotExist = 1;
    for (int i = 0; i < n; ++i)
    {
        if (pi[i].dwProcessId == id)
        {
            isNotExist = 0;
            if (status[i] == 0)
            {
                status[i] = 1;
                cout << "Process " << cString[i] << " resumed successfully\n\n";
                ResumeThread(pi[i].hThread);
                break;
            }
            else
            {
                cout << "Process " << cString[i] << " is still running\n\n";
                break;
            }
        }
    }
    if (isNotExist)
    {
        cout << "No process with id " << id << "\n\n";
    }
}

// open file/run process in foreground or background
void open(string mode, string id)
{
    if (!mode.compare("-fore") || !mode.compare("-f"))
        fore(id);
    else if (!mode.compare("-back") || !mode.compare("-b"))
        back(id);
    else
        cout << "Invalid mode, use [-fore|-f] for foreground and [-back|-b] for background\n\n";
}

// Foreground and background
void fore(string s)
{
    PROCESS_INFORMATION pi;
    STARTUPINFO si = {sizeof(STARTUPINFO)};
    LPSTR cString = strdup(s.c_str());
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    if (!CreateProcess(cString, NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
    {
        cout << "Changing of directory or opening file not successful !\n\n";
        return;
    }
    hForeProcess = pi.hProcess;
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
}

void back(string s)
{
    n++;
    status[n - 1] = 1;
    si[n - 1] = {sizeof(STARTUPINFO)};
    ZeroMemory(&si[n - 1], sizeof(si[n - 1]));
    si[n - 1].cb = sizeof(si[n - 1]);
    cString[n - 1] = strdup(s.c_str());
    if (!CreateProcess(cString[n - 1], NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si[n - 1], &pi[n - 1]))
    {
        TerminateProcess(pi[n - 1].hProcess, 0);
        CloseHandle(pi[n - 1].hThread);
        CloseHandle(pi[n - 1].hProcess);
        n--;
        cout << "Changing of directory or opening file not successful !\n\n";
        return;
    }
}

// Current running process
void list()
{
    cout << "List of running process:\n";
    cout << format("Number", 8) << format("IdProcess", 12) << format("hProcess", 12) << format("Status", 12) << format("Name", 60) << endl;
    for (int i = 0; i < n; i++)
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
            string stat((status[i] == 0) ? "Stopped" : "Running");
            cout << format(to_string(i + 1), 8) << format(to_string(pi[i].dwProcessId), 12) << pi[i].hProcess << format(" ", 7) << stat << format(" ", 4) << cString[i] << endl;
        }
    }
}

void run(string *cmd)
{
    if (!cmd[1].compare("notepad") || !cmd[1].compare("n"))
    {
        if (cmd[2].empty())
        {
            cout << "Invalid command: missing <Mode>\n";
            cout << "To see a list of supported commands, run:\n  run -help\n\n";
        }
        else if (!cmd[2].compare("-fore") || !cmd[2].compare("-f"))
        {
            fore("C:\\windows\\system32\\notepad.exe");
        }
        else if (!cmd[2].compare("-back") || !cmd[2].compare("-b"))
        {
            back("C:\\windows\\system32\\notepad.exe");
        }
    }
    else if (!cmd[1].compare("calc") || !cmd[1].compare("c"))
    {
        if (cmd[2].empty())
        {
            cout << "Invalid command: missing <Mode>\n";
            cout << "To see a list of supported commands, run:\n  run -help\n\n";
        }
        else if (!cmd[2].compare("-fore") || !cmd[2].compare("-f"))
        {
            fore("C:\\windows\\system32\\calc.exe");
        }
        else if (!cmd[2].compare("-back") || !cmd[2].compare("-b"))
        {
            back("C:\\windows\\system32\\calc.exe");
        }
    }
    else if (!cmd[1].compare("-help") || !cmd[1].compare("-h"))
    {
        cout << "Usage:\n";
        cout << format("  run <Path/to/exe>", 25) << "Run executable file\n";
        cout << format("  run <Path/to/bat>", 25) << "Run batch file\n\n";
        cout << "Options:\n";
        cout << "[-fore|-f][-back|-b]\n[-h|-help]\n\n";
        cout << "aliases: r\n\n";
    }
    else if (!cmd[1].compare(cmd[1].length() - 4, 4, ".exe"))
    {
        if (!cmd[2].empty())
        {
            runExe(cmd[1], cmd[2]);
        }
        else
        {
            cout << "Invalid command: Missing <mode>\n";
            cout << "To see a list of supported commands, run:\n  run -help\n\n";
        }
    }
    else if (!cmd[1].compare(cmd[1].length() - 4, 4, ".bat"))
    {
        if (cmd[2].empty())
        {
            runBat(cmd[1]);
        }
        else
        {
            cout << "Unknown command: " << cmd[2] << endl;
            cout << "To see a list of supported commands, run:\n  run -help\n\n";
        }
    }
    else
    {
        cout << "Unknown command: " << cmd[1] << endl;
        cout << "To see a list of supported commands, run:\n  run -help\n\n";
    }
}

void runExe(string path, string mode)
{
    if (!mode.compare("-fore") || !mode.compare("-f"))
        fore(path);
    else if (!mode.compare("-back") || !mode.compare("-b"))
        back(path);
    else
        cout << "Invalid mode, use [-fore|-f] for foreground and [-back|-b] for background\n\n";
}

void runBat(string path)
{
    system(path.c_str());
}

void process(string *cmd)
{
    if (!cmd[1].compare("list") || !cmd[1].compare("l"))
    {
        if (cmd[2].empty())
        {
            list();
        }
        else
        {
            cout << "Unknown command: \"" << cmd[2] << "\"\n\n";
            cout << "To see a list of supported commands, run:\n  process -help\n\n";
        }
    }
    else if (!cmd[1].compare("stop") || !cmd[1].compare("s"))
    {
        if (cmd[2].empty())
        {
            cout << "Invalid command: Missing <Id>\n";
            cout << "To see a list of supported commands, run:\n  process -help\n\n";
        }
        else if (!cmd[3].empty())
        {
            cout << "Unknown command: \"" << cmd[3] << "\"\n\n";
            cout << "To see a list of supported commands, run:\n  process -help\n\n";
        }
        else
        {
            stop(cmd[2]);
        }
    }
    else if (!cmd[1].compare("resume") || !cmd[1].compare("r"))
    {
        if (cmd[2].empty())
        {
            cout << "Invalid command: Missing <Id>\n";
            cout << "To see a list of supported commands, run:\n  process -help\n\n";
        }
        else if (!cmd[3].empty())
        {
            cout << "Unknown command: \"" << cmd[3] << "\"\n\n";
            cout << "To see a list of supported commands, run:\n  process -help\n\n";
        }
        else
        {
            resume(cmd[2]);
        }
    }
    else if (!cmd[1].compare("kill") || !cmd[1].compare("k"))
    {
        if (cmd[2].empty())
        {
            cout << "Invalid command: Missing <Id>\n";
            cout << "To see a list of supported commands, run:\n  process -help\n\n";
        }
        else if (!cmd[3].empty())
        {
            cout << "Unknown command: \"" << cmd[3] << "\"\n\n";
            cout << "To see a list of supported commands, run:\n  process -help\n\n";
        }
        else if (!cmd[2].compare("-a") || !cmd[2].compare("-all"))
        {
            killAll();
        }
        else
        {
            kill(cmd[2]);
        }
    }
    else if (!cmd[1].compare("-help") || !cmd[1].compare("-h"))
    {
        cout << "Usage:\n";
        cout << format("  process [list|l]", 30) << "Display list of processes\n";
        cout << format("  process [stop|s] <Id>", 30) << "Stop process with id <Id>\n";
        cout << format("  process [resume|r] <Id>", 30) << "Resume process with id <Id>\n";
        cout << format("  process [kill|k] <Id>", 30) << "Kill process with id <Id>\n";
        cout << format("  process [kill|k] [-a|-all]", 30) << "Kill all process\n\n";
        cout << "Options:\n";
        cout << "[-fore|-f][-back|-b]\n[-h|-help]\n\n";
        cout << "aliases: p\n";
    }
    else
    {
        cout << "Unknown command: \"" << cmd[1] << "\"\n\n";
        cout << "To see a list of supported commands, run:\n  process -help\n\n";
    }
}

#endif