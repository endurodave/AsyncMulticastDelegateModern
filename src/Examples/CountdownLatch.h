#ifndef _COUNTDOWN_LATCH_H
#define _COUNTDOWN_LATCH_H

// DelegateAsync.h
// @see https://github.com/endurodave/cpp-async-delegate
// David Lafreniere, Aug 2020.

/// @file
/// @brief Implement a countdown latch using C++20 std::latch and delegates. 

#include "DelegateLib.h"
#include <mutex>
#include "SysDataTypes.h"

/// Execute the countdown latch example
void CountdownLatchExample();

#endif