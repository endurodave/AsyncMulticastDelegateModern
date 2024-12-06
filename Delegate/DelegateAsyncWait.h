#ifndef _DELEGATE_ASYNC_WAIT_H
#define _DELEGATE_ASYNC_WAIT_H

// DelegateAsyncWait.h
// @see https://github.com/endurodave/AsyncMulticastDelegateModern
// David Lafreniere, Aug 2020.

#include "DelegateAsync.h"
#include "DelegateThread.h"
#include "DelegateInvoker.h"
#include "Semaphore.h"
#include <optional>
#include <any>
#include <chrono>

// Asynchronous member delegate that invokes the target function on the specified thread of control
// and waits for the function to be executed or a timeout occurs. Use IsSuccess() to determine if 
// asynchronous call succeeded.
// 
// Delegate "AsyncWait" series of classes used to invoke a function asynchronously and wait for 
// completion by the destination target thread. Invoking a function asynchronously requires making 
// a clone of the object to be sent to the destination thread message queue. The destination thread 
// calls DelegateInvoke() to invoke the target function. The source thread blocks on a semaphore 
// waiting for the destination thread to complete the function invoke. If the caller timeout expires, 
// the target function is not invoked. 
// 
// The m_lock mutex is used to protect shared state data between the source and destination 
// threads using the two thread safe functions below:
//
// RetType operator()(Args... args) - called by the source thread to initiate the async
// function call.
//
// void DelegateInvoke(std::shared_ptr<DelegateMsgBase> msg) - called by the destination
// thread to invoke the target function. The destination thread must not call any other
// delegate instance functions.
//
// Code within <common_code> and </common_code> is updated using sync_src.py. Manually update 
// the code within the DelegateFreeAsyncWait common_code tags, then run the script to 
// propagate to the remaining delegate classes to simplify code maintenance.
// 
// python src_dup.py DelegateAsyncWait.h

namespace DelegateLib {

#undef max  // Prevent compiler error on next line if max is defined
constexpr auto WAIT_INFINITE = std::chrono::milliseconds::max();

template <class R>
struct DelegateFreeAsyncWait; // Not defined

// DelegateFreeAsyncWait class asynchronously block invokes a free target function.
template <class RetType, class... Args>
class DelegateFreeAsyncWait<RetType(Args...)> : public DelegateFreeAsync<RetType(Args...)> {
public:
    typedef RetType(*FreeFunc)(Args...);
    using ClassType = DelegateFreeAsyncWait<RetType(Args...)>;
    using BaseType = DelegateFreeAsync<RetType(Args...)>;

    // Contructors take a free function, delegate thread and timeout
    ClassType(FreeFunc func, DelegateThread& thread, std::chrono::milliseconds timeout) :
        BaseType(func, thread), m_timeout(timeout) {
        Bind(func, thread);
    }

    /// Bind a free function to a delegate. 
    void Bind(FreeFunc func, DelegateThread& thread) {
        BaseType::Bind(func, thread);
    }

    // <common_code>
    ClassType(const ClassType& rhs) :
        BaseType(rhs) {
        Assign(rhs);
    }
    ClassType() = delete;

    void Assign(const ClassType& rhs) {
        m_timeout = rhs.m_timeout;
        BaseType::Assign(rhs);
    }

    virtual ClassType* Clone() const override {
        return new ClassType(*this);
    }

    ClassType& operator=(const ClassType& rhs) {
        if (&rhs != this) {
            BaseType::operator=(rhs);
            Assign(rhs);
        }
        return *this;
    }

    virtual bool operator==(const DelegateBase& rhs) const override {
        auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
        return derivedRhs &&
            m_timeout == derivedRhs->m_timeout &&
            BaseType::operator==(rhs);
    }

    // Invoke delegate function asynchronously and block for function return value
    virtual RetType operator()(Args... args) override {
        if (this->GetSync())
            return BaseType::operator()(args...);
        else
        {
            // Create a clone instance of this delegate 
            auto delegate = std::shared_ptr<ClassType>(Clone());
            delegate->m_invokerWaiting = true;

            // Create a new message instance 
            auto msg = std::make_shared<DelegateMsg<Args...>>(delegate, args...);

            // Dispatch message onto the callback destination thread. DelegateInvoke()
            // will be called by the target thread. 
            GetThread().DispatchDelegate(msg);

            // Wait for target thread to execute the delegate function
            if ((m_success = delegate->m_sema.Wait(m_timeout)))
                m_retVal = delegate->m_retVal;

            const std::lock_guard<std::mutex> lock(m_lock);
            delegate->m_invokerWaiting = false;

            if constexpr (std::is_void<RetType>::value == false)
            {
                if (m_retVal.has_value())
                    return std::any_cast<RetType>(m_retVal);
                else
                    return RetType();
            }
        }
    }

