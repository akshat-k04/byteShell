#ifndef SHELL_H
#define SHELL_H

#include <bits/stdc++.h>
using namespace std ;


extern string path;         // Declaration only
extern string command;      // Declaration only
extern vector<string> args; // Declaration only
extern vector<string> history_commands ;
extern string curr_input_command;

// Function declarations
void initialize_values() ;
void extract() ;
void execute() ;


#endif // SHELL_H