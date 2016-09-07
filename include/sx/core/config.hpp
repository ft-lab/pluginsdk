// created by configure.py
#pragma once
SXCORE

#define SXASSERTDEFINED 1
#define SXGLASSERTDEFINED 0
#define SXMXASSERTDEFINED 0
#define SXTHASSERTDEFINED 0
#define SXIMASSERTDEFINED 0
#define SXTESTDEFINED 1
#define SXSIGNATURE 1
#define SXMUTEXTIMEOUT 0.0
#define SXSEMAPHORETIMEOUT 0.0
#if _WIN32
	#if _WIN64
		#define SXSIMD 3
	#else
		#define SXSIMD 2
	#endif
#elif SXIOS
	#define SXSIMD 1
#elif __SSE__
	#define SXSIMD 3
#else
	#define SXSIMD 0
#endif
#define SXSHAREDMUTEXBLOCKING 1
