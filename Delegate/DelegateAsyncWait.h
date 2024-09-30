#ifndef _DELEGATE_ASYNC_WAIT_H
#define _DELEGATE_ASYNC_WAIT_H

// DelegateAsyncWait.h
// @see https://github.com/endurodave/AsyncMulticastDelegateModern
// David Lafreniere, Aug 2020.

#include "Delegate.h"
#include "DelegateThread.h"
#include "DelegateInvoker.h"
#include "Semaphore.h"
#include <optional>
#include <any>
#include <chrono>

/// @brief Asynchronous member delegate that invokes the target function on the specified thread of control
/// and waits for the function to be executed or a timeout occurs. Use IsSuccess() to determine if asynchronous 
/// call succeeded.

namespace DelegateLib {

#undef max  // Prevent compiler error on next line if max is defined
constexpr auto WAIT_INFINITE = std::chrono::milliseconds::max();

template <class R>
struct DelegateFreeAsyncWait; // Not defined

template <class RetType, class... Args>
class DelegateFreeAsyncWait<RetType(Args...)> : public DelegateFree<RetType(Args...)>, public IDelegateInvoker {
public:
    typedef RetType(*FreeFunc)(Args...);
    using ClassType = DelegateFreeAsyncWait<RetType(Args...)>;
    using BaseType = DelegateFree<RetType(Args...)>;

    // Contructors take a free function, delegate thread and timeout
    DelegateFreeAsyncWait(FreeFunc func, DelegateThread& thread, std::chrono::milliseconds timeout) : 
        BaseType(func), m_thread(thread), m_timeout(timeout) {
        Bind(func, thread);
    }
    DelegateFreeAsyncWait(const DelegateFreeAsyncWait& rhs) : BaseType(rhs), m_thread(rhs.m_thread), m_sync(false) {
        Swap(rhs);
    }
    DelegateFreeAsyncWait() = delete;

    virtual ClassType* Clone() const override {
        return new ClassType(*this);
    }

    /// Bind a free function to a delegate. 
    void Bind(FreeFunc func, DelegateThread& thread) {
        m_thread = thread;
        BaseType::Bind(func);
    }

    virtual bool operator==(const DelegateBase& rhs) const override {
        auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
        return derivedRhs &&
            &m_thread == &derivedRhs->m_thread &&
            BaseType::operator==(rhs);
    }

    DelegateFreeAsyncWait& operator=(const DelegateFreeAsyncWait& rhs) {
        if (&rhs != this) {
            BaseType::operator=(rhs);
            Swap(rhs);
        }
        return *this;
    }

    /// Invoke delegate function asynchronously
    virtual RetType operator()(Args... args) override {
        if (m_sync)
            return BaseType::operator()(args...);
        else
        {
            // Create a clone instance of this delegate 
            auto delegate = std::shared_ptr<ClassType>(Clone());

            // Create a new message instance 
            auto msg = std::make_shared<DelegateMsg<Args...>>(delegate, args...);

            // Dispatch message onto the callback destination thread. DelegateInvoke()
            // will be called by the target thread. 
            m_thread.DispatchDelegate(msg);

            // Wait for target thread to execute the delegate function
            if ((m_success = delegate->m_sema.Wait(m_timeout)))
                m_retVal = delegate->m_retVal;

            if constexpr (std::is_void<RetType>::value == false)
            {
                if (m_retVal.has_value())
                    return std::any_cast<RetType>(m_retVal);
                else
                    return RetType();
            }
        }
    }

    /// Invoke delegate function asynchronously
    auto AsyncInvoke(Args... args)
    {
        if constexpr (std::is_void<RetType>::value == true)
        {
            operator()(args...);
            return IsSuccess() ? std::optional<bool>(true) : std::optional<bool>();
        }
        else
        {
            auto retVal = operator()(args...);
            return IsSuccess() ? std::optional<RetType>(retVal) : std::optional<RetType>();
        }
    }

