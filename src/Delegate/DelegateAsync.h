#ifndef _DELEGATE_ASYNC_H
#define _DELEGATE_ASYNC_H

// DelegateAsync.h
// @see https://github.com/endurodave/AsyncMulticastDelegateModern
// David Lafreniere, Aug 2020.
//
// Delegate "Async" series of classes used to invoke a function asynchronously. The classes 
// are not thread safe. Invoking a function asynchronously requires sending a clone of the object 
// to the destination thread message queue. The destination thread calls DelegateInvoke() to 
// invoke the target function.
// 
// Code within <common_code> and </common_code> is updated using sync_src.py. Manually update 
// the code within the DelegateFreeAsync common_code tags, then run the script to 
// propagate to the remaining delegate classes to simplify code maintenance.
// 
// python src_dup.py DelegateAsync.h

#include "Delegate.h"
#include "DelegateThread.h"
#include "DelegateInvoker.h"
#include <tuple>

namespace DelegateLib {

/// @brief Stores all function arguments suitable for non-blocking asynchronous calls.
/// Argument data is stored in the heap.
template <class...Args>
class DelegateAsyncMsg : public DelegateMsg
{
public:
    /// Constructor
    /// @param[in] invoker - the invoker instance
    /// @param[in] args - a parameter pack of all target function arguments
    DelegateAsyncMsg(std::shared_ptr<IDelegateInvoker> invoker, Args... args) : DelegateMsg(invoker),
        m_args(make_tuple_heap(m_heapMem, m_start, args...))
    {
    }

    virtual ~DelegateAsyncMsg() = default;

    /// Get all function arguments that were created on the heap
    /// @return A tuple of all function arguments
    std::tuple<Args...>& GetArgs() { return m_args; }

private:
    /// A list of heap allocated argument memory blocks
    xlist<std::shared_ptr<heap_arg_deleter_base>> m_heapMem;

    /// An empty starting tuple
    std::tuple<> m_start;

    /// A tuple with each element stored within the heap
    std::tuple<Args...> m_args;
};

template <class R>
struct DelegateFreeAsync; // Not defined

// DelegateFreeAsync class asynchronously invokes a free target function.
template <class RetType, class... Args>
class DelegateFreeAsync<RetType(Args...)> : public DelegateFree<RetType(Args...)>, public IDelegateInvoker {
public:
    typedef RetType(*FreeFunc)(Args...);
    using ClassType = DelegateFreeAsync<RetType(Args...)>;
    using BaseType = DelegateFree<RetType(Args...)>;

    DelegateFreeAsync(FreeFunc func, DelegateThread& thread) :
        BaseType(func), m_thread(thread) { 
        Bind(func, thread); 
    }
    DelegateFreeAsync(const ClassType& rhs) :
        BaseType(rhs), m_thread(rhs.m_thread) {
        Assign(rhs);
    }
    DelegateFreeAsync() = delete;

    // Bind a free function to the delegate.
    void Bind(FreeFunc func, DelegateThread& thread) {
        m_thread = thread;
        BaseType::Bind(func);
    }

    // <common_code>
    void Assign(const ClassType& rhs) {
        m_thread = rhs.m_thread;
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
            &m_thread == &derivedRhs->m_thread &&
            BaseType::operator==(rhs);
    }

