/**
 * Xutils
 * Copyright (C) 2005  Artur Dorochowicz
 *
 * Released under the terms of Lesser General Public License (LGPL).
 *
 */


#include "Xutils.h"

/*---------------------------------------------------------------------------*/

PowerProServices * PPServices = NULL;

/**
 * Library's main entry point, no special processing needed
 */
BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD dwReason, PVOID lpvReserved )
{
	return TRUE;
}

void ShowErrorMessage( const char * message )
{
	if( PPServices != NULL )
	{
		PPServices->ErrMessage( (char*) message, NULL );
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
	
	static const char ARGS_MAX_1 [] = "The service needs at most one argument.";
	static const char ARGS_MAX_3 [] = "The service needs at most three arguments.";
	static const char ARGS_MAX_4 [] = "The service needs at most four arguments.";

	static const char ARGS_MIN_1 [] = "The service needs at least one argument.";

	switch( service )
	{
		case ServiceSudo:
		{
			if( nArgs > 3 )
			{
				nArgsOk = FALSE;
				ShowErrorMessage( ARGS_MAX_3 );
			}
			else if( nArgs < 1 )
			{
				nArgsOk = FALSE;
				ShowErrorMessage( ARGS_MIN_1 );
			}
			break;
		}

		case ServiceRunas:
		{
			if( nArgs > 4 )
			{
				nArgsOk = FALSE;
				ShowErrorMessage( ARGS_MAX_4 );
			}
			else if( nArgs < 1 )
			{
				nArgsOk = FALSE;
				ShowErrorMessage( ARGS_MIN_1 );
			}
			break;
		}

		case ServiceEjectcd:
		{
			if( nArgs > 1 )
			{
				nArgsOk = FALSE;
				ShowErrorMessage( ARGS_MAX_1 );
			}
			break;
		}

		case ServiceLoadcd:
		{
			if( nArgs > 1 )
			{
				nArgsOk = FALSE;
				ShowErrorMessage( ARGS_MAX_1 );
			}
			break;
		}

		case ServiceDisableidletimers:
		{
			if( nArgs > 1 )
			{
				nArgsOk = FALSE;
				ShowErrorMessage( ARGS_MAX_1 );
			}
			break;
		}

		case ServiceEnableidletimers:
		{
			if( nArgs > 1 )
			{
				nArgsOk = FALSE;
				ShowErrorMessage( ARGS_MAX_1 );
			}
			break;
		}

		case ServiceScrollup:
		{
			if( nArgs > 1 )
			{
				nArgsOk = FALSE;
				ShowErrorMessage( ARGS_MAX_1 );
			}
			break;
		}

		case ServiceScrolldown:
		{
			if( nArgs > 1 )
			{
				nArgsOk = FALSE;
				ShowErrorMessage( ARGS_MAX_1 );
			}
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


int main( int argc, char **argv )
{	
	//OutputDebugString( "tttdasdasdasd" );
	//SuDo( L"c:\\windows\\system32\\cmd.exe", L"", L"" );

//	EjectCd( argv[1][0] );
char * p;
	double pi = strtod( "asd", NULL);
	int i = GetWheelScrollLines();
	char pp;
	BOOL ii = FindFirstCdDrive( &pp );
//	LoadCd( argv[1][0] );

	return 0;
}