    /// Called by the target thread to invoke the delegate function 
    virtual void DelegateInvoke(std::shared_ptr<DelegateMsgBase> msg) override {
        // Typecast the base pointer to back to the templatized instance
        auto delegateMsg = std::dynamic_pointer_cast<DelegateMsg<Args...>>(msg);
        if (delegateMsg == nullptr)
            throw std::invalid_argument("Invalid DelegateMsg cast");

        // Invoke the delegate function then signal the waiting thread
        m_sync = true;
        if constexpr(std::is_void<RetType>::value == true)
            std::apply(&BaseType::operator(), std::tuple_cat(std::make_tuple(this), delegateMsg->GetArgs()));
        else
            m_retVal = std::apply(&BaseType::operator(), std::tuple_cat(std::make_tuple(this), delegateMsg->GetArgs()));
        m_sema.Signal();
    }

    /// Returns true if asynchronous function successfully invoked on target thread
    bool IsSuccess() { return m_success; }

    /// Returns the async function return value
    RetType GetRetVal() { return std::any_cast<RetType>(m_retVal); }

private:
    void Swap(const DelegateFreeAsyncWait& s) {
        m_thread = s.m_thread;
        m_timeout = s.m_timeout;
        m_success = s.m_success;
    }

    DelegateThread& m_thread;               // Target thread to invoke the delegate function
    bool m_success = false;			        // Set to true if async function succeeds
    std::chrono::milliseconds m_timeout;    // Time in mS to wait for async function to invoke
    Semaphore m_sema;				        // Semaphore to signal waiting thread
    bool m_sync = false;                    // Set true when synchronous invocation is required
    std::any m_retVal;                      // Return value of the invoked function
};

template <class C, class R>
struct DelegateMemberAsyncWait; // Not defined

template <class TClass, class RetType, class... Args>
class DelegateMemberAsyncWait<TClass, RetType(Args...)> : public DelegateMember<TClass, RetType(Args...)>, public IDelegateInvoker {
public:
    typedef TClass* ObjectPtr;
    typedef RetType(TClass::*MemberFunc)(Args...);
    typedef RetType(TClass::*ConstMemberFunc)(Args...) const;
    using ClassType = DelegateMemberAsyncWait<TClass, RetType(Args...)>;
    using BaseType = DelegateMember<TClass, RetType(Args...)>;

    // Contructors take a class instance, member function, and delegate thread
    DelegateMemberAsyncWait(ObjectPtr object, MemberFunc func, DelegateThread& thread, std::chrono::milliseconds timeout) : 
        BaseType(object, func), m_thread(thread), m_timeout(timeout) {
        Bind(object, func, thread);
    }
    DelegateMemberAsyncWait(ObjectPtr object, ConstMemberFunc func, DelegateThread& thread, std::chrono::milliseconds timeout) : 
        BaseType(object, func), m_thread(thread), m_timeout(timeout) {
        Bind(object, func, thread);
    }
    DelegateMemberAsyncWait(const DelegateMemberAsyncWait& rhs) : BaseType(rhs), m_thread(rhs.m_thread), m_sync(false) {
        Swap(rhs);
    }
    DelegateMemberAsyncWait() = delete;

    virtual ClassType* Clone() const override {
        return new ClassType(*this);
    }

    /// Bind a member function to a delegate. 
    void Bind(ObjectPtr object, MemberFunc func, DelegateThread& thread) {
        m_thread = thread;
        BaseType::Bind(object, func);
    }

    /// Bind a const member function to a delegate. 
    void Bind(ObjectPtr object, ConstMemberFunc func, DelegateThread& thread) {
        m_thread = thread;
        BaseType::Bind(object, func);
    }

    virtual bool operator==(const DelegateBase& rhs) const override {
        auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
        return derivedRhs &&
            &m_thread == &derivedRhs->m_thread &&
            BaseType::operator==(rhs);
    }

    DelegateMemberAsyncWait& operator=(const DelegateMemberAsyncWait& rhs) {
        if (&rhs != this) {
            BaseType::operator=(rhs);
            Swap(rhs);
        }
        return *this;
    }

