#include <windows.h>
#include <bits/stdc++.h>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

string rawCommands;
boolean validCommands;


void help()
{
    // Display all commands

    printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
    cout << "For more information on a specific command, type help command-name\n";
    cout.width(20);
    cout << left << "1.  help"
         << "Provide Help information for Windows commands\n";
    cout.width(20);
    cout << left << "2.  cd .."
         << "Change to the parrent directory of the current directory\n";
    cout.width(20);
    cout << left << "3.  cd 'path'"
         << "Change current directory to this path\n";
    cout.width(20);
    cout << left << "4.  dir"
         << "Display list of files in parent directory\n";
    cout.width(20);
    cout << left << "5.  date"
         << "Display date\n";
    cout.width(20);
    cout << left << "6.  time"
         << "Display time\n";
    cout.width(20);
    cout << left << "7.  clock"
         << "Display clock\n";
    cout.width(20);
    cout << left << "8.  countdown"
         << "Display countdown clock, add fore or back mode, example: countdown fore, countdown back\n";
    cout.width(20);
    cout << left << "9.  notepad"
         << "Open system notepad, add fore or back mode, example: notepad fore, notepad back\n";
    cout.width(20);
    cout << left << "10. stop 'ID'"
         << "Stop a running process\n";
    cout.width(20);
    cout << left << "11. resume 'ID'"
         << "Resume a stopping process\n";
    cout.width(20);
    cout << left << "12. kill 'ID'"
         << "Kill a running process\n";
    cout.width(20);
    cout << left << "13. kill -1"
         << "Kill all running processes\n";
    cout.width(20);
    cout << left << "14. list"
         << "Display list of processes\n";
    cout.width(20);
    cout << left << "15. [*.bat]"
         << "Read *.bat file and run list of command lines\n";
    cout.width(20);
    cout << left << "16. path of *.exe"
         << "Run *.exe file, add fore or back mode\n";
    cout.width(20);
    cout << left << "17. env a"
         << "Display the value of the environment variable a. Ex: env path\n";
    cout.width(20);
    cout << left << ""
         << "If a = null, display all the environment variables and their values\n";
    cout.width(20);
    cout << left << "18. addenv (a) b"
         << "Add the environment variable a with its name b\n";
    cout.width(20);
    cout << left << "19. delenv a"
         << "Delete the environment variable a\n";
    cout.width(20);
    cout << left << "20. history"
         << "Print all used commands\n";
    cout.width(20);
    cout << left << "21. clean"
         << "Clean the history\n";
    cout.width(20);
    cout << left << "22. clear"
         << "Clear tiny shell\n";
    cout.width(20);
    cout << left << "23. exit"
         << "Exit process\n";
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
}