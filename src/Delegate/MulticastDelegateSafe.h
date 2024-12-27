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
    using DelegateType = Delegate<RetType(Args...)>;
    using BaseType = MulticastDelegate<RetType(Args...)>;

    MulticastDelegateSafe() = default;
    ~MulticastDelegateSafe() = default; 
    MulticastDelegateSafe(const MulticastDelegateSafe& rhs) = delete;
    MulticastDelegateSafe(MulticastDelegateSafe&& rhs) = delete;

    /// Invoke the bound target function for all stored delegate instances.
    /// A void return value is used since multiple targets invoked.
    /// @param[in] args The farguments used when invoking the target function
    void operator()(Args... args) {
        const std::lock_guard<std::mutex> lock(m_lock);
        BaseType::operator ()(args...);
    }

    /// Insert a delegate into the container.
    /// @param[in] delegate A delegate target to insert
    void operator+=(const Delegate<RetType(Args...)>& delegate) {
        const std::lock_guard<std::mutex> lock(m_lock);
        BaseType::operator +=(delegate);
    }

    /// Insert a delegate into the container.
    /// @param[in] delegate A delegate target to insert
    void operator+=(const Delegate<RetType(Args...)>&& delegate) {
        const std::lock_guard<std::mutex> lock(m_lock);
        BaseType::operator +=(delegate);
    }

    /// Remove a delegate from the container.
    /// @param[in] delegate A delegate target to remove
    void operator-=(const Delegate<RetType(Args...)>& delegate) {
        const std::lock_guard<std::mutex> lock(m_lock);
        BaseType::operator -=(delegate);
    }

    /// Remove a delegate from the container.
    /// @param[in] delegate A delegate target to remove
    void operator-=(const Delegate<RetType(Args...)>&& delegate) {
        const std::lock_guard<std::mutex> lock(m_lock);
        BaseType::operator -=(delegate);
    }

    /// @brief Assignment operator that assigns the state of one object to another.
    /// @param[in] rhs The object whose state is to be assigned to the current object.
    /// @return A reference to the current object.
    MulticastDelegateSafe& operator=(const MulticastDelegateSafe& rhs) {
        const std::lock_guard<std::mutex> lock(m_lock);
        BaseType::operator=(rhs);
        return *this;
    }

    /// @brief Move assignment operator that transfers ownership of resources.
    /// @param[in] rhs The object to move from.
    /// @return A reference to the current object.
    MulticastDelegateSafe& operator=(MulticastDelegateSafe&& rhs) {
        const std::lock_guard<std::mutex> lock(m_lock);
        BaseType::operator=(std::forward<MulticastDelegateSafe>(rhs));
        return *this;
    }

    /// @brief Clear the all target functions.
    virtual void operator=(std::nullptr_t) noexcept { 
        const std::lock_guard<std::mutex> lock(m_lock);
        return BaseType::Clear(); 
    }

    /// Any registered delegates?
    /// @return `true` if delegate container is empty.
    bool Empty() {
        const std::lock_guard<std::mutex> lock(m_lock);
        return BaseType::Empty();
    }

    /// Removal all registered delegates.
    void Clear() {
       const std::lock_guard<std::mutex> lock(m_lock);
       BaseType::Clear();
    }

    /// Get the number of delegates stored.
    /// @return The number of delegates stored.
    std::size_t Size() { 
        const std::lock_guard<std::mutex> lock(m_lock);
        return BaseType::Size(); 
    }

    /// @brief Implicit conversion operator to `bool`.
    /// @return `true` if the container is not empty, `false` if the container is empty.
    explicit operator bool() {
        const std::lock_guard<std::mutex> lock(m_lock);
        return BaseType::operator bool();
    }

private:
    /// Lock to make the class thread-safe
    std::mutex m_lock;
};

}

#endif