    // Invoke delegate function asynchronously and block for function return value
    auto AsyncInvoke(Args... args) {
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
        const std::lock_guard<std::mutex> lock(m_lock);
        if (this->m_invokerWaiting)
        {
            // Typecast the base pointer to back to the templatized instance
            auto delegateMsg = std::dynamic_pointer_cast<DelegateMsg<Args...>>(msg);
            if (delegateMsg == nullptr)
                throw std::invalid_argument("Invalid DelegateMsg cast");

            // Invoke the delegate function then signal the waiting thread
            SetSync(true);
            if constexpr (std::is_void<RetType>::value == true)
                std::apply(&BaseType::operator(), std::tuple_cat(std::make_tuple(this), delegateMsg->GetArgs()));
            else
                m_retVal = std::apply(&BaseType::operator(), std::tuple_cat(std::make_tuple(this), delegateMsg->GetArgs()));
            m_sema.Signal();
        }
    }

    /// Returns true if asynchronous function successfully invoked on target thread
    bool IsSuccess() { return m_success; }

    /// Returns the async function return value
    RetType GetRetVal() { return std::any_cast<RetType>(m_retVal); }

private:
    bool m_success = false;			        // Set to true if async function succeeds
    std::chrono::milliseconds m_timeout;    // Time in mS to wait for async function to invoke
    Semaphore m_sema;				        // Semaphore to signal waiting thread
    std::any m_retVal;                      // Return value of the invoked function
    bool m_invokerWaiting = false;          // True if caller thread is waiting for invoke complete
    std::mutex m_lock;
    // </common_code>
};

template <class C, class R>
struct DelegateMemberAsyncWait; // Not defined

// DelegateMemberAsyncWait class asynchronously block invokes a class member target function.
template <class TClass, class RetType, class... Args>
class DelegateMemberAsyncWait<TClass, RetType(Args...)> : public DelegateMemberAsync<TClass, RetType(Args...)> {
public:
    typedef TClass* ObjectPtr;
    typedef RetType(TClass::*MemberFunc)(Args...);
    typedef RetType(TClass::*ConstMemberFunc)(Args...) const;
    using ClassType = DelegateMemberAsyncWait<TClass, RetType(Args...)>;
    using BaseType = DelegateMemberAsync<TClass, RetType(Args...)>;

    // Contructors take a class instance, member function, and delegate thread
    ClassType(ObjectPtr object, MemberFunc func, DelegateThread& thread, std::chrono::milliseconds timeout) :
        BaseType(object, func, thread), m_timeout(timeout) {
        Bind(object, func, thread);
    }
    ClassType(ObjectPtr object, ConstMemberFunc func, DelegateThread& thread, std::chrono::milliseconds timeout) :
        BaseType(object, func, thread), m_timeout(timeout) {
        Bind(object, func, thread);
    }

    /// Bind a member function to a delegate. 
    void Bind(ObjectPtr object, MemberFunc func, DelegateThread& thread) {
        BaseType::Bind(object, func, thread);
    }

    /// Bind a const member function to a delegate. 
    void Bind(ObjectPtr object, ConstMemberFunc func, DelegateThread& thread) {
        BaseType::Bind(object, func, thread);
    }

    // <common_code>
    ClassType(const ClassType& rhs) :
        BaseType(rhs) {
        Assign(rhs);
    }
    ClassType() = delete;

    void Assign(const ClassType& rhs) {
        m_timeout = rhs.m_timeout;
        BaseType::Assign(rhs);
    }

    virtual ClassType* Clone() const override {
        return new ClassType(*this);
    }

    ClassType& operator=(const ClassType& rhs) {
        if (&rhs != this) {
            BaseType::operator=(rhs);
            Assign(rhs);
        }
        return *this;
    }

    virtual bool operator==(const DelegateBase& rhs) const override {
        auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
        return derivedRhs &&
            m_timeout == derivedRhs->m_timeout &&
            BaseType::operator==(rhs);
    }

