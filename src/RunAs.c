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


#include <stdlib.h>

/* need Windows XP and later APIs */
#define _WIN32_WINNT 0x0501
#include <Windows.h>
#include <wincred.h>
#include <Sddl.h>

#include "Xutils.h"


/**
 * Prepare command line.
 * Caller is responsible for freeing allocated string.
 */
BOOL CreateCommandLine( const wchar_t * programPath, const wchar_t * programArguments, wchar_t ** commandLine )
{
	BOOL isOk = FALSE;
	size_t programPathLength = wcslen( programPath );

	// commandLine = ["] + programPath + ["] + [space] + programArguments
	*commandLine = malloc( ( programPathLength + wcslen( programArguments ) + 4 ) * sizeof( wchar_t ) );
	if( *commandLine != NULL )
	{
		wchar_t * p = *commandLine;
		*p++ = L'"';
		wcscpy( p, programPath );
		p += programPathLength;
		*p++ = L'"';
		*p++ = L' ';
		wcscpy( p, programArguments );

		isOk = TRUE;
	}

	return isOk;
}

/**
 * Prepare label for prompt-for-credentials dialog.
 * Caller is responsible for freeing allocated string.
 */
BOOL CreateCredUILabel( const wchar_t * commandLine, wchar_t ** label )
{
	BOOL isOk = FALSE;

	// label = commandLine
	// copy up to CREDUI_MAX_MESSAGE_LENGTH characters
	size_t labelLength = wcslen( commandLine );
	if( NULL != ( *label = malloc( ( labelLength + 1 ) * sizeof( wchar_t ) ) ) )
	{
		wcsncpy( *label, commandLine, 1 + min( labelLength, CREDUI_MAX_MESSAGE_LENGTH ) );

		isOk = TRUE;
	}

	return isOk;
}

/**
 * Show logon dialog with preselected user name,
 * then use obtained credentials to run module specified in programPath.
 * None of the (wchar_t *) arguments can be NULL!
 */
BOOL RunAs( const wchar_t * programPath, const wchar_t * programArguments, const wchar_t * preselectedUserName, const wchar_t * workingDirectory )
{
	BOOL isOk = FALSE;
	wchar_t * commandLine = NULL;

	if( TRUE == CreateCommandLine( programPath, programArguments, &commandLine ) )
	{		
		CREDUI_INFOW credUIInfo;
		BOOL saveState = FALSE;
		DWORD errCode;
		STARTUPINFOW startupInfo = {0};
		PROCESS_INFORMATION processInfo = {0};

		credUIInfo.cbSize = sizeof( CREDUI_INFO );
		credUIInfo.hwndParent = NULL;
		credUIInfo.pszCaptionText = L"RunAs";
		credUIInfo.hbmBanner = NULL;

		if( TRUE == CreateCredUILabel( commandLine, &( (wchar_t*) credUIInfo.pszMessageText ) ) )
		{
			wchar_t userName [CREDUI_MAX_USERNAME_LENGTH + 1];
			wchar_t password [CREDUI_MAX_PASSWORD_LENGTH + 1];
			BOOL repeat = FALSE;

			SecureZeroMemory( userName, sizeof( userName ) );
			SecureZeroMemory( password, sizeof( password ) );
			wcsncpy( userName, preselectedUserName, CREDUI_MAX_USERNAME_LENGTH + 1 );
			
			do
			{
				repeat = FALSE;

				// Ask for credentials.
				if( !CredUIPromptForCredentialsW(
					&credUIInfo,                        // CREDUI_INFO structure
					NULL,								// Target for credentials, usually a server
					NULL,                               // Reserved
					0,                                  // Reason
					userName,                           // User name
					CREDUI_MAX_USERNAME_LENGTH + 1,     // Max number of char for user name
					password,                           // Password
					CREDUI_MAX_PASSWORD_LENGTH + 1,     // Max number of char for password
					&saveState,                         // State of save check box
					CREDUI_FLAGS_GENERIC_CREDENTIALS |  // flags
					CREDUI_FLAGS_ALWAYS_SHOW_UI |
					CREDUI_FLAGS_DO_NOT_PERSIST ) )
				{
					if( CreateProcessWithLogonW( userName, NULL, password, LOGON_WITH_PROFILE,
						programPath, commandLine,
						0, NULL, workingDirectory, &startupInfo, &processInfo ) )
					{
						CloseHandle( processInfo.hProcess );
						CloseHandle( processInfo.hThread );
						isOk = TRUE;
					}
					else
					{
						errCode = GetLastError( );
						ShowLastError( NULL );

						if( ERROR_LOGON_FAILURE == errCode )
							repeat = TRUE;
					}
				}
			} while( repeat );
			
			// Erase credentials from memory.
			SecureZeroMemory( userName, sizeof( userName) );
			SecureZeroMemory( password, sizeof( password ) );
		}
		free( (void*) credUIInfo.pszMessageText );
	}
	free( commandLine );

	return isOk;
}

/**
 * Run specified program using built-in local administrator account.
 */
BOOL SuDo( const wchar_t * programPath, const wchar_t * programArguments, const wchar_t * workingDirectory )
{
	BOOL retVal = FALSE;

	PSID adminSid = NULL;
	wchar_t adminName [CREDUI_MAX_USERNAME_LENGTH + 1];
	DWORD adminNameLength = sizeof( adminName ) / sizeof( wchar_t );
	wchar_t domainName [CREDUI_MAX_USERNAME_LENGTH + 1];
	DWORD referencedDomainNameLength = sizeof( domainName ) / sizeof( wchar_t );
	SID_NAME_USE sidNameUse;

	if( ConvertStringSidToSid( SDDL_LOCAL_ADMIN, &adminSid ) )
	{
		if( LookupAccountSidW( NULL, adminSid, adminName, &adminNameLength, domainName, &referencedDomainNameLength, &sidNameUse ) )
		{
			retVal = RunAs( programPath, programArguments, adminName, workingDirectory );
		}
		LocalFree( adminSid );
	}

	return retVal;
}


/*---------------------------------------------------------------------------*/


BEGIN_PPRO_SVC( runas )
{
	wchar_t * preselectedUserName = NULL;
	wchar_t * programPath = NULL;
	wchar_t * programArguments = NULL;
	wchar_t * workingDirectory = NULL;

	if( CheckArgumentsCount( ServiceRunas, &pp ) )
	{
		if( ConvertMultiByteToWideChar( pp.argv[0], &programPath )
			&& ConvertMultiByteToWideChar( pp.argv[1], &programArguments )
			&& ConvertMultiByteToWideChar( pp.argv[2], &preselectedUserName )
			&& ConvertMultiByteToWideChar( pp.argv[3], &workingDirectory ) )
		{
			RunAs( programPath, programArguments, preselectedUserName, workingDirectory );
		}

		free( programPath );
		free( programArguments );
		free( preselectedUserName );
		free( workingDirectory );
	}
}
END_PPRO_SVC


BEGIN_PPRO_SVC( sudo )
{
	wchar_t * programPath = NULL;
	wchar_t * programArguments = NULL;
	wchar_t * workingDirectory = NULL;

	if( CheckArgumentsCount( ServiceSudo, &pp ) )
	{
		if( ConvertMultiByteToWideChar( pp.argv[0], &programPath )
			&& ConvertMultiByteToWideChar( pp.argv[1], &programArguments )
			&& ConvertMultiByteToWideChar( pp.argv[2], &workingDirectory ) )
		{
			SuDo( programPath, programArguments, workingDirectory );
		}

		free( programPath );
		free( programArguments );
		free( workingDirectory );
	}
}
END_PPRO_SVC