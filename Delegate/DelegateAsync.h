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

template <class RetType, class... Args>
class DelegateFreeAsync<RetType(Args...)> : public DelegateFree<RetType(Args...)>, public IDelegateInvoker {
public:
    typedef RetType(*FreeFunc)(Args...);
    using ClassType = DelegateFreeAsync<RetType(Args...)>;
    using BaseType = DelegateFree<RetType(Args...)>;

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
    virtual RetType operator()(Args... args) override {
        if (m_sync)
            return BaseType::operator()(args...);
        else
        {
            // Create a clone instance of this delegate 
            auto delegate = std::shared_ptr<ClassType>(Clone());

            // Create the delegate message
            auto msg = std::make_shared<DelegateMsgHeapArgs<Args...>>(delegate, args...);

            // Dispatch message onto the callback destination thread. DelegateInvoke()
            // will be called by the target thread. 
            m_thread.DispatchDelegate(msg);

            // Do not wait for return value from async function call
            return RetType();

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

    /// Invoke delegate function asynchronously
    void AsyncInvoke(Args... args)
    {
        operator()(args...);   
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

template <class TClass, class RetType, class... Args>
class DelegateMemberAsync<TClass, RetType(Args...)> : public DelegateMember<TClass, RetType(Args...)>, public IDelegateInvoker {
public:
    typedef TClass* ObjectPtr;
    typedef RetType (TClass::*MemberFunc)(Args...);
    typedef RetType (TClass::*ConstMemberFunc)(Args...) const;
    using ClassType = DelegateMemberAsync<TClass, RetType(Args...)>;
    using BaseType = DelegateMember<TClass, RetType(Args...)>;

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
    virtual RetType operator()(Args... args) override {
        if (m_sync)
            return BaseType::operator()(args...);
        else
        {
            // Create a clone instance of this delegate 
            auto delegate = std::shared_ptr<ClassType>(Clone());

            // Create the delegate message
            auto msg = std::make_shared<DelegateMsgHeapArgs<Args...>>(delegate, args...);

            // Dispatch message onto the callback destination thread. DelegateInvoke()
            // will be called by the target thread. 
            m_thread.DispatchDelegate(msg);

            // Do not wait for return value from async function call
            return RetType();

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

    /// Invoke delegate function asynchronously
    void AsyncInvoke(Args... args)
    {
        operator()(args...);
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

template <class TClass, class RetType, class... Args>
DelegateMemberAsync<TClass, RetType(Args...)> MakeDelegate(TClass* object, RetType(TClass::* func)(Args... args), DelegateThread& thread) {
    return DelegateMemberAsync<TClass, RetType(Args...)>(object, func, thread);
}

template <class TClass, class RetType, class... Args>
DelegateMemberAsync<TClass, RetType(Args...)> MakeDelegate(TClass* object, RetType(TClass::* func)(Args... args) const, DelegateThread& thread) {
    return DelegateMemberAsync<TClass, RetType(Args...)>(object, func, thread);
}

template <class RetType, class... Args>
DelegateFreeAsync<RetType(Args...)> MakeDelegate(RetType(*func)(Args... args), DelegateThread& thread) {
    return DelegateFreeAsync<RetType(Args...)>(func, thread);
}

}

#endif
