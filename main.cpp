#include <iostream>
#include <string>
#include "IDEA.h"
using namespace std;

// ************************************************************
// From the paper, the same test key is the Hex digits below. * 
// Test key 1101 1100 0110 1111 0011 1111 0101 1001           *
//            d   c     6    f    3    f    5    9            *
// ************************************************************
int main() {
    IDEA mysecretcode;
    char byte; 
    string instring;
    string message; 
    string outstring;
      getline(cin,instring);
   
      mysecretcode.GenerateKeySchedule(instring);

      instring.clear();
   
      getline(cin,message); 

    mysecretcode.GenerateSecretCode(message);
    mysecretcode.PrintSecretCode();
    return 0;
}
