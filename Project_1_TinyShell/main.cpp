#include "library.h"

using namespace std;

string currentDirectory = "";
vector<string> history;

int main()
{
    WELCOME();
    string command;
    char *buffer1 = _getcwd(NULL, 0);
    int i = 0;
        while(buffer1[i] != '\0') {
            currentDirectory += buffer1[i];
            i++;
        }
    while (true)
    {
    	char *buffer = _getcwd(NULL, 0);    
    	printf("%s", buffer);
        cout << ">";
        getline(cin, command);
        trim(command);
        // if (command == EXIT_COMMAND)
        // {
        //     GOODBYE();
        //     printf("Sending signal to kill all child processes ...\n");
        //     kill_All();
        //     this_thread::sleep_for(chrono::milliseconds(1500));
        //     break;
        // }
        // else
        // {
        //     run(command);
        // }
        run(command);
    }
    
}