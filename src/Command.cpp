#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <windows.h>
#include <time.h>
#include <sys/types.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <direct.h>
#include "Analyse.h"
#include "Process.h"
#include "Env.h"

#define MAX_CWD_LENS 128
#define MAX_BUFFER_SIZE 64
#define MAX_TOK_BUFSIZE 64
#define TOKEN_DELIMETERS " \t\r\n\a"

using namespace std;

/* Mảng các câu lệnh command được hỗ trợ */
const char *command[] = {
    "cd",     
    "cls",    
    "echo",  
    "exit", 
    "help", 
    "del",
    "dir",
    "mkdir", 
    "date",
    "time",
    "pc",
    "calc",
    "run",
    "env",
};

/* Mảng các lệnh command*/
int (*activate_command[])(char **) = { 
  &f_cd,
  &f_cls,
  &f_echo,
  &f_exit,
  &f_help,
  &f_del,
  &f_dir,
  &f_mkdir,
  &f_date,
  &f_time,
  &f_pc,
  &f_calc,
  &f_run,
  &f_env,
}; 

/**
 * Chạy lệnh với lệnh là arg[0] như cd, dir, exit, help
 **/
int execute(char ** args){
    /**
     * Chạy lệnh với lệnh là arg[0] như cd, dir, exit, help
     **/
    if (args[0] == NULL){
        return 0;
    }
    for(int i=0; i < size_of_command() ; i++){
        if(strcmp(args[0],command[i]) == 0){ /* Kiểm tra xem người dùng nhập lệnh nào trong tập lệnh */
            return (*activate_command[i])(args);
        }
    }
    cout << "'" << args[0] << "'" <<" is not recognized as an internal or external command, operable program or batch file.\n";
    return 0;
}


/**
 * Trả về số câu lệnh trong tập các lệnh 
 **/
int size_of_command(){
    /**
     * Trả về số câu lệnh trong tập các lệnh 
     **/
    return sizeof(command) / sizeof(char *);
}


//////////////////////////////////////////
////////// Danh sách câu lệnh //////////// 
//////////////////////////////////////////

