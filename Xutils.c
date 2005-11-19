/**
 * Xutils plugin for PowerPro
 * Version 0.1
 *
 * Originally created by Artur Dorochowicz in 2005.
 * Public domain - no copyrights.
 *
 */


#include "Xutils.h"


PowerProServices * PPServices = NULL;

/**
 * Library's main entry point, no special processing needed
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
		MessageBox( 0, message, "Error", MB_OK | MB_ICONERROR );
	}
}

void ShowLastError( )
{
	char * messageBuffer;

	FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError( ),
		MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
		(char*) &messageBuffer,
		0, NULL );

	ShowErrorMessage( messageBuffer );
	LocalFree( messageBuffer );
}

BOOL CheckArgumentsCount( Services service, int nArgs )
{
	BOOL nArgsOk = TRUE;

	switch( service )
	{
		case sudoService:
		{
			if( nArgs > 3 )
			{
				nArgsOk = FALSE;
				ShowErrorMessage( "The service needs at most three arguments." );
			}
			else if( nArgs < 1 )
			{
				nArgsOk = FALSE;
				ShowErrorMessage( "The service needs at least one argument." );
			}
			break;
		}

		case runasService:
		{
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
			break;
		}

		case ejectcdService:
		{

			break;
		}

		case loadcdService:
		{
			break;
		}
	}

	return nArgsOk;
}

BOOL ConvertMultiByteToWideChar( const char * ansiStr, wchar_t ** wideStr )
{
	BOOL isOk = FALSE;
	int wideCharsNeeded = MultiByteToWideChar( CP_THREAD_ACP, MB_ERR_INVALID_CHARS, ansiStr, -1, NULL, 0 );

	if( wideCharsNeeded > 0 )
	{		
		if( NULL != ( *wideStr = malloc( wideCharsNeeded * sizeof( wchar_t ) ) ) )
		{
			if( 0 != MultiByteToWideChar( CP_THREAD_ACP, MB_ERR_INVALID_CHARS, ansiStr, -1, *wideStr, wideCharsNeeded ) )
			{
				isOk = TRUE;
			}
			else
			{
				free( wideStr );
				*wideStr = NULL;
			}
		}
	}

	return isOk;
}
