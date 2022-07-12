#include <winnt.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <windows.h>
#include <time.h>
#include <sys/types.h>
#include <psapi.h>
#include <tlhelp32.h>

#define MAX_NAME 16383
#define MAX_VAL 100000

using namespace std;

void envList();
void envFind(string name);
void envAdd(string name, string val);
void envDel(string name);