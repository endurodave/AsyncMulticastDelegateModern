#ifndef _DELEGATE_OPT_H
#define _DELEGATE_OPT_H

// Define either USE_WIN32_THREADS or USE_STD_THREADS to specify WIN32 or std::thread threading model.
// Define USE_CPLUSPLUS_11 if the compiler supports C++ 11 features.

// Define USE_CPLUSPLUS_11 if using a C++11 compliant compiler. Using Visual Studio, if the _MSC_VER 
// is >= 1700 then C++11 features are available. 
#if _MSC_VER >= 1700
	#define USE_CPLUSPLUS_11 1
	//#define USE_STD_THREADS	1
	#define USE_WIN32_THREADS 1 
#elif _MSC_VER >= 1500
	#define USE_WIN32_THREADS 1
#endif

#if __GNUC__ >= 5
	#define USE_CPLUSPLUS_11 1
	#define USE_STD_THREADS	1
#elif __GNUC__ == 4 
	#define USE_STD_THREADS	1
#endif

#if defined(USE_STD_THREADS) && defined(USE_WIN32_THREADS)
	#error Define only one. Either USE_STD_THREADS or USE_WIN32_THREADS
#endif

#if !defined(USE_STD_THREADS) && !defined(USE_WIN32_THREADS)
	#error Must define either USE_STD_THREADS or USE_WIN32_THREADS
#endif

#if defined(__GNUC__) && defined(USE_WIN32_THREADS)
	#error GCC does not support WIN32 API. Define USE_STD_THREADS.
#endif

#if defined(USE_STD_THREADS) && !defined(USE_CPLUSPLUS_11)
	#error C++11 compiler required to use std::thread API
#endif

#endif
