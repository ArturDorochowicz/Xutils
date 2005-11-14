/**
 * Xutils plugin for PowerPro
 * Version 0.1
 *
 * Originally created by Artur Dorochowicz in 2005.
 * Public domain - no copyrights.
 *
 */

#include <stdio.h>
#include "Xutils.h"

PowerProServices * PPServices = NULL;

/**
 * Library's main entry point, no special processing needed
 *
 */
BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD dwReason, PVOID lpvReserved )
{
	return TRUE;
}

void ShowErrorMessage( char * message )
{
	if( PPServices != NULL )
	{
		PPServices->ErrMessage( message, NULL );
	}
	else
	{
		MessageBox( 0, message, "Error", MB_OK );
	}
}

void ShowLastError( )
{
	char buffer [1024];
	void * messageBuffer;
	DWORD errorCode = GetLastError( );

	FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		errorCode,
		MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
		(char * ) &messageBuffer,
		0, NULL );

	sprintf( buffer, "Failed with error %d: %s", errorCode, messageBuffer );
	LocalFree( messageBuffer );

	ShowErrorMessage( buffer );
}

BOOL CheckArgumentsCount( int nArgs )
{
	BOOL nArgsOk = TRUE;

	if( nArgs > 4 )
	{
		nArgsOk = FALSE;
		ShowErrorMessage( "The service needs at most four arguments." );
	}
	else if( nArgs < 1 )
	{
		nArgsOk = FALSE;
		ShowErrorMessage( "The service needs at least one argument." );
	}

	return nArgsOk;
}

_declspec( dllexport ) void eject( PSTR szv, PSTR szx, BOOL (*GetVar)(PSTR, PSTR), void (*SetVar)(PSTR, PSTR), DWORD * pFlags, UINT nArgs, PSTR * szargs, PowerProServices * ppsv )
{
	PPServices = ppsv;
	**szargs = '\0';

	if( TRUE == CheckArgumentsCount( nArgs, ppsv ) )
	{

	}
}

int main()
{
	PowerProServices ppsv;
	PPServices = &ppsv;
	test();

}