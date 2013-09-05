////////////////////////////////////////////////////////
//Student work                                        //
//st. grp. KI-08-3 Zagnoyko D.E.                      //
//KNURE 2010                                          //
////////////////////////////////////////////////////////

#include "mn.h"
#include <commctrl.h> 

//toolbox button ID
#define IDTB_1 1700 // save
#define IDTB_2 1701 // zoom in
#define IDTB_3 1702 // zoom out
#define IDTB_4 1703 // about

// min window dimension
#define MIN_WIDTH  480
#define MIN_HEIGHT 640

HINSTANCE    hInstance;                         // main window instance
TCHAR		 szFileName[_MAX_FNAME + _MAX_EXT]; // bmp filename
OPENFILENAME of;                                // open file struct

LRESULT CALLBACK WndProc   (HWND, UINT, WPARAM, LPARAM);                           // main window proc
LRESULT CALLBACK ClntWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);  // image (client) window proc
HWND             CreateToolbar(HWND hWndPrnt);                                     // create toolbar
void             LoadStrTable();                                                   // loading resource                                               //сохранить в файл

// string data
char  szAppTitle[MAX_STRLEN], szAbout[MAX_STRLEN], szAboutTxt[MAX_STRLEN], szBtnBuild[MAX_STRLEN];
char  szGrpF[MAX_STRLEN], szGrpCoord[MAX_STRLEN], szAboutTTT[MAX_STRLEN], szSave[MAX_STRLEN];
char  szZoomIn[MAX_STRLEN],szZoomOut[MAX_STRLEN], szExitTxt[MAX_STRLEN], szExitTtl[MAX_STRLEN];
char  szErrorTxt[MAX_STRLEN],szError[MAX_STRLEN];
char  szWndClsName[MAX_STRLEN]="graph_builder",szClntClass[MAX_STRLEN]="ClntCls" ;

// windows handle
HWND  tbar;
HWND  hClntWnd, hEditWnd, hBldBtnWnd, hGrpFWnd;
HWND  hCXWnd, hCYWnd, hGrpCoordWnd, hTXWnd, hTYWnd;

// coord for windows
POINT ptGrpFormula, ptGrpCoord; 

