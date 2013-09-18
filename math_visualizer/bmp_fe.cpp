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

////////////////////////////////////////////////////////
//Class for work with BMP (bitmap)                    //
////////////////////////////////////////////////////////
#include "bmp_fe.h"

HBITMAP BitmapCapture::createBitmap(HWND hSrc) 
{
	RECT rc;
	HDC  hDC;
	
	hDC=GetDC(hSrc); //capture window DC
	HDC hdcCompatible = CreateCompatibleDC(hDC);           //create compatible Device Context
	GetClientRect(hSrc,&rc);
	HBITMAP hbmScreen = CreateCompatibleBitmap(hDC, rc.right, rc.bottom); //create bitmap 
 
	HGDIOBJ old = SelectObject(hdcCompatible, hbmScreen);  //assign with compatible DC
	
	BitBlt(hdcCompatible, 0,0, rc.right, rc.bottom,        //wrote image to bitmap through DC
               hDC, 0,0, SRCCOPY);

	SelectObject(hdcCompatible, old);   // select previous DC
	DeleteObject(hdcCompatible);        // delete compatible DC
	ReleaseDC(hSrc,hDC);                // release captured DC 
	DeleteDC(hDC);                      // delete DC
	return hbmScreen;
}

PBITMAPINFO BitmapCapture::createBitmapInfoStruct(HBITMAP hBmp)
{ 
    BITMAP      bmp; 
    PBITMAPINFO pbmi; 
    WORD        cClrBits; 

    // get colour format and bitmap dimension. 
    GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp);

    // color format to bits ammount 
    cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel); 
    if (cClrBits == 1) 
        cClrBits = 1; 
    else if (cClrBits <= 4) 
        cClrBits = 4; 
    else if (cClrBits <= 8) 
        cClrBits = 8; 
    else if (cClrBits <= 16) 
        cClrBits = 16; 
    else if (cClrBits <= 24) 
        cClrBits = 24; 
    else cClrBits = 32; 

    // Allocate memory for BITMAPINFO struct (which consist of 
    // BITMAPINFOHEADER struct and RGBQUAD struct array) 

     if (cClrBits != 24) 
         pbmi = (PBITMAPINFO) LocalAlloc(LPTR, 
                    sizeof(BITMAPINFOHEADER) + 
                    sizeof(RGBQUAD) * (1<< cClrBits)); 
     else 
		 // 24-bpp format do not use RGBQUAD struct. 
         pbmi = (PBITMAPINFO) LocalAlloc(LPTR, 
                    sizeof(BITMAPINFOHEADER)); 

    // init BITMAPINFO struct 
    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
    pbmi->bmiHeader.biWidth = bmp.bmWidth; 
    pbmi->bmiHeader.biHeight = bmp.bmHeight; 
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes; 
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel; 
    if (cClrBits < 24) 
        pbmi->bmiHeader.biClrUsed = (1<<cClrBits); 

    // if bmp not compressed - use flag BI_RGB. 
    pbmi->bmiHeader.biCompression = BI_RGB; 

    // calculates ammount of bits in colour indexes 
    // and save result into biSizeImage. 
    // Windows NT required DWORD for width, 
    // Windows 95/98/Me required WORD for width 
    // This is caused by RLE-compression.
    pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits +31) & ~31) /8
                                  * pbmi->bmiHeader.biHeight;

    // should use biClrImportant = 0,  
    // all supported colours are important 
     pbmi->bmiHeader.biClrImportant = 0; 
	 return pbmi; 
 } 

bool BitmapCapture::saveToFile(LPTSTR pszFile, HWND hSrc) 
 { 
    HANDLE  hf;                 // file handle 
	HDC     hDC;
	HBITMAP hBmp;				
	PBITMAPINFO hBmpInfo;
    BITMAPFILEHEADER hdr;       // bitmap file header 
    PBITMAPINFOHEADER pbih;     // bitmap info header 
    LPBYTE lpBits;              // memory ptr
    DWORD  dwTotal;             // total ammount of bytes
    DWORD  cb;                  // calculated ammount of bytes
    BYTE  *hp;                  // ptr to bytes
    DWORD  dwTmp; 

	hBmp = createBitmap(hSrc);
	hBmpInfo = createBitmapInfoStruct(hBmp);
    pbih = (PBITMAPINFOHEADER) hBmpInfo;  
    lpBits = (LPBYTE) LocalAlloc(GMEM_FIXED, pbih->biSizeImage);

    // get color table  (array RGBQUAD) and bits 
    // (array of pallete indexes) with DIB. 
	hDC = GetDC(hSrc);
    GetDIBits(hDC, hBmp, 0, (WORD) pbih->biHeight, lpBits, hBmpInfo, 
        DIB_RGB_COLORS);
	ReleaseDC(hSrc, hDC);

    // create <pszFile>.BMP file. 
    hf = CreateFile(pszFile, 
                   GENERIC_READ | GENERIC_WRITE, 
                   (DWORD) 0, 
                    NULL, 
                   CREATE_ALWAYS, 
                   FILE_ATTRIBUTE_NORMAL, 
                   (HANDLE) NULL); 
	if (hf == INVALID_HANDLE_VALUE) {
		LocalFree((HGLOBAL)lpBits);
		DeleteObject(hBmp);
		LocalFree(hBmpInfo);
		return EXIT_FAILURE;
	}

    hdr.bfType = 0x4d42;        // bmp file magic: 0x42 = "B" 0x4d = "M" 

    // calculate file size. 
    hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) + 
                 pbih->biSize + pbih->biClrUsed 
                 * sizeof(RGBQUAD) + pbih->biSizeImage); 
    hdr.bfReserved1 = 0; 
    hdr.bfReserved2 = 0; 

    // calculate offset for pallete indexes array 
    hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + 
                    pbih->biSize + pbih->biClrUsed 
                    * sizeof (RGBQUAD); 

    // copy BITMAPFILEHEADER to .BMP file
    WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER), 
        (LPDWORD) &dwTmp,  NULL); 

    // copy BITMAPINFOHEADER and RGBQUAD array to file 
    WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER) 
                  + pbih->biClrUsed * sizeof (RGBQUAD), 
                  (LPDWORD) &dwTmp, ( NULL));

    // copy pallete indexes array to .BMP file 
    dwTotal = cb = pbih->biSizeImage; 
    hp = lpBits; 
    WriteFile(hf, (LPSTR) hp, (int) cb, (LPDWORD) &dwTmp,NULL); 

    // close .BMP file handle 
    CloseHandle(hf); 

    // Deallocate memory. 
    LocalFree((HGLOBAL)lpBits);
	DeleteObject(hBmp);
	LocalFree(hBmpInfo);

	return EXIT_SUCCESS;
}
