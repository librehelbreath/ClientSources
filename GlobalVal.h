#ifndef _ABOUTDLGPROC_H
#define _ABOUTDLGPROC_H

#include <windows.h>


int		G_iLevel;
char	G_cWorldServerName[32];
char	G_cPlayerName[12];
char	G_cMapName[11];
char	G_cMapMessage[30];

#ifdef _DEBUG	//	mando 030125
	int		G_iColor = 0;
	int		G_iColorMax = 15;
#endif

#endif