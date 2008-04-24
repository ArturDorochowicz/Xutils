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


#include <Winioctl.h>


HANDLE OpenCdVolume( char driveLetter )
{
	HANDLE volume = INVALID_HANDLE_VALUE;
	char volumeName [] = "\\\\.\\X:";
	char rootName [] = "X:\\";

	rootName[0] = driveLetter;

	if( DRIVE_CDROM == GetDriveType( rootName ) )
	{
		volumeName[4] = driveLetter;
		volume = CreateFile( volumeName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL );
	}

	return volume;
}

BOOL FindFirstCdDrive( char * cdDriveLetter )
{
	char rootName [] = "X:\\";
	char driveLetter;

	for( driveLetter = 'A'; driveLetter <= 'Z'; ++driveLetter )
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


/*! <service name="EjectCd">
/*!  <description>Eject CD in the specified CD drive. Without parameter ejects first CD drive in the system.</description>
/*!  <argument name="cdDriveLetter" type="string" optional="true">Letter of CD drive to use.</argument>
/*! </service> */
BEGIN_PPRO_SVC( ejectcd )
{
	if( CheckArgumentsCount( ServiceEjectcd, &pp ) )
	{
		char cdDriveLetter;

		if( 1 == pp.argc && 1 == strlen( pp.argv[0] ) )
		{
			cdDriveLetter = toupper( pp.argv[0][0] );
			if( cdDriveLetter >= 'A' && cdDriveLetter <= 'Z' )
				EjectCd( cdDriveLetter );
		}
		else if( 0 == pp.argc && FindFirstCdDrive( &cdDriveLetter ) )
			EjectCd( cdDriveLetter );
	}
}
END_PPRO_SVC


BEGIN_PPRO_SVC( loadcd )
{
	if( CheckArgumentsCount( ServiceLoadcd, &pp ) )
	{
		char cdDriveLetter;

		if( 1 == pp.argc && 1 == strlen( pp.argv[0] ) )
		{
			cdDriveLetter = tolower( pp.argv[0][0] );
			if( cdDriveLetter >= 'a' && cdDriveLetter <= 'z' )
				LoadCd( cdDriveLetter );
		}
		else if( 0 == pp.argc && FindFirstCdDrive( &cdDriveLetter ) )
			LoadCd( cdDriveLetter );
	}
}
END_PPRO_SVC