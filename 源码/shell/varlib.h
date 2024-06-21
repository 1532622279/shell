// varlib.h
#ifndef VARLIB_H
#define VARLIB_H

#include <cstring>
#include <cstdlib> // For size_t

// Define MAXVARS if not already defined
#ifndef MAXVARS
#define MAXVARS 200
#endif
using std::size_t;

// Forward declaration of the Var struct
struct Var;

int VLenviron2table(char**);
int VLexport(const char*);
char* VLlookup(const char*);
void VLlist();
int VLstore(const char*, const char*);
char** VLtable2environ();

#endif // VARLIB_H