// main intrance point
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, 
                    PSTR szCmdLine, int iCmdShow) 
{ 
     HWND         hwnd;
     MSG          msg; 
	 HACCEL		  hAccelTable;
     WNDCLASSEX   wndclass, wcls;

     //MainWndClass - main window class
     wndclass.cbSize        = sizeof(wndclass); 
     wndclass.style         = CS_HREDRAW | CS_VREDRAW; 
     wndclass.lpfnWndProc   = WndProc;
     wndclass.cbClsExtra    = 0; 
     wndclass.cbWndExtra    = 0; 
     wndclass.hInstance     = hInst; 
     wndclass.hIcon         = LoadIcon(hInst,(LPCSTR)IDI_ICON); 
     wndclass.hCursor       = LoadCursor(NULL,(LPCSTR)IDC_ARROW); 
     wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW); 
	 wndclass.lpszMenuName  = (LPCSTR)IDR_START_MENU; 
     wndclass.lpszClassName = szWndClsName; 
     wndclass.hIconSm       = LoadIcon(hInst,(LPCSTR)IDI_ICON); 
 
     RegisterClassEx(&wndclass); 
	
	 //ClntWndClass - client window class
	 wcls.cbSize			= sizeof(wndclass); 
	 wcls.style			= CS_HREDRAW | CS_VREDRAW;
	 wcls.lpfnWndProc	= ClntWndProc;
	 wcls.cbClsExtra		= 0;
	 wcls.cbWndExtra		= 0;
	 wcls.hInstance		= hInst;
	 wcls.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	 wcls.hCursor		= LoadCursor(NULL, IDC_ARROW);
	 wcls.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	 wcls.lpszMenuName	= NULL;
	 wcls.lpszClassName	= szClntClass;
	 wcls.hIconSm		= LoadIcon(NULL, IDI_APPLICATION); 
	
	 RegisterClassEx(&wcls); 

	 LoadStrTable();

	 // create windows
	 // main window
	 hwnd = CreateWindow(szWndClsName, szAppTitle,                
                          WS_OVERLAPPEDWINDOW | WS_BORDER, 
                          CW_USEDEFAULT, CW_USEDEFAULT, 
						  640, 480, 
                          NULL, NULL, hInst, NULL); 
	 
	 // image window
	 hClntWnd=CreateWindowEx(WS_EX_CLIENTEDGE, szClntClass, "",   
                              WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | WS_VSCROLL , 
                              0,0,0,0, 
                              hwnd,NULL, hInstance, NULL);

	 // group window for expression (title) 
	 hGrpFWnd=CreateWindow("button", szGrpF,                      
                              WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 
                              0,0,0,0, 
                              hwnd,NULL, hInstance, NULL);
	 
	 // group window for coord (title) 
	 hGrpCoordWnd=CreateWindow("button", szGrpCoord,              
                              WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 
                              0,0,0,0, 
                              hwnd,NULL, hInstance, NULL);
 
	 // expression input form 
	 hEditWnd=CreateWindowEx(WS_EX_CLIENTEDGE,"edit", "",        
                              WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT |ES_AUTOHSCROLL |ES_LOWERCASE , 
                              0,0,0,0, 
                              hwnd,NULL, hInstance, NULL);
	 
	 // coord X output form 
	 hCXWnd=CreateWindowEx(WS_EX_CLIENTEDGE,"edit", "",           
                              WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT |ES_AUTOHSCROLL |ES_LOWERCASE |ES_READONLY , 
                              0,0,0,0, 
                              hwnd,NULL, hInstance, NULL);

	 // coord Y output form 
	 hCYWnd=CreateWindowEx(WS_EX_CLIENTEDGE,"edit", "",           //окно отображения координаты Y функции
							  WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT |ES_AUTOHSCROLL |ES_LOWERCASE |ES_READONLY , 
                              0,0,0,0, 
                              hwnd,NULL, hInstance, NULL);

	 // coord X (title) 
	 hTXWnd=CreateWindow("static", "X:", 
                              WS_CHILD | WS_VISIBLE, 
                              0,0,0,0, 
                              hwnd,NULL, hInstance, NULL);

	 // coord Y (title) 
	 hTYWnd=CreateWindow("static", "Y:", 
                              WS_CHILD | WS_VISIBLE, 
                              0,0,0,0, 
                              hwnd,NULL, hInstance, NULL);

	 // button "Build"
	 hBldBtnWnd=CreateWindow("button", szBtnBuild,                
                              WS_CHILD | WS_VISIBLE | WS_BORDER , 
                              0,0,0,0, 
                              hwnd,NULL, hInstance, NULL);


     ShowWindow(hwnd, iCmdShow); 
     UpdateWindow(hwnd);

	 // load hotkeys table
	 hAccelTable = LoadAccelerators(hInst,(LPCSTR)IDR_ACCEL);
	 hInstance=hInst;
	 tbar=CreateToolbar(hwnd);
	
	 // main loop
     while(GetMessage(&msg, NULL, 0, 0)) 
     {
		 if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		 {
            TranslateMessage(&msg); 
            DispatchMessage (&msg);
		 }
     } 
     return msg.wParam; 
} 
 
