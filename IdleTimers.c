
#include "Xutils.h"


_declspec( dllexport ) void disableidletimers( PSTR szv, PSTR szx, BOOL (*GetVar)(PSTR, PSTR), void (*SetVar)(PSTR, PSTR), DWORD * pFlags, UINT nArgs, PSTR * szargs, PowerProServices * ppsv )
{
	// return nothing
	**szargs = '\0';
	PPServices = ppsv;

	if( CheckArgumentsCount( disableidletimersService, nArgs ) )
	{
		SetThreadExecutionState( ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED | ES_CONTINUOUS );

		if( 1 == strlen( szargs[1] ) && szargs[1][0] == '1' )
		{
			// use PP's built-in functionality
			PPServices->RunCmd( "ScreenSaver.disable", NULL, NULL );
		}
	}
}

_declspec( dllexport ) void enableidletimers( PSTR szv, PSTR szx, BOOL (*GetVar)(PSTR, PSTR), void (*SetVar)(PSTR, PSTR), DWORD * pFlags, UINT nArgs, PSTR * szargs, PowerProServices * ppsv )
{
	// return nothing
	**szargs = '\0';
	PPServices = ppsv;

	if( CheckArgumentsCount( enableidletimersService, nArgs ) )
	{
		SetThreadExecutionState( ES_CONTINUOUS );

		if( 1 == strlen( szargs[1] ) && szargs[1][0] == '1' )
		{
			// use PP's built-in functionality
			PPServices->RunCmd( "ScreenSaver.enable", NULL, NULL );
		}
	}
}

int main( int argc, char **argv )
{	
	//OutputDebugString( "tttdasdasdasd" );
	//SuDo( L"c:\\windows\\system32\\cmd.exe", L"", L"" );

//	EjectCd( argv[1][0] );

	Sleep( 3000 );

//	LoadCd( argv[1][0] );

	return 0;
}