    // Invoke delegate function asynchronously and block for function return value
    virtual RetType operator()(Args... args) override {
        if (this->GetSync())
            return BaseType::operator()(args...);
        else
        {
            // Create a clone instance of this delegate 
            auto delegate = std::shared_ptr<ClassType>(Clone());
            delegate->m_invokerWaiting = true;

            // Create a new message instance 
            auto msg = std::make_shared<DelegateMsg<Args...>>(delegate, args...);

            // Dispatch message onto the callback destination thread. DelegateInvoke()
            // will be called by the target thread. 
            GetThread().DispatchDelegate(msg);

            // Wait for target thread to execute the delegate function
            if ((m_success = delegate->m_sema.Wait(m_timeout)))
                m_retVal = delegate->m_retVal;

            const std::lock_guard<std::mutex> lock(m_lock);
            delegate->m_invokerWaiting = false;

            if constexpr (std::is_void<RetType>::value == false)
            {
                if (m_retVal.has_value())
                    return std::any_cast<RetType>(m_retVal);
                else
                    return RetType();
            }
        }
    }

    // Invoke delegate function asynchronously and block for function return value
    auto AsyncInvoke(Args... args) {
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
        const std::lock_guard<std::mutex> lock(m_lock);
        if (this->m_invokerWaiting)
        {
            // Typecast the base pointer to back to the templatized instance
            auto delegateMsg = std::dynamic_pointer_cast<DelegateMsg<Args...>>(msg);
            if (delegateMsg == nullptr)
                throw std::invalid_argument("Invalid DelegateMsg cast");

            // Invoke the delegate function then signal the waiting thread
            SetSync(true);
            if constexpr (std::is_void<RetType>::value == true)
                std::apply(&BaseType::operator(), std::tuple_cat(std::make_tuple(this), delegateMsg->GetArgs()));
            else
                m_retVal = std::apply(&BaseType::operator(), std::tuple_cat(std::make_tuple(this), delegateMsg->GetArgs()));
            m_sema.Signal();
        }
    }

    /// Returns true if asynchronous function successfully invoked on target thread
    bool IsSuccess() { return m_success; }

    /// Returns the async function return value
    RetType GetRetVal() { return std::any_cast<RetType>(m_retVal); }

private:
    bool m_success = false;			        // Set to true if async function succeeds
    std::chrono::milliseconds m_timeout;    // Time in mS to wait for async function to invoke
    Semaphore m_sema;				        // Semaphore to signal waiting thread
    std::any m_retVal;                      // Return value of the invoked function
    bool m_invokerWaiting = false;          // True if caller thread is waiting for invoke complete
    std::mutex m_lock;
    // </common_code>
};

template <class C, class R>
struct DelegateMemberSpAsyncWait; // Not defined

// DelegateMemberSpAsyncWait class asynchronously block invokes a std::shared_ptr target function.
template <class TClass, class RetType, class... Args>
class DelegateMemberSpAsyncWait<TClass, RetType(Args...)> : public DelegateMemberSpAsync<TClass, RetType(Args...)> {
public:
    typedef std::shared_ptr<TClass> ObjectPtr;
    typedef RetType(TClass::* MemberFunc)(Args...);
    typedef RetType(TClass::* ConstMemberFunc)(Args...) const;
    using ClassType = DelegateMemberSpAsyncWait<TClass, RetType(Args...)>;
    using BaseType = DelegateMemberSpAsync<TClass, RetType(Args...)>;

    // Contructors take a class instance, member function, and delegate thread
    ClassType(ObjectPtr object, MemberFunc func, DelegateThread& thread, std::chrono::milliseconds timeout) :
        BaseType(object, func, thread), m_timeout(timeout) {
        Bind(object, func, thread);
    }
    ClassType(ObjectPtr object, ConstMemberFunc func, DelegateThread& thread, std::chrono::milliseconds timeout) :
        BaseType(object, func, thread), m_timeout(timeout) {
        Bind(object, func, thread);
    }

    /// Bind a member function to a delegate. 
    void Bind(ObjectPtr object, MemberFunc func, DelegateThread& thread) {
        BaseType::Bind(object, func, thread);
    }

    /// Bind a const member function to a delegate. 
    void Bind(ObjectPtr object, ConstMemberFunc func, DelegateThread& thread) {
        BaseType::Bind(object, func, thread);
    }

    // <common_code>
    ClassType(const ClassType& rhs) :
        BaseType(rhs) {
        Assign(rhs);
    }
    ClassType() = delete;

    void Assign(const ClassType& rhs) {
        m_timeout = rhs.m_timeout;
        BaseType::Assign(rhs);
    }

    virtual ClassType* Clone() const override {
        return new ClassType(*this);
    }

    ClassType& operator=(const ClassType& rhs) {
        if (&rhs != this) {
            BaseType::operator=(rhs);
            Assign(rhs);
        }
        return *this;
    }

    virtual bool operator==(const DelegateBase& rhs) const override {
        auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
        return derivedRhs &&
            m_timeout == derivedRhs->m_timeout &&
            BaseType::operator==(rhs);
    }

