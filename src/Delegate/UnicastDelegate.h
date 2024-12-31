#ifndef _UNICAST_DELEGATE_H
#define _UNICAST_DELEGATE_H

/// @file
/// @brief Delegate container for storing an invoking a single delegate instance. 
/// Class is not thread-safe.

#include "Delegate.h"
#include <memory>

namespace DelegateLib {

template <class R>
struct UnicastDelegate; // Not defined

/// @brief A non-thread safe delegate container storing one delegate. Void and  
/// non-void return values supported. 
template<class RetType, class... Args>
class UnicastDelegate<RetType(Args...)>
{
public:
    using DelegateType = Delegate<RetType(Args...)>;

    UnicastDelegate() = default;
    ~UnicastDelegate() { Clear(); }

    /// @brief Copy constructor that creates a copy of the given instance.
    /// @details This constructor initializes a new object as a copy of the 
    /// provided `rhs` (right-hand side) object. The `rhs` object is used to 
    /// set the state of the new instance.
    /// @param[in] rhs The object to copy from.
    UnicastDelegate(const UnicastDelegate& rhs) { 
        auto delegateClone = rhs.m_delegate->Clone();
        std::shared_ptr<DelegateType> sharedDelegate(delegateClone);
        m_delegate = sharedDelegate;
    }

    /// @brief Move constructor that transfers ownership of resources.
    /// @param[in] rhs The object to move from.
    UnicastDelegate(UnicastDelegate&& rhs) noexcept : m_delegate(std::move(rhs.m_delegate)) { }

    /// Invoke the bound target.
    /// @param[in] args The arguments used when invoking the target function
    /// @return The target function return value. 
    RetType operator()(Args... args) {
        return (*m_delegate)(args...);	// Invoke delegate callback
    }

    /// Invoke the bound target functions. 
    /// @param[in] args The arguments used when invoking the target function
    void Broadcast(Args... args) {
        (*this)(args...);
    }

    /// Assign a delegate to the container.
    /// @param[in] rhs A delegate target to assign
    void operator=(const DelegateType& rhs) {
        auto delegateClone = rhs.Clone();
        std::shared_ptr<DelegateType> sharedDelegate(delegateClone);
        m_delegate = sharedDelegate;
    }

    /// Assign a delegate to the container.
    /// @param[in] rhs A delegate target to assign
    void operator=(DelegateType&& rhs) {
        auto delegateClone = rhs.Clone();
        std::shared_ptr<DelegateType> sharedDelegate(delegateClone);
        m_delegate = std::move(sharedDelegate);
    }

    /// @brief Assignment operator that assigns the state of one object to another.
    /// @param[in] rhs The object whose state is to be assigned to the current object.
    /// @return A reference to the current object.
    UnicastDelegate& operator=(const UnicastDelegate& rhs) {
        if (&rhs != this) {
            auto delegateClone = rhs.m_delegate->Clone();
            std::shared_ptr<DelegateType> sharedDelegate(delegateClone);
            m_delegate = sharedDelegate;
        }
        return *this;
    }

    /// @brief Move assignment operator that transfers ownership of resources.
    /// @param[in] rhs The object to move from.
    /// @return A reference to the current object.
    UnicastDelegate& operator=(UnicastDelegate&& rhs) noexcept {
        if (&rhs != this) {
            m_delegate = rhs.m_delegate;
            rhs.Clear();
        }
        return *this;
    }

    /// @brief Clear the all target functions.
    virtual void operator=(std::nullptr_t) noexcept { Clear(); }

    /// Any registered delegates?
    /// @return `true` if delegate container is empty.
    bool Empty() const { return !m_delegate; }

    /// Remove the registered delegate
    void Clear() { m_delegate = nullptr; }

    /// Get the number of delegates stored.
    /// @return The number of delegates stored.
    std::size_t Size() const { return m_delegate == nullptr ? 0 : 1; }

    /// @brief Implicit conversion operator to `bool`.
    /// @return `true` if the container is not empty, `false` if the container is empty.
    explicit operator bool() const { return !Empty(); }

private:
    /// Registered delegate.
    std::shared_ptr<DelegateType> m_delegate = nullptr;
};

}

#endif