int f_help(char **args){
    if (args[1] == NULL)
	{
		printf("Type \"help -[command]\" for more information about a specific command.\n");
		//printf("Suppoted commands:\n cd\n date\n time\n dir\n cls\n echo\n del\n mkdir\n pc\n exit\n ");
		printf("Usage:\n\t <command> [option]\n\tEXAMPLE: help cd\n");
		printf("%-30s%s\n%-30s%s", " cd",
			"Change the current directory. You must write the new directory after this command.",
			" ", "EXAMPLES: \"cd C:/\"\n\n");
		printf("%-30s%s\n%-30s%s", " date",
			"Show the today's date.",
			" ", "EXAMPLES: \"date\"\n\n");
		printf("%-30s%s\n%-30s%s", " time",
			"Show the current time.",
			" ", "EXAMPLES: \"time\"\n\n");
		printf("%-30s%s\n%-30s%s", " dir",
			"Show all files and folders in the current directory.",
			" ", "EXAMPLES: \"dir\"\n\n");
        printf("%-30s%s\n%-30s%s", " cls",
			"Clear the console screen.",
            " ", "EXAMPLES: \"cls\"\n\n");
        printf("%-30s%s\n%-30s%s", " echo",
			"Print a message on the screen.",
            " ", "EXAMPLES: \"echo [message]\"\n\n");
        printf("%-30s%s\n%-30s%s", " del",
			"Delete a file or folder in the current directory.",
            " ", "EXAMPLES: \"del [Folder/Filename]\"\n\n");
        printf("%-30s%s\n%-30s%s", " mkdir",
			"Make a new directory.",
            " ", "EXAMPLES: \"mkdir[Foldername]\"\n\n");
        printf("%-30s%s\n%-30s%s", " run",
               "Run .bat file only.",
               " ", "EXAMPLES: \"run [filename.bat]\"\n\n");
        printf("%-30s%s\n%-30s%s", " env",
            "Working with environment variables",
            " ", "EXAMPLES: \"env all\"\n\n");
        printf("%-30s%s\n%-30s%s\n%-30s%s", " pc",
            "Create process.", " ",
            "You must enter the options in the 2nd argument, such as f and b",
            " ", "EXAMPLES: \"pc bg\"\n\n");
		printf("%-30s%s", " exit", "Exit this tiny shell \n");
	}
    else if(!strcmp(args[1],"cd")){
        cout << "Change the current directory." << endl;
        cout << "       cd           : Show the current directory" << endl;
        cout << "       cd ..        : Show the parent directory of the current directory" << endl;
        cout << "       cd [path]    : Change the current directory to [path]" << endl;
        cout << "EXAMPLE: cd C:\\Users\\X1 carbon\\ => change current directory to C:\\User\\X1 carbon" << endl;
    }
    else if(!strcmp(args[1],"date")){
        cout << "Display the current date to screen." << endl;
        cout << "This command does not support any options." << endl;
    }
    else if(!strcmp(args[1],"time")){
        cout << "Diplay the current time to screen." << endl;
        cout << "This command does not support any options." << endl;
    }
    else if(!strcmp(args[1],"dir")){
        cout << "Display the list of files and folder in the directory to the screen." << endl;
        cout << "       dir         : Show the files and folders in the current directory" << endl;
        cout << "EXAMPLE: \"dir\"" << endl;
    }
    else if(!strcmp(args[1],"cls")){
        cout << "Clear all line displaying on the console screen." << endl;
        cout << "This command does not support any options." << endl;
    }
    else if(!strcmp(args[1],"echo")){
        cout << "Print the message on the screen." << endl;
        cout << "This command does not support any options." << endl;
        cout << "EXAMPLE: \"echo Hello World\"" << endl;
        cout << "       => \"Hello World\"" << endl;
    }
    else if(!strcmp(args[1],"del")){
        cout << "Delete file or folder in the current directory." << endl;
        cout << "Recommend: Folder/File name contains no space." << endl;
        cout << "       del [Folder/Filename] : Delete folder/file (Folder/Filename) in the current dirrectory" << endl;
        cout << "EXAMPLE: \"del ABC\"" << endl; 
    }
    else if(!strcmp(args[1],"mkdir")){
        cout << "Make a new directory in the current directory." << endl;
        cout << "Recommend: Foldername contains no space." << endl;
        cout << "       mkdir [Foldername] : Make folder [Foldername] in the current directory" << endl;
        cout << "EXAMPLE: \"mkdir ABC\"" << endl;
    }
    else if (!strcmp(args[1], "run")){
        cout << "Run .bat file only with commands which our shell supports" << endl;
        cout << "       run [Filename.bat] : Run .bat file with commands which our shell supports " << endl;
        cout << "EXAMPLE: \"run command.bat\"" << endl;
    }
    else if (!strcmp(args[1], "env"))
    {
        cout << "Supported options:" << endl;
        cout << "        env all                :Display all environment variables" << endl;
        cout << "        env [Id]               :Display the value of the environment variable [Id]" << endl;
        cout << "        env add [Id] [value]   :Add the environment variable [Id] with its value [Value]," << endl;
        cout << "        env del [Id]           :Delete the environment variable <Id>" << endl;
        cout << "EXAMPLE: \"env del TEMP \"" << endl;
    }
    else if(!strcmp(args[1],"pc")){
        cout << "Supported options:" << endl;
        cout << "        all     Show list of all running processes" << endl;
        cout << "        list    Show list of backgroud running processes" << endl;
		cout << "        find    Get pid of specific program(s) by name" << endl;
		cout << "        suspend Suspend a program by process id" << endl;
		cout << "        resume  Resume a program by process id" << endl;
		cout << "        kill    Terminate all program by name" << endl;
        cout << "        killid  Terminate a program by process id" << endl;
        cout << "        bg      Run a program in background mode by path to program" << endl;
		cout << "        fg      Run a program in foregound mode by path to program" << endl;
        cout << "EXAMPLE: \"pc fg C:\\Users\\Admin\\ABC\"" << endl;
    }
    else if(!strcmp(args[1],"exit")){
        cout << "Exit the TinyShell." << endl;
        cout << "This command does not support any options." << endl; 
    }
    else if (!strcmp(args[1], "env")){
        
    }
    
    return 0;
}


