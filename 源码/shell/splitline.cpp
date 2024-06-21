// splitline.cpp
#include <iostream>
#include <sstream>
#include <cstring>
#include <vector>
#include "smsh.h"
#include <algorithm>

using namespace std;

// 假设 BUFSIZ 已在 smsh.h 中定义
extern char* next_cmd(const char* prompt, FILE* fp);
extern void fatal(const char* s1, const char* s2, int n);
extern void* emalloc(std::size_t n);
extern void* erealloc(void* p, std::size_t n);
extern char** splitline(const std::string& line);

bool is_delim(char ch) {
    return ch == ' ' || ch == '\t';
}
// 新的 splitline 函数，返回一个字符串向量
char** splitline(const std::string& line) {
    const char* delim = " \t\n";
    char** tokens = nullptr;
    char* str = strdup(line.c_str()); // 复制输入字符串以使用strtok
    char* token;

    int count = 0;
    token = strtok(str, delim);
    while (token != nullptr) {
        ++count;
        token = strtok(nullptr, delim);
    }

    // 分配返回数组，包括空指针
    tokens = static_cast<char**>(malloc((count + 1) * sizeof(char*)));
    if (!tokens) {
        free(str);
        return nullptr;
    }

    // 再次使用strtok来填充tokens
    token = strtok(str, delim);
    int index = 0;
    while (token != nullptr) {
        tokens[index++] = strdup(token);
        token = strtok(nullptr, delim);
    }
    tokens[index] = nullptr; // 空指针结束

    free(str); // 释放复制的字符串
    return tokens;
}

char* next_cmd(const char* prompt, FILE* fp) {
    stringstream buffer;
    char buf[BUFSIZ];
    int c;

    cout << prompt; // 输出提示符
    while ((c = getc(fp)) != EOF) {
        if (c == '\n') break;
        buffer.put(c);
    }

    if (buffer.str().empty()) return nullptr;

    char* cmdline = static_cast<char*>(emalloc(buffer.str().size() + 1));
    strcpy(cmdline, buffer.str().c_str());
    return cmdline;
}

// 以下函数需要根据实际情况进行适配或替换
void freelist(char** list) {
    // C++中通常使用 delete[] 来释放数组内存
    delete[] list;
}

char* newstr(const char* s, size_t l) {
    char* rv = static_cast<char*>(emalloc(l + 1));
    strncpy(rv, s, l);
    rv[l] = '\0';
    return rv;
}

void* emalloc(std::size_t n) {
    void* rv = malloc(n);
    if (!rv) fatal("out of memory", "", 1);
    return rv;
}

void* erealloc(void* p, std::size_t n) {
    void* rv = realloc(p, n);
    if (!rv) fatal("realloc() failed", "", 1);
    return rv;
}
