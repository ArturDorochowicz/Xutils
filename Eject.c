
#include "Xutils.h"


HANDLE OpenVolume(TCHAR cDriveLetter);
BOOL LockVolume(HANDLE hVolume);
BOOL DismountVolume(HANDLE hVolume);
BOOL PreventRemovalOfVolume(HANDLE hVolume, BOOL fPrevent);
BOOL AutoEjectVolume(HANDLE hVolume);
BOOL CloseVolume(HANDLE hVolume);

LPTSTR szVolumeFormat = TEXT("\\\\.\\%c:");
LPTSTR szRootFormat = TEXT("%c:\\");

HANDLE OpenVolume( char driveLetter )
{
	HANDLE hVolume;
	UINT uDriveType;
	char szVolumeName[8];
	char szRootName[5];
	DWORD dwAccessFlags;

	sprintf( szRootName, szRootFormat, driveLetter );

	uDriveType = GetDriveType( szRootName );
	switch( uDriveType )
	{
	   case DRIVE_REMOVABLE:
		   dwAccessFlags = GENERIC_READ | GENERIC_WRITE;
		   break;

	   case DRIVE_CDROM:
		   dwAccessFlags = GENERIC_READ;
		   break;

	   default:
		   ShowErrorMessage( "Cannot eject. Drive type is incorrect." );
		   return INVALID_HANDLE_VALUE;
	}

	sprintf( szVolumeName, szVolumeFormat, driveLetter );

	if( INVALID_HANDLE_VALUE == ( hVolume = CreateFile( szVolumeName, dwAccessFlags,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, 0, NULL ) ) )
	{
		OutputDebugString( "CreateFile error." );
	}

	return hVolume;
}

BOOL CloseVolume( HANDLE volume )
{
	return CloseHandle( volume );
}

#define LOCK_TIMEOUT 10000       // 10 Seconds
#define LOCK_RETRIES 20

BOOL LockVolume( HANDLE hVolume )
{
	DWORD dwBytesReturned;
	DWORD dwSleepAmount;
	int nTryCount;

	dwSleepAmount = LOCK_TIMEOUT / LOCK_RETRIES;

	// Do this in a loop until a timeout period has expired
	for( nTryCount = 0; nTryCount < LOCK_RETRIES; ++nTryCount )
	{
		if( DeviceIoControl( hVolume,
			FSCTL_LOCK_VOLUME,
			NULL, 0,
			NULL, 0,
			&dwBytesReturned,
			NULL ) )
			return TRUE;

		Sleep( dwSleepAmount );
	}

	return FALSE;
}

BOOL DismountVolume( HANDLE hVolume )
{
	DWORD dwBytesReturned;

	return DeviceIoControl( hVolume,
		FSCTL_DISMOUNT_VOLUME,
		NULL, 0,
		NULL, 0,
		&dwBytesReturned,
		NULL );
}

BOOL PreventRemovalOfVolume( HANDLE hVolume, BOOL fPreventRemoval )
{
	DWORD dwBytesReturned;
	PREVENT_MEDIA_REMOVAL PMRBuffer;

	PMRBuffer.PreventMediaRemoval = (BOOLEAN) fPreventRemoval;

	return DeviceIoControl( hVolume,
		IOCTL_STORAGE_MEDIA_REMOVAL,
		&PMRBuffer, sizeof( PREVENT_MEDIA_REMOVAL ),
		NULL, 0,
		&dwBytesReturned,
		NULL );
}

BOOL AutoEjectVolume( HANDLE hVolume )
{
	DWORD dwBytesReturned;

	return DeviceIoControl( hVolume, 
		IOCTL_STORAGE_EJECT_MEDIA,
		NULL, 0,
		NULL, 0,
		&dwBytesReturned,
		NULL );
}

BOOL EjectCd( char driveLetter )
{
	HANDLE hVolume;
	BOOL fRemoveSafely = FALSE;
	BOOL fAutoEject = FALSE;

	// Open the volume.
	hVolume = OpenVolume( driveLetter );
	if( hVolume == INVALID_HANDLE_VALUE )
		return FALSE;

	// Lock and dismount the volume.
	if( LockVolume( hVolume ) && DismountVolume( hVolume ) )
	{
		fRemoveSafely = TRUE;

		// Set prevent removal to false and eject the volume.
		if( PreventRemovalOfVolume( hVolume, FALSE) && AutoEjectVolume( hVolume ) )
			fAutoEject = TRUE;
	}

	// Close the volume so other processes can use the drive.
	if( !CloseVolume( hVolume ) )
		return FALSE;

	if( fAutoEject )
		printf( "Media in Drive %c has been ejected safely.\n", driveLetter );
	else if( fRemoveSafely )
		printf( "Media in Drive %c can be safely removed.\n", driveLetter );

	return TRUE;
}

BOOL LoadCd( char driveLetter )
{
	BOOL isOk = FALSE;
	HANDLE volume;
	DWORD bytesRet;
	
	// Open the volume.	
	if( INVALID_HANDLE_VALUE != ( volume = OpenVolume( driveLetter ) ) )
	{
		if( DeviceIoControl( volume, IOCTL_STORAGE_LOAD_MEDIA, NULL, 0, NULL, 0, &bytesRet, NULL ) )
		{
			isOk = TRUE;
		}
	
		// Close the volume so other processes can use the drive.
		if( !CloseVolume( volume ) )
			isOk = FALSE;
	}

	return isOk;
}

/*---------------------------------------------------------------------------*/

_declspec( dllexport ) void ejectcd( PSTR szv, PSTR szx, BOOL (*GetVar)(PSTR, PSTR), void (*SetVar)(PSTR, PSTR), DWORD * pFlags, UINT nArgs, PSTR * szargs, PowerProServices * ppsv )
{
	// return nothing
	**szargs = '\0';
	PPServices = ppsv;

	if( TRUE == CheckArgumentsCount( ejectcdService, nArgs ) )
	{
		EjectCd( szargs[1][0] );
	}
}

_declspec( dllexport ) void loadcd( PSTR szv, PSTR szx, BOOL (*GetVar)(PSTR, PSTR), void (*SetVar)(PSTR, PSTR), DWORD * pFlags, UINT nArgs, PSTR * szargs, PowerProServices * ppsv )
{
	// return nothing
	**szargs = '\0';
	PPServices = ppsv;

	if( TRUE == CheckArgumentsCount( loadcdService, nArgs ) )
	{
		LoadCd( szargs[1][0] );
	}
}



int main( int argc, char **argv )
{	
	OutputDebugString( "tttdasdasdasd" );
	//SuDo( L"c:\\windows\\system32\\cmd.exe", L"", L"" );

	EjectCd( 'x' );

	Sleep( 3000 );

	LoadCd( 'x' );

	return 0;
}