    // Invoke delegate function asynchronously and block for function return value
    virtual RetType operator()(Args... args) override {
        if (this->GetSync())
            return BaseType::operator()(args...);
        else
        {
            // Create a clone instance of this delegate 
            auto delegate = std::shared_ptr<ClassType>(Clone());
            delegate->m_invokerWaiting = true;

            // Create a new message instance 
            auto msg = std::make_shared<DelegateMsg<Args...>>(delegate, args...);

            // Dispatch message onto the callback destination thread. DelegateInvoke()
            // will be called by the target thread. 
            GetThread().DispatchDelegate(msg);

            // Wait for target thread to execute the delegate function
            if ((m_success = delegate->m_sema.Wait(m_timeout)))
                m_retVal = delegate->m_retVal;

            const std::lock_guard<std::mutex> lock(m_lock);
            delegate->m_invokerWaiting = false;

            if constexpr (std::is_void<RetType>::value == false)
            {
                if (m_retVal.has_value())
                    return std::any_cast<RetType>(m_retVal);
                else
                    return RetType();
            }
        }
    }

    // Invoke delegate function asynchronously and block for function return value
    auto AsyncInvoke(Args... args) {
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
        const std::lock_guard<std::mutex> lock(m_lock);
        if (this->m_invokerWaiting)
        {
            // Typecast the base pointer to back to the templatized instance
            auto delegateMsg = std::dynamic_pointer_cast<DelegateMsg<Args...>>(msg);
            if (delegateMsg == nullptr)
                throw std::invalid_argument("Invalid DelegateMsg cast");

            // Invoke the delegate function then signal the waiting thread
            SetSync(true);
            if constexpr (std::is_void<RetType>::value == true)
                std::apply(&BaseType::operator(), std::tuple_cat(std::make_tuple(this), delegateMsg->GetArgs()));
            else
                m_retVal = std::apply(&BaseType::operator(), std::tuple_cat(std::make_tuple(this), delegateMsg->GetArgs()));
            m_sema.Signal();
        }
    }

    /// Returns true if asynchronous function successfully invoked on target thread
    bool IsSuccess() { return m_success; }

    /// Returns the async function return value
    RetType GetRetVal() { return std::any_cast<RetType>(m_retVal); }

private:
    bool m_success = false;			        // Set to true if async function succeeds
    std::chrono::milliseconds m_timeout;    // Time in mS to wait for async function to invoke
    Semaphore m_sema;				        // Semaphore to signal waiting thread
    std::any m_retVal;                      // Return value of the invoked function
    bool m_invokerWaiting = false;          // True if caller thread is waiting for invoke complete
    std::mutex m_lock;
    // </common_code>
};

template <class R>
struct DelegateFunctionAsyncWait; // Not defined

// DelegateFunctionAsync class asynchronously block invokes a std::function target function.
template <class RetType, class... Args>
class DelegateFunctionAsyncWait<RetType(Args...)> : public DelegateFunctionAsync<RetType(Args...)> {
public:
    using FunctionType = std::function<RetType(Args...)>;
    using ClassType = DelegateFunctionAsyncWait<RetType(Args...)>;
    using BaseType = DelegateFunctionAsync<RetType(Args...)>;

    // Contructors take a std::function, delegate thread and timeout
    ClassType(FunctionType func, DelegateThread& thread, std::chrono::milliseconds timeout) :
        BaseType(func, thread), m_timeout(timeout) {
        Bind(func, thread);
    }

    /// Bind a std::function to a delegate. 
    void Bind(FunctionType func, DelegateThread& thread) {
        BaseType::Bind(func, thread);
    }

    // <common_code>
    ClassType(const ClassType& rhs) :
        BaseType(rhs) {
        Assign(rhs);
    }
    ClassType() = delete;

    void Assign(const ClassType& rhs) {
        m_timeout = rhs.m_timeout;
        BaseType::Assign(rhs);
    }

    virtual ClassType* Clone() const override {
        return new ClassType(*this);
    }

    ClassType& operator=(const ClassType& rhs) {
        if (&rhs != this) {
            BaseType::operator=(rhs);
            Assign(rhs);
        }
        return *this;
    }

    virtual bool operator==(const DelegateBase& rhs) const override {
        auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
        return derivedRhs &&
            m_timeout == derivedRhs->m_timeout &&
            BaseType::operator==(rhs);
    }

