#ifndef _DELEGATE_LIB_H
#define _DELEGATE_LIB_H

// Delegate.h
// @see https://github.com/endurodave/AsyncMulticastDelegateModern
// David Lafreniere, Aug 2020.

/// @file
/// @brief DelegateLib.h is a single include to obtain all delegate functionality. 
///
/// @mainpage C++ Delegate Library Documentation
///
/// A C++ delegate library capable of invoking any callable function either synchronously   
/// or asynchronously on a user specified thread of control.
/// 
/// Github responsitory location:  
/// https://github.com/endurodave/AsyncMulticastDelegateModern
///
/// See README.md, DETAILS.md, and source code Doxygen comments for more information.

#include "DelegateOpt.h"
#include "MulticastDelegateSafe.h"
#include "SinglecastDelegate.h"
#include "DelegateAsync.h"
#include "DelegateAsyncWait.h"

#endif