// main window proc
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) 
{  
    int			wmId, wmEvent;
	HDC			hdc;
	PAINTSTRUCT ps;
	RECT rc;

	switch (iMsg)
	{
		// initialize
		case WM_CREATE:
		{	
			SetScrollRange(hClntWnd, SB_HORZ, 0, 255, FALSE); 
			SetScrollPos  (hClntWnd, SB_HORZ, 0, FALSE); 
			
			// fill open file struct
			of.lStructSize=sizeof(OPENFILENAME);                 
			of.hwndOwner=hwnd;
			of.hInstance=hInstance;
			of.lpstrFilter="Windows Bitmap(*.BMP)\0*.bmp\0\0"; 
			of.lpstrCustomFilter=NULL;
			of.nMaxCustFilter=0;
			of.nFilterIndex=0;
			of.lpstrFile = szFileName;
			of.nMaxFile=_MAX_PATH;
			of.lpstrFileTitle=NULL;
			of.nMaxFileTitle=_MAX_FNAME + _MAX_EXT;
			of.lpstrInitialDir=NULL;
			of.lpstrTitle=NULL;
			of.Flags = OFN_OVERWRITEPROMPT;
			of.nFileOffset=0;
			of.nFileExtension=0;
			of.lpstrDefExt="bmp";
			of.lCustData=0L;
			of.lpfnHook=NULL;
			of.lpTemplateName=NULL;
		}
		// resizing
		case WM_SIZE: 
		{
			SendMessage(tbar, TB_AUTOSIZE, 0, 0);
			GetClientRect(hwnd,&rc);
			
			ptGrpFormula.x=(rc.right-rc.left)-190;
			ptGrpFormula.y=rc.top+40;
			ptGrpCoord.x=ptGrpFormula.x;
			ptGrpCoord.y=(rc.bottom-rc.top)-100;
			
			// move elements of main window
			MoveWindow(hClntWnd,rc.left, rc.top+30, (rc.right-rc.left)-200,(rc.bottom-rc.top)-30,0);
			
			MoveWindow(hGrpFWnd,ptGrpFormula.x, ptGrpFormula.y, 180,55,0);
			MoveWindow(hEditWnd,ptGrpFormula.x+10, ptGrpFormula.y+20, 160,25,0);
			
			MoveWindow(hGrpCoordWnd,ptGrpCoord.x, ptGrpCoord.y, 180,55,0);
			MoveWindow(hTXWnd,ptGrpCoord.x+10, ptGrpCoord.y+20, 12,25,0);
			MoveWindow(hCXWnd,ptGrpCoord.x+25, ptGrpCoord.y+20, 60,25,0);
			MoveWindow(hTYWnd,ptGrpCoord.x+94, ptGrpCoord.y+20, 13,25,0);
			MoveWindow(hCYWnd,ptGrpCoord.x+110, ptGrpCoord.y+20, 60,25,0);

			MoveWindow(hBldBtnWnd,ptGrpFormula.x, (rc.bottom-rc.top)-35, 180,25,0);
			
			InvalidateRect(hwnd,NULL,1);
		}break;
		case WM_SIZING:
		{
			rc=*(LPRECT)lParam;
			
			// check min size
			if ((rc.bottom-rc.top)<MIN_WIDTH) 
				((LPRECT)lParam)->bottom=(rc.top+MIN_WIDTH);
			if ((rc.right-rc.left)<MIN_HEIGHT) 
				((LPRECT)lParam)->right=(rc.left+MIN_HEIGHT);
		}break;
		// catch commands
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 

			// if button "Build" is pressed 
			if ((HWND)lParam==hBldBtnWnd) 
			{
				int iText;
				char cText[100]="";
				
				// get expression as string
				iText=(GetWindowTextLength(hEditWnd)+1);
				GetWindowText(hEditWnd,cText,iText); 
				
				// convert expression to data
				if (cText[0]==0) MathDraw::instance().setReady(false);
				else MathDraw::instance().setReady(true);
				if (MathDraw::instance().loadData(cText,iText) == EXIT_FAILURE) MessageBox(hwnd,szErrorTxt, szError, MB_OK | MB_ICONEXCLAMATION); //выдать предупреждение об ошибке
				
				// update window area to repaint
				InvalidateRect(hClntWnd,NULL,1);
				SetFocus(hwnd);
			}

			// toolbar and menu
			switch (wmId)
			{	
				case IDTB_1:
				case IDA_SAVE:
				case IDM_FILE_SAVE:
					if (GetSaveFileName(&of))
					{
						// save image
						BitmapCapture::instance().saveToFile(szFileName,hClntWnd);
					}
					break;
				case IDTB_4:
				case IDM_ABOUT:
				   // about
				   MessageBox(hwnd,szAboutTxt, szAbout, MB_OK | MB_ICONINFORMATION); 
				   break;
				case IDA_EXIT:
				case IDM_FILE_EXIT:
				   SendMessage(hwnd,WM_CLOSE,NULL,NULL);
				   break;
			    // scale
				case IDTB_2:
				   MathDraw::instance().scaleIncrease();
				   InvalidateRect(hwnd,NULL,1);
				   break;
				case IDTB_3:
				   MathDraw::instance().scaleDecrease();
				   InvalidateRect(hwnd,NULL,1);
				   break;
				default:
				   return DefWindowProc(hwnd, iMsg, wParam, lParam);
			}
			break;
		// keyboard events / special
		case WM_KEYDOWN: 
			{
			WORD wScrollNotify = 0xFFFF; 
			bool hor=0;

			switch(wParam)
			{	
				case VK_LEFT:
					hor=1;
				case VK_UP: 
					wScrollNotify = SB_LINEUP; 
					break; 
				case VK_HOME:
					hor=1;
			    case VK_PRIOR: 
					wScrollNotify = SB_PAGEUP; 
					break; 
				case VK_END: 
					hor=1;
			    case VK_NEXT: 
					wScrollNotify = SB_PAGEDOWN; 
					break; 
				case VK_RIGHT:
					hor=1;
				case VK_DOWN: 
					wScrollNotify = SB_LINEDOWN; 
					break; 
			}
			if (wScrollNotify != -1) 
			{
				if (!hor) SendMessage(hClntWnd, WM_VSCROLL, MAKELONG(wScrollNotify, 0), 0L); 
				else SendMessage(hClntWnd, WM_HSCROLL, MAKELONG(wScrollNotify, 0), 0L); 
			}
			}
			break;
		// keyboard events / chars
		case WM_CHAR: 
			switch(wParam)
			{
			case '-':
				MathDraw::instance().scaleDecrease();
				break;
			case '=':
				MathDraw::instance().scaleIncrease();
				break;
			}
			InvalidateRect(hwnd,NULL,1);
			break;
		case WM_PAINT:
			hdc = BeginPaint(hwnd, &ps);
		    // no paint event for main window
			EndPaint(hwnd, &ps);
			break;
		// tool tips
		case WM_NOTIFY:
		{
		 switch (((LPNMHDR) lParam)->code)   		
		 {
			case TTN_NEEDTEXT: 			
			{
				LPTOOLTIPTEXT lptttext;         
				lptttext = (LPTOOLTIPTEXT) lParam;    
				lptttext->hinst = hInstance;         
				switch(lptttext->hdr.idFrom)   
				{
					case IDTB_1:
					{
						lptttext->lpszText = szSave;
					 }break;
					case IDTB_2:
					{
						lptttext->lpszText = szZoomIn;
					}break;
					case IDTB_3:
					{
						lptttext->lpszText = szZoomOut;
					}break;
					case IDTB_4:
					{
						lptttext->lpszText = szAboutTTT;
					 }break;

				}
			}
		 }
		}break;
		// close event / ask user (yes|no|cancel)
		case WM_CLOSE: 
			{
				int sel;
				sel=MessageBox(hwnd,szExitTxt, szExitTtl, MB_YESNOCANCEL | MB_ICONQUESTION);
				switch (sel)
				{
					 case IDYES:
						 if (GetSaveFileName(&of))
 						 {
							BitmapCapture::instance().saveToFile(szFileName,hClntWnd);
						 }
						 DestroyWindow(hwnd);
						 break;
					 case IDNO:
						 DestroyWindow(hwnd);
						 break;
					 case IDCANCEL:
						 break;
				 }
				 break;
			}
			break;
		// terminate app / free memory
		case WM_DESTROY :  
			BitmapCapture::instance().freeMemory();
			MathDraw::instance().freeMemory();
			PostQuitMessage(0); 
			return 0; 
		default: 
			return DefWindowProc(hwnd, iMsg, wParam, lParam); 
	}

	return 0;
} 

