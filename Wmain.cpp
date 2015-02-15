// -------------------------------------------------------------- 
//                      Helbreath Client 						  
//
//                      1998.10 by Soph
//
// --------------------------------------------------------------


#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <stdlib.h> 
#include <winbase.h>
#include <mmsystem.h>
#include <process.h>
#include "resource.h"
#include "XSocket.h"
#include "winmain.h"
#include "Game.h"
#include "GlobalDef.h"

#ifdef DEF_HTMLCOMMOM
	HWND	m_hwndWebCtrl;
#endif

#if DEF_LANGUAGE == 3
// v2.03 과금 한미르 
#import "ClientLogon.dll" no_namespace
#endif
extern "C" __declspec( dllimport) int __FindHackingDll__(char *);

// --------------------------------------------------------------

#define WM_USER_TIMERSIGNAL		WM_USER + 500
#define WM_USER_CALCSOCKETEVENT WM_USER + 600

int				G_iAddTable31[64][510], G_iAddTable63[64][510]; // 스프라이트에서 참조하는 테이블. 속도를 위해 전역 
int				G_iAddTransTable31[510][64], G_iAddTransTable63[510][64]; 

long    G_lTransG100[64][64], G_lTransRB100[64][64];
long    G_lTransG70[64][64], G_lTransRB70[64][64];
long    G_lTransG50[64][64], G_lTransRB50[64][64];
long    G_lTransG25[64][64], G_lTransRB25[64][64];
long    G_lTransG2[64][64], G_lTransRB2[64][64];

char			szAppClass[32];
HWND			G_hWnd = NULL;
HWND			G_hEditWnd = NULL;
HINSTANCE       G_hInstance = NULL;
MMRESULT		G_mmTimer;

#ifdef DEF_HTMLCOMMOM	//	Html 다이얼로그 부분..
	HWND G_hInternetWnd = NULL;
	BOOL G_bDlg1 = FALSE;
#endif


char             G_cSpriteAlphaDegree;	// 기본적으로 주간모드 

class CGame * G_pGame;

class XSocket * G_pCalcSocket = NULL;
BOOL  G_bIsCalcSocketConnected = TRUE;
DWORD G_dwCalcSocketTime = NULL, G_dwCalcSocketSendTime = NULL;

char G_cCmdLine[256], G_cCmdLineTokenA[120], G_cCmdLineTokenA_Lowercase[120], G_cCmdLineTokenB[120], G_cCmdLineTokenC[120], G_cCmdLineTokenD[120], G_cCmdLineTokenE[120];

//BOOL WINAPI DecryptFunc(LPTSTR cipher_text, LPTSTR plain_text );

// --------------------------------------------------------------

LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam, LPARAM lParam)
{ 
	if(G_pGame->GetText( hWnd, message, wParam, lParam)) return 0;

	switch (message) {
	case WM_USER_CALCSOCKETEVENT:
		// 과금 소켓이 끊겼다. 게임중에 끊겼다면 정상 사용자가 아니다.
		G_pGame->_CalcSocketClosed();
		break;
	
	case WM_CLOSE:
		if ( (G_pGame->m_cGameMode == DEF_GAMEMODE_ONMAINGAME) && ( G_pGame->m_bForceDisconn == FALSE ) )
		{
#ifdef _DEBUG
			if (G_pGame->m_cLogOutCount == -1 || G_pGame->m_cLogOutCount > 2) G_pGame->m_cLogOutCount = 1; 
#else
			if (G_pGame->m_cLogOutCount == -1 || G_pGame->m_cLogOutCount > 11) G_pGame->m_cLogOutCount = 11; 
#endif
		}
		else if (G_pGame->m_cGameMode == DEF_GAMEMODE_ONLOADING) return (DefWindowProc(hWnd, message, wParam, lParam));
		else if (G_pGame->m_cGameMode == DEF_GAMEMODE_ONMAINMENU) G_pGame->ChangeGameMode(DEF_GAMEMODE_ONQUIT);
		break;
	
	case WM_SYSCOMMAND:
		// 스크린 세이버의 동작을 막는다.
		if((wParam&0xFFF0)==SC_SCREENSAVE || (wParam&0xFFF0)==SC_MONITORPOWER) 
			return 0; 
		return DefWindowProc(hWnd, message, wParam, lParam);
			
	case WM_USER_TIMERSIGNAL:
		G_pGame->OnTimer();
		break;

	case WM_KEYDOWN:
		G_pGame->OnKeyDown(wParam);
		return (DefWindowProc(hWnd, message, wParam, lParam));
		
	case WM_KEYUP:
		G_pGame->OnKeyUp(wParam);
		return (DefWindowProc(hWnd, message, wParam, lParam));

	case WM_SYSKEYDOWN:
		G_pGame->OnSysKeyDown(wParam);
		return (DefWindowProc(hWnd, message, wParam, lParam));
		break;

	case WM_SYSKEYUP:
		G_pGame->OnSysKeyUp(wParam);
		return (DefWindowProc(hWnd, message, wParam, lParam));
		break;

	case WM_ACTIVATEAPP:

#ifdef DEF_HTMLCOMMOM	//	mando	020910	비활성 됐다가 활성 됐을때..알트+탭..
		::SetFocus(G_hWnd);
#endif

		if( wParam == 0 ) {

#ifdef DEF_HTMLCOMMOM	//	Html 다이얼로그 부분..
				G_bDlg1 = FALSE;
				EndDialog( G_hInternetWnd, 0 );			
				::SetFocus(G_hWnd);
#endif

			G_pGame->m_bIsProgramActive = FALSE;
			G_pGame->m_DInput.SetAcquire(FALSE);
		}
		else {
			G_pGame->m_bIsProgramActive = TRUE;
			G_pGame->m_DInput.SetAcquire(TRUE);
			G_pGame->m_bCtrlPressed = FALSE;// 키보드 루틴 
			
			//Alt +Tab 후 파일 변경 검사   //20030619 용주니
			if (G_pGame->bCheckImportantFile() == FALSE) {
				MessageBox(G_pGame->m_hWnd, "File checksum error! Get Update again please!", "ERROR1", MB_ICONEXCLAMATION | MB_OK);
				PostQuitMessage(0);
				return 0;
			}
			
			// v2.04 태스크 전환할 검사한다.
			if (__FindHackingDll__("CRCCHECK") != 1) {
				G_pGame->ChangeGameMode(DEF_GAMEMODE_ONQUIT);
				return NULL;
			}
		}
		return DefWindowProc(hWnd, message, wParam, lParam);

		
#ifdef DEF_HTMLCOMMOM	//	mando	다이얼로그박스가 떠있을 경우 포커스이동시 박스 닫기^^; 후루~~우끄!!
	case WM_SETFOCUS:
//		if(G_bDlg1)
//			::SetFocus(G_hInternetWnd);

		if(G_bDlg1)
		{
			short msX, msY, msZ;
			char cLB, cRB;
			DXC_dinput m_DInput2;

			if (m_DInput2.bInit(G_hWnd,G_hInstance) == FALSE) {
				MessageBox(G_hWnd, "This program requires DirectX7.0a!","ERROR",MB_ICONEXCLAMATION | MB_OK);
				return FALSE;
			}

			//	마우스입력 버튼업일경우에만....
			m_DInput2.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB);
//			m_DInput2.UpdateMouseState(0, 0, 0, &cLB, &cRB);

			//	눌린 버튼 이 없을 경우..
			if(!cLB && !cRB)
			{
//			::GetCursorPos(&pPoint);
				G_bDlg1 = FALSE;
				EndDialog( G_hInternetWnd, 0 );			
				::SetFocus(G_hWnd);
//			::SetCursorPos(pPoint.x, pPoint.y);
//			::SetCursorPos(10, 10);
			}
			else	//	다이얼로그밖으로 나가서 클릭했을때..
			{
				SetFocus(G_hInternetWnd);

				if( G_pGame->m_DDraw.m_bFullMode == TRUE )
				{
					::SetCursorPos(510, 400);
				}
				else
				{
					int cx = GetSystemMetrics(SM_CXFULLSCREEN)/2;
					int cy = GetSystemMetrics(SM_CYFULLSCREEN)/2;
					if(cy>280) cy -= 40;
					::SetCursorPos(510 + (cx-320), 400 + (cy-240));
				}

			}
//			m_DInput2.Quit();
		}
		break;
#endif

	case WM_SETCURSOR:
		SetCursor(NULL);
		return TRUE;

	case WM_DESTROY:
		OnDestroy();
		return DefWindowProc(hWnd, message, wParam, lParam);
		
	case WM_USER_GAMESOCKETEVENT:
		G_pGame->OnGameSocketEvent(wParam, lParam);
		break;

	case WM_USER_LOGSOCKETEVENT:
		G_pGame->OnLogSocketEvent(wParam, lParam);
		break;
	
	default: 
		return (DefWindowProc(hWnd, message, wParam, lParam));
	}
	
	return NULL;
}

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
               LPSTR lpCmdLine, int nCmdShow )
{
	// v1.432
	HINSTANCE hDll;
	char cSearchDll[] = "rd`qbg-ckk";
	char cRealName[12];

	// v2.171
	srand((unsigned)time(NULL));
	char *pJammer = new char[(rand() % 100) +1];
	G_pGame = new class CGame; // v2.171

	ZeroMemory(cRealName, sizeof(cRealName));
	strcpy(cRealName, cSearchDll);
	for (WORD i = 0; i < strlen(cRealName); i++)
	if (cRealName[i] != NULL) cRealName[i]++;

	hDll = LoadLibrary(cRealName);
	if( hDll == NULL ) {
		MessageBox(NULL, "don't find search.dll", "ERROR!", MB_OK);
		return 0;
	}

#ifdef DEF_USING_WIN_IME
	HINSTANCE hRichDll = LoadLibrary( "Riched20.dll" );
#endif
	typedef int (MYPROC)(char *) ;
	MYPROC *pFindHook; 
	pFindHook = (MYPROC *) GetProcAddress(hDll, "__FindHackingDll__") ;

	if (pFindHook== NULL) {
		MessageBox(NULL, "can't find search.dll", "ERROR!", MB_OK);
		return 0 ;
	}else if ((*pFindHook)("CRCCHECK") != 1) {
		return 0 ;
	}
	FreeLibrary(hDll);

	sprintf( szAppClass, "Client-I%d", hInstance);
	if (!InitApplication( hInstance))		return (FALSE);
    if (!InitInstance(hInstance, nCmdShow)) return (FALSE);

	Initialize((char *)lpCmdLine);

#ifndef _DEBUG
	if (OpenMutex(MUTEX_ALL_ACCESS, FALSE, "0543kjg3j31%") != NULL) {
		MessageBox(NULL, "Only one Helbreath client program allowed!", "ERROR!", MB_OK);
		return 0;
	}
	HANDLE hMutex = CreateMutex(NULL, FALSE, "0543kjg3j31%");
#endif
	
	EventLoop();

#ifndef _DEBUG
	ReleaseMutex(hMutex);
	CloseHandle(hMutex);
#endif

	delete[] pJammer; // v2.171
	delete G_pGame; // v2.171

#ifdef DEF_USING_WIN_IME
	FreeLibrary(hRichDll);
#endif

	return 0;
}

