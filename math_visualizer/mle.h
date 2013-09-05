////////////////
//  mle.h     //
////////////////

#include <windows.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>

#pragma once

// structure for point of double type
typedef struct tagDWPOINT
{
    double  x;
    double  y;
} DWPOINT, *PDWPOINT, NEAR *NPDWPOINT, FAR *LPDWPOINT;

class MathDraw
{
public:
	void freeMemory();
	static MathDraw& instance(){ 
      if (mle_ == nullptr) mle_ = new MathDraw();
	  return *mle_;
    }              

	static int getPointLimit() {return POINT_COUNT;}
	static int getPixelFactor(){return PIXEL_IMPL_FACTOR;}

	bool loadData(char* cFSTR, int iLong);

	bool buildGraphic(HDC hDC, HWND hTarget, int shx, int shy);
	void buildGrid(HDC hDC, HWND hTarget, int shx, int shy);

	bool showMLine(HDC hDC, HWND hWnd, int hshift, POINT CCoord, char *&bx, char *&by);

	int getScale(){ return scale_; }
	void scaleIncrease(){ if (scale_<MAX_SCALE) scale_++; }
	void scaleDecrease(){ if (scale_>MIN_SCALE) scale_--; }

	bool isReady(){ return formulaLoaded_; }
	void setReady(bool b){ formulaLoaded_ = b; }

private:
	MathDraw();
	~MathDraw();
	MathDraw& operator=( const MathDraw& );  
    MathDraw( const MathDraw& );

	DWPOINT* preLoadFormula(char* cFSTR, int iLong);

	void buildGrid(HDC hDC, HWND hTarget, int shx, int shy, int ptCnt);

	bool     getOperands(const char* str, int n, DWPOINT **pO1, DWPOINT **pO2, char a);
	double   getDWPX(const DWPOINT* tPT,int n, double d);

	DWPOINT* doBuild(char* str, int n, int ptCnt,char a, double (MathDraw::*func)(double, double));
	DWPOINT* doBuild(DWPOINT *pO1, DWPOINT *pO2, int ptCnt, double (MathDraw::* func)(double, double));
	DWPOINT* doBuild(double d, int ptCnt, double (MathDraw::* func)(double));

	DWPOINT* buildSin (DWPOINT* gPt, int ptCnt);
	DWPOINT* buildCos (DWPOINT* gPt, int ptCnt);
	DWPOINT* buildTan (DWPOINT* gPt, int ptCnt);
	DWPOINT* buildLine(double   dg,  int ptCnt);
	DWPOINT* buildX   (int   ptCnt);

	DWPOINT* buildMinus(char* str, int n, int ptCnt);
	DWPOINT* buildPlus (char* str, int n, int ptCnt);
	DWPOINT* buildMltpl(char* str, int n, int ptCnt);
	DWPOINT* buildDiv  (char* str, int n, int ptCnt);
	DWPOINT* buildPow  (char* str, int n, int ptCnt);

	double dltX(double a){return a+1.0/((scale_*3*PIXEL_IMPL_FACTOR)/scale_);}
	double calcLine(double a){return a;}
	inline double calcPlus(double a, double b){return a+b;}
	inline double calcMinus(double a, double b){return a-b;}
	inline double calcMltpl(double a, double b){return a*b;}
	double calcDiv(double a, double b){return ( b==0 ? 0 : a/b );}
	double calcPow(double a, double b){return pow(a,b);}
	double calcSin(double a, double b){return sin(a);}
	double calcCos(double a, double b){return cos(a);}
	double calcTan(double a, double b){return tan(a);}

	/// private members
	//////////////////////////////////////////////////
	static MathDraw* mle_;

	static const int   POINT_COUNT = 32000;     //ptn
	static const int   PIXEL_IMPL_FACTOR = 10;  //pxl_imp
	static const int   MAX_SCALE = 30;
	static const int   MIN_SCALE = 1;
	static const float ACCURACY;

	int  startPt_;
	int  scale_;
	bool formulaLoaded_;
	int alterStartPt_;

	DWPOINT* drawData_;
};
