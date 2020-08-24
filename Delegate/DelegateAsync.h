#ifndef _DELEGATE_ASYNC_H
#define _DELEGATE_ASYNC_H

// DelegateAsync.h
// @see https://www.codeproject.com/Articles/5277036/Asynchronous-Multicast-Delegates-in-Modern-Cplusplus
// David Lafreniere, Aug 2020.

#include "Delegate.h"
#include "DelegateThread.h"
#include "DelegateInvoker.h"
#include <tuple>

namespace DelegateLib {

template <class R>
struct DelegateFreeAsync; // Not defined

template <class... Args> 
class DelegateFreeAsync<void(Args...)> : public DelegateFree<void(Args...)>, public IDelegateInvoker {
public:
    typedef void(*FreeFunc)(Args...);

    DelegateFreeAsync(FreeFunc func, DelegateThread* thread) : m_sync(false) { Bind(func, thread); }
    DelegateFreeAsync() : m_thread(nullptr), m_sync(false) { }

    /// Bind a free function to the delegate.
    void Bind(FreeFunc func, DelegateThread* thread) {
        m_thread = thread;
        DelegateFree<void(Args...)>::Bind(func);
    }

    virtual DelegateFreeAsync<void(Args...)>* Clone() const {
        return new DelegateFreeAsync<void(Args...)>(*this);
    }

    virtual bool operator==(const DelegateBase& rhs) const {
        const DelegateFreeAsync<void(Args...)>* derivedRhs = dynamic_cast<const DelegateFreeAsync<void(Args...)>*>(&rhs);
        return derivedRhs &&
            m_thread == derivedRhs->m_thread &&
            DelegateFree<void(Args...)>::operator == (rhs);
    }

    // Invoke delegate function asynchronously
    virtual void operator()(Args... args) {
        if (m_thread == nullptr || m_sync)
            DelegateFree<void(Args...)>::operator()(args...);
        else
        {
            // Create a clone instance of this delegate 
            auto delegate = std::shared_ptr<DelegateFreeAsync<void(Args...)>>(Clone());

            // Create the delegate message
            auto msg = std::shared_ptr<DelegateMsgHeapArgs<Args...>>(new DelegateMsgHeapArgs<Args...>(delegate, args...));

            // Dispatch message onto the callback destination thread. DelegateInvoke()
            // will be called by the target thread. 
            m_thread->DispatchDelegate(msg);
        }
    }

    // Called to invoke the delegate function on the target thread of control
    virtual void DelegateInvoke(std::shared_ptr<DelegateMsgBase> msg) {
        // Typecast the base pointer to back to the templatized instance
        auto delegateMsg = static_cast<DelegateMsgHeapArgs<Args...>*>(msg.get());

        // Invoke the delegate function
        m_sync = true;
        std::apply(&DelegateFree<void(Args...)>::operator(), 
            std::tuple_cat(std::make_tuple(this), delegateMsg->GetArgs()));
    }

private:
    DelegateThread* m_thread; 
    bool m_sync;
};

template <class C, class R>
struct DelegateMemberAsync; // Not defined

template <class TClass, class... Args>
class DelegateMemberAsync<TClass, void(Args...)> : public DelegateMember<TClass, void(Args...)>, public IDelegateInvoker {
public:
    typedef TClass* ObjectPtr;
    typedef void (TClass::*MemberFunc)(Args...);
    typedef void (TClass::*ConstMemberFunc)(Args...) const;

    // Contructors take a class instance, member function, and callback thread
    DelegateMemberAsync(ObjectPtr object, MemberFunc func, DelegateThread* thread) : m_sync(false)
        { Bind(object, func, thread); }
    DelegateMemberAsync(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread) : m_sync(false)
        { Bind(object, func, thread); }
    DelegateMemberAsync() : m_thread(nullptr), m_sync(false) { }

    /// Bind a member function to a delegate. 
    void Bind(ObjectPtr object, MemberFunc func, DelegateThread* thread) {
        m_thread = thread;
        DelegateMember<TClass, void(Args...)>::Bind(object, func);
    }

    /// Bind a const member function to a delegate. 
    void Bind(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread) {
        m_thread = thread;
        DelegateMember<TClass, void(Args...)>::Bind(object, func);
    }

    virtual DelegateMemberAsync<TClass, void(Args...)>* Clone() const {
        return new DelegateMemberAsync<TClass, void(Args...)>(*this);
    }

    virtual bool operator==(const DelegateBase& rhs) const {
        const DelegateMemberAsync<TClass, void(Args...)>* derivedRhs = dynamic_cast<const DelegateMemberAsync<TClass, void(Args...)>*>(&rhs);
        return derivedRhs &&
            m_thread == derivedRhs->m_thread &&
            DelegateMember<TClass, void(Args...)>::operator == (rhs);
    }

    /// Invoke delegate function asynchronously
    virtual void operator()(Args... args) {
        if (m_thread == nullptr || m_sync)
            DelegateMember<TClass, void(Args...)>::operator()(args...);
        else
        {
            // Create a clone instance of this delegate 
            auto delegate = std::shared_ptr<DelegateMemberAsync<TClass, void(Args...)>>(Clone());

            // Create the delegate message
            auto msg = std::shared_ptr<DelegateMsgHeapArgs<Args...>>(new DelegateMsgHeapArgs<Args...>(delegate, args...));

            // Dispatch message onto the callback destination thread. DelegateInvoke()
            // will be called by the target thread. 
            m_thread->DispatchDelegate(msg);
        }
    }

    /// Called by the target thread to invoke the delegate function 
    virtual void DelegateInvoke(std::shared_ptr<DelegateMsgBase> msg) {
        // Typecast the base pointer to back to the templatized instance
        auto delegateMsg = static_cast<DelegateMsgHeapArgs<Args...>*>(msg.get());

        // Invoke the delegate function
        m_sync = true;
        std::apply(&DelegateMember<TClass, void(Args...)>::operator(),
            std::tuple_cat(std::make_tuple(this), delegateMsg->GetArgs()));
    }

private:
    /// Target thread to invoke the delegate function
    DelegateThread* m_thread;
    bool m_sync;
};

template <class TClass, class... Args>
DelegateMemberAsync<TClass, void(Args...)> MakeDelegate(TClass* object, void(TClass::*func)(Args... args), DelegateThread* thread) {
    return DelegateMemberAsync<TClass, void(Args...)>(object, func, thread);
}

template <class TClass, class... Args>
DelegateMemberAsync<TClass, void(Args...)> MakeDelegate(TClass* object, void(TClass::*func)(Args... args) const, DelegateThread* thread) {
    return DelegateMemberAsync<TClass, void(Args...)>(object, func, thread);
}

template <class... Args>
DelegateFreeAsync<void(Args...)> MakeDelegate(void(*func)(Args... args), DelegateThread* thread) {
    return DelegateFreeAsync<void(Args...)>(func, thread);
}

}

#endif
