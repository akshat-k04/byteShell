#include <bits/stdc++.h>
#include "shell.h"
using namespace std ;

int main() {
    // run the function that will get the path and initialize all the global variables
    initialize_values() ;

    
    cout<<"<-----------------Welcome to the ByteShell----------------->\n" ;
    
    while (true) {
        // Asking for input
        cout << path<<">";
        if(curr_input_command=="") getline(cin, curr_input_command);  
        else cout<<curr_input_command<<endl ;


        // extraxting the commands and arguments 
        extract() ; 

        // execute the command
        execute() ;
    }

    return 0;
}
