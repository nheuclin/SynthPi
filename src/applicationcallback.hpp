//applicationcallback.hpp

#ifndef APPLICATIONCALLBACK_H
#define APPLICATIONCALLBACK_H

#include "defs.hpp"

namespace SYNTHPI {

/*! Abstract application callback class. */
class ApplicationCallback {
public:
    /*! Virtual function to be overridden by derived class. */
    //virtual void interpretKeyPress(int key) = 0;
    
    /*! Virtual function to be overridden by derived class. */
    //virtual void setState(stateLabel_t newstate) = 0;
    
    /*! Running flag for the application. */
    bool running;
};

} //namespace SYNTHPI

#endif  //APPLICATIONCALLBACK_H
