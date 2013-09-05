#include "mle.h"
#include "str_pp.h"

MathDraw* MathDraw::mle_ = nullptr;
const float MathDraw::ACCURACY  = 0.005f;

///////////////////////////////////////////////
////////	Math Analys and Build Func
///////////////////////////////////////////////

// Init members
MathDraw::MathDraw()
{
	alterStartPt_=startPt_=-100;
	scale_=1;
	formulaLoaded_=false;
	drawData_=nullptr;
}

MathDraw::~MathDraw()
{
	if (drawData_ != nullptr) delete[] drawData_;
}

// dealocate memory
void MathDraw::freeMemory()
{
	if (mle_ != nullptr) delete mle_;
}

// fill array of points using formula 
bool MathDraw::loadData(char* cFSTR, int iLong)
{
	if (formulaLoaded_)
	{
		if (drawData_!=nullptr) delete[] drawData_;
		drawData_ = preLoadFormula(cFSTR,iLong);
		return EXIT_SUCCESS;
	}

	return EXIT_FAILURE;
}

// analize of the formula
DWPOINT* MathDraw::preLoadFormula(char* cFSTR, int iLong) 
{
	int lf=0,inner=0,inner_end=0, prefrmd=0;
	int  ib=0,ki=0,pwi=0,InrStrC=0;
	int non_sens=0;
	DWPOINT* preRes;
	preRes=NULL;  // set in null
	
	if(cFSTR=="") return NULL; // return if null
	
	// if string is number, then buld point for line
	if (StringAnalizer::isStrNumeric(cFSTR,iLong))  preRes = buildLine(atof(cFSTR),POINT_COUNT);
	// do multiplication 
	else if (StringAnalizer::isMltpl(cFSTR,iLong))  preRes = buildMltpl(cFSTR,iLong,POINT_COUNT);
	// do division
	else if (StringAnalizer::isDiv(cFSTR,iLong))	preRes = buildDiv(cFSTR,iLong,POINT_COUNT);
	// do substraction
	else if (StringAnalizer::isMinus(cFSTR,iLong))  preRes = buildMinus(cFSTR,iLong,POINT_COUNT);
	// do powering
	else if (StringAnalizer::isPow(cFSTR,iLong))    preRes = buildPow(cFSTR,iLong,POINT_COUNT);
	// do addition
	else if (StringAnalizer::isPlus(cFSTR,iLong))   preRes = buildPlus(cFSTR,iLong,POINT_COUNT);
	else
	{
		// if none was returned, continue
		for (int i=0; i<iLong; i++)
		{
			char tmp[100];
			char inrbuf[100];

			tmp[ib++]=cFSTR[i];
			if (tmp[ib-1]=='(') // reached '(' - next data may be func argument of...
			{
				tmp[ib-1]='\0';
				if(non_sens==0)
				{
					if(tmp[0]=='s' && tmp[1]=='i' && tmp[2]=='n') lf=1; //...sin
					if(tmp[0]=='c' && tmp[1]=='o' && tmp[2]=='s') lf=2; //...cos
					if(tmp[0]=='t' && tmp[1]=='a' && tmp[2]=='n') lf=3; //...tan
				}
				inner++;
				non_sens++;
				ib=0;
			}
			if (tmp[ib-1]==')') // reached ')'; end of the argument
			{		
				tmp[ib-1]='\0';
				inner--;
				ib=0;
				non_sens--;
				inner_end=1;
			}
			if (!inner && tmp[ib-1]=='x') // func Х()
			{
				lf=4;
				ib=0;
			}

			if(inner) // if inside "(..)" ...
			{
				inrbuf[InrStrC++]=cFSTR[i+1]; // ...write-up content 
			
			}

			if(!inner && inner_end) // exited from "(..)"
			{
				inrbuf[InrStrC-1]='\0';

				// send content recursively
				preRes=preLoadFormula(inrbuf, InrStrC);
				inner_end=0;
			}
		}
	}
		
    switch (lf) // select correct function
	{
	case 1:
		return buildSin(preRes,POINT_COUNT);
	case 2:
		return buildCos(preRes,POINT_COUNT);
	case 3:
		return buildTan(preRes,POINT_COUNT);
	case 4:
		return buildX(POINT_COUNT);
	default: 
		return preRes; // or return as is
	}
	 
}

///////////////////////////////////////////////
////////	MLE Build Func
///////////////////////////////////////////////

