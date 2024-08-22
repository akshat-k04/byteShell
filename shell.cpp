#include<bits/stdc++.h> // Includes all standard C++ libraries
#include "shell.h" // Custom header file containing function declarations and global variables for the shell
#include <unistd.h> // Provides access to the POSIX operating system API, used for system calls like `fork()`, `exec()`, etc.
#include <sstream> // Provides string stream classes for manipulating strings
#include <fstream> // Allows file creation, reading, and writing operations
#include <sys/stat.h> // for directory creation
using namespace std ;

// Define global variables
string path = "";  
string command = "";
vector<string> args;
vector<string> history_commands ;
string curr_input_command ="" ;

string get_current_path() {
    char buffer[PATH_MAX];
    if (getcwd(buffer, sizeof(buffer)) != nullptr) {
        return string(buffer);
    } else {
        cerr << "Error getting current working directory" << endl;
        return "";
    }
}


void initialize_values(){
    path = get_current_path();
}

void extract(){
    // make empty 
    args ={} ;
    command ="" ;

    // isstringstream is a class and iss is object created 
    istringstream iss(curr_input_command);

    // this is how we can give values in the variables 
    iss >> command;
    if(command!=""){
        history_commands.push_back(curr_input_command) ;
    }
    string arg;
    while (iss >> arg) {
        args.push_back(arg);
    }
    curr_input_command ="" ;
}



