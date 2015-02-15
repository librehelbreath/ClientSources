#include <windows.h>
#include <winbase.h>

class CMyDib
{
public:
	CMyDib(char *szFilename, unsigned long dwFilePointer);
	~CMyDib();
	void PaintImage(HDC hDC);
	WORD m_wWidthX;
	WORD m_wWidthY;
	WORD m_wColorNums;//현재 bmp의 컬러수
	LPSTR m_lpDib;
	LPBITMAPINFO m_bmpInfo; //bmp 정보헤더
};