/**
 * Chuyển directory hiện tại sang directory mới
 * Câu lệnh: cd [path] 
 * cd : trả về đường dẫn directory hiện tại 
 * cd .. :trả về đường dẫn directory cha
 * cd [path]: chuyển current working directory sang directory mới
 * 
 **/
int f_cd(char **args){
   
    if(args[1] == NULL){ /* Nếu chỉ gõ lệnh cd */
        system("cd"); /* Dùng luôn lệnh cd có sẵn của Windows */
        cout << endl;
        return EXIT_SUCCESS;
    }
    /* Nếu cd [path] */
    else {
        char* path = combinePath(args,1); /* Chuẩn hóa path */
        if(SetCurrentDirectory(path)==FALSE){ /* Tìm đường dẫn nếu có */
            // cout << args[1] << endl;
            fprintf(stdout,"Not able to set current working directory\n");
        }
    }

    return 0;
}

/**
 * Liệt kê các folder, file trong directory 
 * Câu lệnh: dir
 * 
 **/
int f_dir(char **args){
    /**
     * Liệt kê các folder, file trong directory
     * Câu lệnh: dir [path]
     *
     **/
    // char *buffer;

    // // Get the current working directory:

    // if ((buffer = _getcwd(NULL, 0)) == NULL)
    //     perror("_getcwd error");
    // else
    // {
    //     //        printf("%s \n", buffer);
    // }
    // if (_chdir(buffer))
    // {
    //     switch (errno)
    //     {
    //     case ENOENT:
    //         printf("No such file or directory.\n");
    //         break;
    //     case EINVAL:
    //         printf("Invalid buffer.\n");
    //         break;
    //     default:
    //         printf("Unknown error.\n");
    //         break;
    //     }
    // }
    // else
    //     system("dir");

    // free(buffer);

    char *buffer;
    if ((buffer = _getcwd(NULL, 0)) == NULL)
        cout << "_getcwd error";
    system("dir");
    free(buffer);
    return 0;
}
/**
 * Tạo folder trong directory hiện tại
 * Câu lệnh: mkdir [foldername]
 * 
 **/
int f_mkdir(char **args){
    /**
     * Tạo folder trong directory hiện tại
     * Câu lệnh: mkdir [foldername]
     * 
     **/
    if(args[1] == NULL){ /* Chỉ gõ lệnh mkdir */
        cout << "ERROR: Command mk_dir need filename" << endl;
        cout << "Command: mkdir [filename]" << endl;
        cout << "Recommend: filename should not have any space" << endl;
        return 0;
    }
    mkdir(args[1]); /* Lệnh mkdir tạo folder có sẵn */
    return 0;
}


/**
 * In ra message đi kèm với echo
 * Câu lệnh: echo [message]
 *  
 **/
int f_echo(char **args){
    /**
     * In ra message đi kèm với echo
     * Câu lệnh: echo [message]
     *  
     **/
    if(args[1] == NULL){
        cout << "ERROR: Echo + [message]" << endl;
        return 0;
    }
    int i=0;
    while(args[++i] != NULL){
        for(int j=0; j<strlen(args[i]); j++){
            cout << args[i][j];
        }
        cout << " ";
    }
    cout << endl;
    return 0;
}

