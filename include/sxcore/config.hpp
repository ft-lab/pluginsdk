#if SXWINDOWS
// created by configure.py with args:  sxapi_mach sxapi_posix sxapi_core_foundation sxapi_cocoa sxapi_cocoa_touch sxapi_reflest sxos_osx sxos_ios autoforce force
#pragma once
//#define sxassert(a) // uncomment to disable sxassert
#define SXRELEASE 0
#define SXSVECTOR SX32
#define SXASSERTDEFINED !(SXRELEASE)
#define SXINVARIANT !(SXRELEASE)
#define SXTESTDEFINED !(SXRELEASE)
#define SXSIGNATURE !(SXRELEASE)
#define SXOS_OSX              0
#define SXOS_IOS              0
#define SXOS_ANDROID          0
#define SXOS_WINDOWS          1
#define SXOS_WINDOWS_RT       0
#define SXOS_WINDOWS_PHONE    0
#define SXOS_LINUX            0
#define SXAPI_WIN32           1
#define SXAPI_CORE_WIN32      1
#define SXAPI_MFC             1
#define SXAPI_WINRT           0
#define SXAPI_MACH            0
#define SXAPI_POSIX           0
#define SXAPI_CORE_FOUNDATION 0
#define SXAPI_COCOA           0
#define SXAPI_COCOA_TOUCH     0

#else
// created by configure.py with args:  sxapi_mach sxapi_posix sxapi_core_foundation sxapi_cocoa sxapi_cocoa_touch sxapi_reflest sxos_osx sxos_ios autoforce force
#pragma once
//#define sxassert(a) // uncomment to disable sxassert
#define SXRELEASE 0
#define SXSVECTOR SX32
#define SXASSERTDEFINED !(SXRELEASE)
#define SXINVARIANT !(SXRELEASE)
#define SXTESTDEFINED !(SXRELEASE)
#define SXSIGNATURE !(SXRELEASE)
#define SXOS_OSX              1
#define SXOS_IOS              1
#define SXOS_ANDROID          0
#define SXOS_WINDOWS          0
#define SXOS_WINDOWS_RT       0
#define SXOS_WINDOWS_PHONE    0
#define SXOS_LINUX            0
#define SXAPI_WIN32           0
#define SXAPI_CORE_WIN32      0
#define SXAPI_MFC             0
#define SXAPI_WINRT           0
#define SXAPI_MACH            1
#define SXAPI_POSIX           1
#define SXAPI_CORE_FOUNDATION 1
#define SXAPI_COCOA           1
#define SXAPI_COCOA_TOUCH     1

#endif