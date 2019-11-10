#ifndef ___COMMAND_H__
#define ___COMMAND_H__

#include "base.h"
#include <cstdlib>
#include <vector>
#include <string>

using namespace std;

class Command : public Base {
    private:
        //char* str_command;
        vector <string> cmdvector;
    
    public:
        //Command() : str_command('\0') {};
        //Command(string str1) : str_command(const_cast<char*>(str1.c_str())) {};
        Command(vector <string> str_command) {
            for (int i = 0; i < str_command.size(); ++i) { //This for loop erases the extra connector symbol at the end of a command.
                if ((str_command.at(i).at(str_command.at(i).size() - 1) == '&') || (str_command.at(i).at(str_command.at(i).size() - 1) == '|') || (str_command.at(i).at(str_command.at(i).size() - 1) == ';') || (str_command.at(i).at(str_command.at(i).size() - 1) == '#')) 
                    str_command.at(i).erase(str_command.at(i).size() - 1);
                // cout << "this is the constructor " << str_command.at(i) << " " << str_command.at(i) << " . " << endl;
                cmdvector.push_back(str_command.at(i));  
                // for (int j = 0; j < cmdvector.size(); j++) {
                //     cout << "this is element: " << j << " of cmdVector   " << cmdvector.at(j) << endl;
                // }
            }
        };
        
        //Execute command calls the proper syscalls for execution of a command. 
        //It pushes a vector of strings into a char pointer array.
        bool execute_command() {
            // vector <char*> cmdvector;
            // cout << str_command << ">" << endl;
            // cmdvector.push_back(str_command);
            // cmdvector.push_back(NULL);

            if (cmdvector.at(0) == "exit") 
                exit(0);

            char *cmd[cmdvector.size() + 1];//&cmdvector[0];
            
            for(int i = 0; i < cmdvector.size(); i++) { //Converting strings into char*.
                cmd[i] = (char*) cmdvector[i].c_str();
            }
            
            cmd[cmdvector.size()] = '\0'; //Adds null char.

		    pid_t pid = fork();
		    pid_t wait;
		    int abc;

    		if (pid > 0) {
	    		wait = waitpid(pid, &abc, 0);
		    	if (wait == -1) {
			    	perror("wait error");
				    exit(EXIT_FAILURE);
			    }

    			if (WEXITSTATUS(abc) == 0) 
	    			return true;
		    }

    		else if (pid == 0) {
	    		if (execvp(*cmd, cmd) < 0) {
		    		perror("Error");
	                exit(1);
			    }
		    }

    		else {
	    		perror("Should not happen but eh?");
		    	exit(1);
	    	}
            
            return false;
        };
};
#endif //___COMMAND_H__