// load string resources
void LoadStrTable() 
{
		LoadString(hInstance,IDS_TITLE,szAppTitle, MAX_STRLEN);
		LoadString(hInstance,IDS_TITLE,szAbout, MAX_STRLEN); 
		LoadString(hInstance,IDS_ABOUT,szAboutTxt, MAX_STRLEN); 
		LoadString(hInstance,IDS_BTN_BUILD,szBtnBuild, MAX_STRLEN);
		LoadString(hInstance,IDS_GRP_FORMULA,szGrpF, MAX_STRLEN);
		LoadString(hInstance,IDS_GRP_COORD,szGrpCoord, MAX_STRLEN);

		LoadString(hInstance,IDS_SAVE,szSave, MAX_STRLEN); 
		LoadString(hInstance,IDS_ZOOM_IN,szZoomIn, MAX_STRLEN);
		LoadString(hInstance,IDS_ZOOM_OUT,szZoomOut, MAX_STRLEN);
		LoadString(hInstance,IDS_ABOUT_TTT,szAboutTTT, MAX_STRLEN);
		LoadString(hInstance,IDS_EXIT_TXT,szExitTxt, MAX_STRLEN);
		LoadString(hInstance,IDS_EXIT_TTL,szExitTtl, MAX_STRLEN);
		LoadString(hInstance,IDS_ERROR_TXT,szErrorTxt, MAX_STRLEN);
		LoadString(hInstance,IDS_ERROR,szError, MAX_STRLEN);
}

