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
