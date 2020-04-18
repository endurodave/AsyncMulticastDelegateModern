#ifndef _MULTICAST_DELEGATE_SAFE_H
#define _MULTICAST_DELEGATE_SAFE_H

#include "MulticastDelegate.h"
#include "LockGuard.h"

namespace DelegateLib {

template <class R>
struct MulticastDelegateSafe; // Not defined

/// @brief Thread-safe multicast delegate container class. 
template<class RetType, class... Args>
class MulticastDelegateSafe<RetType(Args...)> : public MulticastDelegate<RetType(Args...)>
{
public:
    MulticastDelegateSafe() { LockGuard::Create(&m_lock); }
    ~MulticastDelegateSafe() { LockGuard::Destroy(&m_lock); }

    void operator+=(const Delegate<RetType(Args...)>& delegate) {
        LockGuard lockGuard(&m_lock);
        MulticastDelegate<RetType(Args...)>::operator +=(delegate);
    }
    void operator-=(const Delegate<RetType(Args...)>& delegate) {
        LockGuard lockGuard(&m_lock);
        MulticastDelegate<RetType(Args...)>::operator -=(delegate);
    }
    void operator()(Args... args) {
        LockGuard lockGuard(&m_lock);
        MulticastDelegate<RetType(Args...)>::operator ()(args...);
    }
    bool Empty() {
        LockGuard lockGuard(&m_lock);
        return MulticastDelegate<RetType(Args...)>::Empty();
    }
    void Clear() {
        LockGuard lockGuard(&m_lock);
        MulticastDelegate<RetType(Args...)>::Clear();
    }

    explicit operator bool() {
        LockGuard lockGuard(&m_lock);
        return MulticastDelegate<RetType(Args...)>::operator bool();
    }

private:
    // Prevent copying objects
    MulticastDelegateSafe(const MulticastDelegateSafe&) = delete;
    MulticastDelegateSafe& operator=(const MulticastDelegateSafe&) = delete;

    /// Lock to make the class thread-safe
    LOCK m_lock;
};

}

#endif
