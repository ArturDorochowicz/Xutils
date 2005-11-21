
#include "Xutils.h"


HANDLE OpenCdVolume( char driveLetter )
{
	static char volumeFormat [] = "\\\\.\\%c:";
	static char rootFormat [] = "%c:\\";

	HANDLE volume = INVALID_HANDLE_VALUE;
	char volumeName [8];
	char rootName [5];

	sprintf( rootName, rootFormat, driveLetter );

	if( DRIVE_CDROM == GetDriveType( rootName ) )
	{
		sprintf( volumeName, volumeFormat, driveLetter );
		volume = CreateFile( volumeName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL );
	}

	return volume;
}

BOOL EjectCd( char driveLetter )
{
	BOOL isOk = FALSE;
	HANDLE volume;
	DWORD bytesReturned;

	// Open the volume.
	if( INVALID_HANDLE_VALUE != ( volume = OpenCdVolume( driveLetter ) ) )
	{
		if( DeviceIoControl( volume, IOCTL_STORAGE_EJECT_MEDIA, NULL, 0, NULL, 0, &bytesReturned, NULL ) )
			isOk = TRUE;

		// Close the volume so other processes can use the drive.
		if( !CloseHandle( volume ) )
			isOk = FALSE;
	}

	return isOk;
}

BOOL LoadCd( char driveLetter )
{
	BOOL isOk = FALSE;
	HANDLE volume;
	DWORD bytesReturned;
	
	// Open the volume.	
	if( INVALID_HANDLE_VALUE != ( volume = OpenCdVolume( driveLetter ) ) )
	{
		if( DeviceIoControl( volume, IOCTL_STORAGE_LOAD_MEDIA, NULL, 0, NULL, 0, &bytesReturned, NULL ) )
			isOk = TRUE;
	
		// Close the volume so other processes can use the drive.
		if( !CloseHandle( volume ) )
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