// create toolbar
HWND CreateToolbar(HWND hWndPrnt) 
{
	HWND  hTbar; 
    
 TBBUTTON tbb[] = {
  { {0}, {0}, {TBSTATE_ENABLED}, {TBSTYLE_SEP}, {0}, {0}, {0}},
  { {2}, {IDTB_1}, {TBSTATE_ENABLED}, {TBSTYLE_BUTTON}, {0}, {0}, {0}},
  { {0}, {0}, {TBSTATE_ENABLED}, {TBSTYLE_SEP}, {0}, {0}, {0}},
  { {4}, {IDTB_2}, {TBSTATE_ENABLED}, {TBSTYLE_BUTTON}, {0}, {0}, {0}},
  { {5}, {IDTB_3}, {TBSTATE_ENABLED}, {TBSTYLE_BUTTON}, {0}, {0}, {0}},
  { {0}, {0}, {TBSTATE_ENABLED}, {TBSTYLE_SEP}, {0}, {0}, {0}},
  { {6}, {IDTB_4}, {TBSTATE_ENABLED}, {TBSTYLE_BUTTON}, {0}, {0}, {0}},
  { {0}, {0}, {TBSTATE_ENABLED}, {TBSTYLE_SEP}, {0}, {0}, {0}}
	};
    
	 hTbar=CreateToolbarEx(hWndPrnt,WS_CHILD | WS_BORDER | WS_VISIBLE 	|
							TBSTYLE_WRAPABLE | TBSTYLE_TOOLTIPS,-1,6,
							hInstance,IDB_TOOLBAR,tbb,8,16,16,16,16,sizeof(TBBUTTON));

	 SendMessage(hTbar, TB_AUTOSIZE, 0, 0);
    return hTbar;
}