BOOL InitApplication( HINSTANCE hInstance)
{     
 WNDCLASS  wc;

	wc.style = (CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS);
	wc.lpfnWndProc   = (WNDPROC)WndProc;             
	wc.cbClsExtra    = 0;                            
	wc.cbWndExtra    = sizeof (int);
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadCursor(NULL, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = szAppClass;
        
	return (RegisterClass(&wc));
}

BOOL InitInstance( HINSTANCE hInstance, int nCmdShow )
{
	int cx = GetSystemMetrics(SM_CXFULLSCREEN)/2;
	int cy = GetSystemMetrics(SM_CYFULLSCREEN)/2;
	if(cy>280) cy -= 40;
//#ifdef DEF_FULL_SCREEN
	G_hWnd = CreateWindowEx(NULL, szAppClass, "Helbreath Crusade", WS_POPUP, cx-320, cy-240, 
							640, 480, NULL, NULL, hInstance, NULL);     
//#else
//	G_hWnd = CreateWindowEx( 0, szAppClass, "HClient", WS_POPUP | WS_BORDER, cx-320, cy-240,
//							640, 480, NULL, NULL, hInstance, NULL );
//#endif
    if (!G_hWnd) return FALSE;
    G_hInstance	= hInstance;

#ifdef DEF_HTMLCOMMOM	//	mando..
	#ifdef WEBCTRL_AUTOINIT
	   m_hwndWebCtrl = CreateWebControl(
		  m_szHTML,
		  WS_CHILD | WS_VISIBLE,
		  0, 0, 0, 0,
		  m_hwnd,
		  IDC_WEBCTRL );
	#else
	   InitWebCtrl();

	   m_hwndWebCtrl = CreateWindow(
		  WC_WEBCTRL,
		  "",
		  WS_CHILD | WS_VISIBLE,
		  0, 0, 0, 0,
		  G_hWnd,
		  (HMENU) IDC_WEBCTRL,
		  G_hInstance,//hInst,
		   NULL );
	#endif
#endif


	ShowWindow(G_hWnd, SW_SHOWDEFAULT);
	UpdateWindow(G_hWnd);

#ifdef DEF_HTMLCOMMOM	//	mando..
	::SetFocus(G_hWnd);
#endif

	return TRUE;
}

void EventLoop()
{
 register MSG msg;
	while( 1 ) {
		if( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ) {
			if( !GetMessage( &msg, NULL, 0, 0 ) ) return;// msg.wParam;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
		}
		else if (G_pGame->m_bIsProgramActive) G_pGame->UpdateScreen();
		else if (G_pGame->m_cGameMode == DEF_GAMEMODE_ONLOADING) G_pGame->UpdateScreen_OnLoading( FALSE );
		else WaitMessage();
	}
}

void OnDestroy()
{
	G_pGame->m_bIsProgramActive = FALSE;
		
	_StopTimer(G_mmTimer);

	G_pGame->Quit();
	WSACleanup();
#if DEF_LANGUAGE < 3 //대만과 중국만 종료시 홈페이지로 가장..
	GoHomepage();
#endif
	PostQuitMessage(0);
}

void CALLBACK _TimerFunc(UINT wID, UINT wUser, DWORD dwUSer, DWORD dw1, DWORD dw2)
{
	PostMessage(G_hWnd, WM_USER_TIMERSIGNAL, wID, NULL);
}

MMRESULT _StartTimer(DWORD dwTime)
{
 TIMECAPS caps;

	timeGetDevCaps(&caps, sizeof(caps));
	timeBeginPeriod(caps.wPeriodMin);
	return timeSetEvent(dwTime,0,_TimerFunc,0, (UINT)TIME_PERIODIC);
}

void _StopTimer(MMRESULT timerid)
{
 TIMECAPS caps;

	if (timerid != 0) {
		timeKillEvent(timerid);
		timerid = 0;
		timeGetDevCaps(&caps, sizeof(caps));
		timeEndPeriod(caps.wPeriodMin);
	}
}

#if DEF_LANGUAGE == 3
unsigned __stdcall ThreadProc(void *ch)
{
 char * cp, * token, cPacket[1024], cIPaddr[20], cPort[20], cTemp[256], cCmdLine[256];
 char seps[] = "& ";
 int iSended;
 BOOL bRet;
 
	G_bIsCalcSocketConnected = FALSE;
	// 접속 시도 시간 입력
	G_dwCalcSocketTime = timeGetTime();
	G_dwCalcSocketSendTime = G_dwCalcSocketTime;

	ZeroMemory(cPacket, sizeof(cPacket));
	cp = (char *)cPacket;
	
	if (memcmp(G_cCmdLineTokenA, "/KWG", 4) == 0) {
		// 코넷 월드
		memcpy(cp, "KwGwMGS", 7);
		cp += 8;
		memcpy(cp, "CONNECT", 7);
		cp += 8;
		memcpy(cp, G_cCmdLineTokenB, strlen(G_cCmdLineTokenB));
		cp += 16;
		memcpy(cp, G_cCmdLineTokenC, strlen(G_cCmdLineTokenC));
		cp += 16;
		memcpy(cp, G_cCmdLineTokenD, strlen(G_cCmdLineTokenD));
		cp += 16;
		memcpy(cp, G_cCmdLineTokenE, strlen(G_cCmdLineTokenE));
		cp += strlen(G_cCmdLineTokenE);

		G_pCalcSocket = NULL;
		G_pCalcSocket = new class XSocket(G_hWnd, 100); 
		bRet = G_pCalcSocket->bBlockConnect("game.kornetworld.com", 9000, NULL); 

		if (bRet == TRUE) {
			G_pCalcSocket->bInitBufferSize(30000);
			iSended = G_pCalcSocket->iSendMsgBlockingMode(cPacket, 256);
			// 소켓이 연결되었음을 표시.
			G_bIsCalcSocketConnected = TRUE;
			// 소켓을 논블록킹 모드로 
			u_long arg = 1;
			ioctlsocket(G_pCalcSocket->m_Sock, FIONBIO, &arg);
			// 이벤트 메시지 생성
			WSAAsyncSelect(G_pCalcSocket->m_Sock, G_hWnd, WM_USER_CALCSOCKETEVENT, FD_CLOSE);
		}
		else {
		}
	}
	else if (memcmp(G_cCmdLineTokenA, "/hitel", 6) == 0) {
		// 하이텔
		ZeroMemory(cIPaddr, sizeof(cIPaddr));
		strcpy(cIPaddr, (char *)(G_cCmdLineTokenB +6));
		ZeroMemory(cPort, sizeof(cPort));
		strcpy(cPort, (char *)(G_cCmdLineTokenC +8));

		strcpy(cPacket, (G_cCmdLineTokenD +4));
		strcat(cPacket, "\n");

		G_pCalcSocket = NULL;
		G_pCalcSocket = new class XSocket(G_hWnd, 100); 
		bRet = G_pCalcSocket->bBlockConnect(cIPaddr, atoi(cPort), NULL); 

		if (bRet == TRUE) {
			G_pCalcSocket->bInitBufferSize(30000);
			iSended = G_pCalcSocket->iSendMsgBlockingMode(cPacket, strlen(cPacket));
			// 소켓이 연결되었음을 표시.
			G_bIsCalcSocketConnected = TRUE;
			// 소켓을 논블록킹 모드로 
			u_long arg = 1;
			ioctlsocket(G_pCalcSocket->m_Sock, FIONBIO, &arg);
			// 이벤트 메시지 생성
			WSAAsyncSelect(G_pCalcSocket->m_Sock, G_hWnd, WM_USER_CALCSOCKETEVENT, FD_CLOSE);
		}
		else {
		}
	}
	else if (memcmp(G_cCmdLineTokenA_Lowercase, "wisetop", 7) == 0) {
		// 로그인 게임
		// !!! 로그인 게임과 게임 익시트 등의 소켓은 연결후 iSendMsgBlockingMode()로 메시지를 보내면 Xsocket 헤더가 다르기 때문에 곧바로 연결이 끊긴다.
		// 그러므로 커넥션만 유지하고 메시지는 보내지 않는다.
	
		ZeroMemory(cCmdLine, sizeof(cCmdLine));
		strcpy(cCmdLine, G_cCmdLine);

		token = strtok(cCmdLine, seps);
		while (token != NULL) {
			if (memcmp(token, "IP:", 3) == 0) {
				// IP주소
				ZeroMemory(cTemp, sizeof(cTemp));
				strcpy(cTemp, (char *)(token+3));
				ZeroMemory(cIPaddr, sizeof(cIPaddr));
				strcpy(cIPaddr, cTemp);
			}
			else if (memcmp(token, "PO:", 3) == 0) {
				// Port 넘버
				strcpy(cTemp, (char *)(token+3));
				ZeroMemory(cPort, sizeof(cPort));
				strcpy(cPort, cTemp);
			}
			token = strtok(NULL, seps);
		}
	
		// 소켓 연결
		G_pCalcSocket = NULL;
		G_pCalcSocket = new class XSocket(G_hWnd, 100); 
		bRet = G_pCalcSocket->bBlockConnect(cIPaddr, atoi(cPort), NULL); 

		if (bRet == TRUE) {
			G_pCalcSocket->bInitBufferSize(30000);
			// 소켓이 연결되었음을 표시.
			G_bIsCalcSocketConnected = TRUE;
			// 소켓을 논블록킹 모드로 
			u_long arg = 1;
			ioctlsocket(G_pCalcSocket->m_Sock, FIONBIO, &arg);
			// 이벤트 메시지 생성
			WSAAsyncSelect(G_pCalcSocket->m_Sock, G_hWnd, WM_USER_CALCSOCKETEVENT, FD_CLOSE);
		}
	}
	
	else if (memcmp(G_cCmdLineTokenA, "gameexit", 8) == 0) { 
		// 게임 익시트
		CPSocketConnet() ;
	}
	else if (memcmp(G_cCmdLineTokenA, "mezzysys", 8) == 0) {
		// 메지 시스템
		CPSocketConnet() ;
	}
	else if (memcmp(G_cCmdLineTokenA_Lowercase, "/pcbang", 7) == 0) { 
		// 게임방 닷컴
		CPSocketConnet() ;
	}
	else if (memcmp(G_cCmdLineTokenA_Lowercase, "moya", 4) == 0) { 
		// 인터코리아 모야 
		CPSocketConnet() ;
	}
	else if (memcmp(G_cCmdLineTokenA_Lowercase, "joy", 3) == 0) { 
		// 겨레정보(인포삽 조이)
		CPSocketConnet() ;
	}
	else if (memcmp(G_cCmdLineTokenA_Lowercase, "nice", 4) == 0) { 
		// 나이스 인터넷
		CPSocketConnet() ;
	}
	else if (memcmp(G_cCmdLineTokenA_Lowercase, "battletop", 9) == 0) { 
		// 배틀탑
		CPSocketConnet() ;
	}
	else if (memcmp(G_cCmdLineTokenA_Lowercase, "hanasoft", 8) == 0) { 
		// 하나소프트
		CPSocketConnet() ;
	}
	else if (memcmp(G_cCmdLineTokenA_Lowercase, "chollian", 8) == 0) { 
		// 천리안
		CPSocketConnet() ;
	}
	else if (memcmp(G_cCmdLineTokenA_Lowercase, "iloveschool", 11) == 0) { 
		// 에드빌 소프트
		CPSocketConnet() ;
	}
	else if (memcmp(G_cCmdLineTokenA_Lowercase, "dataq", 5) == 0) { 
		// 인포테라 데이타큐
		CPSocketConnet() ;
	}
	else if (memcmp(G_cCmdLineTokenA_Lowercase, "gluon", 5) == 0) { 
		// 글루온
		CPSocketConnet() ;
	}
	else if (memcmp(G_cCmdLineTokenA_Lowercase, "elthe", 5) == 0) { 
		// 큰사람커뮤터 elthe
		CPSocketConnet() ;
	}
	else if (memcmp(G_cCmdLineTokenA_Lowercase, "shinbiro", 8) == 0) { 
		// 신비로 shinbiro
		CPSocketConnet() ;
	}
	else if (memcmp(G_cCmdLineTokenA_Lowercase, "mihosoft", 8) == 0) { 
		// 하나로 v1.4334
		ZeroMemory(cCmdLine, sizeof(cCmdLine));
		strcpy(cCmdLine, G_cCmdLine);

		token = strtok(cCmdLine, seps);
		int iTokenCount = 0 ;
		while (token != NULL) {
			if (iTokenCount == 1) {
				// IP주소
				ZeroMemory(cTemp, sizeof(cTemp));
				strcpy(cTemp, (char *)(token));
				ZeroMemory(cIPaddr, sizeof(cIPaddr));
				strcpy(cIPaddr, cTemp);
			
			}
			else if (iTokenCount == 2) {
				// Port 넘버
				strcpy(cTemp, (char *)(token));
				ZeroMemory(cPort, sizeof(cPort));
				strcpy(cPort, cTemp);
			}
			iTokenCount++ ;
			token = strtok(NULL, seps);
		}
	

		// 소켓 연결
		G_pCalcSocket = NULL;
		G_pCalcSocket = new class XSocket(G_hWnd, 100); 
		bRet = G_pCalcSocket->bBlockConnect(cIPaddr, atoi(cPort), NULL); 


		if (bRet == TRUE) {
			G_pCalcSocket->bInitBufferSize(30000);
			// 소켓이 연결되었음을 표시.
			G_bIsCalcSocketConnected = TRUE;
			// 소켓을 논블록킹 모드로 
			u_long arg = 1;
			ioctlsocket(G_pCalcSocket->m_Sock, FIONBIO, &arg);
			// 이벤트 메시지 생성
			WSAAsyncSelect(G_pCalcSocket->m_Sock, G_hWnd, WM_USER_CALCSOCKETEVENT, FD_CLOSE);
		}
		HINSTANCE hDll;		
		if((hDll = LoadLibrary("cjconnector.dll")) == NULL) {
			//MessageBox(NULL, "don't find cjconnector.dll", "ERROR!", MB_OK);
			
			return 0;
		}

		typedef int (MYPROC)(SOCKET) ;
		MYPROC *pConnectorKey;
		pConnectorKey = (MYPROC *) GetProcAddress(hDll, "ConnectorKey");

		int iRet = 1 ;

		if (pConnectorKey== NULL) {
			MessageBox(NULL, "pConnectorKey is NULL", "ERROR!", MB_OK);
			FreeLibrary(hDll);
			return 0 ;
		}else if ((iRet=(*pConnectorKey)(G_pCalcSocket->m_Sock)) != 1) {
			FreeLibrary(hDll);
			return 0 ;
		}
		FreeLibrary(hDll);
	}
	else if (memcmp(G_cCmdLineTokenA_Lowercase, "korea", 5) == 0) { 
		// 코리아닷컴
		CPSocketConnet() ;
	}
	else if (memcmp(G_cCmdLineTokenA_Lowercase, "engames", 7) == 0) { 
		// 코리아닷컴
		CPSocketConnet() ;
	}
	else if (memcmp(G_cCmdLineTokenA_Lowercase, "hyunju", 6) == 0) { 
		// 현주컴퓨터
		CPSocketConnet() ;
	}
	else if (memcmp(G_cCmdLineTokenA_Lowercase, "hanstay", 7) == 0) { 
		// 한스테이닷컴
		CPSocketConnet() ;
	}
	else if (memcmp(G_cCmdLineTokenA_Lowercase, "unitel", 6) == 0) { 
		// v2.03 과금 한스테이닷컴
	char  * token, cIPaddr[20], cPort[20], cTemp[256], cCmdLine[256]; ;
	char seps[] = "& ";
	BOOL bRet;

	ZeroMemory(cCmdLine, sizeof(cCmdLine));
	strcpy(cCmdLine, G_cCmdLine);

	token = strtok(cCmdLine, seps);
	while (token != NULL) {
		if (memcmp(token, "IP:", 3) == 0) {
			// IP주소
			ZeroMemory(cTemp, sizeof(cTemp));
			strcpy(cTemp, (char *)(token+3));
			ZeroMemory(cIPaddr, sizeof(cIPaddr));
			//strcpy(cIPaddr, cTemp);
			strcpy(cIPaddr, cTemp);
		}
		else if (memcmp(token, "PO:", 3) == 0) {
			// Port 넘버
			strcpy(cTemp, (char *)(token+3));
			ZeroMemory(cPort, sizeof(cPort));
			strcpy(cPort, cTemp);
		}
		token = strtok(NULL, seps);
	}

	// 소켓 연결
	G_pCalcSocket = NULL;
	G_pCalcSocket = new class XSocket(G_hWnd, 100); 
	//bRet = G_pCalcSocket->bBlockConnect(cIPaddr, atoi(cPort), NULL);
	bRet = G_pCalcSocket->bBlockConnect("211.45.21.1", atoi(cPort), NULL);
	//G_pCalcSocket->iSendMsg( "nothing", 10, NULL );
	if (bRet == TRUE) {
		G_pCalcSocket->bInitBufferSize(30000);
		// 소켓이 연결되었음을 표시.
		G_bIsCalcSocketConnected = TRUE;
		// 소켓을 논블록킹 모드로 
		u_long arg = 1;
		ioctlsocket(G_pCalcSocket->m_Sock, FIONBIO, &arg);
		// 이벤트 메시지 생성
		WSAAsyncSelect(G_pCalcSocket->m_Sock, G_hWnd, WM_USER_CALCSOCKETEVENT, FD_CLOSE);
		G_pCalcSocket->iSendMsg( "nothing", 10, NULL );
	}
	else {
	}
	}
	else if (memcmp(G_cCmdLineTokenA_Lowercase, "/egparam", 8) == 0) { 
		// 한미르
		char bstrCmdLine[1024];
		ZeroMemory(cCmdLine, sizeof(cCmdLine));
		strcpy(cCmdLine, G_cCmdLine);

		HRESULT hr = ::CoInitialize(NULL);

		ILogInPtr pClient;
		hr = pClient.CreateInstance(__uuidof(LogIn));
		if(FAILED(hr))
		{
			MessageBox(NULL, "ERROR CreateInstance ...\n", "ERROR!", MB_OK);
			return 0;
		}
		strcpy(bstrCmdLine,GetCommandLine());

		pClient->Parameters = (_bstr_t)cCmdLine;

		ZeroMemory(cIPaddr, sizeof(cIPaddr));
		strcpy(cIPaddr, _bstr_t(pClient->IP).operator const char*( ));

		ZeroMemory(cPort, sizeof(cPort));
		wsprintf(cPort, "%d", pClient->Port);

		G_pCalcSocket = NULL;
		G_pCalcSocket = new class XSocket(G_hWnd, 100); 
		//bRet = G_pCalcSocket->bBlockConnect(cIPaddr, atoi(cPort), NULL); 
		bRet = G_pCalcSocket->bBlockConnect(cIPaddr, atoi(cPort), NULL); 
		if (bRet == TRUE) {
			G_pCalcSocket->bInitBufferSize(30000);
			// 소켓이 연결되었음을 표시.
			G_bIsCalcSocketConnected = TRUE;
			// 소켓을 논블록킹 모드로 
			u_long arg = 1;
			ioctlsocket(G_pCalcSocket->m_Sock, FIONBIO, &arg);
			// 이벤트 메시지 생성
			WSAAsyncSelect(G_pCalcSocket->m_Sock, G_hWnd, WM_USER_CALCSOCKETEVENT, FD_CLOSE);
		}
		else {
		}
		

		unsigned int nRet;
		hr = pClient->LogIn( G_pCalcSocket->m_Sock, pClient->Parameter, &nRet, 10000 );
		
	}

	G_dwCalcSocketTime = NULL;
	_endthread(); 
	return 0;
}

// v2.03 과금 
void CPSocketConnet() {
	char  * token, cIPaddr[20], cPort[20], cTemp[256], cCmdLine[256]; ;
	char seps[] = "& ";
	BOOL bRet;

	ZeroMemory(cCmdLine, sizeof(cCmdLine));
	strcpy(cCmdLine, G_cCmdLine);

	token = strtok(cCmdLine, seps);
	while (token != NULL) {
		if (memcmp(token, "IP:", 3) == 0) {
			// IP주소
			ZeroMemory(cTemp, sizeof(cTemp));
			strcpy(cTemp, (char *)(token+3));
			ZeroMemory(cIPaddr, sizeof(cIPaddr));
			strcpy(cIPaddr, cTemp);
		}
		else if (memcmp(token, "PO:", 3) == 0) {
			// Port 넘버
			strcpy(cTemp, (char *)(token+3));
			ZeroMemory(cPort, sizeof(cPort));
			strcpy(cPort, cTemp);
		}
		token = strtok(NULL, seps);
	}

	// 소켓 연결
	G_pCalcSocket = NULL;
	G_pCalcSocket = new class XSocket(G_hWnd, 100); 
	bRet = G_pCalcSocket->bBlockConnect(cIPaddr, atoi(cPort), NULL); 

	if (bRet == TRUE) {
		G_pCalcSocket->bInitBufferSize(30000);
		// 소켓이 연결되었음을 표시.
		G_bIsCalcSocketConnected = TRUE;
		// 소켓을 논블록킹 모드로 
		u_long arg = 1;
		ioctlsocket(G_pCalcSocket->m_Sock, FIONBIO, &arg);
		// 이벤트 메시지 생성
		WSAAsyncSelect(G_pCalcSocket->m_Sock, G_hWnd, WM_USER_CALCSOCKETEVENT, FD_CLOSE);
	}
	else {
	}
}
#endif

void Initialize(char * pCmdLine)
{
 int iX, iY, iSum;

 int     iErrCode;
 WORD	 wVersionRequested;
 WSADATA wsaData;

	// 소켓의 버젼을 체크한다.
	wVersionRequested = MAKEWORD( 2, 2 ); 
	iErrCode = WSAStartup( wVersionRequested, &wsaData );
	if ( iErrCode ) {
		MessageBox(G_hWnd, "Winsock-V1.1 not found! Cannot execute program.","ERROR",MB_ICONEXCLAMATION | MB_OK);
		PostQuitMessage(0);
		return;
	}

	if (G_pGame->bInit(G_hWnd, G_hInstance, pCmdLine) == FALSE) {
		PostQuitMessage(0);
		return;
	}
	
	// 0.5초에 한번씩 타이머 발생 
	G_mmTimer = _StartTimer(1000);

	for (iX = 0; iX < 64; iX++)
	for (iY = 0; iY < 510; iY++) {
		
		iSum = iX + (iY - 255);
		if (iSum <= 0)  iSum = 1;
		if (iSum >= 31) iSum = 31;
		G_iAddTable31[iX][iY] = iSum; 

		iSum = iX + (iY - 255);
		if (iSum <= 0)  iSum = 1;
		if (iSum >= 63) iSum = 63;
		G_iAddTable63[iX][iY] = iSum; 

		if ((iY - 255) < iX) G_iAddTransTable31[iY][iX] = iX;
		else if ((iY - 255) > 31) G_iAddTransTable31[iY][iX] = 31;
		else G_iAddTransTable31[iY][iX] = iY-255;

		if ((iY - 255) < iX) G_iAddTransTable63[iY][iX] = iX;
		else if ((iY - 255) > 63) G_iAddTransTable63[iY][iX] = 63;
		else G_iAddTransTable63[iY][iX] = iY-255;
	}

	// 쓰레드 통신을 위한 이벤트 생성 
//	G_hEvent1 = CreateEvent(NULL, FALSE, FALSE, NULL);
}

LONG GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata)
{
    HKEY hkey;
    LONG retval = RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);

    if (retval == ERROR_SUCCESS) {
        long datasize = MAX_PATH;
        TCHAR data[MAX_PATH];
        RegQueryValue(hkey, NULL, data, &datasize);
        lstrcpy(retdata,data);
        RegCloseKey(hkey);
    }

    return retval;
}

