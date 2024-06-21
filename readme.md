使用c++编写的Shell程序
------
## 1.项目介绍
 - shell程序使用C++，结合C++标准和Linux系统调用编写而成；
 - shell程序能够提供命令的输入，执行并显示执行结果的功能；
 - shell程序能够提供可shell编程的功能；
 - shell程序能够执行简单的脚本程序。
 
-----
## 2.项目目录
```
│  readme.md
│  
├─readme_img
│      find.png
│      img.png
│      img_1.png
│      img_2.png
│      img_5.png
│      img_6.png
│      img_7.png
│      img_8.png
│      ps.png
│      sh.png
│      tree.png
│      
├─源码
│  └─shell
│          builtin.cpp
│          CMakeLists.txt
│          controlflow.cpp
│          execute.cpp
│          process2.cpp
│          smsh.h
│          smsh2.cpp
│          splitline.cpp
│          varlib.cpp
│          varlib.h
│          
└─相关材料
        shell程序答辩PPT.pptx
        shell程序答辩视频.mp4
        项目开发文档.doc
        
```
## 3.项目构建
### 3.1首先进入目录“源码/shell”，在命令行中运行命令cmake
使用cmake生成构建文件
![img_6.png](readme_img%2Fimg_6.png)
### 3.2命令make编译项目
使用make构建项目
![img_7.png](readme_img%2Fimg_7.png)
### 3.3运行程序
编译完成后，生成可执行文件shell，运行其即可使用其功能
![img_8.png](readme_img%2Fimg_8.png)

## 4.项目的运行效果图
### 4.1 命令ls，pwd,tree，date,ps,ps-f的使用
![tree.png](readme_img%2Ftree.png)
![ps.png](readme_img%2Fps.png)
### 4.2通过sh运行脚本文件test.sh和test2.sh
![img.png](readme_img%2Fimg.png)
### 4.3命令find /home name core mtime +3 -print
![img_1.png](readme_img%2Fimg_1.png)
## 5.功能实现与流程图
![img_2.png](readme_img%2Fimg_2.png)
### 5.1重要函数
#### splitline(const std::string& line)
splitlims,将一个字符串分解为字符串数组,并返回这个数组。它调用malloc
并 分配内存以接受任意参数个数的命令行。这个数组由NULL标记结束。
```
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
```
#### next_cmd(const char* prompt, FILE* fp)
next.cmd 从输人流中读人下一个命令。它调用malloc来分配内存以接受任意
长度的命令行。碰到文件结束符,它返回 NULL.
```
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

```
#### execute(char *argv[])
execute 使用 fork、sxESyp,和 wait 来运行一个命令。execute 返回命令的结束
状态。
```
int execute(char *argv[]) {
    pid_t pid;
    int child_info = -1;

    if (argv[0] == NULL) /* nothing succeeds */
        return 0;

    if ((pid = fork()) == -1) {
        std::perror("fork");
    } else if (pid == 0) {
        // Child process
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        if (execvp(argv[0], argv) == -1) {
            std::perror("cannot execute command");
            std::exit(1);
        }
    } else {
        // Parent process
        if (wait(&child_info) == -1) {
            std::perror("wait");
        }
    }
    return child_info;
}
```

## 6.作者
莫菲20210516177

李鑫鑫2021051615241

## 7.参考文献
参考文献：

**《Uinx-Linux编程实践教程》**