/**
 * Clear toàn màn hình console
 * Câu lệnh: cls
 **/
int f_cls(char **args){ 
    system("cls");
    return 0;
}


/**
 * In ra màn hình ngày hiện tại
 * Câu lệnh: date
 * 
 **/
int f_date(char **args){
    if (args[1] != NULL)
	{
		cout << "Command \"date\" does not support any option !\n";
		return EXIT_FAILURE;
	}
	time_t t = time(0);
	struct tm * now = localtime(&t);
	cout << "Current Date: " << now->tm_mday << '/'
		<< (now->tm_mon + 1) << '/'
		<< (now->tm_year + 1900)
		<< endl;
	return 0;
}


/**
 * In ra màn hình thời gian (giờ:phút:giây) hiện tại
 * Câu lệnh: time
 * 
 */
int f_time(char **args){
    if (args[1] != NULL)
	{
		cout << "Command \"time\" does not support any option !\n";
		return EXIT_FAILURE;
	}
	time_t t = time(0);
	struct tm * now = localtime(&t);
	cout << "Current time: " << now->tm_hour << ':' << now->tm_min << ':' << now->tm_sec << endl;
	return 0;
}

/**
 * Delete file hoặc folder
 * Câu lệnh: del [file/foldername]
 *  
 **/
int f_del(char **args){
    /**
     * Delete file hoặc folder
     * Câu lệnh: del [file/foldername]
     *  
     **/
    if(args[1] == NULL){
        cout << "ERROR: Command 'del' need path" << endl;
        cout << "Command: del [path]" << endl;
        return 0;
    }
    else if (args[2] != NULL){
        cout << "ERROR: Command 'del' cannot delete more than 1 file" << endl;
        cout << "Command: del [path]" << endl;
        return 0;
    }
    if(remove(args[1]) == -1 && rmdir(args[1]) == -1){ /* Nếu arg[1] là file thì xóa file, folder thì xóa folder. Nếu k có thì in chỗ dưới */
        cout << "ERROR: Unable to find file to be deleted. Try again" << endl;
        cout << "Command: del [path]" << endl;
        return 0;
    }
    return 0;
}


/**
 * Thoát chương trình
 * Câu lệnh: exit
 * 
 **/
int f_exit(char **args){
    /**
     * Thoát chương trình
     * Câu lệnh: exit
     * 
     **/
    char* term = (char*)malloc(64*sizeof(char));
    if(args[1] != NULL){
        term = combinePath(args,1);
        cout << "ERROR: Term " << term << "is not recognized for EXIT" << endl;
        cout << "Command: exit" << endl;
        return 0;
    }
    /* Trả 1 về cho biết stop trong main */
    return 1;
}


/**
 * Các lệnh với tiến trình
 * Câu lệnh: pc (Process)
 * 
 **/
int f_pc(char **args) {
    if (args[1] == NULL) {
        cout << "ERROR: Too few argument" << endl;
        return 0;
    }
    if (strcmp(args[1], "all") == 0) {
        if (getProcessListAll()) {
            return 0;
        }
        return 1;
    }
    if (strcmp(args[1], "list") == 0) {
        if (list()) {
            return 0;
        }
        return 1;
    }
    if (strcmp(args[1], "find") == 0){
		if (args[2] == NULL)
		{
			printf("ERROR: Too few arguments\n");
			return 0;
		}
		// Tìm ID Process
		if (findProcess(args[2]))
			return 0;
        return 1;
	}

    if (strcmp(args[1], "kill") == 0) {
        if (args[2] == NULL) {
            cout << "ERROR: Too few argument" << endl;
            return 0;
        }

        if (killProcess(args[2])) {
            return 0;
        }
        return 1;
    }

    if (strcmp(args[1], "killid") == 0) {
        if (args[2] == NULL) {
            cout << "ERROR: Too few argument" << endl;
            return 0;
        }
        DWORD process_id = atoi(args[2]);
        if (killProcessID(process_id)) {
            return 0;
        }
        return 1;
    }

    if (strcmp(args[1], "suspend") == 0) {
        if (args[2] == NULL) {
            cout << "ERROR: Too few argument" << endl;
            return 0;
        }
        DWORD process_id = atoi(args[2]);
        suspendProcess(process_id);
        return 0;
    }

    if (strcmp(args[1], "resume") == 0) {
        if (args[2] == NULL) {
            cout << "ERROR: Too few argument" << endl;
            return 0;
        }
        DWORD process_id = atoi(args[2]);
        resumeProcess(process_id);
        return 0;
    }

    if (strcmp(args[1], "bg") == 0 || strcmp(args[1], "fg") == 0) {
        if(args[2] == NULL) {
            cout << "ERROR: Too few argument" << endl;
            return 0;
        } else {
            createNewProcess(args);
        }
        return 0;
    } 
    else {
        cout << "ERROR: Too few argument" << endl;
        return 0;
    }
}


