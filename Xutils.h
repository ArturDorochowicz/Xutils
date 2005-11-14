/**
 * Xutils plugin for PowerPro
 * Version 0.1
 *
 * Originally created by Artur Dorochowicz in 2005.
 * Public domain - no copyrights.
 *
 */

#ifndef XUTILS_H
#define XUTILS_H

#include <windows.h>

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
	UINT (*SetDebug)(PSTR sz,PSTR sz2 );
	UINT (*ScriptCancel)(PSTR sz );
	void (*GetCurrentDir)(HWND h,PSTR szt);
} PowerProServices;

extern PowerProServices * PPServices;

/**
 * Display error message.
 * Message is displayed with PPServices->ErrMessage
 * or an ordinary MessageBox, when PPServices is NULL.
 */
void ShowErrorMessage( char * message );

/**
 * Format and display error message for error code obtained from GetLastError().
 */
void ShowLastError( );

/**
 * Check number of arguments.
 */
BOOL CheckArgumentsCount( int nArgs );


#endif // #ifndef XUTILS_H
