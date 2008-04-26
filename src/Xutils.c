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


BOOL CheckArgumentsCount( PPROHELPER *pp, unsigned int minArgsRequired, unsigned int maxArgsRequired )
{
	if( pp->argc < minArgsRequired )
	{
		const char *errorMsg;

		switch( minArgsRequired )
		{
		case 1:
			errorMsg = "The service needs at least one argument.";
			break;
		default:
			errorMsg = "Not enough arguments for the service.";
			break;
		}

		ShowErrorMessage( errorMsg, pp->svcs );
		return FALSE;
	}
	else if( pp->argc > maxArgsRequired )
	{
		const char *errorMsg;
		
		switch( maxArgsRequired )
		{
		case 1:
			errorMsg = "The service needs at most one argument.";
			break;
		case 3:
			errorMsg = "The service needs at most three arguments.";
			break;
		case 4:
			errorMsg = "The service needs at most four arguments.";
			break;
		default:
			errorMsg = "Too many arguments for the service.";
			break;
		}

		ShowErrorMessage( errorMsg, pp->svcs );
		return FALSE;
	}

	return TRUE;
}
