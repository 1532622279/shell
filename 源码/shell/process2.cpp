
// process2.cpp
#include <iostream>
#include "smsh.h"

extern int is_control_command(const char*);
extern int do_control_command(char**);
extern int ok_to_execute();
extern int builtin_command(char**, int*);
extern int execute(char**);

int process(char **args) {
    /*
     * purpose: process user command
     * returns: result of processing command
     * details: if a built-in then call appropriate function, if not execute()
     *  errors: arise from subroutines, handled there
     */
    int rv = 0;

    if (args[0] == NULL)
        rv = 0;
    else if (is_control_command(args[0]))
        rv = do_control_command(args);
    else if (ok_to_execute())
        if (!builtin_command(args, &rv))
            rv = execute(args);
    return rv;
}
