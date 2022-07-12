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

void envList()
{
    HKEY hKey;
    BYTE value[MAX_VAL];
    RegOpenKeyEx(HKEY_CURRENT_USER, "Environment", 0, KEY_ALL_ACCESS, &hKey);
    for (int i = 0;; i++)
    {
        TCHAR name[MAX_NAME];
        DWORD namesz = MAX_NAME;
        value[0] = '\0';
        DWORD valSize = MAX_VAL;
        if (RegEnumValue(hKey, i, name, &namesz, NULL, NULL, value, &valSize) == 0)
        {
            cout << (i < 9 ? "0" : "") << i + 1 << ". " << name << " = " << value << endl;
        }
        else
            break;
    }
    RegCloseKey(hKey);
}

void envFind(string name)
{
    HKEY hKey;
    BYTE value[MAX_VAL];
    DWORD valSize = sizeof(value);
    RegOpenKeyEx(HKEY_CURRENT_USER, "Environment", 0, KEY_ALL_ACCESS, &hKey);
    if (RegQueryValueEx(hKey, name.c_str(), NULL, NULL, value, &valSize) == 0)
    {
        cout << "Value of  " << name << " : " << value << "\n";
    }
    else
        cout << "There no variables has the name " << name << "\n\n";
    RegCloseKey(hKey);
}

void envAdd(string name, string val)
{
    HKEY hKey;
    static BYTE value[MAX_VAL];
    DWORD valSize = sizeof(value);
    RegOpenKeyEx(HKEY_CURRENT_USER, "Environment", 0, KEY_ALL_ACCESS, &hKey);
    if (RegQueryValueEx(hKey, name.c_str(), NULL, NULL, value, &valSize) == 0)
    {
        string envName(reinterpret_cast<char *>(value));
        envName = envName + " ; " + val;
        BYTE byteVal[MAX_NAME];
        std::copy(envName.begin(), envName.end(), byteVal);
        byteVal[envName.length()] = 0;
        RegSetValueEx(hKey, name.c_str(), 0, REG_SZ, byteVal, envName.length());
    }
    else
    {
        BYTE byteVal[MAX_NAME];
        std::copy(val.begin(), val.end(), byteVal);
        byteVal[val.length()] = 0;
        RegSetValueEx(hKey, name.c_str(), 0, REG_SZ, byteVal, val.length());
    }
    cout << "The environment variable " << name << " is added\n";
    RegCloseKey(hKey);
}

void envDel(string name)
{
    HKEY hKey;
    RegOpenKeyEx(HKEY_CURRENT_USER, "Environment", 0, KEY_ALL_ACCESS, &hKey);
    if (RegDeleteValue(hKey, name.c_str()))
    {
        cout << "The environment variable " << name << " is deleted\n\n";
    }
    else
    {
        cout << "Variable \"" << name << "\" not found\n\n";
    }
    RegCloseKey(hKey);
}
