
#include <stdio.h>

// at least Windows XP needed
#define _WIN32_WINNT 0x0501
#include <windows.h>
#include <wincred.h>

#include "Xutils.h"


/**
 * Prepare command line.
 * Caller is responsible for freeing allocated string.
 *
 */
BOOL CreateCommandLine( const wchar_t * programPath, const wchar_t * programArguments, wchar_t ** commandLine )
{
	BOOL isOk = FALSE;		
	int programPathLength = wcslen( programPath );

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
 * Prepare label for prompt for credentials dialog.
 * Caller is responsible for freeing allocated string.
 *
 */
BOOL CreateCredUILabel( const wchar_t * commandLine, wchar_t ** label )
{
	BOOL isOk = FALSE;
	// label = commandLine
	int labelLength = wcslen( commandLine ) + 1;
	if( labelLength < CREDUI_MAX_MESSAGE_LENGTH )
	{
		*label = malloc( labelLength * sizeof( wchar_t ) );
		if( *label != NULL )
		{
			wcscpy( *label, commandLine );
			isOk = TRUE;
		}
	}

	return isOk;
}

/**
 * Show logon dialog with preselected user name,
 * then use obtained credentials to run module specified in programPath.
 * None of the (wchar_t *) arguments can be NULL!
 *
 */
BOOL RunAs( const wchar_t * programPath, const wchar_t * programArguments, const wchar_t * preselectedUserName, const wchar_t * workingDirectory, PowerProServices * ppsv )
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

		if( TRUE == CreateCredUILabel( commandLine, &credUIInfo.pszMessageText ) )
		{
			wchar_t userName [CREDUI_MAX_USERNAME_LENGTH + 1];
			wchar_t password [CREDUI_MAX_PASSWORD_LENGTH + 1];

			SecureZeroMemory( userName, sizeof( userName ) );
			SecureZeroMemory( password, sizeof( password ) );

			wcsncpy( userName, preselectedUserName, CREDUI_MAX_USERNAME_LENGTH );

			// Ask for credentials.
			errCode = CredUIPromptForCredentialsW(
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
				CREDUI_FLAGS_DO_NOT_PERSIST );

			if( !errCode )
			{
				if( CreateProcessWithLogonW( userName, NULL, password, LOGON_WITH_PROFILE,
					programPath, commandLine,
					0, NULL, workingDirectory, &startupInfo, &processInfo ) )
				{
					isOk = TRUE;
				}
				else
				{
					ShowLastError( ppsv );
				}

				// Erase credentials from memory.
				SecureZeroMemory( userName, sizeof( userName) );
				SecureZeroMemory( password, sizeof( password ) );
			}
		}
		free( credUIInfo.pszMessageText );
	}
	free( commandLine );

	return isOk;
}

/*---------------------------------------------------------------------------*/

_declspec( dllexport ) void runas( PSTR szv, PSTR szx, BOOL (*GetVar)(PSTR, PSTR), void (*SetVar)(PSTR, PSTR), DWORD * pFlags, UINT nArgs, PSTR * szargs, PowerProServices * ppsv )
{
	wchar_t * preselectedUserName = NULL;
	wchar_t * programPath = NULL;
	wchar_t * programArguments = NULL;
	wchar_t * workingDirectory = NULL;

	// return nothing
	**szargs = '\0';
	PPServices = ppsv;


	if( TRUE == CheckArgumentsCount( runasService, nArgs ) )
	{
		if( ConvertMultiByteToWideChar( szargs[1], &programPath )
			&& ConvertMultiByteToWideChar( szargs[2], &programArguments )
			&& ConvertMultiByteToWideChar( szargs[3], &preselectedUserName )
			&& ConvertMultiByteToWideChar( szargs[4], &workingDirectory ) )
		{
			RunAs( programPath, programArguments, preselectedUserName, workingDirectory, ppsv );
		}

		free( programPath );
		free( programArguments );
		free( preselectedUserName );
		free( workingDirectory );
	}
}
