
// smsh2.cpp
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "smsh.h"
#include <vector>

using namespace std;

extern char* next_cmd(const char* prompt, FILE* fp);
extern char** splitline(const std::string& line);


// 定义默认提示符
//const string DFL_PROMPT = "> ";
string DFL_PROMPT = "> ";

int main() {
    char *cmdline, *prompt;
    char **arglist;
    int result;
    void setup();

    const char * constc=nullptr;
    constc=DFL_PROMPT.c_str();
    prompt=const_cast<char *>(constc);

    // prompt = (char *)DFL_PROMPT;

    setup(); // 初始化shell

    while (true) {
        cmdline = next_cmd(prompt, stdin);
        if (cmdline == NULL) break; // 如果到达文件末尾，退出循环

        arglist = splitline(cmdline);
        if (arglist != NULL) {
            result = process(arglist);
            freelist(arglist); // 释放由splitline分配的内存
        }
        free(cmdline); // 释放由next_cmd分配的内存
    }

    return 0;
}

void setup() {
    // 初始化shell
    // 忽略SIGINT和SIGQUIT信号
    signal(SIGINT,  SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}

void fatal(const char *s1, const char *s2, int n) {
    cerr << "Error: " << s1 << "," << s2 << endl;
    exit(n);
}