    // Invoke delegate function asynchronously. Do not wait for return value, if any.
    // Called by the source thread. Dispatches the delegate data into the destination 
    // thread message queue. DelegateInvoke() must be called by the destination thread 
    // to invoke the target function.
    virtual RetType operator()(Args... args) override {
        if (this->GetSync())
            return BaseType::operator()(args...);
        else
        {
            // Create a clone instance of this delegate 
            auto delegate = std::shared_ptr<ClassType>(Clone());

            // Create the delegate message
            auto msg = std::make_shared<DelegateAsyncMsg<Args...>>(delegate, args...);

            // Dispatch message onto the callback destination thread. DelegateInvoke()
            // will be called by the destintation thread. 
            GetThread().DispatchDelegate(msg);

            // Do not wait for destination thread return value from async function call
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

    // Invoke delegate function asynchronously. Do not wait for return value, if any.
    // Called by the source thread.
    void AsyncInvoke(Args... args) {
        operator()(args...);   
    }

    // Invoke the delegate function on the destination thread. Each source thread call
    // to operator() generate a call to DelegateInvoke() on the destination thread. 
    // Unlike DelegateAsyncWait, a lock is not required between source and destination 
    // delegateMsg access because the source thread is not waiting for the function call 
    // to complete.
    virtual void DelegateInvoke(std::shared_ptr<DelegateMsg> msg) {
        // Typecast the base pointer to back correct derived to instance
        auto delegateMsg = std::dynamic_pointer_cast<DelegateAsyncMsg<Args...>>(msg);
        if (delegateMsg == nullptr)
            throw std::invalid_argument("Invalid DelegateMsgHeapArgs cast");

        // Invoke the delegate function asynchronously
        SetSync(true);
        std::apply(&BaseType::operator(), 
            std::tuple_cat(std::make_tuple(this), delegateMsg->GetArgs()));
    }

    // Get the destination thread that the target function is invoked on
    DelegateThread& GetThread() { return m_thread; }

protected:
    bool GetSync() { return m_sync; }
    void SetSync(bool sync) { m_sync = sync; }

private:
    DelegateThread& m_thread;   // The target thread to invoke the delegate function.
    bool m_sync = false;        // Flag to control synchronous vs asynchronous behavior.
    // </common_code>
};

template <class C, class R>
struct DelegateMemberAsync; // Not defined

// DelegateMemberAsync class asynchronously invokes a class member target function.
template <class TClass, class RetType, class... Args>
class DelegateMemberAsync<TClass, RetType(Args...)> : public DelegateMember<TClass, RetType(Args...)>, public IDelegateInvoker {
public:
    typedef TClass* ObjectPtr;
    typedef RetType (TClass::*MemberFunc)(Args...);
    typedef RetType (TClass::*ConstMemberFunc)(Args...) const;
    using ClassType = DelegateMemberAsync<TClass, RetType(Args...)>;
    using BaseType = DelegateMember<TClass, RetType(Args...)>;

    DelegateMemberAsync(ObjectPtr object, MemberFunc func, DelegateThread& thread) : BaseType(object, func), m_thread(thread)
        { Bind(object, func, thread); }
    DelegateMemberAsync(ObjectPtr object, ConstMemberFunc func, DelegateThread& thread) : BaseType(object, func), m_thread(thread)
        { Bind(object, func, thread); }
    DelegateMemberAsync(const ClassType& rhs) :
        BaseType(rhs), m_thread(rhs.m_thread) {
        Assign(rhs);
    }
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

    // <common_code>
    void Assign(const ClassType& rhs) {
        m_thread = rhs.m_thread;
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
            &m_thread == &derivedRhs->m_thread &&
            BaseType::operator==(rhs);
    }

    // Invoke delegate function asynchronously. Do not wait for return value, if any.
    // Called by the source thread. Dispatches the delegate data into the destination 
    // thread message queue. DelegateInvoke() must be called by the destination thread 
    // to invoke the target function.
    virtual RetType operator()(Args... args) override {
        if (this->GetSync())
            return BaseType::operator()(args...);
        else
        {
            // Create a clone instance of this delegate 
            auto delegate = std::shared_ptr<ClassType>(Clone());

            // Create the delegate message
            auto msg = std::make_shared<DelegateAsyncMsg<Args...>>(delegate, args...);

            // Dispatch message onto the callback destination thread. DelegateInvoke()
            // will be called by the destintation thread. 
            GetThread().DispatchDelegate(msg);

            // Do not wait for destination thread return value from async function call
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

    // Invoke delegate function asynchronously. Do not wait for return value, if any.
    // Called by the source thread.
    void AsyncInvoke(Args... args) {
        operator()(args...);   
    }

    // Invoke the delegate function on the destination thread. Each source thread call
    // to operator() generate a call to DelegateInvoke() on the destination thread. 
    // Unlike DelegateAsyncWait, a lock is not required between source and destination 
    // delegateMsg access because the source thread is not waiting for the function call 
    // to complete.
    virtual void DelegateInvoke(std::shared_ptr<DelegateMsg> msg) {
        // Typecast the base pointer to back correct derived to instance
        auto delegateMsg = std::dynamic_pointer_cast<DelegateAsyncMsg<Args...>>(msg);
        if (delegateMsg == nullptr)
            throw std::invalid_argument("Invalid DelegateMsgHeapArgs cast");

        // Invoke the delegate function asynchronously
        SetSync(true);
        std::apply(&BaseType::operator(), 
            std::tuple_cat(std::make_tuple(this), delegateMsg->GetArgs()));
    }

    // Get the destination thread that the target function is invoked on
    DelegateThread& GetThread() { return m_thread; }

protected:
    bool GetSync() { return m_sync; }
    void SetSync(bool sync) { m_sync = sync; }

private:
    DelegateThread& m_thread;   // The target thread to invoke the delegate function.
    bool m_sync = false;        // Flag to control synchronous vs asynchronous behavior.
    // </common_code>
};

template <class C, class R>
struct DelegateMemberSpAsync; // Not defined

// DelegateMemberSpAsync class asynchronously invokes a std::shared_ptr target function.
template <class TClass, class RetType, class... Args>
class DelegateMemberSpAsync<TClass, RetType(Args...)> : public DelegateMemberSp<TClass, RetType(Args...)>, public IDelegateInvoker {
public:
    typedef std::shared_ptr<TClass> ObjectPtr;
    typedef RetType(TClass::* MemberFunc)(Args...);
    typedef RetType(TClass::* ConstMemberFunc)(Args...) const;
    using ClassType = DelegateMemberSpAsync<TClass, RetType(Args...)>;
    using BaseType = DelegateMemberSp<TClass, RetType(Args...)>;

    // Contructors take a class instance, member function, and callback thread
    DelegateMemberSpAsync(ObjectPtr object, MemberFunc func, DelegateThread& thread) : BaseType(object, func), m_thread(thread) {
        Bind(object, func, thread);
    }
    DelegateMemberSpAsync(ObjectPtr object, ConstMemberFunc func, DelegateThread& thread) : BaseType(object, func), m_thread(thread) {
        Bind(object, func, thread);
    }
    DelegateMemberSpAsync(const ClassType& rhs) :
        BaseType(rhs), m_thread(rhs.m_thread) {
        Assign(rhs);
    }
    DelegateMemberSpAsync() = delete;

    // Bind a member function to a delegate. 
    void Bind(ObjectPtr object, MemberFunc func, DelegateThread& thread) {
        m_thread = thread;
        BaseType::Bind(object, func);
    }

    // Bind a const member function to a delegate. 
    void Bind(ObjectPtr object, ConstMemberFunc func, DelegateThread& thread) {
        m_thread = thread;
        BaseType::Bind(object, func);
    }

    // <common_code>
    void Assign(const ClassType& rhs) {
        m_thread = rhs.m_thread;
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
            &m_thread == &derivedRhs->m_thread &&
            BaseType::operator==(rhs);
    }

    // Invoke delegate function asynchronously. Do not wait for return value, if any.
    // Called by the source thread. Dispatches the delegate data into the destination 
    // thread message queue. DelegateInvoke() must be called by the destination thread 
    // to invoke the target function.
    virtual RetType operator()(Args... args) override {
        if (this->GetSync())
            return BaseType::operator()(args...);
        else
        {
            // Create a clone instance of this delegate 
            auto delegate = std::shared_ptr<ClassType>(Clone());

            // Create the delegate message
            auto msg = std::make_shared<DelegateAsyncMsg<Args...>>(delegate, args...);

            // Dispatch message onto the callback destination thread. DelegateInvoke()
            // will be called by the destintation thread. 
            GetThread().DispatchDelegate(msg);

            // Do not wait for destination thread return value from async function call
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

    // Invoke delegate function asynchronously. Do not wait for return value, if any.
    // Called by the source thread.
    void AsyncInvoke(Args... args) {
        operator()(args...);   
    }

    // Invoke the delegate function on the destination thread. Each source thread call
    // to operator() generate a call to DelegateInvoke() on the destination thread. 
    // Unlike DelegateAsyncWait, a lock is not required between source and destination 
    // delegateMsg access because the source thread is not waiting for the function call 
    // to complete.
    virtual void DelegateInvoke(std::shared_ptr<DelegateMsg> msg) {
        // Typecast the base pointer to back correct derived to instance
        auto delegateMsg = std::dynamic_pointer_cast<DelegateAsyncMsg<Args...>>(msg);
        if (delegateMsg == nullptr)
            throw std::invalid_argument("Invalid DelegateMsgHeapArgs cast");

        // Invoke the delegate function asynchronously
        SetSync(true);
        std::apply(&BaseType::operator(), 
            std::tuple_cat(std::make_tuple(this), delegateMsg->GetArgs()));
    }

    // Get the destination thread that the target function is invoked on
    DelegateThread& GetThread() { return m_thread; }

protected:
    bool GetSync() { return m_sync; }
    void SetSync(bool sync) { m_sync = sync; }

private:
    DelegateThread& m_thread;   // The target thread to invoke the delegate function.
    bool m_sync = false;        // Flag to control synchronous vs asynchronous behavior.
    // </common_code>
};

template <class R>
struct DelegateFunctionAsync; // Not defined

// DelegateFunctionAsync class asynchronously invokes a std::function target function.
template <class RetType, class... Args>
class DelegateFunctionAsync<RetType(Args...)> : public DelegateFunction<RetType(Args...)>, public IDelegateInvoker {
public:
    using FunctionType = std::function<RetType(Args...)>;
    using ClassType = DelegateFunctionAsync<RetType(Args...)>;
    using BaseType = DelegateFunction<RetType(Args...)>;

    DelegateFunctionAsync(FunctionType func, DelegateThread& thread) :
        BaseType(func), m_thread(thread) {
        Bind(func, thread);
    }

    DelegateFunctionAsync(const ClassType& rhs) :
        BaseType(rhs), m_thread(rhs.m_thread) {
        Assign(rhs);
    }
    DelegateFunctionAsync() = delete;

    // Bind a std::function to the delegate.
    void Bind(FunctionType func, DelegateThread& thread) {
        m_thread = thread;
        BaseType::Bind(func);
    }

    // <common_code>
    void Assign(const ClassType& rhs) {
        m_thread = rhs.m_thread;
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
            &m_thread == &derivedRhs->m_thread &&
            BaseType::operator==(rhs);
    }

    // Invoke delegate function asynchronously. Do not wait for return value, if any.
    // Called by the source thread. Dispatches the delegate data into the destination 
    // thread message queue. DelegateInvoke() must be called by the destination thread 
    // to invoke the target function.
    virtual RetType operator()(Args... args) override {
        if (this->GetSync())
            return BaseType::operator()(args...);
        else
        {
            // Create a clone instance of this delegate 
            auto delegate = std::shared_ptr<ClassType>(Clone());

            // Create the delegate message
            auto msg = std::make_shared<DelegateAsyncMsg<Args...>>(delegate, args...);

            // Dispatch message onto the callback destination thread. DelegateInvoke()
            // will be called by the destintation thread. 
            GetThread().DispatchDelegate(msg);

            // Do not wait for destination thread return value from async function call
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

    // Invoke delegate function asynchronously. Do not wait for return value, if any.
    // Called by the source thread.
    void AsyncInvoke(Args... args) {
        operator()(args...);   
    }

    // Invoke the delegate function on the destination thread. Each source thread call
    // to operator() generate a call to DelegateInvoke() on the destination thread. 
    // Unlike DelegateAsyncWait, a lock is not required between source and destination 
    // delegateMsg access because the source thread is not waiting for the function call 
    // to complete.
    virtual void DelegateInvoke(std::shared_ptr<DelegateMsg> msg) {
        // Typecast the base pointer to back correct derived to instance
        auto delegateMsg = std::dynamic_pointer_cast<DelegateAsyncMsg<Args...>>(msg);
        if (delegateMsg == nullptr)
            throw std::invalid_argument("Invalid DelegateMsgHeapArgs cast");

        // Invoke the delegate function asynchronously
        SetSync(true);
        std::apply(&BaseType::operator(), 
            std::tuple_cat(std::make_tuple(this), delegateMsg->GetArgs()));
    }

    // Get the destination thread that the target function is invoked on
    DelegateThread& GetThread() { return m_thread; }

protected:
    bool GetSync() { return m_sync; }
    void SetSync(bool sync) { m_sync = sync; }

private:
    DelegateThread& m_thread;   // The target thread to invoke the delegate function.
    bool m_sync = false;        // Flag to control synchronous vs asynchronous behavior.
    // </common_code>
};

template <class RetType, class... Args>
DelegateFreeAsync<RetType(Args...)> MakeDelegate(RetType(*func)(Args... args), DelegateThread& thread) {
    return DelegateFreeAsync<RetType(Args...)>(func, thread);
}

template <class TClass, class RetType, class... Args>
DelegateMemberAsync<TClass, RetType(Args...)> MakeDelegate(TClass* object, RetType(TClass::* func)(Args... args), DelegateThread& thread) {
    return DelegateMemberAsync<TClass, RetType(Args...)>(object, func, thread);
}

template <class TClass, class RetType, class... Args>
DelegateMemberAsync<TClass, RetType(Args...)> MakeDelegate(TClass* object, RetType(TClass::* func)(Args... args) const, DelegateThread& thread) {
    return DelegateMemberAsync<TClass, RetType(Args...)>(object, func, thread);
}

template <class TClass, class RetVal, class... Args>
DelegateMemberSpAsync<TClass, RetVal(Args...)> MakeDelegate(std::shared_ptr<TClass> object, RetVal(TClass::* func)(Args... args), DelegateThread& thread) {
    return DelegateMemberSpAsync<TClass, RetVal(Args...)>(object, func, thread);
}

template <class TClass, class RetVal, class... Args>
DelegateMemberSpAsync<TClass, RetVal(Args...)> MakeDelegate(std::shared_ptr<TClass> object, RetVal(TClass::* func)(Args... args) const, DelegateThread& thread) {
    return DelegateMemberSpAsync<TClass, RetVal(Args...)>(object, func, thread);
}

template <class RetType, class... Args>
DelegateFunctionAsync<RetType(Args...)> MakeDelegate(std::function<RetType(Args...)> func, DelegateThread& thread) {
    return DelegateFunctionAsync<RetType(Args...)>(func, thread);
}

}

#endif
