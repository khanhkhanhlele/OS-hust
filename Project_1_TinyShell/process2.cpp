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
int status[100];
int n = 0;

int NewProcess(char **args)
{
    // Cài wait time cho các tiến trình
    int wait_time;
    char *run_file = combinePath(args, 2); // Ghép lại tên tiến trình hoặc đường dẫn

    if (strcmp(args[1], "bg") == 0)
    {
        wait_time = 0;
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        si.wShowWindow = SW_SHOW;
        si.dwFlags = STARTF_USESHOWWINDOW;
        si.lpTitle = args[1];
        ZeroMemory(&pi, sizeof(pi));

        // Khởi tạo tiến trình con
        if (!CreateProcess(NULL, run_file, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
        {
            int error = GetLastError(); // Kiểm tra lỗi
            if (error == 2)
                cout << "The batch file or execute file '" << run_file << "' is not found." << endl;
            else
                cout << "Can't run this file" << endl;
            return 1;
        }
    }
    else if (strcmp(args[1], "fg") == 0)
    {
        wait_time = INFINITE;
        ++n;
        status[n] = 1;
        si[n] = {sizeof(STARTUPINFO)};     // lpStartupInfo // lpProcessInformation
        pi[n];                             // cpp string must be modified to use in c
        ZeroMemory(&si[n], sizeof(si[n])); // fill this block with zeros
        si[n].cb = sizeof(si[n]);
        cString[n] = args[2]; // CreateProcess(cString, NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
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
            return 1;
        }
    return 0;
    }
}
    
 
    
    