// image window proc
LRESULT CALLBACK ClntWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{  
	HDC			hdc;
	PAINTSTRUCT ps;
	SCROLLINFO  si;
	RECT        rt;
	static POINT  MCoord;    // coord of pointer line
	static int  h1=0,v1=0;   // horizontal and vertical shift
	static bool set_meter=0; // can call pointer line?
	char        szMagn[5]="x", buf[3]; // scale text
          
	switch (iMsg)
	{
		// init
		case WM_CREATE:
		{	
			// fill scroll bar structs
			si.cbSize = sizeof(si); 
			si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS; 
			si.nMin   = 0; 
			si.nMax   = 25;
			si.nPage  = 10; 
			si.nPos   = 1; 
			SetScrollInfo(hwnd, SB_HORZ, &si, TRUE); 
			SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
		}
		// resizing
		case WM_SIZE: 
		{
			RECT rc;
			GetClientRect(hwnd,&rc);

			// control scroll bars
			si.cbSize = sizeof(si); 
			si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS; 
			si.nMin   = 0; 
			si.nMax   = 200;
			si.nPage  = 10; 
			si.nPos   = h1 = 40; //rc.bottom/2; 
			SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);

			si.cbSize = sizeof(si); 
			si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS; 
			si.nMin   = 0; 
			si.nMax   = 200;
			si.nPage  = 10; 
			si.nPos   = v1 = 5;
			SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
		}break;
		// LMB pressed
		case WM_LBUTTONDOWN: 
			set_meter=1;
			MCoord.x = LOWORD(lParam); 
			MCoord.y = HIWORD(lParam); 
			InvalidateRect(hwnd,NULL,1);
			break;
		// mouse moved (LMB pressed)
		case WM_MOUSEMOVE: 
			if (wParam & MK_LBUTTON)
            { 
				MCoord.x = LOWORD(lParam); 
				MCoord.y = HIWORD(lParam); 
				InvalidateRect(hwnd,NULL,1);
			}
			break;
		// LB released
		case WM_LBUTTONUP: 
			set_meter=0;
			InvalidateRect(hwnd,NULL,1);
			break;
		// draw grid, image, pointer line
		case WM_PAINT: 
			char *bx, *by;
			hdc = BeginPaint(hwnd, &ps);

			// draw grid
			MathDraw::instance().buildGrid(hdc,hwnd,h1,v1); 
			// draw image
			MathDraw::instance().buildGraphic(hdc,hwnd,h1,v1);

			// draw pointer line
			if (set_meter && !MathDraw::instance().showMLine(hdc, hClntWnd, h1, MCoord, bx, by)) 
			{
				SetWindowText(hCXWnd,bx);
				SetWindowText(hCYWnd,by);
			}

			// show scale text
			_itoa(MathDraw::instance().getScale(),buf,10);
			strcat(szMagn,buf);
			GetClientRect(hwnd,&rt);
			TextOut(hdc,rt.right-25,rt.bottom-20,szMagn,strlen(szMagn));
			EndPaint(hwnd, &ps);
			break;
		// horisontal scroll bar events
		case WM_HSCROLL : 
               switch(LOWORD(wParam)) 
                    { 
                    case SB_PAGEDOWN : 
                         h1 += 15; 
						 break;
                    case SB_LINEDOWN : 
                         h1 = min(2555, h1 + 1);
                         break; 
                    case SB_PAGEUP : 
						 h1 -= 15; 
                         break;                        
                    case SB_LINEUP : 
                         h1 = max(0, h1 - 1); 
                         break; 
                    case SB_TOP : 
                         h1 = 0; 
                         break; 
                    case SB_BOTTOM : 
                         h1 = 5255;
                         break; 
                    case SB_THUMBPOSITION : 
                    case SB_THUMBTRACK : 
                         h1 = HIWORD(wParam); 
                         break; 
 
                    default : 
                         break; 
                    }
			   si.cbSize = sizeof(si); 
			   si.fMask  = SIF_POS; 
			   si.nPos   = h1; 

			   SetScrollInfo(hwnd, SB_HORZ, &si, TRUE); 
			   InvalidateRect(hwnd,NULL,1);
               break; 
		// vertical scroll bar events
	    case WM_VSCROLL: 
               switch(LOWORD(wParam)) 
                    { 
                    case SB_PAGEDOWN : 
                         v1 += 15; 
						 break;                           
                    case SB_LINEDOWN : 
                         v1 = min(255, v1 + 1); 
                         break;  
                    case SB_PAGEUP : 
						 v1 -= 15; 
                         break;                        
                    case SB_LINEUP : 
                         v1 = max(0, v1 - 1); 
                         break;  
                    case SB_TOP : 
                         v1 = 0; 
                         break;  
                    case SB_BOTTOM : 
                         v1 = 255;
                         break;  
                    case SB_THUMBPOSITION : 
                    case SB_THUMBTRACK : 
                         v1 = HIWORD(wParam); 
                         break; 
 
                    default : 
                         break; 
                    }
			   si.cbSize = sizeof(si); 
			   si.fMask  = SIF_POS; 
			   si.nPos   = v1; 

			   SetScrollInfo(hwnd, SB_VERT, &si, TRUE); 
			   InvalidateRect(hwnd,NULL,1);
               break; 
		case WM_DESTROY : 
			PostQuitMessage(0); 
			return 0; 
		default: 
			return DefWindowProc(hwnd, iMsg, wParam, lParam); 
	}

	return 0;
} 
