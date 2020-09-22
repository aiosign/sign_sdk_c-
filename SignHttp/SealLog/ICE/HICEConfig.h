// **********************************************************************
//
// Copyright (c) 2003-2005 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************
#pragma once

#include "HICEComonInclude.h"


#if defined(_WIN32)


#ifdef H_OS_VER_WIN9X

#   ifdef _WIN32_WINNT
#   undef _WIN32_WINNT
#   endif 

#else
//
// Comment out the following block if you want to run on Windows 9x
// or Windows NT 3.51.
//
#   ifndef _WIN32_WINNT
//
// Necessary for TryEnterCriticalSection.
//
#   define _WIN32_WINNT 0x0400
#   endif

#endif

#   define _WINSOCKAPI_ //added by wz, to prevent including winsock.h

#   include <windows.h>

//
// MFC applications that include afxwin.h before this header will cause
// windows.h to skip inclusion of winsock.h, so we include it here if
// necessary.
// 
#   ifndef _WINSOCK2API_
#      include <winsock2.h>
#   endif

// '...' : forcing value to bool 'true' or 'false' (performance warning)
#   pragma warning( disable : 4800 )
// ... identifier was truncated to '255' characters in the debug information
#   pragma warning( disable : 4786 )
// 'this' : used in base member initializer list
#   pragma warning( disable : 4355 )
// class ... needs to have dll-interface to be used by clients of class ...
#   pragma warning( disable : 4251 )
// ... : inherits ... via dominance
#   pragma warning( disable : 4250 )
// non dll-interface class ... used as base for dll-interface class ...
#   pragma warning( disable : 4275 )
//  ...: decorated name length exceeded, name was truncated
#   pragma warning( disable : 4503 )  

#elif (defined(__sun) && defined(__sparc)) || (defined(__hpux))
#   include <inttypes.h>
#else
//
// The ISO C99 standard specifies that in C++ implementations the
// macros for minimum/maximum integer values should only be defined if
// explicitly requested with __STDC_LIMIT_MACROS.
//
#   ifndef  __STDC_LIMIT_MACROS
#      define __STDC_LIMIT_MACROS
#   endif
#   include <stdint.h>
#endif

//
// Some include files we need almost everywhere.
//
#include <cassert>
#include <iostream>
#include <sstream>

#ifndef _WIN32
#   include <pthread.h>
#   include <errno.h>
#endif

//
// If we use Visual C++ 6.0, we must use STLport
//
#if defined(_MSC_VER) && (_MSC_VER < 1300) && !defined(_STLP_BEGIN_NAMESPACE)
#   error "Ice for Visual C++ 6.0 requires STLport"
#endif

//
// By deriving from this class, other classes are made non-copyable.
//
namespace HICEUtil
{

//
// TODO: Constructor and destructor should not be inlined, as they are
// not performance critical.
//
// TODO: Naming conventions?
//
class noncopyable
{
protected:

    noncopyable() { }
    ~noncopyable() { } // May not be virtual! Classes without virtual operations also derive from noncopyable.

private:

    noncopyable(const noncopyable&);
    const noncopyable& operator=(const noncopyable&);
};

//
// Some definitions for 64-bit integers.
//
#if defined(_MSC_VER)

typedef __int64 Int64;
const Int64 Int64Min = -9223372036854775808i64;
const Int64 Int64Max =  9223372036854775807i64;

#elif defined(__SUNPRO_CC)

#   if defined(ICE_64)
typedef long Int64;
const Int64 Int64Min = -0x7fffffffffffffffL-1L;
const Int64 Int64Max = 0x7fffffffffffffffL;
#   else
typedef long long Int64;
const Int64 Int64Min = -0x7fffffffffffffffLL-1LL;
const Int64 Int64Max = 0x7fffffffffffffffLL;
#   endif

#else

//
// Assumes ISO C99 types
//
typedef int64_t Int64;
#   ifdef INT64_MIN
const Int64 Int64Min = INT64_MIN;
#   else
const Int64 Int64Min = -0x7fffffffffffffffLL-1LL;
#   endif
#   ifdef INT64_MIN
const Int64 Int64Max = INT64_MAX;
#   else
const Int64 Int64Max = 0x7fffffffffffffffLL;
#   endif

#endif

#if defined(_MSC_VER)
#   define ICE_INT64(n) n##i64
#elif defined(__HP_aCC)
#   define ICE_INT64(n) n
#elif defined(ICE_64)
#   define ICE_INT64(n) n##L
#else
#   define ICE_INT64(n) n##LL
#endif

}
