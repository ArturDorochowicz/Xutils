/* Xutils Plugin for PowerPro
 * Copyright (c) 2005-2008 Artur Dorochowicz
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
**/


/* Somehow this is needed for INPUT & POINT in VC6&Feb2003SDK, 0x0500 = Windows 2000 */
#define WINVER         0x0500
#define _WIN32_WINNT   0x0500
#define _WIN32_WINDOWS 0x0500
#define _WIN32_IE      0x0500
#include <Windows.h>

#include "Xutils.h"


typedef enum ScrollDirections
{
	SCROLL_UP,
	SCROLL_DOWN
} ScrollDirection;


static BOOL Scroll( ScrollDirection scrollDirection, double scrollLines )
{
	/*
	 * Don't know why this works, but it works.
	 * Simply simulate mouse wheel scroll, without directing the messages
	 * to a specific window - the OS takes care of this.
	*/

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


static int GetWheelScrollLines( )
{
	int scrollLines;

	SystemParametersInfo( SPI_GETWHEELSCROLLLINES, 0, &scrollLines, 0 );	
	return scrollLines;
}



/*! <service name="ScrollUp">
/*!  <description>Emulate scrolling up with mouse scroll wheel.</description>
/*!  <argument name="scrollLinesCount" type="int" optional="true">Specifies the speed of scrolling. If this parameter is not present, the service reads the value from the system settings.</argument>
/*! </service> */
BEGIN_PPRO_SVC( scrollup )
{
	if( TRUE == CheckArgumentsCount( pp, 0, 1 ) )
	{
		double scrollLines;
		
		/* get the argument or read it from the system */
		if( 1 == pp->argc )
			scrollLines = pp->svcs->DecodeFloat( pp->argv[0] );
		else
			scrollLines = GetWheelScrollLines( );

		Scroll( SCROLL_UP, scrollLines );	
	}
}
END_PPRO_SVC


/*! <service name="ScrollDown">
/*!  <description>Emulate scrolling down with mouse scroll wheel.</description>
/*!  <argument name="scrollLinesCount" type="int" optional="true">Specifies the speed of scrolling. If this parameter is not present, the service reads the value from the system settings.</argument>
/*! </service> */
BEGIN_PPRO_SVC( scrolldown )
{
	if( TRUE == CheckArgumentsCount( pp, 0, 1 ) )
	{
		double scrollLines;

		/* get the argument or read it from the system */
		if( 1 == pp->argc )		
			scrollLines = pp->svcs->DecodeFloat( pp->argv[0] );
		else
			scrollLines = GetWheelScrollLines( );

		Scroll( SCROLL_DOWN, scrollLines );	
	}
}
END_PPRO_SVC
