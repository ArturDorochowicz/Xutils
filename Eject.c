
#include <stdio.h>
#include <windows.h>

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

BOOL CloseVolume( HANDLE hVolume )
{
	return CloseHandle( hVolume );
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

	PMRBuffer.PreventMediaRemoval = fPreventRemoval;

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

BOOL Load( char driveLetter )
{
	HANDLE hVolume;
	DWORD bytesRet;
	
	// Open the volume.
	hVolume = OpenVolume( driveLetter );
	if( hVolume == INVALID_HANDLE_VALUE )
		return FALSE;

	DeviceIoControl( hVolume, IOCTL_STORAGE_LOAD_MEDIA, NULL, 0, NULL, 0, &bytesRet, NULL );
	
	// Close the volume so other processes can use the drive.
	if( !CloseVolume( hVolume ) )
		return FALSE;

	return TRUE;
}

BOOL Eject( char driveLetter )
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

/*---------------------------------------------------------------------------*/

_declspec( dllexport ) void eject( PSTR szv, PSTR szx, BOOL (*GetVar)(PSTR, PSTR), void (*SetVar)(PSTR, PSTR), DWORD * pFlags, UINT nArgs, PSTR * szargs, PowerProServices * ppsv )
{
	// return nothing
	**szargs = '\0';
	PPServices = ppsv;

	if( TRUE == CheckArgumentsCount( ejectmediaService, nArgs ) )
	{
		Eject( szargs[1][0] );
	}
}

_declspec( dllexport ) void ejectmedia( PSTR szv, PSTR szx, BOOL (*GetVar)(PSTR, PSTR), void (*SetVar)(PSTR, PSTR), DWORD * pFlags, UINT nArgs, PSTR * szargs, PowerProServices * ppsv )
{
	// return nothing
	**szargs = '\0';
	PPServices = ppsv;

	if( TRUE == CheckArgumentsCount( ejectmediaService, nArgs ) )
	{
		Eject( szargs[1][0] );
	}
}

_declspec( dllexport ) void loadmedia( PSTR szv, PSTR szx, BOOL (*GetVar)(PSTR, PSTR), void (*SetVar)(PSTR, PSTR), DWORD * pFlags, UINT nArgs, PSTR * szargs, PowerProServices * ppsv )
{
	// return nothing
	**szargs = '\0';
	PPServices = ppsv;

	if( TRUE == CheckArgumentsCount( loadmediaService, nArgs ) )
	{
		Load( szargs[1][0] );
	}
}
