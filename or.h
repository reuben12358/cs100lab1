#ifndef ___OR_H__
#define ___OR_H__

#include "connector.h"

using namespace std;

class Or : public Connector { 
    public:
        Or() : Connector() {};
        Or(Base* b, Base* c) : Connector(b,c) {};
    
        bool execute_command() {
            // if (!(current -> execute_command())) 
            //     return next -> execute_command();
                
            // return true;
            if (current -> execute_command()) {
                return true;
            }
            return next -> execute_command();
        };
};

#endif //___OR_H__