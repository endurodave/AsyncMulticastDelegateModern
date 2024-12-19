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
/// Asynchronous function calls support both non-blocking and blocking modes with a timeout. 
/// The library supports all types of target functions, including free functions, class 
/// member functions, static class functions, lambdas, and `std::function`. It is capable of 
/// handling any function signature, regardless of the number of arguments or return value. 
/// All argument types are supported, including by value, pointers, pointers to pointers, 
/// and references. The delegate library takes care of the intricate details of function 
/// invocation across thread boundaries. Thread-safe delegate containers stores delegate 
/// instances with a matching function signature.
/// 
///  A delegate instance can be:
///
/// * Copied freely.
/// * Compared to same type delegatesand `nullptr`.
/// * Reassigned.
/// * Called.
/// 
/// Typical use cases are:
///
/// * Publish / Subscribe(Event - Driven Programming)
/// * Thread - Safe Asynchronous API(Subsystem / Library)
/// * Anonymous Thread - Safe Callbacks on Specified Thread
///
/// The delegate library differs from `std::async` in that the caller-specified thread of 
/// control is used to invoke the target function bound to the delegate. The asynchronous 
/// variants copy the argument data into the event queue, ensuring safe transport to the 
/// destination thread, regardless of the argument type. This approach offers 'fire and forget' 
/// functionality without the caller waiting.
/// 
/// The `Async` and `AsyncWait` class variants may throw `std::alloc` if heap allocation 
/// fails within `operator()(Args... args)`. All other delegate class functions do not throw 
/// exceptions.
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
