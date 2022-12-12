#ifndef _MULTICAST_DELEGATE_H
#define _MULTICAST_DELEGATE_H

#include "Delegate.h"
#include <list>
#include <algorithm>

namespace DelegateLib {

template <class R>
struct MulticastDelegate; // Not defined

/// @brief Not thread-safe multicast delegate container class. The class has a linked  
/// list of Delegate<> instances. When invoked, each Delegate instance within the invocation 
/// list is called. MulticastDelegate<> does not support return values. A void return  
/// must always be used.
template<class RetType, class... Args>
class MulticastDelegate<RetType(Args...)>
{
public:
    MulticastDelegate() = default;
    ~MulticastDelegate() { Clear(); }

    RetType operator()(Args... args) {
        for (Delegate<RetType(Args...)>* delegate : m_delegates)
            (*delegate)(args...);	// Invoke delegate callback
    }

    void operator+=(const Delegate<RetType(Args...)>& delegate) { 
        m_delegates.push_back(delegate.Clone());         
    }
    void operator-=(const Delegate<RetType(Args...)>& delegate) { 
        for (auto it = m_delegates.begin(); it != m_delegates.end(); ++it) 
        {
            if (*((DelegateBase*)&delegate) == *((DelegateBase*)(*it)))
            {
                delete (*it);
                m_delegates.erase(it);
                break;
            }
        }
    }

    /// Any registered delegates?
    bool Empty() const { return m_delegates.empty(); }

    /// Removal all registered delegates.
    void Clear() { 
        auto it = m_delegates.begin();
        while (it != m_delegates.end())
        {
            delete (*it);
            it = m_delegates.erase(it);
        }
    }

    explicit operator bool() const { return !Empty(); }

private:
    // Prevent copying objects
    MulticastDelegate(const MulticastDelegate&) = delete;
    MulticastDelegate& operator=(const MulticastDelegate&) = delete;

    /// List of registered delegates
    std::list<Delegate<RetType(Args...)>*> m_delegates;
};

}

#endif