#ifndef _DELEGATE_MSG_H
#define _DELEGATE_MSG_H

#include "Fault.h"
#include "DelegateInvoker.h"
#include "DelegateOpt.h"
#include "Semaphore.h"
#include "make_tuple_heap.h"
#include <tuple>
#include <list>
#include <memory>
#include <mutex>
#include <stdexcept>

namespace DelegateLib {

/// @brief Base class for all delegate messages
class DelegateMsgBase
{
public:
	/// Constructor
	/// @param[in] invoker - the invoker instance the delegate is registered with.
	DelegateMsgBase(std::shared_ptr<IDelegateInvoker> invoker) :
		m_invoker(invoker)
	{
        if (!invoker)
            throw std::invalid_argument("Invoker must not be null");
	}

    virtual ~DelegateMsgBase() {}

	/// Get the delegate invoker instance the delegate is registered with.
	/// @return The invoker instance. 
	std::shared_ptr<IDelegateInvoker> GetDelegateInvoker() const { return m_invoker; }

private:
	/// The IDelegateInvoker instance used to invoke the target function 
    /// on the destination thread of control
	std::shared_ptr<IDelegateInvoker> m_invoker;
};

/// @brief Stores all function arguments suitable for blocking asynchronous calls.
/// Argument data is not stored in the heap.
template <class...Args>
class DelegateMsg : public DelegateMsgBase
{
public:
    /// Constructor
    /// @param[in] invoker - the invoker instance
    /// @param[in] args - a parameter pack of all target function arguments
    DelegateMsg(std::shared_ptr<IDelegateInvoker> invoker, Args... args) : DelegateMsgBase(invoker),
        m_args(args...)
    {
    }

    virtual ~DelegateMsg() {}

    /// Get all function arguments 
    /// @return A tuple of all function arguments
    std::tuple<Args...>& GetArgs() { return m_args; }

    // Get the semaphore used to signal the sending thread that the receiving 
    // thread has invoked the target function. 
    Semaphore& GetSema() { return m_sema; }

    // Get a mutex shared between sender and receiver threads
    std::mutex& GetLock() { return m_lock; }

    // True if the sending thread is waiting for the receiver thread to call the function.
    // False if the sending thread delegate timeout occurred and is not waiting.
    bool GetInvokerWaiting() { return m_invokerWaiting; }

    // Set to true when source thread is waiting for destination thread to complete the 
    // function call.
    void SetInvokerWaiting(bool invokerWaiting) { m_invokerWaiting = invokerWaiting; }

private:
    /// An empty starting tuple
    std::tuple<> m_start;

    /// A tuple with each function argument element 
    std::tuple<Args...> m_args;

    Semaphore m_sema;				        // Semaphore to signal waiting thread
    std::mutex m_lock;                      // Lock to protect shared data  
    bool m_invokerWaiting = false;          // True if caller thread is waiting for invoke complete
};

/// @brief Stores all function arguments suitable for non-blocking asynchronous calls.
/// Argument data is stored in the heap.
template <class...Args>
class DelegateMsgHeapArgs : public DelegateMsgBase
{
public:
    /// Constructor
    /// @param[in] invoker - the invoker instance
    /// @param[in] args - a parameter pack of all target function arguments
    DelegateMsgHeapArgs(std::shared_ptr<IDelegateInvoker> invoker, Args... args) : DelegateMsgBase(invoker), 
        m_args(make_tuple_heap(m_heapMem, m_start, args...))
    { 
    }

    virtual ~DelegateMsgHeapArgs() = default;

    /// Get all function arguments that were created on the heap
    /// @return A tuple of all function arguments
    std::tuple<Args...>& GetArgs() { return m_args; }

private:
    /// A list of heap allocated argument memory blocks
    xlist<std::shared_ptr<heap_arg_deleter_base>> m_heapMem;

    /// An empty starting tuple
    std::tuple<> m_start;

    /// A tuple with each element stored within the heap
    std::tuple<Args...> m_args;
};

}

#endif
