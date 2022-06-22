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

