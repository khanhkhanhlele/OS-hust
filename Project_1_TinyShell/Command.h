#pragma once 

#ifndef _COMMAND_H
#define _COMMAND_H

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <windows.h>
#include <time.h>
#include <sys/types.h>
#include <psapi.h>
#include <tlhelp32.h>


#define MAX_CWD_LENS 128
#define MAX_BUFFER_SIZE 64
#define MAX_TOK_BUFSIZE 64
#define TOKEN_DELIMETERS " \t\r\n\a"

// char cur_directory[MAX_CWD_LENS];




/* Các câu lệnh thực hiện command */
int f_cd(char **args); // giống cd trong cmd
int f_cls(char **args); // clear 
int f_dir(char **args); // directory
int f_echo(char **args); // in ra lại câu nhập vào // còn lỗi
int f_exit(char **args); // thoát
int f_help(char **args); // hỗ trợ các câu lệnh
int f_mkdir(char **args); // tạo folder
int f_del(char **args); // xóa 1 mục hoặc folder
int f_date(char **args); // trả về ngày, tháng, năm
int f_time(char **args); // 
int f_pc(char **args);
int f_calc(char **args);
int f_run(char **args);
int f_process(char **args);
bool cmdCheck(char **args);

int execute(char ** args);
int size_of_command();
int printPrompt(char* cur_dir);


#endif
