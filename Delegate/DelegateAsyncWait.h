#ifndef _DELEGATE_ASYNC_WAIT_H
#define _DELEGATE_ASYNC_WAIT_H

// DelegateAsyncWait.h
// @see https://www.codeproject.com/Articles/1160934/Asynchronous-Multicast-Delegates-in-Cplusplus
// David Lafreniere, Dec 2016.

#include "Delegate.h"
#include "DelegateThread.h"
#include "DelegateInvoker.h"
#include "Semaphore.h"

/// @brief Asynchronous member delegate that invokes the target function on the specified thread of control
/// and waits for the function to be executed or a timeout occurs. Use IsSuccess() to determine if asynchronous 
/// call succeeded.

namespace DelegateLib {

const int WAIT_INFINITE = -1;

template <class R>
struct DelegateFreeAsyncWaitInvoke; // Not defined

template <class... Args>
class DelegateFreeAsyncWaitInvoke<void(Args...)>
{
public:
    void operator()(DelegateFree<void(Args...)>* instance, std::tuple<Args...>& args) {
        std::apply(&DelegateFree<void(Args...)>::operator(),
            std::tuple_cat(std::make_tuple(instance), args));
    }
    void GetRetVal() { }
};

template <class RetType, class... Args>
class DelegateFreeAsyncWaitInvoke<RetType(Args...)>
{
public:
    void operator()(DelegateFree<RetType(Args...)>* instance, std::tuple<Args...>& args) {
        m_retVal = std::apply(&DelegateFree<RetType(Args...)>::operator(),
            std::tuple_cat(std::make_tuple(instance), args));
    }
    RetType GetRetVal() { return m_retVal; }
private:
    RetType m_retVal;
};

template <class C, class R>
struct DelegateMemberAsyncWaitInvoke; // Not defined

template <class TClass, class... Args>
class DelegateMemberAsyncWaitInvoke<TClass, void(Args...)>
{
public:
    void operator()(DelegateMember<TClass, void(Args...)>* instance, std::tuple<Args...>& args) {
        std::apply(&DelegateMember<TClass, void(Args...)>::operator(),
            std::tuple_cat(std::make_tuple(instance), args));
    }
    void GetRetVal() { }
};

template <class TClass, class RetType, class... Args>
class DelegateMemberAsyncWaitInvoke<TClass, RetType(Args...)>
{
public:
    void operator()(DelegateMember<TClass, RetType(Args...)>* instance, std::tuple<Args...>& args) {
        m_retVal = std::apply(&DelegateMember<TClass, RetType(Args...)>::operator(),
            std::tuple_cat(std::make_tuple(instance), args));
    }
    RetType GetRetVal() { return m_retVal; }
private:
    RetType m_retVal;
};

template <class R>
struct DelegateFreeAsyncWait; // Not defined

template <class RetType, class... Args>
class DelegateFreeAsyncWait<RetType(Args...)> : public DelegateFree<RetType(Args...)>, public IDelegateInvoker {
public:
    typedef RetType(*FreeFunc)(Args...);

    // Contructors take a free function, delegate thread and timeout
    DelegateFreeAsyncWait(FreeFunc func, DelegateThread* thread, int timeout) : m_success(false), m_timeout(timeout), m_refCnt(0), m_sync(false) {
        Bind(func, thread);
        LockGuard::Create(&m_lock);
    }
    DelegateFreeAsyncWait(const DelegateFreeAsyncWait& rhs) : DelegateFree<RetType(Args...)>(rhs), m_refCnt(0), m_sync(false) {
        LockGuard::Create(&m_lock);
        Swap(rhs);
    }
    DelegateFreeAsyncWait() : m_thread(0), m_success(false), m_timeout(0), m_refCnt(0), m_sync(false) { LockGuard::Create(&m_lock); }
    virtual ~DelegateFreeAsyncWait() { LockGuard::Destroy(&m_lock); }

    virtual DelegateFreeAsyncWait<RetType(Args...)>* Clone() const {
        return new DelegateFreeAsyncWait<RetType(Args...)>(*this);
    }

    /// Bind a free function to a delegate. 
    void Bind(FreeFunc func, DelegateThread* thread) {
        m_thread = thread;
        DelegateFree<RetType(Args...)>::Bind(func);
    }

    virtual bool operator==(const DelegateBase& rhs) const {
        const DelegateFreeAsyncWait<RetType(Args...)>* derivedRhs = dynamic_cast<const DelegateFreeAsyncWait<RetType(Args...)>*>(&rhs);
        return derivedRhs &&
            m_thread == derivedRhs->m_thread &&
            DelegateFree<RetType(Args...)>::operator==(rhs);
    }

