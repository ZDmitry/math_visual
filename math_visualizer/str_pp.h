///////////////////
//  str_pp.h     //
///////////////////
#include <iostream>
#include <stdlib.h>

#pragma once

class StringAnalizer
{
public:
	static bool isStrNumeric(const char* str, int n);
	static bool isMltpl(const char* str, int n);
	static bool isPlus(const char* str, int n);
	static bool isMinus(const char* str, int n);
	static bool isDiv(const char* str, int n);
	static bool isPow(const char* str, int n);
	static bool isSin(const char* str,int n); 
	static bool isCos(const char* str,int n); 
	static bool isTan(const char* str,int n);  

private:
	static bool consistSign(const char* str, int n, char c);

};

char* ftoa(double d, int count);