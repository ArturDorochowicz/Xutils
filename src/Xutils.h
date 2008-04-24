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


#ifndef _XUTILS_H
#define _XUTILS_H


/* PowerPro 4.5.12 is the first version to support EncodeFloat/DecodeFloat */
#define PPRO_VERSION 4512
#include "powerpro.h"


typedef enum Services
{
	ServiceRunAs,
	ServiceSuDo,
	ServiceEjectCd,
	ServiceLoadCd,
	ServiceDisableIdleTimers,
	ServiceEnableIdleTimers,
	ServiceScrollUp,
	ServiceScrollDown
} Services;


/**
 * Display error message.
 * Message is displayed with PPROSERVICES->ErrMessage
 * or an ordinary MessageBox, when PPROSERVICES is NULL.
 */
void ShowErrorMessage( const char *message, PPROSERVICES *pproServices );


/**
 * Format and display error message for error code obtained from GetLastError().
**/
void ShowLastError( PPROSERVICES *pproServices );


/**
 * Check number of arguments.
**/
BOOL CheckArgumentsCount( Services service, PPROHELPER *pp );


/**
 * Convert specified ANSI string to Unicode string.
 * Caller is responsible for freeing allocated wide string with free().
**/
BOOL ConvertMultiByteToWideChar( const char *ansiStr, wchar_t **wideStr );


#endif   // #ifndef _XUTILS_H
