#include <stdio.h>		// Standard I/O library for basic input and output operations
#include <stdlib.h>		// Standard library for dynamic memory allocation and utility functions
#include <string.h>		// String library for operations like strcmp, strcpy, etc.
#include <ctype.h>		// Character type library for functions like isdigit, isspace
#include <math.h>		// Math library for mathematical operations like pow
#include "for_all.h"		// Custom header file with utility functions and declarations

struct Data{
    unsigned long long by;          // Who is calling i.e. user
    unsigned long long to;          // To whome user is calling
    char start[50];                 // Stores Time when call started or SMS was sent
    unsigned long long duration;    // Stores Duration of the call
    char SMS[10000];                // Stores the SMS sent by the user
    float amount;                   // Stores the amount Charged for either Call or SMS
    float paid                      // Stores Amount Paid by user
}


//UDFs




//Global Variable
int Data_cnt = 0;           // Stores the Data Count in Data Base
float P_charge = 0.05;      // Stores Charge for Voice call per second
float S_charge = 0.01;      // Stores Charge for SMS per Character


int main(){
    
    
    
    
    return 0;
}


