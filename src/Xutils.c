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


#include <stdlib.h>


/*---------------------------------------------------------------------------*/

void ShowErrorMessage( const char *message, PPROSERVICES *pproServices )
{
	if( pproServices != NULL )
	{
		pproServices->ErrMessage( (char*) message, "" );
	}
	else
	{
		MessageBox( 0, message, "Error", MB_OK | MB_ICONERROR );
	}
}

void ShowLastError( PPROSERVICES *pproServices )
{
	char * messageBuffer;

	FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError( ),
		MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
		(char*) &messageBuffer,
		0, NULL );

	ShowErrorMessage( messageBuffer, pproServices );
	LocalFree( messageBuffer );
}

BOOL CheckArgumentsCount( Services service, PPROHELPER *pp )
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
			if( pp->argc > 3 )
			{
				nArgsOk = FALSE;
				ShowErrorMessage( ARGS_MAX_3, pp->svcs );
			}
			else if( pp->argc < 1 )
			{
				nArgsOk = FALSE;
				ShowErrorMessage( ARGS_MIN_1, pp->svcs );
			}
			break;
		}

		case ServiceRunas:
		{
			if( pp->argc > 4 )
			{
				nArgsOk = FALSE;
				ShowErrorMessage( ARGS_MAX_4, pp->svcs );
			}
			else if( pp->argc < 1 )
			{
				nArgsOk = FALSE;
				ShowErrorMessage( ARGS_MIN_1, pp->svcs );
			}
			break;
		}

		case ServiceEjectcd:
		{
			if( pp->argc > 1 )
			{
				nArgsOk = FALSE;
				ShowErrorMessage( ARGS_MAX_1, pp->svcs );
			}
			break;
		}

		case ServiceLoadcd:
		{
			if( pp->argc > 1 )
			{
				nArgsOk = FALSE;
				ShowErrorMessage( ARGS_MAX_1, pp->svcs );
			}
			break;
		}

		case ServiceDisableidletimers:
		{
			if( pp->argc > 1 )
			{
				nArgsOk = FALSE;
				ShowErrorMessage( ARGS_MAX_1, pp->svcs );
			}
			break;
		}

		case ServiceEnableidletimers:
		{
			if( pp->argc > 1 )
			{
				nArgsOk = FALSE;
				ShowErrorMessage( ARGS_MAX_1, pp->svcs );
			}
			break;
		}

		case ServiceScrollup:
		{
			if( pp->argc > 1 )
			{
				nArgsOk = FALSE;
				ShowErrorMessage( ARGS_MAX_1, pp->svcs );
			}
			break;
		}

		case ServiceScrolldown:
		{
			if( pp->argc > 1 )
			{
				nArgsOk = FALSE;
				ShowErrorMessage( ARGS_MAX_1, pp->svcs );
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