    DelegateFreeAsyncWait& operator=(const DelegateFreeAsyncWait& rhs) {
        if (&rhs != this) {
            DelegateFree<RetType(Args...)>::operator=(rhs);
            Swap(rhs);
        }
        return *this;
    }

    /// Invoke delegate function asynchronously
    virtual RetType operator()(Args... args) {
        if (this->m_thread == 0 || m_sync)
            return DelegateFree<RetType(Args...)>::operator()(args...);
        else {
            // Create a clone instance of this delegate 
            auto delegate = std::shared_ptr<DelegateFreeAsyncWait<RetType(Args...)>>(Clone());
            delegate->m_refCnt = 2;
            delegate->m_sema.Create();
            delegate->m_sema.Reset();

            // Create a new message instance 
            auto msg = std::shared_ptr<DelegateMsg<Args...>>(new DelegateMsg<Args...>(delegate, std::forward<Args>(args)...)); // TODO forward here?

            // Dispatch message onto the callback destination thread. DelegateInvoke()
            // will be called by the target thread. 
            this->m_thread->DispatchDelegate(msg);

            // Wait for target thread to execute the delegate function
            if ((m_success = delegate->m_sema.Wait(m_timeout)))
                m_invoke = delegate->m_invoke;

            {
                LockGuard lockGuard(&delegate->m_lock);
                delegate->m_refCnt--;
            }
            return m_invoke.GetRetVal();
        }
    }

    /// Called by the target thread to invoke the delegate function 
    virtual void DelegateInvoke(std::shared_ptr<DelegateMsgBase> msg) {
        // Typecast the base pointer to back to the templatized instance
        auto delegateMsg = static_cast<DelegateMsg<Args...>*>(*msg);

        LockGuard lockGuard(&this->m_lock);
        if (this->m_refCnt == 2) {
            // Invoke the delegate function then signal the waiting thread
            m_sync = true;
            m_invoke(this, delegateMsg->GetArgs());
            this->m_sema.Signal();
        }

        this->m_refCnt--;
    }

    /// Returns true if asynchronous function successfully invoked on target thread
    bool IsSuccess() { return m_success; }

    /// Returns the async function return value
    RetType GetRetVal() { return m_invoke.GetRetVal(); }

private:
    void Swap(const DelegateFreeAsyncWait& s) {
        m_thread = s.m_thread;
        m_timeout = s.m_timeout;
        m_success = s.m_success;
    }

    DelegateThread * m_thread;		// Target thread to invoke the delegate function
    bool m_success;					// Set to true if async function succeeds
    int m_timeout;					// Time in mS to wait for async function to invoke
    Semaphore m_sema;				// Semaphore to signal waiting thread
    LOCK m_lock;					// Lock to synchronize threads
    int m_refCnt;					// Ref count to determine when to delete objects
    bool m_sync;                    // Set true when synchronous invocation is required
    DelegateFreeAsyncWaitInvoke<RetType(Args...)> m_invoke;
};

template <class C, class R>
struct DelegateMemberAsyncWait; // Not defined

template <class TClass, class RetType, class... Args>
class DelegateMemberAsyncWait<TClass, RetType(Args...)> : public DelegateMember<TClass, RetType(Args...)>, public IDelegateInvoker {
public:
    typedef TClass* ObjectPtr;
    typedef RetType(TClass::*MemberFunc)(Args...);
    typedef RetType(TClass::*ConstMemberFunc)(Args...) const;

    // Contructors take a class instance, member function, and delegate thread
    DelegateMemberAsyncWait(ObjectPtr object, MemberFunc func, DelegateThread* thread, int timeout) : m_success(false), m_timeout(timeout), m_refCnt(0), m_sync(false) {
        Bind(object, func, thread);
        LockGuard::Create(&m_lock);
    }
    DelegateMemberAsyncWait(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread, int timeout) : m_success(false), m_timeout(timeout), m_refCnt(0), m_sync(false) {
        Bind(object, func, thread);
        LockGuard::Create(&m_lock);
    }
    DelegateMemberAsyncWait(const DelegateMemberAsyncWait& rhs) : DelegateMember<TClass, RetType(Args...)>(rhs), m_refCnt(0), m_sync(false) {
        LockGuard::Create(&m_lock);
        Swap(rhs);
    }
    DelegateMemberAsyncWait() : m_thread(0), m_success(false), m_timeout(0), m_refCnt(0), m_sync(false) { LockGuard::Create(&m_lock); }
    virtual ~DelegateMemberAsyncWait() { LockGuard::Destroy(&m_lock); }

    virtual DelegateMemberAsyncWait<TClass, RetType(Args...)>* Clone() const {
        return new DelegateMemberAsyncWait<TClass, RetType(Args...)>(*this);
    }