void GoHomepage() 
{
#if DEF_LANGUAGE == 1
	LPCTSTR	url = "http://www.helbreath.com.tw/";
#elif DEF_LANGUAGE == 2
	LPCTSTR	url = "http://www.helbreath.com.cn/";
#else
	LPCTSTR	url = "http://211.51.62.243/";
#endif
#ifdef _DEBUG
	return;
#endif
	int		showcmd = SW_SHOW;
	char	key[MAX_PATH + MAX_PATH];
	
    // First try ShellExecute()
    HINSTANCE result = ShellExecute(NULL, "open", url, NULL,NULL, showcmd);

    // If it failed, get the .htm regkey and lookup the program
    if ((UINT)result <= HINSTANCE_ERROR) {

        if (GetRegKey(HKEY_CLASSES_ROOT, ".htm", key) == ERROR_SUCCESS) {
            lstrcat(key, "\\shell\\open\\command");

            if (GetRegKey(HKEY_CLASSES_ROOT,key,key) == ERROR_SUCCESS) {
                char *pos;
                pos = strstr(key, "\"%1\"");
                if (pos == NULL) {                     // No quotes found
                    pos = strstr(key, "%1");      // Check for %1, without quotes 
                    if (pos == NULL)                   // No parameter at all...
                        pos = key+lstrlen(key)-1;
                    else
                        *pos = '\0';                   // Remove the parameter
                }
                else
                    *pos = '\0';                       // Remove the parameter

                lstrcat(pos, " ");
                lstrcat(pos, url);

                result = (HINSTANCE) WinExec(key,showcmd);
            }
        }
    }
}