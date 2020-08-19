#ifndef _DELEGATE_SP_ASYNC_H
#define _DELEGATE_SP_ASYNC_H

// DelegateSpAsync.h
// @see https://www.codeproject.com/Articles/5277036/Asynchronous-Multicast-Delegates-in-Modern-Cplusplus
// David Lafreniere, Aug 2020.


#include "DelegateSp.h"
#include "DelegateThread.h"
#include "DelegateInvoker.h"

namespace DelegateLib {

template <class C, class R>
struct DelegateMemberAsyncSp; // Not defined

/// @brief The DelegateMemberAsyncSp delegate implemenation asynchronously binds and 
/// and invokes class instance member functions. The std::shared_ptr<TClass> is used in 
/// lieu of a raw TClass* pointer. 
template <class TClass, class... Args>
class DelegateMemberAsyncSp<TClass, void(Args...)> : public DelegateMemberSp<TClass, void(Args...)>, public IDelegateInvoker {
public:
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
    DelegateMemberAsyncSp() : m_thread(nullptr), m_sync(false) { }

    /// Bind a member function to a delegate. 
    void Bind(ObjectPtr object, MemberFunc func, DelegateThread* thread) {
        m_thread = thread;
        DelegateMemberSp<TClass, void(Args...)>::Bind(object, func);
    }

    /// Bind a const member function to a delegate. 
    void Bind(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread) {
        m_thread = thread;
        DelegateMemberSp<TClass, void(Args...)>::Bind(object, func);
    }

    virtual DelegateMemberAsyncSp<TClass, void(Args...)>* Clone() const {
        return new DelegateMemberAsyncSp<TClass, void(Args...)>(*this);
    }

    virtual bool operator==(const DelegateBase& rhs) const {
        const DelegateMemberAsyncSp<TClass, void(Args...)>* derivedRhs = dynamic_cast<const DelegateMemberAsyncSp<TClass, void(Args...)>*>(&rhs);
        return derivedRhs &&
            m_thread == derivedRhs->m_thread &&
            DelegateMemberSp<TClass, void(Args...)>::operator == (rhs);
    }

    /// Invoke delegate function asynchronously
    virtual void operator()(Args... args) {
        if (m_thread == nullptr || m_sync)
            DelegateMemberSp<TClass, void(Args...)>::operator()(args...);
        else
        {
            // Create a clone instance of this delegate 
            auto delegate = std::shared_ptr<DelegateMemberAsyncSp<TClass, void(Args...)>>(Clone());

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
        std::apply(&DelegateMemberSp<TClass, void(Args...)>::operator(),
            std::tuple_cat(std::make_tuple(this), delegateMsg->GetArgs()));
    }

private:
    /// Target thread to invoke the delegate function
    DelegateThread * m_thread;
    bool m_sync;
};

template <class TClass, class... Args>
DelegateMemberAsyncSp<TClass, void(Args...)> MakeDelegate(std::shared_ptr<TClass> object, void(TClass::*func)(Args... args), DelegateThread* thread) {
    return DelegateMemberAsyncSp<TClass, void(Args...)>(object, func, thread);
}

template <class TClass, class... Args>
DelegateMemberAsyncSp<TClass, void(Args...)> MakeDelegate(std::shared_ptr<TClass> object, void(TClass::*func)(Args... args) const, DelegateThread* thread) {
    return DelegateMemberAsyncSp<TClass, void(Args...)>(object, func, thread);
}

}

#endif
