#ifndef _DELEGATE_ASYNC_H
#define _DELEGATE_ASYNC_H

// Delegate.h
// @see https://www.codeproject.com/Articles/1160934/Asynchronous-Multicast-Delegates-in-Cplusplus
// David Lafreniere, Dec 2016.

#include "Delegate.h"
#include "DelegateThread.h"
#include "DelegateInvoker.h"
#include <tuple>

namespace DelegateLib {

template <class R>
struct DelegateFreeAsync; // Not defined

// TODO - remove class RetType and replace with void? Then get rid of static_assert below.
template <class RetType, class... Args> 
class DelegateFreeAsync<RetType(Args...)> : public DelegateFree<RetType(Args...)>, public IDelegateInvoker {
public:
    static_assert(std::is_same<RetType, void>::value, "RetType must be void on DelegateFreeAsync.");

    typedef RetType(*FreeFunc)(Args...);

    DelegateFreeAsync(FreeFunc func, DelegateThread* thread) : m_sync(false) { Bind(func, thread); }
    DelegateFreeAsync() : m_thread(0), m_sync(false) { }

    /// Bind a free function to the delegate.
    void Bind(FreeFunc func, DelegateThread* thread) {
        m_thread = thread;
        DelegateFree<RetType(Args...)>::Bind(func);
    }

    virtual DelegateFreeAsync<RetType(Args...)>* Clone() const {
        return new DelegateFreeAsync<RetType(Args...)>(*this);
    }

    virtual bool operator==(const DelegateBase& rhs) const {
        const DelegateFreeAsync<RetType(Args...)>* derivedRhs = dynamic_cast<const DelegateFreeAsync<RetType(Args...)>*>(&rhs);
        return derivedRhs &&
            m_thread == derivedRhs->m_thread &&
            DelegateFree<RetType(Args...)>::operator == (rhs);
    }

    // Invoke delegate function asynchronously
    virtual void operator()(Args... args) {
        if (m_thread == 0 || m_sync)
            DelegateFree<RetType(Args...)>::operator()(args...);
        else
        {
            // Create a clone instance of this delegate 
            auto delegate = std::shared_ptr<DelegateFreeAsync<RetType(Args...)>>(Clone());

            // Create the delegate message
            auto msg = std::shared_ptr<DelegateMsg<Args...>>(new DelegateMsg<Args...>(delegate, args...));

            // Dispatch message onto the callback destination thread. DelegateInvoke()
            // will be called by the target thread. 
            m_thread->DispatchDelegate(msg);
        }
    }

    // Called to invoke the delegate function on the target thread of control
    virtual void DelegateInvoke(std::shared_ptr<DelegateMsgBase> msg) {
        // Typecast the base pointer to back to the templatized instance
        auto delegateMsg = static_cast<DelegateMsg<Args...>*>(msg.get());

        // Invoke the delegate function
        m_sync = true;
        std::apply(&DelegateFree<RetType(Args...)>::operator(), 
            std::tuple_cat(std::make_tuple(this), delegateMsg->GetArgs()));
    }

private:
    DelegateThread* m_thread; 
    bool m_sync;
};

template <class C, class R>
struct DelegateMemberAsync; // Not defined

template <class TClass, class RetType, class... Args>
class DelegateMemberAsync<TClass, RetType(Args...)> : public DelegateMember<TClass, RetType(Args...)>, public IDelegateInvoker {
public:
    static_assert(std::is_same<RetType, void>::value, "RetType must be void on DelegateMemberAsync.");

    typedef TClass* ObjectPtr;
    typedef void (TClass::*MemberFunc)(Args...);
    typedef void (TClass::*ConstMemberFunc)(Args...) const;

    // Contructors take a class instance, member function, and callback thread
    DelegateMemberAsync(ObjectPtr object, MemberFunc func, DelegateThread* thread) : m_sync(false)
        { Bind(object, func, thread); }
    DelegateMemberAsync(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread) : m_sync(false)
        { Bind(object, func, thread); }
    DelegateMemberAsync() : m_thread(0), m_sync(false) { }

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

    virtual DelegateMemberAsync<TClass, RetType(Args...)>* Clone() const {
        return new DelegateMemberAsync<TClass, RetType(Args...)>(*this);
    }

    virtual bool operator==(const DelegateBase& rhs) const {
        const DelegateMemberAsync<TClass, RetType(Args...)>* derivedRhs = dynamic_cast<const DelegateMemberAsync<TClass, RetType(Args...)>*>(&rhs);
        return derivedRhs &&
            m_thread == derivedRhs->m_thread &&
            DelegateMember<TClass, RetType(Args...)>::operator == (rhs);
    }

    /// Invoke delegate function asynchronously
    virtual void operator()(Args... args) {
        if (m_thread == 0 || m_sync)
            DelegateMember<TClass, RetType(Args...)>::operator()(args...);
        else
        {
            // Create a clone instance of this delegate 
            auto delegate = std::shared_ptr<DelegateMemberAsync<TClass, RetType(Args...)>>(Clone());

            // Create the delegate message
            auto msg = std::shared_ptr<DelegateMsg<Args...>>(new DelegateMsg<Args...>(delegate, args...));

            // Dispatch message onto the callback destination thread. DelegateInvoke()
            // will be called by the target thread. 
            m_thread->DispatchDelegate(msg);
        }
    }

    /// Called by the target thread to invoke the delegate function 
    virtual void DelegateInvoke(std::shared_ptr<DelegateMsgBase> msg) {
        // Typecast the base pointer to back to the templatized instance
        auto delegateMsg = static_cast<DelegateMsg<Args...>*>(msg.get());

        // Invoke the delegate function
        m_sync = true;
        std::apply(&DelegateMember<TClass, RetType(Args...)>::operator(),
            std::tuple_cat(std::make_tuple(this), delegateMsg->GetArgs()));
    }

private:
    /// Target thread to invoke the delegate function
    DelegateThread* m_thread;
    bool m_sync;
};

template <class TClass, class RetType, class... Args>
DelegateMemberAsync<TClass, RetType(Args...)> MakeDelegate(TClass* object, RetType(TClass::*func)(Args... args), DelegateThread* thread) {
    return DelegateMemberAsync<TClass, RetType(Args...)>(object, func, thread);
}

template <class TClass, class RetType, class... Args>
DelegateMemberAsync<TClass, RetType(Args...)> MakeDelegate(TClass* object, RetType(TClass::*func)(Args... args) const, DelegateThread* thread) {
    return DelegateMemberAsync<TClass, RetType(Args...)>(object, func, thread);
}

template <class RetType, class... Args>
DelegateFreeAsync<RetType(Args...)> MakeDelegate(RetType(*func)(Args... args), DelegateThread* thread) {
    return DelegateFreeAsync<RetType(Args...)>(func, thread);
}

}

#endif
