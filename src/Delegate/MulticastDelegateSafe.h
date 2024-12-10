#ifndef _MULTICAST_DELEGATE_SAFE_H
#define _MULTICAST_DELEGATE_SAFE_H

/// @file
/// @brief Delegate container for storing and iterating over a collection of 
/// delegate instances. Class is thread safe.

#include "MulticastDelegate.h"
#include <mutex>

namespace DelegateLib {

template <class R>
struct MulticastDelegateSafe; // Not defined

/// @brief Thread-safe multicast delegate container class. 
template<class RetType, class... Args>
class MulticastDelegateSafe<RetType(Args...)> : public MulticastDelegate<RetType(Args...)>
{
public:
    MulticastDelegateSafe() = default;
    ~MulticastDelegateSafe() = default; 

    /// Invoke the bound target function for all stored delegate instances.
    /// A void return value is used since multiple targets invoked.
    /// @param[in] args The farguments used when invoking the target function
    void operator()(Args... args) {
        const std::lock_guard<std::mutex> lock(m_lock);
        MulticastDelegate<RetType(Args...)>::operator ()(args...);
    }

    /// Insert a delegate into the container.
    /// @param[in] delegate A delegate target to insert
    void operator+=(const Delegate<RetType(Args...)>& delegate) {
        const std::lock_guard<std::mutex> lock(m_lock);
        MulticastDelegate<RetType(Args...)>::operator +=(delegate);
    }

    /// Remove a delegate from the container.
    /// @param[in] delegate A delegate target to remove
    void operator-=(const Delegate<RetType(Args...)>& delegate) {
        const std::lock_guard<std::mutex> lock(m_lock);
        MulticastDelegate<RetType(Args...)>::operator -=(delegate);
    }

    /// Any registered delegates?
    /// @return `true` if delegate container is empty.
    bool Empty() {
        const std::lock_guard<std::mutex> lock(m_lock);
        return MulticastDelegate<RetType(Args...)>::Empty();
    }

    /// Removal all registered delegates.
    void Clear() {
        const std::lock_guard<std::mutex> lock(m_lock);
        MulticastDelegate<RetType(Args...)>::Clear();
    }

    /// @brief Implicit conversion operator to `bool`.
    /// @return `true` if the container is not empty, `false` if the container is empty.
    explicit operator bool() {
        const std::lock_guard<std::mutex> lock(m_lock);
        return MulticastDelegate<RetType(Args...)>::operator bool();
    }

private:
    // Prevent copying objects
    MulticastDelegateSafe(const MulticastDelegateSafe&) = delete;
    MulticastDelegateSafe& operator=(const MulticastDelegateSafe&) = delete;

    /// Lock to make the class thread-safe
    std::mutex m_lock;
};

}

#endif