    /// Invoke delegate function asynchronously
    virtual RetType operator()(Args... args) override {
        if (m_sync)
            return BaseType::operator()(args...);
        else
        {
            // Create a clone instance of this delegate 
            auto delegate = std::shared_ptr<ClassType>(Clone());

            // Create a new message instance 
            auto msg = std::make_shared<DelegateMsg<Args...>>(delegate, args...);

            // Dispatch message onto the callback destination thread. DelegateInvoke()
            // will be called by the target thread. 
            m_thread.DispatchDelegate(msg);

            // Wait for target thread to execute the delegate function
            if ((m_success = delegate->m_sema.Wait(m_timeout)))
                m_retVal = delegate->m_retVal;

            if constexpr (std::is_void<RetType>::value == false)
            {
                if (m_retVal.has_value())
                    return std::any_cast<RetType>(m_retVal);
                else
                    return RetType();
            }
        }
    }

    /// Invoke delegate function asynchronously
    auto AsyncInvoke(Args... args)
    {
		if constexpr (std::is_void<RetType>::value == true)
		{
			operator()(args...);
			return IsSuccess() ? std::optional<bool>(true) : std::optional<bool>();
		}
		else
		{
			auto retVal = operator()(args...);
			return IsSuccess() ? std::optional<RetType>(retVal) : std::optional<RetType>();
		}
    }

    /// Called by the target thread to invoke the delegate function 
    virtual void DelegateInvoke(std::shared_ptr<DelegateMsgBase> msg) override {
        // Typecast the base pointer to back to the templatized instance
        auto delegateMsg = std::dynamic_pointer_cast<DelegateMsg<Args...>>(msg);
        if (delegateMsg == nullptr)
            throw std::invalid_argument("Invalid DelegateMsg cast");

        // Invoke the delegate function then signal the waiting thread
        m_sync = true;
        if constexpr (std::is_void<RetType>::value == true)
            std::apply(&BaseType::operator(), std::tuple_cat(std::make_tuple(this), delegateMsg->GetArgs()));
        else
            m_retVal = std::apply(&BaseType::operator(), std::tuple_cat(std::make_tuple(this), delegateMsg->GetArgs()));
        m_sema.Signal();
    }

    /// Returns true if asynchronous function successfully invoked on target thread
    bool IsSuccess() { return m_success; }

    /// Returns the async function return value
    RetType GetRetVal() { return std::any_cast<RetType>(m_retVal); }

private:
    void Swap(const DelegateMemberAsyncWait& s) {
        m_thread = s.m_thread;
        m_timeout = s.m_timeout;
        m_success = s.m_success;
    }

    DelegateThread& m_thread;	            // Target thread to invoke the delegate function
    bool m_success = false;					// Set to true if async function succeeds
    std::chrono::milliseconds m_timeout ;   // Time in mS to wait for async function to invoke
    Semaphore m_sema;				        // Semaphore to signal waiting thread
    bool m_sync = false;                    // Set true when synchronous invocation is required
    std::any m_retVal;                      // Return value of the invoked function
};

template <class TClass, class RetType, class... Args>
DelegateMemberAsyncWait<TClass, RetType(Args...)> MakeDelegate(TClass* object, RetType(TClass::*func)(Args... args), DelegateThread& thread, std::chrono::milliseconds timeout) {
    return DelegateMemberAsyncWait<TClass, RetType(Args...)>(object, func, thread, timeout);
}

template <class TClass, class RetType, class... Args>
DelegateMemberAsyncWait<TClass, RetType(Args...)> MakeDelegate(TClass* object, RetType(TClass::*func)(Args... args) const, DelegateThread& thread, std::chrono::milliseconds timeout) {
    return DelegateMemberAsyncWait<TClass, RetType(Args...)>(object, func, thread, timeout);
}

template <class RetType, class... Args>
DelegateFreeAsyncWait<RetType(Args...)> MakeDelegate(RetType(*func)(Args... args), DelegateThread& thread, std::chrono::milliseconds timeout) {
    return DelegateFreeAsyncWait<RetType(Args...)>(func, thread, timeout);
}

} 

#endif
