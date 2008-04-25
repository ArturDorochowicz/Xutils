/**
 * Xutils
 * Copyright (C) 2005  Artur Dorochowicz
 *
 * Released under the terms of Lesser General Public License (LGPL).
 *
 */


#ifndef XUTILS_H
#define XUTILS_H

#include <stdio.h>

// need Windows XP and later APIs
#define _WIN32_WINNT 0x0501
#include <windows.h>

typedef enum tagServices
{
	ServiceRunas,
	ServiceSudo,
	ServiceEjectcd,
	ServiceLoadcd,
	ServiceDisableidletimers,
	ServiceEnableidletimers,
	ServiceScrollup,
	ServiceScrolldown
} Services;

typedef struct tagPowerProServices
{
	void (*ErrMessage)(PSTR, PSTR);
	BOOL (*MatchCaption)(HWND, PSTR);
	HWND (*FindMatchingWindow)(PSTR,BOOL);
	BOOL (*IsRolled)(HWND hw);
	BOOL (*IsTrayMinned)(HWND hw);
	void (*GetExeFullPath)(HWND hw, PSTR szt);
	void (*RollUp)(HWND hw);
	void (*TrayMin)(HWND hw);
	void (*SendKeys)(PSTR sz);
	BOOL (*EvalExpr)(PSTR sz, PSTR szo);
	void (*Debug)(PSTR sz1, PSTR sz2,PSTR sz3, PSTR sz4, PSTR sz5, PSTR sz6);
	PSTR (*AllocTemp)(UINT leng);
	void (*ReturnString)(PSTR sz, PSTR* szargs);
	PSTR (*GetVarAddr)(PSTR var);
	PSTR (*SetVar)(PSTR var, PSTR val);
	void (*IgnoreNextClip)();
	void (*Show)(HWND h);
	void (*RunCmd)(PSTR szCmd, PSTR szParam, PSTR szWork);
	BOOL (*InsertStringForBar)( PSTR szStr, PSTR szCmd);
	void (*ResetFocus)();
	HWND (*NoteOpen)(PSTR szFile, PSTR szKeyWords);
	BOOL (*PumpMessages)();
	BOOL (*RegForConfig)(void ( *callback )(PSTR szList), BOOL bReg );
	void (*SetPreviousFocus)(HWND h );
	UINT (*SetDebug)(PSTR sz );
	UINT (*ScriptCancel)(PSTR sz );
	void (*GetCurrentDir)(HWND h,PSTR szt);
} PowerProServices;

extern PowerProServices * PPServices;

/**
 * Display error message.
 * Message is displayed with PPServices->ErrMessage
 * or an ordinary MessageBox, when PPServices is NULL.
 */
void ShowErrorMessage( const char * message );

/**
 * Format and display error message for error code obtained from GetLastError().
 */
void ShowLastError( );

/**
 * Check number of arguments.
 */
BOOL CheckArgumentsCount( Services service, int nArgs );

/**
 * Convert specified ANSI string to Unicode string.
 * Caller is responsible for freeing allocated wide string.
 */
BOOL ConvertMultiByteToWideChar( const char * ansiStr, wchar_t ** wideStr );

#endif // #ifndef XUTILS_H
