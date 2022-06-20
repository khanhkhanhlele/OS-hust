#include <windows.h>
#include <bits/stdc++.h>
#include <string.h>
#include <vector>
#include <thread>
#include <chrono>
#include "function1.h"
using namespace std;

vector<string> createJobs(string command)
{
    vector<string> jobs;
    string s = "";
    int a = command.length();
    for (int i = 0; i < a; ++i)
    {
        if (command[i] != '>')
        {
            s = s + command[i];
        }
        else
        {
            jobs.push_back(s);
            s = "";
        }
    }
    jobs.push_back(s);
    return jobs;
}
string trim(string command)
{
    int j = -1, k = command.length(), g = command.length();
    for (int i = 0; i < g; ++i)
    {
        if (command[i] == ' ')
        {
            j = i;
        }
        else
        {
            break;
        }
    }

    for (int i = g - 1; i > j; i--)
    {
        if (command[i] == ' ')
        {
            k = i;
        }
        else
            break;
    }
    string s = "";
    for (int i = j + 1; i < k; ++i)
    {
        s = s + command[i];
    }
    return s;
}

void runable(string command) 
{
    if (command.compare("help") == 0)
    {
        help();
    }
}

void run(string command)
{
    vector<string> jobs;
    jobs = createJobs(command);
    for (int i = 0; i < jobs.size(); ++i)
    {
        runable(trim(jobs[i]));
    }
}