int f_calc(char **args){
    system("calc");
    return 0;
}



//////////////////////////////////////////
//////// Dành riêng cho file .bat ////////
//////////////////////////////////////////

/**
 * Kiểm tra xem câu lệnh có được hỗ trợ trong shell không
 * 
 **/
bool cmdCheck(char **args){
    if (args[0] == NULL){
        return 0;
    }
    for(int i=0; i < size_of_command() ; i++){
        if(strcmp(args[0],command[i]) == 0){ /* Kiểm tra xem người dùng nhập lệnh nào trong tập lệnh */
            return true;
        } 
    }
    return false;
}

/**
 * Chạy riêng cho file .bat  
 * Câu lệnh: run [Filename.bat]
 * 
 **/

int f_run(char **args){
    char w[255],a[255];
    char *run_file = combinePath(args, 1);
    FILE *f=fopen(run_file,"rt");
    if(f==NULL) {
        printf("\nLoi doc file.\n");
        // getch();
        return 0;
	}
    else {
        while(!feof(f)) {
            fgets(w,255,f);
            strcpy(a,w);
            char **arg = separate_line(a); 
            printf("");
            // cout << w << endl; 
            if(cmdCheck(arg)){
                int stop = execute(arg); // system ???
            }
            else {
                system(w);

            }
        }
        fclose(f);
    }
    return 0;
}

int f_env(char **args){
    if (strcmp(args[1], "all") == 0)
    {
        if (args[2]!=NULL)
        {
            cout << "Unknown command: \"" << args[2] << "\"\n\n";
            cout << "To see a list of supported commands, run:\n  env -help\n\n";
        }
        else
        {
            envList();
        }
        return 0;
    }
    else if (strcmp(args[1], "add") == 0)
    {
        if (args[2]==NULL)
        {
            cout << "Wrong command: missing <Id> and <Value>\n\n";
        }
        else if (args[3]==NULL)
        {
            cout << "Wrong command: missing <Value>\n\n";
        }
        else if (args[4]!=NULL)
        {
            cout << "Unknown command: \"" << args[4] << "\"\n\n";
        }
        else
        {
            envAdd(args[2], args[3]);
        }
        return 0;
    }
    else if (strcmp(args[1], "del") == 0)
    {
        if (args[2]==NULL)
        {
            cout << "Wrong command: missing <Path>\n\n";
        }
        else if (args[3] != NULL)
        {
            cout << "Unknown command: \"" << args[3] << "\"\n\n";
        }
        else
        {
            envDel(args[2]);
        }
        return 0;
    }
    else if (args[1] != NULL)
    {
        if (args[2] != NULL)
        {
            cout << "Unknown command: \"" << args[2] << "\"\n\n";
        }
        else
            envFind(args[1]);
        return 0;
        }
    else
    {
        cout << "Unknown command: \"" << args[1] << "\"\n\n";
        return 0;
    }
}

