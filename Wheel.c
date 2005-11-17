
#include "Xutils.h"

#include <stdio.h>

/*---------------------------------------------------------------------------*/

const int SCROLL_WINDOW_UNDER_MOUSE = 1;
const int SCROLL_FOREGROUND_WINDOW = 2;

const int SCROLL_UP = 1;
const int SCROLL_DOWN = 2;

/*---------------------------------------------------------------------------*/

void ScrollForegroundWindow( int scrollDirection, double deltaMultiple )
{
	//
	// Don't know why this works, but it works.
	// Simply simulate mouse wheel scroll, without directing the messages to a specific window - the OS takes care of this
	//

	INPUT mouseInput;
	POINT mousePosition;
	
	// set the scrolling direction
	if( SCROLL_DOWN == scrollDirection )
		deltaMultiple *= -1;

	if( FALSE == GetCursorPos( &mousePosition ) )
		return;

	mouseInput.type = INPUT_MOUSE;
	mouseInput.mi.dwFlags = MOUSEEVENTF_WHEEL;
	mouseInput.mi.mouseData = WHEEL_DELTA * deltaMultiple;
	mouseInput.mi.dx = mousePosition.x;
	mouseInput.mi.dy = mousePosition.y;
	mouseInput.mi.time = 0;
	mouseInput.mi.dwExtraInfo = 0;

	SendInput( 1, &mouseInput, sizeof( mouseInput ) );
}

/*---------------------------------------------------------------------------*/

void ScrollWindowUnderMouse( int scrollDirection, double deltaMultiple )
{
	//
	// Send WM_MOUSEWHEEL to window under the mouse cursor
	//

	POINT mousePosition;
	WPARAM wParam; 
	LPARAM lParam;
	HWND window = NULL;

	// set the scrolling direction
	if( SCROLL_DOWN == scrollDirection )
		deltaMultiple *= -1;

	//
	// Get current mouse position and handle of window under the mouse cursor
	//
	if( TRUE == GetCursorPos( &mousePosition ) )
	{			
		window = WindowFromPoint( mousePosition );
	}

	if( window != NULL )
	{
		wParam = MAKEWPARAM( 0, WHEEL_DELTA * deltaMultiple );
		lParam = MAKELPARAM( mousePosition.x, mousePosition.y );
		PostMessage( window, WM_MOUSEWHEEL, wParam, lParam );
	}
}

/*---------------------------------------------------------------------------*/

void ScrollWheel( PSTR * szargs, int nArgs, int scrollDirection, int whichWindow )
{
	double deltaMultiple;

	// return nothing
	**szargs = '\0';

	if (nArgs > 1)
	{
		PPServices->ErrMessage( "The service needs at most one argument.", NULL );
		return;
	}

	// get the argument or read it from the system
	if( 1 == nArgs )
	{
		char * stoppingChar;
		deltaMultiple = max( 1, strtod( szargs[1], &stoppingChar ) );
	}
	else
	{
		int temp;
		SystemParametersInfo( SPI_GETWHEELSCROLLLINES, 0, &temp, 0 );
		deltaMultiple = temp;
	}

	if( SCROLL_FOREGROUND_WINDOW == whichWindow )
		ScrollForegroundWindow( scrollDirection, deltaMultiple );
	else if( SCROLL_WINDOW_UNDER_MOUSE == whichWindow )
		ScrollWindowUnderMouse( scrollDirection, deltaMultiple );
	else
		return;	
}

/*---------------------------------------------------------------------------*/

_declspec(dllexport) void under_mouse_up( PSTR szv, PSTR szx, BOOL (*GetVar)(PSTR, PSTR), void (*SetVar)(PSTR, PSTR), DWORD * pFlags, UINT nArgs, PSTR * szargs, PowerProServices * ppsv )
{
	**szargs = 0;
	PPServices = ppsv;	

	ScrollWheel( szargs, nArgs, SCROLL_UP, SCROLL_WINDOW_UNDER_MOUSE );	
}

_declspec(dllexport) void under_mouse_down( PSTR szv, PSTR szx, BOOL (*GetVar)(PSTR, PSTR), void (*SetVar)(PSTR, PSTR), DWORD * pFlags, UINT nArgs, PSTR * szargs, PowerProServices * ppsv )
{
	**szargs = 0;
	PPServices = ppsv;

	ScrollWheel( szargs, nArgs, SCROLL_DOWN, SCROLL_WINDOW_UNDER_MOUSE );	
}

_declspec(dllexport) void active_up( PSTR szv, PSTR szx, BOOL (*GetVar)(PSTR, PSTR), void (*SetVar)(PSTR, PSTR), DWORD * pFlags, UINT nArgs, PSTR * szargs, PowerProServices * ppsv )
{
	**szargs = 0;
	PPServices = ppsv;

	ScrollWheel( szargs, nArgs, SCROLL_UP, SCROLL_FOREGROUND_WINDOW );	
}

_declspec(dllexport) void active_down( PSTR szv, PSTR szx, BOOL (*GetVar)(PSTR, PSTR), void (*SetVar)(PSTR, PSTR), DWORD * pFlags, UINT nArgs, PSTR * szargs, PowerProServices * ppsv )
{
	**szargs = 0;
	PPServices = ppsv;

	ScrollWheel( szargs, nArgs, SCROLL_DOWN, SCROLL_FOREGROUND_WINDOW );	
}
