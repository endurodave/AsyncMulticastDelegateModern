#ifndef _DELEGATE_ASYNC_H
#define _DELEGATE_ASYNC_H

// DelegateAsync.h
// @see https://github.com/endurodave/AsyncMulticastDelegateModern
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
    using ClassType = DelegateFreeAsync<void(Args...)>;
    using BaseType = DelegateFree<void(Args...)>;

    DelegateFreeAsync(FreeFunc func, DelegateThread& thread) : BaseType(func), m_thread(thread) { Bind(func, thread); }
    DelegateFreeAsync() = delete;

    /// Bind a free function to the delegate.
    void Bind(FreeFunc func, DelegateThread& thread) {
        m_thread = thread;
        BaseType::Bind(func);
    }

    virtual ClassType* Clone() const override {
        return new ClassType(*this);
    }

    virtual bool operator==(const DelegateBase& rhs) const override {
        auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
        return derivedRhs &&
            &m_thread == &derivedRhs->m_thread &&
            BaseType::operator == (rhs);
    }

    // Invoke delegate function asynchronously
    virtual void operator()(Args... args) override {
        if (m_sync)
            BaseType::operator()(args...);
        else
        {
            // Create a clone instance of this delegate 
            auto delegate = std::shared_ptr<ClassType>(Clone());

            // Create the delegate message
            auto msg = std::make_shared<DelegateMsgHeapArgs<Args...>>(delegate, args...);

            // Dispatch message onto the callback destination thread. DelegateInvoke()
            // will be called by the target thread. 
            m_thread.DispatchDelegate(msg);

            // Check if any argument is a shared_ptr with wrong usage
            // std::shared_ptr reference arguments are not allowed with asynchronous delegates as the behavior is 
            // undefined. In other words:
            // void MyFunc(std::shared_ptr<T> data)		// Ok!
            // void MyFunc(std::shared_ptr<T>& data)	// Error if DelegateAsync or DelegateSpAsync target!
            static_assert(!(std::disjunction_v<is_shared_ptr<Args>...> &&
                (std::disjunction_v<std::is_lvalue_reference<Args>, std::is_pointer<Args>> || ...)),
                "std::shared_ptr reference argument not allowed");
        }
    }

    // Called to invoke the delegate function on the target thread of control
    virtual void DelegateInvoke(std::shared_ptr<DelegateMsgBase> msg) {
        // Typecast the base pointer to back to the templatized instance
        auto delegateMsg = std::dynamic_pointer_cast<DelegateMsgHeapArgs<Args...>>(msg);
        if (delegateMsg == nullptr)
            throw std::invalid_argument("Invalid DelegateMsgHeapArgs cast");

        // Invoke the delegate function
        m_sync = true;
        std::apply(&BaseType::operator(), 
            std::tuple_cat(std::make_tuple(this), delegateMsg->GetArgs()));
    }

private:
    DelegateThread& m_thread; 
    bool m_sync = false;
};

template <class C, class R>
struct DelegateMemberAsync; // Not defined

template <class TClass, class... Args>
class DelegateMemberAsync<TClass, void(Args...)> : public DelegateMember<TClass, void(Args...)>, public IDelegateInvoker {
public:
    typedef TClass* ObjectPtr;
    typedef void (TClass::*MemberFunc)(Args...);
    typedef void (TClass::*ConstMemberFunc)(Args...) const;
    using ClassType = DelegateMemberAsync<TClass, void(Args...)>;
    using BaseType = DelegateMember<TClass, void(Args...)>;

    // Contructors take a class instance, member function, and callback thread
    DelegateMemberAsync(ObjectPtr object, MemberFunc func, DelegateThread& thread) : BaseType(object, func), m_thread(thread)
        { Bind(object, func, thread); }
    DelegateMemberAsync(ObjectPtr object, ConstMemberFunc func, DelegateThread& thread) : BaseType(object, func), m_thread(thread)
        { Bind(object, func, thread); }
    DelegateMemberAsync() = delete;

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

    virtual ClassType* Clone() const override {
        return new ClassType(*this);
    }

    virtual bool operator==(const DelegateBase& rhs) const override {
        auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
        return derivedRhs &&
            &m_thread == &derivedRhs->m_thread &&
            BaseType::operator == (rhs);
    }

    /// Invoke delegate function asynchronously
    virtual void operator()(Args... args) override {
        if (m_sync)
            BaseType::operator()(args...);
        else
        {
            // Create a clone instance of this delegate 
            auto delegate = std::shared_ptr<ClassType>(Clone());

            // Create the delegate message
            auto msg = std::make_shared<DelegateMsgHeapArgs<Args...>>(delegate, args...);

            // Dispatch message onto the callback destination thread. DelegateInvoke()
            // will be called by the target thread. 
            m_thread.DispatchDelegate(msg);

            // Check if any argument is a shared_ptr with wrong usage
            // std::shared_ptr reference arguments are not allowed with asynchronous delegates as the behavior is 
            // undefined. In other words:
            // void MyFunc(std::shared_ptr<T> data)		// Ok!
            // void MyFunc(std::shared_ptr<T>& data)	// Error if DelegateAsync or DelegateSpAsync target!
            static_assert(!(std::disjunction_v<is_shared_ptr<Args>...> &&
                (std::disjunction_v<std::is_lvalue_reference<Args>, std::is_pointer<Args>> || ...)),
                "std::shared_ptr reference argument not allowed");
        }
    }

    /// Called by the target thread to invoke the delegate function 
    virtual void DelegateInvoke(std::shared_ptr<DelegateMsgBase> msg) {
        // Typecast the base pointer to back to the templatized instance
        auto delegateMsg = std::dynamic_pointer_cast<DelegateMsgHeapArgs<Args...>>(msg);
        if (delegateMsg == nullptr)
            throw std::invalid_argument("Invalid DelegateMsgHeapArgs cast");

        // Invoke the delegate function
        m_sync = true;
        std::apply(&BaseType::operator(),
            std::tuple_cat(std::make_tuple(this), delegateMsg->GetArgs()));
    }

private:
    /// Target thread to invoke the delegate function
    DelegateThread& m_thread;
    bool m_sync = false;
};

template <class TClass, class... Args>
DelegateMemberAsync<TClass, void(Args...)> MakeDelegate(TClass* object, void(TClass::*func)(Args... args), DelegateThread& thread) {
    return DelegateMemberAsync<TClass, void(Args...)>(object, func, thread);
}

template <class TClass, class... Args>
DelegateMemberAsync<TClass, void(Args...)> MakeDelegate(TClass* object, void(TClass::*func)(Args... args) const, DelegateThread& thread) {
    return DelegateMemberAsync<TClass, void(Args...)>(object, func, thread);
}

template <class... Args>
DelegateFreeAsync<void(Args...)> MakeDelegate(void(*func)(Args... args), DelegateThread& thread) {
    return DelegateFreeAsync<void(Args...)>(func, thread);
}

}

#endif
