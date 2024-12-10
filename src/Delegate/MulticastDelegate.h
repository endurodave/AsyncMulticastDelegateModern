#ifndef _MULTICAST_DELEGATE_H
#define _MULTICAST_DELEGATE_H

/// @file
/// @brief Delegate container for storing and iterating over a collection of 
/// delegate instances. Class is not thread safe.

#include "Delegate.h"
#include <list>
#include <algorithm>

namespace DelegateLib {

template <class R>
struct MulticastDelegate; // Not defined

/// @brief Not thread-safe multicast delegate container class. The class has a list of 
/// `Delegate<>` instances. When invoked, each `Delegate` instance within the invocation 
/// list is called. 
template<class RetType, class... Args>
class MulticastDelegate<RetType(Args...)>
{
public:
    MulticastDelegate() = default;
    ~MulticastDelegate() { Clear(); }

    /// Invoke the bound target function for all stored delegate instances.
    /// A void return value is used since multiple targets invoked.
    /// @param[in] args The farguments used when invoking the target function
    void operator()(Args... args) {
        for (Delegate<RetType(Args...)>* delegate : m_delegates)
            (*delegate)(args...);	// Invoke delegate callback
    }

    /// Insert a delegate into the container.
    /// @param[in] delegate A delegate target to insert
    void operator+=(const Delegate<RetType(Args...)>& delegate) { 
        m_delegates.push_back(delegate.Clone());         
    }

    /// Remove a delegate from the container.
    /// @param[in] delegate A delegate target to remove
    void operator-=(const Delegate<RetType(Args...)>& delegate) { 
        for (auto it = m_delegates.begin(); it != m_delegates.end(); ++it) 
        {
            if (delegate == **it)
            {
                delete (*it);
                m_delegates.erase(it);
                break;
            }
        }
    }

    /// Any registered delegates?
    /// @return `true` if delegate container is empty.
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

    /// Get the number of delegates stored.
    /// @return The number of delegates stored.
    std::size_t Size() const { return m_delegates.size(); }

    /// @brief Implicit conversion operator to `bool`.
    /// @return `true` if the container is not empty, `false` if the container is empty.
    explicit operator bool() const { return !Empty(); }

private:
    // Prevent copying objects
    MulticastDelegate(const MulticastDelegate&) = delete;
    MulticastDelegate& operator=(const MulticastDelegate&) = delete;

    /// List of registered delegates
    xlist<Delegate<RetType(Args...)>*> m_delegates;
};

}

#endif