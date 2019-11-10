#ifndef ___AND_H__
#define ___AND_H__

#include "connector.h"

using namespace std;

class And : public Connector {
    public:
        And() : Connector() {};
        And(Base* b, Base* c) : Connector(b,c) {};

        bool execute_command() {
            if (current -> execute_command())
                return next -> execute_command();
            
            return false;    
        };
};

#endif //___AND_H__