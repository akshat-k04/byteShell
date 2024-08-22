#include<bits/stdc++.h> // Includes all standard C++ libraries
#include "shell.h" // Custom header file containing function declarations and global variables for the shell
#include <unistd.h> // Provides access to the POSIX operating system API, used for system calls like `fork()`, `exec()`, etc.
#include <sstream> // Provides string stream classes for manipulating strings
#include <fstream> // Allows file creation, reading, and writing operations
#include <sys/stat.h> // for directory creation
#include <cstdio> // for file delete
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
    cout << "Built-in commands:" << endl;
    
    cout << endl<< "1. echo [OPTION]... [STRING]..." << endl;
    cout << "   Print the STRING(s) to the standard output." << endl;
    cout << "   Options:" << endl;
    cout << "     -n    Do not print the trailing newline character." << endl;
    cout << "     -e    Enable interpretation of backslash escapes (e.g., \\n, \\t)." << endl;
    
    cout << endl<< "2. cd [DIRECTORY]" << endl;
    cout << "   Change the current working directory to DIRECTORY." << endl;
    cout << "   Special case: '~' for the home directory." << endl;
    
    cout << endl<< "3. exit" << endl;
    cout << "   Exit the shell." << endl;
    
    cout << endl<< "4. pwd" << endl;
    cout << "   Print the current working directory." << endl;
    
    cout << endl<< "5. history [OPTION] [ARGUMENT]" << endl;
    cout << "   Display the command history." << endl;
    cout << "   Options:" << endl;
    cout << "     -c    Clear the command history." << endl;
    cout << "     | grep [SEARCH_STR]  Filter history by SEARCH_STR." << endl;
    
    cout << endl<< "6. !!" << endl;
    cout << "   Re-execute the last command from history." << endl;
    
    cout << endl<< "7. !N" << endl;
    cout << "   Re-execute the command at index N from history." << endl;
    
    cout << endl<< "8. touch [FILENAME]" << endl;
    cout << "   Create a new file named FILENAME." << endl;
    
    cout << endl<< "9. mkdir [DIRECTORY_NAME]" << endl;
    cout << "   Create a new directory named DIRECTORY_NAME." << endl;
    
    cout << endl<< "10. rm [FILENAME]" << endl;
    cout << "    Delete the file named FILENAME." << endl;
    
    cout << endl<< "11. rmdir [DIRECTORY_NAME]" << endl;
    cout << "    Delete the directory named DIRECTORY_NAME." << endl;
    
    cout << endl<< "12. ls [-R]" << endl;
    cout << "    List directory contents." << endl;
    cout << "    Options:" << endl;
    cout << "      -R    Recursively list subdirectories." << endl;
    
    // cout << endl<< "13. ps" << endl;
    // cout << "    Display process information (not implemented yet)." << endl;
    
    // cout << endl<< "14. kill" << endl;
    // cout << "    Terminate a process (not implemented yet)." << endl;
}
void executeEcho() {

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
        cerr << "cd: missing argument" << endl;
        return;
    }
    string newDir = args[0];

    // Handle special case for home directory
    if (newDir == "~") {
        const char* homeDir = getenv("HOME");
        if (homeDir) {
            newDir = homeDir;
        } else {
            cerr << "cd: HOME environment variable not set" << endl;
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
        cout<<"Command History:"<<endl ;
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
                        cout<<"Command History"<<endl ;
                        if(history_commands[e].find(args[2])!=string::npos){
                            cout<<e<<":"<<history_commands[e]<<endl ;
                        } 
                    }
                }
                else{
                    cout<<"Command History"<<endl ;
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
                indx = stoi(command);
            } catch (const invalid_argument& e) {
                cerr << "Invalid argument: " << e.what() << endl;
                return ;
            } catch (const out_of_range& e) {
                cerr << "Out of range: " << e.what() << endl;
                return ;
            }
        }
        if(indx>=0 && indx<history_commands.size()-1){
            // execute the command
            curr_input_command = history_commands[indx] ;
        }
        else {
            cerr << "Out of range: " << indx << endl;
        }
    }
    else {
        cout<<command<<" "<<command.size()<<endl ;
        cerr<<"invalid Command"<<endl ;
    }
}
void executeFileCreate() {
    if (args.size() != 0) {
        string filename = args[0];
        
        // Remove surrounding quotes if present
        if (filename.front() == '"' || filename.front() == '\'') {
            filename.erase(filename.begin());
        }
        if (filename.back() == '"' || filename.back() == '\'') {
            filename.pop_back();
        }

        // Create the file
        ofstream file(filename);
        if (file.is_open()) {
            cout << "File created successfully" << endl;
        } else {
            cerr << "Error creating file" << endl;
        }
    } else {
        cerr << "Name can't be empty" << endl;
    }
}
void executeDirectoryCreate() {
    if (args.size() == 0) {
        cerr << "Specify the directory name" << endl;
        return;
    }

    string dirName = args[0];
    
    // Remove surrounding quotes if present
    if (dirName.front() == '"' || dirName.front() == '\'') {
        dirName.erase(dirName.begin());
    }
    if (dirName.back() == '"' || dirName.back() == '\'') {
        dirName.pop_back();
    }

    // Create the directory
    if (mkdir(dirName.c_str(), 0777) == 0) {
        cout << "Directory created successfully.\n";
    } else {
        perror("Error creating directory");
    }
}
void executeRM() {
    if (args.size() == 0) {
        cerr << "Enter the name of the file" << endl;
        return;
    }

    string fileName = args[0];
    
    // Remove surrounding quotes if present
    if (fileName.front() == '"' || fileName.front() == '\'') {
        fileName.erase(fileName.begin());
    }
    if (fileName.back() == '"' || fileName.back() == '\'') {
        fileName.pop_back();
    }

    // Delete the file
    if (remove(fileName.c_str()) == 0) {
        cout << "File deleted successfully\n";
    } else {
        perror("Error deleting file");
    }
}
void executeRmDir() {
    if (args.size() == 0) {
        cerr << "Directory name not entered" << endl;
        return;
    }

    string dirName = args[0];
    
    // Remove surrounding quotes if present
    if (dirName.front() == '"' || dirName.front() == '\'') {
        dirName.erase(dirName.begin());
    }
    if (dirName.back() == '"' || dirName.back() == '\'') {
        dirName.pop_back();
    }

    // Delete the directory
    if (rmdir(dirName.c_str()) == 0) {
        cout << "Directory deleted successfully.\n";
    } else {
        perror("Error deleting directory");
    }
}
void LStraverse(int i , string temp){
    for (const auto& entry : filesystem::directory_iterator(temp)) {
        // if(entry.path().filename().string() ==".git")continue ;
        for(int e=0 ;e<i ;e++)cout<<"  ";
        cout<< entry.path().filename().string() << endl;
        if( filesystem::is_directory(entry.status())){// it is directory
            string new_tmep =temp +"/" + entry.path().filename().string() ;
            LStraverse(i+1 , new_tmep) ;
        }
    }
}
void executeLs(){
    if(args.size()>0 && args[0]=="-R"){
        
        LStraverse(0,path) ;
    }
    else {
        for (const auto& entry : filesystem::directory_iterator(path)) {
            cout << entry.path().filename().string() << endl;
        }
    }
    
}
void executePs(){
    cout<<"ps and kill is not implemented yet!"<<endl ;
}
void executeKill(){
    cout<<"ps and kill is not implemented yet!"<<endl ;
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
    } else if (command == "history") {
        // call history function
        executeHistory() ;
    } else if(command[0]== '!'){
        excutePreviousCommand() ;
    } else if(command=="touch"){
        executeFileCreate() ;
    } else if(command=="mkdir"){
        executeDirectoryCreate() ;
    } else if(command=="rm"){
        executeRM() ;
    } else if(command=="rmdir"){
        executeRmDir() ;
    } else if(command=="ls"){
        executeLs() ;
    } 
    // else if (command == "kill") {
    //     // call kill function
    //     executeKill() ;
    // } else if(command=="ps"){
    //     // call ps function
    //     executePs() ;
    // } 
    else {
        cerr << "Unknown command: " << command << endl;
    }
}
