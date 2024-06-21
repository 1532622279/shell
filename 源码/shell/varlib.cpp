
// varlib.cpp
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <new>
#define MAXVARS 200

struct Var {
    char* str;  // name=val string
    bool global;  // a boolean
};

static Var tab[MAXVARS];  // the table

static char* new_string(const char* name, const char* val);
static Var* find_item(const char* name, int first_blank);

int VLstore(const char* name, const char* val) {
    Var* itemp;
    char* s;
    int rv = 1;

    if ((itemp = find_item(name, 1)) != nullptr && (s = new_string(name, val)) != nullptr) {
        if (itemp->str) {  // Has a val?
            delete[] itemp->str;  // Yes: remove it
        }
        itemp->str = s;
        itemp->global = false;  // 在重新赋值时重置全局标志
        rv = 0;  // ok!
    }
    return rv;
}

char* new_string(const char* name, const char* val) {
    size_t len = std::strlen(name) + std::strlen(val) + 2;
    char* retval = new char[len];
    if (retval != nullptr) {
        std::sprintf(retval, "%s=%s", name, val);
    }
    return retval;
}

char* VLlookup(const char* name) {
    Var* itemp;

    if ((itemp = find_item(name, 0)) != nullptr) {
        return itemp->str + std::strlen(name) + 1;
    }
    return const_cast<char*>("");  // 没找到返回空字符
}
int VLexport(const char* name) {
    Var* itemp;
    int rv = 1;

    if ((itemp = find_item(name, 0)) != nullptr) {
        itemp->global = true;
        rv = 0;
    } else if (VLstore(name, "") == 0) {
        rv = VLexport(name);
    }
    return rv;
}

static Var* find_item(const char* name, int first_blank) {
    int i;
    int len = strlen(name);

    for ( i = 0; i < MAXVARS && tab[i].str != nullptr; i++) {
        if (std::strncmp(tab[i].str, name, std::strlen(name)) == 0 && tab[i].str[std::strlen(name)] == '=') {
            return &tab[i];
        }
    }
    if (first_blank && i < MAXVARS) {
        return &tab[i];
    }
    return nullptr;
}

void VLlist() {
    for (int i = 0; i < MAXVARS && tab[i].str != nullptr; i++) {
        if (tab[i].global) {
            std::cout << "  * " << tab[i].str << std::endl;
        } else {
            std::cout << "    " << tab[i].str << std::endl;
        }
    }
}

int VLenviron2table(const char** env) {
    int i;
    char *newstring;

    for ( i = 0; env[i] != nullptr; ++i) {
        if (i == MAXVARS)
            return 0;
        newstring = new char[strlen(env[i]) + 1];
        if (newstring == nullptr)
            return 0;
        strcpy(newstring, env[i]);
        tab[i].str = newstring;
        tab[i].global = true;
    }
    for (; i < MAXVARS; ++i) {
        tab[i].str = nullptr;
        tab[i].global = false;
    }
    return 1;
}

char** VLtable2environ() {
    int i, j, n = 0;

    // 计算全局变量的数量
    for (i = 0; i < MAXVARS && tab[i].str != nullptr; ++i)
        if (tab[i].global)
            n++;

    // 为那么多变量以及一个空字符终止符分配空间
    char** envtab = new char*[n + 1];
    if (envtab == nullptr)
        return nullptr;

    // 用指针加载数组
    for (i = 0, j = 0; i < MAXVARS && tab[i].str != nullptr; ++i) {
        if (tab[i].global)
            envtab[j++] = tab[i].str;
    }
    envtab[j] = nullptr;  // Null-terminate the array
    return envtab;
}

