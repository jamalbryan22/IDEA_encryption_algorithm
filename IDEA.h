// ********************************************************************
// Created by David Gaitros on 11/22/23.                              *
// Object Oriented interface to the IDEA simplified algorithm.        *
// ********************************************************************

#ifndef IDEA_H
#define IDEA_H
#include <iostream>
#include<iomanip>
#include <vector>
#include <bitset>
using namespace std;
// **************************************************************
// * Non IDEA class functions.                                  *
// **************************************************************

int ModuloAdd(const int l, const int r); 
int ModuloMul(const int l, const int r); 
int ModuloXor(const int l, const int r); 
int CircularShiftLeft(const int w, int a); 
int GetNibble(const int w, int nibble); 
int ConvertChartoHex(const char x); 
char ConvertInttoChar(const int x); 
class IDEA {
public:
    void GenerateKeySchedule(const string & s);
    string & GenerateSecretCode(const string & s);
    void PrintTable();
    void PrintSecretCode();
    void PrintASCIISecretCode(); 
private:

    int key;			// 32 bit key converted from a string to an integer 
    int keyschedule[6][7];	// Key Table, 0 indexes not used [1-5][1-6] 
    void Round(const int rnum); // Generic Round , rnum can be 1,2,3 or 4 
    void Round5();		// Round 5 
    vector<int> secretcode;	// One hex digit for each word, Make it easy
    int X1,X2,X3,X4;		// X1 through X4 values 
    int step1,			// Results of step1 in each round 
	step2, 			// Results of step2 in each round
	step3, 			// Results of step3 in each round
	step4, 			// Results of step4 in each round
	step5, 			// Results of step5 in each round
	step6, 			// Results of step6 in each round
	step7, 			// Results of step7 in each round 
	step8, 			// Results of step8 in each round
	step9, 			// Results of step9 in each round
	step10;			// Results of step10 in each round
    string codeword;		// The answer 
    string keycode;             // Origial Key  
};


#endif //IDEA_H
