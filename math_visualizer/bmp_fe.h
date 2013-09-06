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
//  bmp_fe.h     //
///////////////////
#pragma once
#include <windows.h>

class BitmapCapture 
{
public:
	static BitmapCapture& instance(){ 
      if (bmp_ == nullptr) bmp_ = new BitmapCapture();
	  return *bmp_;
    }
	void freeMemory(){ if (bmp_ != nullptr) delete bmp_; }
	bool saveToFile(LPTSTR pszFile, HWND hSrc); 
	 
private:
	BitmapCapture(){};
	~BitmapCapture(){};
	BitmapCapture& operator=( const BitmapCapture& );  
    BitmapCapture( const BitmapCapture& );
	
	HBITMAP     createBitmap(HWND hSrc);
	PBITMAPINFO createBitmapInfoStruct(HBITMAP hBmp);

	static BitmapCapture* bmp_;
};
