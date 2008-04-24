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


#include "Xutils.h"



/*! <service name="DisableIdleTimers">
/*!  <description>Disable operating system idle timers.
/*!   There are two idle timers: system idle timer and display idle timer, their
/*!   timeout values can be defined in Power Management applet of Windows Control Panel.
/*!   Calling this service prevents the system from entering the sleeping power 
/*!   state or turning off the display.</description>
/*!  <argument name="disableScreenSaver" type="int" optional="true">Specify 1 to also disable the screen saver.</argument>
/*! </service> */
BEGIN_PPRO_SVC( disableidletimers )
{
	if( CheckArgumentsCount( ServiceDisableidletimers, &pp ) )
	{
		SetThreadExecutionState( ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED | ES_CONTINUOUS );

		if( 1 == strlen( pp.argv[0] ) && pp.argv[0][0] == '1' )
		{
			// use PP's built-in functionality
			pp.svcs->RunCmd( "ScreenSaver.disable", "", "" );
		}
	}
}
END_PPRO_SVC


/*! <service name="EnableIdleTimers">
/*!  <description>Enable operating system idle timers.
/*!   There are two idle timers: system idle timer and display idle timer, their
/*!   timeout values can be defined in Power Management applet of Windows Control Panel.
/*!   Calling this service after DisableIdleTimers() reenables the system to use
/*!   the idle timers. <br />
/*!   Please note that the system keeps track of disabling/enabling requests on
/*!   a per thread basis and calling this service might not enable idle timers,
/*!   if other thread requested disabling them.
/*!   </description>
/*!  <argument name="enableScreenSaver" type="int" optional="true">Specify 1 to also enable the screen saver.</argument>
/*! </service> */
BEGIN_PPRO_SVC( enableidletimers )
{
	if( CheckArgumentsCount( ServiceEnableidletimers, &pp ) )
	{
		SetThreadExecutionState( ES_CONTINUOUS );

		if( 1 == strlen( pp.argv[0] ) && pp.argv[0][0] == '1' )
		{
			// use PP's built-in functionality
			pp.svcs->RunCmd( "ScreenSaver.enable", "", "" );
		}
	}
}
END_PPRO_SVC
