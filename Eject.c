
#include "Xutils.h"

/*---------------------------------------------------------------------------*/

HANDLE OpenCdVolume( char driveLetter )
{
	HANDLE volume = INVALID_HANDLE_VALUE;
	char volumeName [] = "\\\\.\\X:";
	char rootName [] = "X:\\";

	rootName[0] = driveLetter;

	if( DRIVE_CDROM == GetDriveType( rootName ) )
	{
		volumeName[5] = driveLetter;
		volume = CreateFile( volumeName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL );
	}

	return volume;
}

BOOL FindFirstCdDrive( char * cdDriveLetter )
{
	char rootName [] = "X:\\";
	char driveLetter;

	for( driveLetter = 'a'; driveLetter <= 'z'; ++driveLetter )
	{
		rootName[0] = driveLetter;

		if( DRIVE_CDROM == GetDriveType( rootName ) )
		{
			*cdDriveLetter = driveLetter;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL EjectCd( char driveLetter )
{
	BOOL isOk = FALSE;
	HANDLE volume;
	DWORD bytesReturned;

	if( INVALID_HANDLE_VALUE != ( volume = OpenCdVolume( driveLetter ) ) )
	{
		if( DeviceIoControl( volume, IOCTL_STORAGE_EJECT_MEDIA, NULL, 0, NULL, 0, &bytesReturned, NULL ) )
			isOk = TRUE;

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
	
	if( INVALID_HANDLE_VALUE != ( volume = OpenCdVolume( driveLetter ) ) )
	{
		if( DeviceIoControl( volume, IOCTL_STORAGE_LOAD_MEDIA, NULL, 0, NULL, 0, &bytesReturned, NULL ) )
			isOk = TRUE;
	
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

	if( CheckArgumentsCount( ServiceEjectcd, nArgs ) )
	{
		char cdDriveLetter;

		if( 1 == nArgs && 1 == strlen( szargs[1] ) )
		{
			cdDriveLetter = tolower( szargs[1][0] );
			if( cdDriveLetter >= 'a' && cdDriveLetter <= 'z' )
				EjectCd( cdDriveLetter );
		}
		else if( 0 == nArgs && FindFirstCdDrive( &cdDriveLetter ) )
			EjectCd( cdDriveLetter );
	}
}

_declspec( dllexport ) void loadcd( PSTR szv, PSTR szx, BOOL (*GetVar)(PSTR, PSTR), void (*SetVar)(PSTR, PSTR), DWORD * pFlags, UINT nArgs, PSTR * szargs, PowerProServices * ppsv )
{
	// return nothing
	**szargs = '\0';
	PPServices = ppsv;

	if( CheckArgumentsCount( ServiceLoadcd, nArgs ) )
	{
		char cdDriveLetter;

		if( 1 == nArgs && 1 == strlen( szargs[1] ) )
		{
			cdDriveLetter = tolower( szargs[1][0] );
			if( cdDriveLetter >= 'a' && cdDriveLetter <= 'z' )
				LoadCd( cdDriveLetter );
		}
		else if( 0 == nArgs && FindFirstCdDrive( &cdDriveLetter ) )
			LoadCd( cdDriveLetter );
	}
}