    /// Bind a member function to a delegate. 
    void Bind(ObjectPtr object, MemberFunc func, DelegateThread* thread) {
        m_thread = thread;
        DelegateMember<TClass, RetType(Args...)>::Bind(object, func);
    }

    /// Bind a const member function to a delegate. 
    void Bind(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread) {
        m_thread = thread;
        DelegateMember<TClass, RetType(Args...)>::Bind(object, func);
    }

    virtual bool operator==(const DelegateBase& rhs) const {
        const DelegateMemberAsyncWait<TClass, RetType(Args...)>* derivedRhs = dynamic_cast<const DelegateMemberAsyncWait<TClass, RetType(Args...)>*>(&rhs);
        return derivedRhs &&
            m_thread == derivedRhs->m_thread &&
            DelegateMember<TClass, RetType(Args...)>::operator==(rhs);
    }

    DelegateMemberAsyncWait& operator=(const DelegateMemberAsyncWait& rhs) {
        if (&rhs != this) {
            DelegateMember<TClass, RetType(Args...)>::operator=(rhs);
            Swap(rhs);
        }
        return *this;
    }

    /// Invoke delegate function asynchronously
    virtual RetType operator()(Args... args) {
        if (this->m_thread == 0 || m_sync)
            return DelegateMember<TClass, RetType(Args...)>::operator()(args...);
        else {
            // Create a clone instance of this delegate 
            auto delegate = std::shared_ptr<DelegateMemberAsyncWait<TClass, RetType(Args...)>>(Clone());
            delegate->m_refCnt = 2;
            delegate->m_sema.Create();
            delegate->m_sema.Reset();

            // Create a new message instance 
            auto msg = std::shared_ptr<DelegateMsg<Args...>>(new DelegateMsg<Args...>(delegate, std::forward<Args>(args)...));  // TODO forward here?

            // Dispatch message onto the callback destination thread. DelegateInvoke()
            // will be called by the target thread. 
            this->m_thread->DispatchDelegate(msg);

            // Wait for target thread to execute the delegate function
            if ((m_success = delegate->m_sema.Wait(m_timeout)))
                m_invoke = delegate->m_invoke;

            {
                LockGuard lockGuard(&delegate->m_lock);
                delegate->m_refCnt--;
            }
            return m_invoke.GetRetVal();
        }
    }

    /// Called by the target thread to invoke the delegate function 
    virtual void DelegateInvoke(std::shared_ptr<DelegateMsgBase> msg) {
        // Typecast the base pointer to back to the templatized instance
        auto delegateMsg = static_cast<DelegateMsg<Args...>*>(msg.get());

        LockGuard lockGuard(&this->m_lock);
        if (this->m_refCnt == 2) {
            // Invoke the delegate function then signal the waiting thread
            m_sync = true;
            m_invoke(this, delegateMsg->GetArgs());
            this->m_sema.Signal();
        }

        this->m_refCnt--;
    }

    /// Returns true if asynchronous function successfully invoked on target thread
    bool IsSuccess() { return m_success; }

    /// Returns the async function return value
    RetType GetRetVal() { return m_invoke.GetRetVal(); }

private:
    void Swap(const DelegateMemberAsyncWait& s) {
        m_thread = s.m_thread;
        m_timeout = s.m_timeout;
        m_success = s.m_success;
    }

    DelegateThread * m_thread;		// Target thread to invoke the delegate function
    bool m_success;					// Set to true if async function succeeds
    int m_timeout;					// Time in mS to wait for async function to invoke
    Semaphore m_sema;				// Semaphore to signal waiting thread
    LOCK m_lock;					// Lock to synchronize threads
    int m_refCnt;					// Ref count to determine when to delete objects
    bool m_sync;                    // Set true when synchronous invocation is required
    DelegateMemberAsyncWaitInvoke<TClass, RetType(Args...)> m_invoke;
};

template <class TClass, class RetType, class... Args>
DelegateMemberAsyncWait<TClass, RetType(Args...)> MakeDelegate(TClass* object, RetType(TClass::*func)(Args... args), DelegateThread* thread, int timeout) {
    return DelegateMemberAsyncWait<TClass, RetType(Args...)>(object, func, thread, timeout);
}

template <class TClass, class RetType, class... Args>
DelegateMemberAsyncWait<TClass, RetType(Args...)> MakeDelegate(TClass* object, RetType(TClass::*func)(Args... args) const, DelegateThread* thread, int timeout) {
    return DelegateMemberAsyncWait<TClass, RetType(Args...)>(object, func, thread, timeout);
}

template <class RetType, class... Args>
DelegateFreeAsyncWait<RetType(Args...)> MakeDelegate(RetType(*func)(Args... args), DelegateThread* thread, int timeout) {
    return DelegateFreeAsyncWait<RetType(Args...)>(func, thread, timeout);
}

} 

#endif
