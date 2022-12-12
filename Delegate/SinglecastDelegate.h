#ifndef _SINGLECAST_DELEGATE_H
#define _SINGLECAST_DELEGATE_H

#include "Delegate.h"

namespace DelegateLib {

template <class R>
struct SinglecastDelegate; // Not defined

/// @brief A non-thread safe delegate container storing one delegate. Void and  
/// non-void return values supported. 
template<class RetType, class... Args>
class SinglecastDelegate<RetType(Args...)>
{
public:
    SinglecastDelegate() = default;
    ~SinglecastDelegate() { Clear(); }

    RetType operator()(Args... args) {
        return (*m_delegate)(args...);	// Invoke delegate callback
    }

    void operator=(const Delegate<RetType(Args...)>& delegate) {
        Clear();
        m_delegate = delegate.Clone();	// Create a duplicate delegate
    }

    void operator=(const Delegate<RetType(Args...)>* delegate) {
        Clear();
        if (delegate)
            m_delegate = delegate->Clone();  // Create a duplicate delegate
    }

    /// Any registered delegates?
    bool Empty() const { return !m_delegate; }

    /// Remove registered delegate
    void Clear() {
        if (m_delegate) {
            delete m_delegate;
            m_delegate = nullptr;
        }
    }

    explicit operator bool() const { return !Empty(); }

private:
    // Prevent copying objects
    SinglecastDelegate(const SinglecastDelegate&) = delete;
    SinglecastDelegate& operator=(const SinglecastDelegate&) = delete;

    /// Registered delegate.
    Delegate<RetType(Args...)>* m_delegate = nullptr;
};

}

#endif
