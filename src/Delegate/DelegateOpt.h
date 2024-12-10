#ifndef _DELEGATE_OPT_H
#define _DELEGATE_OPT_H

/// @file
/// @brief Delegate library options header file. 

#ifdef USE_ALLOCATOR

// Use stl_allocator fixed block allocator for dynamic storage allocation
#include "xlist.h"
#include "stl_allocator.h"

#else

#include <list>

// Use default std::allocator for dynamic storage allocation
template <typename T, typename Alloc = std::allocator<T>>
using xlist = std::list<T, Alloc>;

#define XALLOCATOR

#endif

#endif