// commands function
void executeHelp() {
    // very simple all set for this 
    cout << "Built-in commands:" << endl;
    cout << "cd <dir> - Change directory" << endl;
    cout << "pwd - Print working directory" << endl;
    cout << "echo <text> - Print text" << endl;
    cout << "exit - Exit the shell" << endl;
}
void executeEcho() {
    /*
    here I have 2 flags
    -n: Do not print the trailing newline character.

    -e: Enable interpretation of backslash escapes (like \n for newline, \t for tab).
        Example: echo -e "Hello\nWorld" prints Hello on one line and World on the next line.
    */
    bool new_line = true;
    bool interpret = false ;
    int i=0 ;
    if(args.size()>0 && args[0]=="-n"){
        new_line = false ;
        i++ ;
        if(args.size()>1 && args[1]=="-e")interpret=true,i++ ;
    }
    else if(args.size()>0 && args[0]=="-e"){
        interpret=true ;
        i++ ;
        if(args.size()>1 && args[1]=="-n")new_line = false,i++ ;
    }
    
    if(i<args.size()){
        if(args[i][0]=='"'){
            if(args[args.size()-1][args[args.size()-1].size()-1]=='"'){
                args[i].erase(0,1) ;
                args[args.size()-1].pop_back() ;
            }
        }
        for(;i<args.size() ; i++){
            bool operate = false ;
            for(int j=0; j<args[i].size()-1 ;j++){
                if(interpret && args[i][j]=='\\'){
                    operate = true ;
                }
                else if(!operate)cout<<args[i][j];
                else {
                    operate =false ;
                    if(args[i][j]=='n')cout<<endl ;
                    else if(args[i][j]=='t')cout<<" ";
                }
            }
            cout<<args[i][args[i].size()-1] ;
            if(i<args.size()-1)cout<<" ";
        }
    }
    if(new_line) cout << endl;
    
}
void executeCd() {

    // checking if path is given or not 
    if (args.empty()) {
        std::cerr << "cd: missing argument" << std::endl;
        return;
    }
    std::string newDir = args[0];

    // Handle special case for home directory
    if (newDir == "~") {
        const char* homeDir = getenv("HOME");
        if (homeDir) {
            newDir = homeDir;
        } else {
            std::cerr << "cd: HOME environment variable not set" << std::endl;
            return;
        }
    }

    // Change directory
    if (chdir(newDir.c_str()) != 0) {
        perror("cd failed");
        return;
    }
    path = get_current_path() ;
}
void executeExit(){
    exit(0) ;
}
void executePWD(){
    cout<<path<<endl ;
}
void executeHistory(){
    if(args.size()==0){
        for(int e=history_commands.size()-2 ;e>=0 ; e--)cout<<e<<":"<<history_commands[e]<<endl ;
    }
    else {
        if(args[0]=="-c"){
            history_commands.clear() ;
        }
        else if(args[0]=="|"){
            if(args.size()>1&& args[1]=="grep"){
                if(args.size()>2){
                    if((args[2][0]=='\''|| args[2][0]=='"')){
                        args[2].erase(args[2].begin()) ;
                        args[2].pop_back() ;
                    }
                    for(int e=history_commands.size()-2 ;e>=0 ; e--){
                        if(history_commands[e].find(args[2])!=string::npos){
                            cout<<e<<":"<<history_commands[e]<<endl ;
                        } 
                    }
                }
                else{
                    for(int e=history_commands.size()-2 ;e>=0 ; e--)cout<<e<<":"<<history_commands[e]<<endl ;
                }
            }
            else {
                cout<<"invalid arguments"<<endl ;
            }
        }
        else {
            cerr<<"invalid flag or argument"<<endl ;
        }
    }

}
void excutePreviousCommand(){
    if(command.size()>1){
        int indx  ;
        if(command[1]=='!'){
            // execute this command 
            indx=  history_commands.size()-2 ;
        }
        else {
            command.erase(command.begin()) ;
            try {
                indx = std::stoi(command);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid argument: " << e.what() << std::endl;
                return ;
            } catch (const std::out_of_range& e) {
                std::cerr << "Out of range: " << e.what() << std::endl;
                return ;
            }
        }
        if(indx>=0 && indx<history_commands.size()-1){
            // execute the command
            curr_input_command = history_commands[indx] ;
        }
        else {
            std::cerr << "Out of range: " << indx << std::endl;
        }
    }
    else {
        cout<<command<<" "<<command.size()<<endl ;
        cerr<<"invalid Command"<<endl ;
    }
}
void executeFileCreate(){
    if(args.size()!=0){
        if(args[0][0]=="'" || args[0][0]=='\"'){
            args[0].erase(args[0].begin()) ;
            args.pop_back() ;
        }
        ofstream file(args[0]);
    }
    else {
        cerr<<"Name can't be empty"<<endl ;
    }
}
void executeDirectoryCreate(){
    if(echo.size()==0){
        cerr<<"Specify the Directory name"<<endl ;
    }
    else if (mkdir(echo[0], 0777) == 0) {
        std::cout << "Directory created successfully.\n";
    } else {
        std::cerr << "Error creating directory.\n";
    }
}
void execute() {
    if(command==""){
        return  ;
    } else if (command == "help") {
        // call help function
        executeHelp() ;
    } else if (command == "echo") {
        // call echo function
        executeEcho() ;
    } else if (command == "cd") {
        // call cd function
        executeCd() ;
    } else if (command == "pwd") {
        // call pwd function
        executePWD() ;
    } else if (command == "exit") {
        // call exit function
        executeExit() ;
    } 
    else if (command == "history") {
        // call history function
        executeHistory() ;
    } 
    else if(command[0]== '!'){
        excutePreviousCommand() ;
    }
    else if(command=="touch"){
        executeFileCreate() ;
    }
    else if(command=="mkdir"){
        executeDirectoryCreate() ;
    }

    // else if (command == "kill") {
    //     // call kill function
    //     executeKill() ;
    // } else if (command == "bg") {
    //     // call bg function
    //     executeBg() ;
    // } else if (command == "alias") {
    //     // call alias function
    //     executeAlias() ;
    // } else if (command == "bind") {
    //     // call bind function
    //     executeBind() ;
    // } else if (command == "caller") {
    //     // call caller function
    //     executeCaller() ;
    // } else if (command == "command") {
    //     // call command function
    //     executeCommand() ;
    // } else if (command == "builtin") {
    //     // call builtin function
    //     executeBuiltin() ;
    // } else if (command == "declare") {
    //     // call declare function
    //     executeDeclare() ;
    // } else if (command == "enable") {
    //     // call enable function
    // } else if (command == "let") {
    //     // call let function
    // } else if (command == "local") {
    //     // call local function
    // } else if (command == "logout") {
    //     // call logout function
    // } else if (command == "mapfile") {
    //     // call mapfile function
    // } else if (command == "read") {
    //     // call read function
    // } else if (command == "printf") {
    //     // call printf function
    // } else if (command == "readarray") {
    //     // call readarray function
    // } else if (command == "type") {
    //     // call type function
    // } else if (command == "typeset") {
    //     // call typeset function
    // } else if (command == "ulimit") {
    //     // call ulimit function
    // } else if (command == "unalias") {
    //     // call unalias function
    // } 
    else {
        cerr << "Unknown command: " << command << endl;
    }
}


// void printWorkingDirectory() {
//     char cwd[1024];
//     if (getcwd(cwd, sizeof(cwd)) != NULL) {
//         cout << cwd << endl;
//     } else {
//         perror("getcwd failed");
//     }
// }