    // Invoke delegate function asynchronously and block for function return value
    virtual RetType operator()(Args... args) override {
        if (this->GetSync())
            return BaseType::operator()(args...);
        else
        {
            // Create a clone instance of this delegate 
            auto delegate = std::shared_ptr<ClassType>(Clone());
            delegate->m_invokerWaiting = true;

            // Create a new message instance 
            auto msg = std::make_shared<DelegateMsg<Args...>>(delegate, args...);

            // Dispatch message onto the callback destination thread. DelegateInvoke()
            // will be called by the target thread. 
            GetThread().DispatchDelegate(msg);

            // Wait for target thread to execute the delegate function
            if ((m_success = delegate->m_sema.Wait(m_timeout)))
                m_retVal = delegate->m_retVal;

            const std::lock_guard<std::mutex> lock(m_lock);
            delegate->m_invokerWaiting = false;

            if constexpr (std::is_void<RetType>::value == false)
            {
                if (m_retVal.has_value())
                    return std::any_cast<RetType>(m_retVal);
                else
                    return RetType();
            }
        }
    }

    // Invoke delegate function asynchronously and block for function return value
    auto AsyncInvoke(Args... args) {
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
        const std::lock_guard<std::mutex> lock(m_lock);
        if (this->m_invokerWaiting)
        {
            // Typecast the base pointer to back to the templatized instance
            auto delegateMsg = std::dynamic_pointer_cast<DelegateMsg<Args...>>(msg);
            if (delegateMsg == nullptr)
                throw std::invalid_argument("Invalid DelegateMsg cast");

            // Invoke the delegate function then signal the waiting thread
            SetSync(true);
            if constexpr (std::is_void<RetType>::value == true)
                std::apply(&BaseType::operator(), std::tuple_cat(std::make_tuple(this), delegateMsg->GetArgs()));
            else
                m_retVal = std::apply(&BaseType::operator(), std::tuple_cat(std::make_tuple(this), delegateMsg->GetArgs()));
            m_sema.Signal();
        }
    }

    /// Returns true if asynchronous function successfully invoked on target thread
    bool IsSuccess() { return m_success; }

    /// Returns the async function return value
    RetType GetRetVal() { return std::any_cast<RetType>(m_retVal); }

private:
    bool m_success = false;			        // Set to true if async function succeeds
    std::chrono::milliseconds m_timeout;    // Time in mS to wait for async function to invoke
    Semaphore m_sema;				        // Semaphore to signal waiting thread
    std::any m_retVal;                      // Return value of the invoked function
    bool m_invokerWaiting = false;          // True if caller thread is waiting for invoke complete
    std::mutex m_lock;
    // </common_code>
};

template <class RetType, class... Args>
DelegateFreeAsyncWait<RetType(Args...)> MakeDelegate(RetType(*func)(Args... args), DelegateThread& thread, std::chrono::milliseconds timeout) {
    return DelegateFreeAsyncWait<RetType(Args...)>(func, thread, timeout);
}

template <class TClass, class RetType, class... Args>
DelegateMemberAsyncWait<TClass, RetType(Args...)> MakeDelegate(TClass* object, RetType(TClass::*func)(Args... args), DelegateThread& thread, std::chrono::milliseconds timeout) {
    return DelegateMemberAsyncWait<TClass, RetType(Args...)>(object, func, thread, timeout);
}

template <class TClass, class RetType, class... Args>
DelegateMemberAsyncWait<TClass, RetType(Args...)> MakeDelegate(TClass* object, RetType(TClass::*func)(Args... args) const, DelegateThread& thread, std::chrono::milliseconds timeout) {
    return DelegateMemberAsyncWait<TClass, RetType(Args...)>(object, func, thread, timeout);
}

template <class TClass, class RetVal, class... Args>
DelegateMemberSpAsyncWait<TClass, RetVal(Args...)> MakeDelegate(std::shared_ptr<TClass> object, RetVal(TClass::* func)(Args... args), DelegateThread& thread, std::chrono::milliseconds timeout) {
    return DelegateMemberSpAsyncWait<TClass, RetVal(Args...)>(object, func, thread, timeout);
}

template <class TClass, class RetVal, class... Args>
DelegateMemberSpAsyncWait<TClass, RetVal(Args...)> MakeDelegate(std::shared_ptr<TClass> object, RetVal(TClass::* func)(Args... args) const, DelegateThread& thread, std::chrono::milliseconds timeout) {
    return DelegateMemberSpAsyncWait<TClass, RetVal(Args...)>(object, func, thread, timeout);
}

template <class RetType, class... Args>
DelegateFunctionAsyncWait<RetType(Args...)> MakeDelegate(std::function<RetType(Args...)> func, DelegateThread& thread, std::chrono::milliseconds timeout) {
    return DelegateFunctionAsyncWait<RetType(Args...)>(func, thread, timeout);
}

} 

#endif
