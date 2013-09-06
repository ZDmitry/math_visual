/*
    This file is part of math visualizer app.

    Copyright 2010      by Dmitry Zagnoyko <hiroshidi@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301  USA.
*/

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