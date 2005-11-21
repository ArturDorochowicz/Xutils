
#include "Xutils.h"

/*---------------------------------------------------------------------------*/

typedef enum tagScrollDirections
{
	SCROLL_UP,
	SCROLL_DOWN
} ScrollDirection;

BOOL Scroll( ScrollDirection scrollDirection, double scrollLines )
{
	//
	// Don't know why this works, but it works.
	// Simply simulate mouse wheel scroll, without directing the messages
	// to a specific window - the OS takes care of this.
	//

	INPUT mouseInput;
	POINT mousePosition;

	// set the scrolling direction
	if( SCROLL_DOWN == scrollDirection )
		scrollLines *= -1;

	if( GetCursorPos( &mousePosition ) )
	{
		mouseInput.type = INPUT_MOUSE;
		mouseInput.mi.dwFlags = MOUSEEVENTF_WHEEL;
		mouseInput.mi.mouseData = (DWORD) ( WHEEL_DELTA * scrollLines );
		mouseInput.mi.dx = mousePosition.x;
		mouseInput.mi.dy = mousePosition.y;
		mouseInput.mi.time = 0;
		mouseInput.mi.dwExtraInfo = 0;

		SendInput( 1, &mouseInput, sizeof( mouseInput ) );

		return TRUE;
	}

	return FALSE;
}

int GetWheelScrollLines( )
{
	int scrollLines;

	SystemParametersInfo( SPI_GETWHEELSCROLLLINES, 0, &scrollLines, 0 );	
	return scrollLines;
}

/*---------------------------------------------------------------------------*/

_declspec(dllexport) void scrollup( PSTR szv, PSTR szx, BOOL (*GetVar)(PSTR, PSTR), void (*SetVar)(PSTR, PSTR), DWORD * pFlags, UINT nArgs, PSTR * szargs, PowerProServices * ppsv )
{
	// return nothing
	**szargs = '\0';
	PPServices = ppsv;

	if( CheckArgumentsCount( ServiceScrollup, nArgs ) )
	{
		double scrollLines;

		// get the argument or read it from the system
		scrollLines = strtod( szargs[1], NULL );
		
		if( 0 == scrollLines )
			scrollLines = GetWheelScrollLines( );

		Scroll( SCROLL_UP, scrollLines );	
	}
}

_declspec(dllexport) void scrolldown( PSTR szv, PSTR szx, BOOL (*GetVar)(PSTR, PSTR), void (*SetVar)(PSTR, PSTR), DWORD * pFlags, UINT nArgs, PSTR * szargs, PowerProServices * ppsv )
{
	// return nothing
	**szargs = '\0';
	PPServices = ppsv;

	if( CheckArgumentsCount( ServiceScrolldown, nArgs ) )
	{
		double scrollLines;

		// get the argument or read it from the system
		scrollLines = strtod( szargs[1], NULL );
		
		if( 0 == scrollLines )
			scrollLines = GetWheelScrollLines( );

		Scroll( SCROLL_DOWN, scrollLines );	
	}
}
