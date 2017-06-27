#ifndef __BLUERUST_UTILITY_H
#define __BLUERUST_UTILITY_H

#ifdef _DEBUG

#include <stdio.h>		// for _vsnprintf
#include <stdarg.h>		// for _vsnprintf
#include <windows.h>
#include <time.h>

#define TRACEBUFFSIZE	512

//	see c:\...\vc98\atl\include\AtlBase.h for AtlTrace definition that
//	I used to pattern this trace after:

inline void _cdecl Trace(LPCWSTR formatString, ...)
{
	va_list args;
	va_start(args, formatString);

	TCHAR buffer[TRACEBUFFSIZE] = L"";

	// note on return value from _vsnprintf:
	// numBytes = "the number of characters written, not including the
	//		terminating null character, or a negative value if an output error
	//		occurs. If the number of characters to write exceeds count, then
	//		count characters are written and –1 is returned."
	int numBytes = _vsnwprintf_s(buffer, TRACEBUFFSIZE, _countof(buffer), formatString, args);
	if (numBytes == -1)		//Output truncated as it was > sizeof(buffer)
	{
		buffer[TRACEBUFFSIZE - 2] = '\n';		// TRACEBUFFSIZE > 2, please
		buffer[TRACEBUFFSIZE - 1] = '\x00';
	}

	OutputDebugString(L"Trace: ");
	OutputDebugString(buffer);
	va_end(args);
}

#define ASSERT( exp )																\
	if ( !(exp) )																	\
	{																				\
		Trace(L"ASSERT(%s): line %d, file %s\n", _T(#exp), __LINE__, _T(__FILE__));	\
		__debugbreak();																\
	}

#else // !_DEBUG

#include <stdio.h>		// for _vsnprintf
#include <stdarg.h>		// for _vsnprintf
#include <windows.h>
#include <time.h>

#define ASSERT( exp, desc){}
inline void _cdecl Trace(LPCTSTR, ...){}

#endif //_DEBUG

#endif //__BLUERUST_UTILITY_H