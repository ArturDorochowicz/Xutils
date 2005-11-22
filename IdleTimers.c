/**
 * Xutils
 * Copyright (C) 2005  Artur Dorochowicz
 *
 * Released under the terms of Lesser General Public License (LGPL).
 *
 */


#include "Xutils.h"


_declspec( dllexport ) void disableidletimers( PSTR szv, PSTR szx, BOOL (*GetVar)(PSTR, PSTR), void (*SetVar)(PSTR, PSTR), DWORD * pFlags, UINT nArgs, PSTR * szargs, PowerProServices * ppsv )
{
	// return nothing
	**szargs = '\0';
	PPServices = ppsv;

	if( CheckArgumentsCount( ServiceDisableidletimers, nArgs ) )
	{
		SetThreadExecutionState( ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED | ES_CONTINUOUS );

		if( 1 == strlen( szargs[1] ) && szargs[1][0] == '1' )
		{
			// use PP's built-in functionality
			PPServices->RunCmd( "ScreenSaver.disable", "", "" );
		}
	}
}

_declspec( dllexport ) void enableidletimers( PSTR szv, PSTR szx, BOOL (*GetVar)(PSTR, PSTR), void (*SetVar)(PSTR, PSTR), DWORD * pFlags, UINT nArgs, PSTR * szargs, PowerProServices * ppsv )
{
	// return nothing
	**szargs = '\0';
	PPServices = ppsv;

	if( CheckArgumentsCount( ServiceEnableidletimers, nArgs ) )
	{
		SetThreadExecutionState( ES_CONTINUOUS );

		if( 1 == strlen( szargs[1] ) && szargs[1][0] == '1' )
		{
			// use PP's built-in functionality
			PPServices->RunCmd( "ScreenSaver.enable", "", "" );
		}
	}
}
