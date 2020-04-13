#include <INT.h>


void int_INIT(void) {
    // setting interrupt zero 
    IT0 = 1 ; // type Edge Sensitive 
    PX0 = 1 ; // Set HIGH priority  
    // setting interrupt one
    IT1 = 1; // type Edge Sensitive
    PX1 = 1 ; // Set HIGH priority  
    
    // Enable 
    EX0 = 1 ; 
    EX1 = 1 ; 

    // Global Interrupt Flag
    EA = 1; 
}

