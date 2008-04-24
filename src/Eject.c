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


#include <Windows.h>
#include <Winioctl.h>

#include "Xutils.h"


static BOOL IsCdRomDrive( char driveLetter )
{
	char rootName[] = "X:\\";
	rootName[0] = driveLetter;

	return DRIVE_CDROM == GetDriveType( rootName );
}


static HANDLE OpenCdRomVolume( char driveLetter )
{
	if( TRUE == IsCdRomDrive( driveLetter ) )
	{
		char volumeName[] = "\\\\.\\X:";
		HANDLE volume;
		
		volumeName[4] = driveLetter;
		volume = CreateFile( volumeName, GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL );
		return volume;
	}

	return INVALID_HANDLE_VALUE;
}


static BOOL FindFirstCdRomDrive( char *cdDriveLetter )
{
	char driveLetter;

	for( driveLetter = 'A'; driveLetter <= 'Z'; ++driveLetter )
	{
		if( TRUE == IsCdRomDrive( driveLetter ) )
		{
			*cdDriveLetter = driveLetter;
			return TRUE;
		}
	}

	return FALSE;
}


static BOOL EjectCd( char driveLetter )
{
	BOOL isOk = FALSE;
	HANDLE volume = OpenCdRomVolume( driveLetter );
	
	if( volume != INVALID_HANDLE_VALUE )
	{
		DWORD bytesReturned;

		isOk = DeviceIoControl( volume, IOCTL_STORAGE_EJECT_MEDIA, NULL, 0,
			NULL, 0, &bytesReturned, NULL );
		CloseHandle( volume );
	}

	return isOk;
}


static BOOL LoadCd( char driveLetter )
{
	BOOL isOk = FALSE;
	HANDLE volume = OpenCdRomVolume( driveLetter );
		
	if( volume != INVALID_HANDLE_VALUE )
	{
		DWORD bytesReturned;
		
		isOk = DeviceIoControl( volume, IOCTL_STORAGE_LOAD_MEDIA, NULL, 0, NULL,
			0, &bytesReturned, NULL );
		CloseHandle( volume );
	}

	return isOk;
}


/*! <service name="EjectCd">
/*!  <description>Eject CD in the specified CD drive. Without the parameter, the service will eject the first CD drive in the system.</description>
/*!  <argument name="cdDriveLetter" type="string" optional="true">The letter of the CD drive to use.</argument>
/*! </service> */
BEGIN_PPRO_SVC( ejectcd )
{
	if( CheckArgumentsCount( ServiceEjectCd, pp ) )
	{
		char cdDriveLetter;

		if( 1 == pp->argc && 1 == strlen( pp->argv[0] ) )
		{
			cdDriveLetter = (char) tolower( pp->argv[0][0] );
			if( cdDriveLetter >= 'a' && cdDriveLetter <= 'z' )
				EjectCd( cdDriveLetter );
		}
		else if( 0 == pp->argc && TRUE == FindFirstCdRomDrive( &cdDriveLetter ) )
			EjectCd( cdDriveLetter );
	}
}
END_PPRO_SVC


/*! <service name="LoadCd">
/*!  <description>Close the door of the specified CD drive. Without the parameter, the service will close the door of the first CD drive in the system.</description>
/*!  <argument name="cdDriveLetter" type="string" optional="true">The letter of the CD drive to use.</argument>
/*! </service> */
BEGIN_PPRO_SVC( loadcd )
{
	if( CheckArgumentsCount( ServiceLoadCd, pp ) )
	{
		char cdDriveLetter;

		if( 1 == pp->argc && 1 == strlen( pp->argv[0] ) )
		{
			cdDriveLetter = (char) tolower( pp->argv[0][0] );
			if( cdDriveLetter >= 'a' && cdDriveLetter <= 'z' )
				LoadCd( cdDriveLetter );
		}
		else if( 0 == pp->argc && FindFirstCdRomDrive( &cdDriveLetter ) )
			LoadCd( cdDriveLetter );
	}
}
END_PPRO_SVC