// get operands from string 
bool MathDraw::getOperands(const char* str, int n, DWPOINT **pO1, DWPOINT **pO2, char a)
{
	int non_sens=0; 
	bool next_st=0, old_st_pt=0;
	char sOp1[100], sOp2[100];
	int  idx=0, n1=0, n2=0;

	for(int i=0; i<n; i++)
	{	
		// take to account brackets
		if(str[i]=='(') non_sens++;
		if(str[i]==')') non_sens--;
		
		if(non_sens==0 && !next_st && str[i]==a) //reached the sign of operation
		{
			// save first operand, start to form secont operand
			next_st=1; sOp1[idx]='\0'; idx=0; i++;
		}
		if(!next_st) { sOp1[idx]=str[i]; n1=idx+2; }
		if(next_st)  { sOp2[idx]=str[i]; n2=idx+1; }
		idx++;
	}	
	sOp2[idx+1]='\0';

	// check that for (a^(1/b)) and (a/b) start point begins with 0;
	if ( (a=='^' && StringAnalizer::isStrNumeric(sOp2,n2) && atof(sOp2)<1) || (a=='/') ) alterStartPt_=0;
	else alterStartPt_=startPt_;
	
	// recurcively analize each operand
	// if any of operands is NULL then result = null
	*pO1=preLoadFormula(sOp1,n1);
	if (pO1==NULL) return EXIT_FAILURE;

	*pO2=preLoadFormula(sOp2,n2);
	alterStartPt_=startPt_;

	if (pO2==NULL)
	{
	   if (pO1!=NULL) delete[] pO1;
	   return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

// common build function for string data
DWPOINT* MathDraw::doBuild(char* str, int n, int ptCnt, char a, double (MathDraw::* func)(double, double))
{
	DWPOINT *pO1=NULL;
	DWPOINT *pO2=NULL;

	// get operands, if any
	if ( getOperands(str, n, &pO1, &pO2, a) ) return NULL;
	return doBuild(pO1,pO2,ptCnt,func);
}

// common build function for DWPOINT data
DWPOINT* MathDraw::doBuild(DWPOINT *pO1, DWPOINT *pO2, int ptCnt, double (MathDraw::* func)(double, double))
{
	DWPOINT *pRes;
	pRes=new DWPOINT[ptCnt];
 
	// do correct operation on array
	// using callback function
	for (int i=0; i<ptCnt; i++)
	{
		pRes[i].x=pO1[i].x;
		if (pO2 != NULL )
			pRes[i].y=(this->*func)(pO1[i].y,pO2[i].y);
		else
			pRes[i].y=(this->*func)(pO1[i].y,0);
	}
	
	// remove operands
	delete[] pO1;
	delete[] pO2;

	// and return result
	return pRes;
}

// common build function for one argument data
DWPOINT* MathDraw::doBuild(double d, int ptCnt, double (MathDraw::* func)(double))
{
	DWPOINT* pPoint;
	double ox=alterStartPt_; 

	// allocate memmory for result
	pPoint=new DWPOINT[ptCnt]; 

	// build result using corresponding
	// callback function
	for (int j=0; j<POINT_COUNT; j++)
	{
		pPoint[j].x=ox;
		pPoint[j].y=d;
		d=(this->*func)(d);
		ox=dltX(ox);
	}

	return pPoint;
}


// build func(N)
DWPOINT* MathDraw::buildLine(double dg,int ptCnt) 
{
	return doBuild(dg,ptCnt,&MathDraw::calcLine);
}

// build func(X)
DWPOINT* MathDraw::buildX(int ptCnt) 
{
	return doBuild((double)alterStartPt_,ptCnt,&MathDraw::dltX);
}

// NOTE: fo trigonometry func
// double angle = ((3.14 / 180) * gPt[i].y); // should uncomment for grad units

// build Sin(a)
DWPOINT* MathDraw::buildSin(DWPOINT* gPt,int ptCnt) 
{
	// if got null array - return null 
	if (gPt==NULL) return NULL;

	return doBuild(gPt,NULL,ptCnt,&MathDraw::calcSin);
}

// build Cos(a)
DWPOINT* MathDraw::buildCos(DWPOINT* gPt,int ptCnt)
{
	// if got null array - return null 
	if (gPt==NULL) return NULL;

	return doBuild(gPt,NULL,ptCnt,&MathDraw::calcCos);
}

// build Tan(a)
DWPOINT* MathDraw::buildTan(DWPOINT* gPt,int ptCnt)
{
	// if got null array - return null 
	if (gPt==NULL) return NULL;

	return doBuild(gPt,NULL,ptCnt,&MathDraw::calcTan);
}

// build (a*b)
DWPOINT* MathDraw::buildMltpl(char* str, int n, int ptCnt) //умножаем
{
	return doBuild(str, n, ptCnt, '*', &MathDraw::calcMltpl);
}

// build (a/b)
DWPOINT* MathDraw::buildDiv(char* str, int n, int ptCnt)
{
	return doBuild(str, n, ptCnt, '/', &MathDraw::calcDiv);
}

// build (a+b)
DWPOINT* MathDraw::buildPlus(char* str, int n, int ptCnt)
{
	return doBuild(str, n, ptCnt, '+', &MathDraw::calcPlus);
}

// build (a-b)
DWPOINT* MathDraw::buildMinus(char* str, int n, int ptCnt)
{
	return doBuild(str, n, ptCnt, '-', &MathDraw::calcMinus);
}

// build (a^b) - powering
DWPOINT* MathDraw::buildPow(char* str, int n, int ptCnt)
{
	return doBuild(str, n, ptCnt, '^', &MathDraw::calcPow);
}

///////////////////////////////////////////////
////////	Graphical Func
///////////////////////////////////////////////

// draw image for custom math function
bool MathDraw::buildGraphic(HDC hDC, HWND hTarget, int shx, int shy) 
{
	POINT* tPt;
	RECT tRC;

	// if everything is ready
	if (formulaLoaded_ && drawData_!=nullptr)
	{
		// select red pen
		HPEN pen=CreatePen(PS_SOLID,0,RGB(255,0,0)); 
		HGDIOBJ old=SelectObject(hDC,pen);
		tPt=new POINT[POINT_COUNT];

		int ox,oy, ox1,oy1, cx, cy;

		// calculate scale
		int iscl=PIXEL_IMPL_FACTOR*scale_; 

		// shift start point
		GetClientRect(hTarget,&tRC);	
		ox=tRC.left;   ox1=tRC.right;
		oy=tRC.bottom; oy1=tRC.top;

		cx=tRC.right-shx*iscl;
		cy=tRC.bottom-shy*iscl;

		// align points to start point 
		for (int i=0; i<POINT_COUNT; i++) 
		{
			tPt[i].x=cx + static_cast<int>(drawData_[i].x*iscl);
			tPt[i].y=cy - static_cast<int>(drawData_[i].y*iscl);
		}

		// draw
		Polyline(hDC,tPt,POINT_COUNT); 

		SelectObject(hDC,old);
		DeleteObject(pen);

		// remove temporary data
		delete[] tPt; 

		// inform SUCCESS
		return EXIT_SUCCESS;
	}
	// else inform FAILURE
	return EXIT_FAILURE;
}

// draw grid
void MathDraw::buildGrid(HDC hDC,HWND hTarget,int shx,int shy, int ptCnt)
{
	RECT tRC;
	int ox,oy, ox1,oy1, cx, cy;

	// calculate scale
	int iscl=PIXEL_IMPL_FACTOR*scale_;

	// select green pen for common grid lines
	HPEN pen=CreatePen(PS_SOLID,0,RGB(0,255,0)); 
	HGDIOBJ old=SelectObject(hDC,pen);

	// shift start point
	GetClientRect(hTarget,&tRC);	
	ox=tRC.left;   ox1=tRC.right;
	oy=tRC.bottom; oy1=tRC.top;

	cx=tRC.right-shx*iscl;
	cy=tRC.bottom-shy*iscl;
 
	// draw vertical lines
	for (int i=ox; i<ox1; i+=iscl) 
	{
		MoveToEx(hDC,ox1-i,oy1,NULL);
		LineTo(hDC, ox1-i, oy);
	}

	// draw horizontal lines
	for (int i=oy1; i<oy; i+=iscl)
	{
		MoveToEx(hDC,ox,oy-i,NULL);
		LineTo(hDC, ox1, oy-i);
	}

	SelectObject(hDC,old);
	DeleteObject(pen);

	// select black pen for main grid lines
	pen=CreatePen(PS_SOLID,0,RGB(0,0,0)); 
	old=SelectObject(hDC,pen);

	// draw two main lines
	MoveToEx(hDC,cx, startPt_*PIXEL_IMPL_FACTOR+cx,NULL);
	LineTo(hDC, cx, (-startPt_)*PIXEL_IMPL_FACTOR+cx);
	MoveToEx(hDC, startPt_*PIXEL_IMPL_FACTOR+cy,cy,NULL);
	LineTo(hDC, (-startPt_)*PIXEL_IMPL_FACTOR+cy, cy);
	
	// free memory
	SelectObject(hDC,old);
	DeleteObject(pen);
}

void MathDraw::buildGrid(HDC hDC, HWND hTarget, int shx, int shy)
{
	buildGrid(hDC, hTarget, shx, shy, POINT_COUNT);
}

// draw pointing line
bool MathDraw::showMLine(HDC hDC, HWND hWnd, int hshift, POINT CCoord, char *&bx, char *&by) 
{
	
	double cur_x, cur_y;
	RECT tRC;
	int cx;

	// if everything is ready
	if (formulaLoaded_ && drawData_!=nullptr)
	{
		GetClientRect(hWnd, &tRC); 

		cx=tRC.right-hshift*scale_*10;

		// draw line
		MoveToEx(hDC, CCoord.x,0,NULL);
		LineTo(hDC, CCoord.x, 800);
	
		// calculate coords
		cur_x=((double)(CCoord.x-cx)/(scale_*10));
		cur_y=getDWPX(drawData_,POINT_COUNT,cur_x);

		// return coords 
		bx=ftoa(cur_x,5);
		by=ftoa(cur_y,5);

		// inform SUCCESS
		return EXIT_SUCCESS;
	}
	// else inform FAILURE
	return EXIT_FAILURE;
}


//calculate Y from X
double MathDraw::getDWPX(const DWPOINT* tPT, int n, double d)
{
	// calculate scale
	int scale=scale_*PIXEL_IMPL_FACTOR;
	double res=0;

	// find X point according to accuracy
	// return corresponding Y point
	for(int i=0; i<n;i++)
		if ( ( tPT[i].x > ( d - ACCURACY ) ) && 
		( tPT[i].x < ( d + ACCURACY ) ) ) res = tPT[i].y;

	return res;
}





