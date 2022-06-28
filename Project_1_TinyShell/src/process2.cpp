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

using namespace std;

PROCESS_INFORMATION pi[100];
STARTUPINFO si[100];
LPSTR cString[100];
HANDLE hHandless[100];
HANDLE hForeProcess;
int status[100];
int n = 0;

int createNewProcess(char **args)
{
    // Cài wait time cho các tiến trình
    int wait_time;
    char *run_file = combinePath(args, 2); // Ghép lại tên tiến trình hoặc đường dẫn

    if (strcmp(args[1], "fg") == 0)
    {
        PROCESS_INFORMATION pi;
        STARTUPINFO si = {sizeof(STARTUPINFO)};
        LPSTR cString = args[2];
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        if (!CreateProcess(cString, NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
        {
            cout << "Changing of directory or opening file not successful !\n\n";
            return 1;
        }
        hForeProcess = pi.hProcess;
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    }
    else if (strcmp(args[1], "bg") == 0)
    {
        n++;
        status[n - 1] = 1;
        si[n - 1] = {sizeof(STARTUPINFO)};
        ZeroMemory(&si[n - 1], sizeof(si[n - 1]));
        si[n - 1].cb = sizeof(si[n - 1]);
        cString[n - 1] = args[2];
        if (!CreateProcess(cString[n - 1], NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si[n - 1], &pi[n - 1]))
        {
            TerminateProcess(pi[n - 1].hProcess, 0);
            CloseHandle(pi[n - 1].hThread);
            CloseHandle(pi[n - 1].hProcess);
            n--;
            cout << "Changing of directory or opening file not successful !\n\n";
            return 1;
        }
    }
    return 1;
    }

int findProcessID(char *name_process){
    return 1;
}
int killProcessID(DWORD process_id){
    return 1;
}
int killProcess(char *name_process){
    return 1;
}
int suspendProcess(DWORD process_id){
    return 1;
}
int resumeProcess(DWORD process_id){
    return 1;
}
string format(string s, unsigned long long len)
{
    string a = s;
    while (a.length() < len)
        a += ' ';
    return a;
}

int getProcessListAll()
{
    // Track running process

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
    return 1;
}