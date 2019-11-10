#ifndef RSHELL_H
#define RSHELL_H
 
#include <iostream>
#include <queue>
#include <unistd.h>
#include "and.h"
#include "or.h"
#include "command.h"
#include "base.h"
#include <climits>
#include "parser.h"

using namespace std;

class rshell {
    private:
        queue<Base*> command_queue;
        vector<string> str1;
        Parser parserObj;        
        int parenthesiscounter;
        
    public:
        void execute_queue() {
            bool exitbool = false;
            while (!exitbool) {            
                char* username = getlogin();
                if (!username) 
                    perror ("no username");
                
                char hostname[HOST_NAME_MAX];
                gethostname(hostname, HOST_NAME_MAX);
            
                cout << username << "@" << hostname;
                cout << " $ ";

                string str;
            
                getline(cin, str);
            
                if (str == "exit") {
                    exitbool = true;
                    break;
                }

                parserObj.parse(str);

                forgeCommands();
            
                if (parenthesiscounter != 0) {
                    cout << "Uneven Number of parenthesis\n";
                }
                else {    
                    while (!command_queue.empty()) {
                        command_queue.front() -> execute_command();
                        command_queue.pop();
                    }
                }
                
                parserObj.clearparser();
            }    
        };
        
    private: /* helper functions */
        void forgeCommands() {
            Base* tempconnector;
            bool istempassigned = false;
            bool isand;

            Base* tempparent;
            bool isparentassigned = false;
            bool isparentand;

            parenthesiscounter = 0;

            // bool issemicolon;

            for (int a = 0; a < parserObj.parsedCommand.size(); ++a) {
                string cmdstring = parserObj.parsedCommand.at(a);

                // fixme: add parenthesis to & | ;

                if (cmdstring == "&") {
                    if (parenthesiscounter == 0) {
                        if (istempassigned) {
                            Command* cmd = new Command(str1);
                            clearVector();

                            Base* tempand;

                            if (isand) {
                                tempand = new And(tempconnector, cmd);    
                            }
                            else {
                                tempand = new Or(tempconnector, cmd);
                            }

                            tempconnector = tempand;
                            istempassigned = true;
                            isand = true;
                        }
                        else {
                            Command* cmd = new Command(str1);
                            clearVector();
                            tempconnector = cmd;
                            istempassigned = true;
                            isand = true;
                        }
                    }
                    else {
                        if (isparentassigned) {
                            Base* cmd;

                            if (!str1.empty()) {
                                Command* cmd2 = new Command(str1);
                                clearVector();
                                cmd = cmd2;
                            }
                            else {
                                cmd = tempconnector;
                            }

                            tempparent = cmd;
                            isparentassigned = true;
                            isparentand = true;
                        }
                        else {
                            Command* cmd = new Command(str1);
                            clearVector();
                            tempparent = cmd;
                            isparentassigned = true;
                            isparentand = true;
                        }
                    }    
                }
                else if (cmdstring == "|") {
                    if (parenthesiscounter == 0) {            
                        if (istempassigned) {
                            Command* cmd = new Command(str1);
                            clearVector();

                            Base* tempor;

                            if (isand) {
                                tempor = new And(tempconnector, cmd);    
                            }
                            else {
                                tempor = new Or(tempconnector, cmd);
                            }

                            tempconnector = tempor;
                            istempassigned = true;
                            isand = false;
                        }
                        else {
                            Command* cmd = new Command(str1);
                            clearVector();
                            tempconnector = cmd;
                            istempassigned = true;
                            isand = false;
                        }
                    }
                    else {
                        if (isparentassigned) {
                            Command* cmd = new Command(str1);
                            clearVector();
                            Or* tempor = new Or(tempparent, cmd);
                            tempparent = tempor;
                            isparentassigned = true;
                            isparentand = false;
                        }
                        else {
                            Command* cmd = new Command(str1);
                            clearVector();
                            tempparent = cmd;
                            isparentassigned = true;
                            isparentand = false;
                        }
                    }
                }
                else if (cmdstring == ";") {
                    if (parenthesiscounter == 0) {
                        if (istempassigned){
                            if (!str1.empty()) {
                                if (isand == true){
                                    Command* cmd = new Command(str1);
                                    clearVector();
                                    And* q_and = new And(tempconnector, cmd);
                                    command_queue.push(q_and);
                                    istempassigned = false;
                                }
                                else {
                                    Command* cmd = new Command(str1);
                                    clearVector();
                                    Or* q_or = new Or(tempconnector, cmd);
                                    command_queue.push(q_or);
                                    istempassigned = false;
                                }    
                            }
                            else {
                                command_queue.push(tempconnector);
                                istempassigned = false;
                            }
                        }
                        else {
                            Command* cmd = new Command(str1);
                            clearVector();
                            command_queue.push(cmd);
                        } 
                    }
                    else {
                        // insert new semicolon class
                        // echo 1 && (echo 2 ; echo 3) && echo 4
                    }    
                }
                else if (cmdstring == "(") {
                    parenthesiscounter++;
                }
                else if (cmdstring == ")") {
                    parenthesiscounter--;
                    
                    if (parenthesiscounter == 0) {
                        Base* cmd;

                        if (isparentassigned) {
                            if (isparentand == true) {
                                cmd = new And(tempparent, cmd);
                                isparentassigned = false;
                            }
                            else {
                                cmd = new Or(tempparent, cmd);
                                isparentassigned = false;
                            }
                        }
                        else {
                            cmd = new Command(str1);
                            clearVector();
                        }
                        // fixme: insert else case

                        if (istempassigned) {
                            if (isand == true) {
                                And* q_and = new And(tempconnector, cmd);
                                tempconnector = q_and;
                            }
                            else {
                                Or* q_or = new Or(tempconnector, cmd);
                                tempconnector = q_or;
                            }
                        }
                        else {
                            tempconnector = cmd;
                            istempassigned = true;
                        }
                    }
                    else {
                        // nested case
                    }
                }
                else //This means there was no connector found in the current string, therefore push back; we will do this loop until we find a connector
                    str1.push_back(cmdstring);
            }

            if (!str1.empty()) { 
                Command* cmd = new Command(str1);
                clearVector();
            
                if (istempassigned) {
                    if (isand == true){
                        And* q_and = new And(tempconnector, cmd);
                        command_queue.push(q_and);
                    }
                    else {
                        Or* q_or = new Or(tempconnector, cmd);
                        command_queue.push(q_or);
                    }
                    istempassigned = false;
                }
                else 
                    command_queue.push(cmd);
            }
            else if (istempassigned) {
                command_queue.push(tempconnector);
            }
        };

        void clearVector() {
            while (!str1.empty()) {
                str1.pop_back();
            }
        };
};

#endif