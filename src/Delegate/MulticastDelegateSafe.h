#ifndef _MULTICAST_DELEGATE_SAFE_H
#define _MULTICAST_DELEGATE_SAFE_H

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

    void operator+=(const Delegate<RetType(Args...)>& delegate) {
        const std::lock_guard<std::mutex> lock(m_lock);
        MulticastDelegate<RetType(Args...)>::operator +=(delegate);
    }
    void operator-=(const Delegate<RetType(Args...)>& delegate) {
        const std::lock_guard<std::mutex> lock(m_lock);
        MulticastDelegate<RetType(Args...)>::operator -=(delegate);
    }
    void operator()(Args... args) {
        const std::lock_guard<std::mutex> lock(m_lock);
        MulticastDelegate<RetType(Args...)>::operator ()(args...);
    }
    bool Empty() {
        const std::lock_guard<std::mutex> lock(m_lock);
        return MulticastDelegate<RetType(Args...)>::Empty();
    }
    void Clear() {
        const std::lock_guard<std::mutex> lock(m_lock);
        MulticastDelegate<RetType(Args...)>::Clear();
    }

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
