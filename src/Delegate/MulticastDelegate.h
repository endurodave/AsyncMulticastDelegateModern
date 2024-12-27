#ifndef _MULTICAST_DELEGATE_H
#define _MULTICAST_DELEGATE_H

/// @file
/// @brief Delegate container for storing and iterating over a collection of 
/// delegate instances. Class is not thread safe.

#include "Delegate.h"
#include <list>
#include <algorithm>
#include <memory>

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
    using DelegateType = Delegate<RetType(Args...)>;

    MulticastDelegate() = default;
    ~MulticastDelegate() { Clear(); }

    /// @brief Copy constructor that creates a copy of the given instance.
    /// @details This constructor initializes a new object as a copy of the 
    /// provided `rhs` (right-hand side) object. The `rhs` object is used to 
    /// set the state of the new instance.
    /// @param[in] rhs The object to copy from.
    MulticastDelegate(const MulticastDelegate& rhs) {
        for (auto delegate : rhs.m_delegates) {
            auto delegateClone = delegate->Clone();
            std::shared_ptr<DelegateType> sharedDelegate(delegateClone);
            m_delegates.push_back(sharedDelegate);
        }
    }

    /// @brief Move constructor that transfers ownership of resources.
    /// @param[in] rhs The object to move from.
    MulticastDelegate(MulticastDelegate&& rhs) noexcept : m_delegates(std::move(rhs.m_delegates)) { }

    /// Invoke the bound target function for all stored delegate instances.
    /// A void return value is used since multiple targets invoked.
    /// @param[in] args The arguments used when invoking the target function
    void operator()(Args... args) {
        for (auto delegate : m_delegates)
            (*delegate)(args...);	// Invoke delegate callback
    }

    /// Insert a delegate into the container.
    /// @param[in] delegate A delegate target to insert
    void operator+=(const DelegateType& delegate) {
        auto delegateClone = delegate.Clone();
        std::shared_ptr<DelegateType> sharedDelegate(delegateClone);
        m_delegates.push_back(sharedDelegate);
    }

    /// Insert a delegate into the container.
    /// @param[in] delegate A delegate target to insert
    void operator+=(const DelegateType&& delegate) {
        auto delegateClone = delegate.Clone(); 
        std::shared_ptr<DelegateType> sharedDelegate(delegateClone);
        m_delegates.push_back(std::move(sharedDelegate));
    }

    /// Remove a delegate from the container.
    /// @param[in] delegate A delegate target to remove
    void operator-=(const DelegateType& delegate) {
        for (auto it = m_delegates.begin(); it != m_delegates.end(); ++it) {
            if (delegate == **it) {
                m_delegates.erase(it);
                break;
            }
        }
    }

    /// Remove a delegate from the container.
    /// @param[in] delegate A delegate target to remove
    void operator-=(DelegateType&& delegate) {
        for (auto it = m_delegates.begin(); it != m_delegates.end(); ++it) {
            if (delegate == **it) {
                m_delegates.erase(it);
                break;
            }
        }
    }

    /// @brief Assignment operator that assigns the state of one object to another.
    /// @param[in] rhs The object whose state is to be assigned to the current object.
    /// @return A reference to the current object.
    MulticastDelegate& operator=(const MulticastDelegate& rhs) {
        if (&rhs != this) {
            Clear();
            for (auto delegate : rhs.m_delegates) {
                auto delegateClone = delegate->Clone();
                std::shared_ptr<DelegateType> sharedDelegate(delegateClone);
                m_delegates.push_back(sharedDelegate);
            }
        }
        return *this;
    }

    /// @brief Move assignment operator that transfers ownership of resources.
    /// @param[in] rhs The object to move from.
    /// @return A reference to the current object.
    MulticastDelegate& operator=(MulticastDelegate&& rhs) noexcept {
        if (&rhs != this) {
            m_delegates = rhs.m_delegates;
            rhs.Clear();
        }
        return *this;
    }

    /// @brief Clear the all target functions.
    virtual void operator=(std::nullptr_t) noexcept { Clear(); }

    /// Any registered delegates?
    /// @return `true` if delegate container is empty.
    bool Empty() const { return m_delegates.empty(); }

    /// Removal all registered delegates.
    void Clear() { m_delegates.clear(); }

    /// Get the number of delegates stored.
    /// @return The number of delegates stored.
    std::size_t Size() const { return m_delegates.size(); }

    /// @brief Implicit conversion operator to `bool`.
    /// @return `true` if the container is not empty, `false` if the container is empty.
    explicit operator bool() const { return !Empty(); }

private:
    /// List of registered delegates
    xlist<std::shared_ptr<DelegateType>> m_delegates;
};

}

#endif