#ifndef _DELEGATE_SP_ASYNC_H
#define _DELEGATE_SP_ASYNC_H

// Delegate.h
// @see https://www.codeproject.com/Articles/1160934/Asynchronous-Multicast-Delegates-in-Cplusplus
// David Lafreniere, Dec 2016.


#include "DelegateSp.h"
#include "DelegateThread.h"
#include "DelegateInvoker.h"

namespace DelegateLib {

template <class C, class R>
struct DelegateMemberAsyncSp; // Not defined

/// @brief The DelegateMemberAsyncSp delegate implemenation asynchronously binds and 
/// and invokes class instance member functions. The std::shared_ptr<TClass> is used in 
/// lieu of a raw TClass* pointer. 
template <class TClass, class RetType, class... Args>
class DelegateMemberAsyncSp<TClass, RetType(Args...)> : public DelegateMemberSp<TClass, RetType(Args...)>, public IDelegateInvoker {
public:
    static_assert(std::is_same<RetType, void>::value, "RetType must be void on DelegateMemberAsyncSp.");

    typedef std::shared_ptr<TClass> ObjectPtr;
    typedef void (TClass::*MemberFunc)(Args...);
    typedef void (TClass::*ConstMemberFunc)(Args...) const;

    // Contructors take a class instance, member function, and callback thread
    DelegateMemberAsyncSp(ObjectPtr object, MemberFunc func, DelegateThread* thread) : m_sync(false) {
        Bind(object, func, thread);
    }
    DelegateMemberAsyncSp(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread) : m_sync(false) {
        Bind(object, func, thread);
    }
    DelegateMemberAsyncSp() : m_thread(0), m_sync(false) { }

    /// Bind a member function to a delegate. 
    void Bind(ObjectPtr object, MemberFunc func, DelegateThread* thread) {
        m_thread = thread;
        DelegateMemberSp<TClass, RetType(Args...)>::Bind(object, func);
    }

    /// Bind a const member function to a delegate. 
    void Bind(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread) {
        m_thread = thread;
        DelegateMemberSp<TClass, RetType(Args...)>::Bind(object, func);
    }

    virtual DelegateMemberAsyncSp<TClass, RetType(Args...)>* Clone() const {
        return new DelegateMemberAsyncSp<TClass, RetType(Args...)>(*this);
    }

    virtual bool operator==(const DelegateBase& rhs) const {
        const DelegateMemberAsyncSp<TClass, RetType(Args...)>* derivedRhs = dynamic_cast<const DelegateMemberAsyncSp<TClass, RetType(Args...)>*>(&rhs);
        return derivedRhs &&
            m_thread == derivedRhs->m_thread &&
            DelegateMemberSp<TClass, RetType(Args...)>::operator == (rhs);
    }

    /// Invoke delegate function asynchronously
    virtual void operator()(Args... args) {
        if (m_thread == nullptr || m_sync)
            DelegateMemberSp<TClass, RetType(Args...)>::operator()(args...);
        else
        {
            // Create a clone instance of this delegate 
            auto delegate = std::shared_ptr<DelegateMemberAsyncSp<TClass, RetType(Args...)>>(Clone());

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
        std::apply(&DelegateMemberSp<TClass, RetType(Args...)>::operator(),
            std::tuple_cat(std::make_tuple(this), delegateMsg->GetArgs()));
    }

private:
    /// Target thread to invoke the delegate function
    DelegateThread * m_thread;
    bool m_sync;
};

template <class TClass, class RetType, class... Args>
DelegateMemberAsyncSp<TClass, RetType(Args...)> MakeDelegate(std::shared_ptr<TClass> object, RetType(TClass::*func)(Args... args), DelegateThread* thread) {
    return DelegateMemberAsyncSp<TClass, RetType(Args...)>(object, func, thread);
}

template <class TClass, class RetType, class... Args>
DelegateMemberAsyncSp<TClass, RetType(Args...)> MakeDelegate(std::shared_ptr<TClass> object, RetType(TClass::*func)(Args... args) const, DelegateThread* thread) {
    return DelegateMemberAsyncSp<TClass, RetType(Args...)>(object, func, thread);
}

}

#endif
