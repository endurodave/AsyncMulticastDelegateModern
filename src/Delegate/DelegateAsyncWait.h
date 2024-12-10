#ifndef _DELEGATE_ASYNC_WAIT_H
#define _DELEGATE_ASYNC_WAIT_H

// DelegateAsyncWait.h
// @see https://github.com/endurodave/AsyncMulticastDelegateModern
// David Lafreniere, Aug 2020.

/// @file
/// @brief Delegate "`AsyncWait`" series of classes used to invoke a function asynchronously and
/// block waiting for completion by the target thread. 
/// 
/// @details Asynchronous delegate that invokes the target function on the specified thread of control
/// and waits for the function to be executed or a timeout occurs. Use IsSuccess() to determine if 
/// asynchronous call succeeded.
/// 
/// Delegate "`AsyncWait`" series of classes used to invoke a function asynchronously and wait for 
/// completion by the destination target thread. Invoking a function asynchronously requires making 
/// a clone of the object to be sent to the destination thread message queue. The destination thread 
/// calls `DelegateInvoke()` to invoke the target function. The source thread blocks on a semaphore 
/// waiting for the destination thread to complete the function invoke. If the caller timeout expires, 
/// the target function is not invoked. 
/// 
/// The `m_lock` mutex is used to protect shared state data between the source and destination 
/// threads using the two thread safe functions below:
///
/// `RetType operator()(Args... args)` - called by the source thread to initiate the async
/// function call.
///
/// `void DelegateInvoke(std::shared_ptr<DelegateMsg> msg)` - called by the destination
/// thread to invoke the target function. The destination thread must not call any other
/// delegate instance functions.
///
/// Code within `<common_code>` and `</common_code>` is updated using sync_src.py. Manually update 
/// the code within the `DelegateFreeAsyncWait` `common_code` tags, then run the script to 
/// propagate to the remaining delegate classes to simplify code maintenance.
/// 
/// `python src_dup.py DelegateAsyncWait.h`

#include "DelegateAsync.h"
#include "DelegateThread.h"
#include "DelegateInvoker.h"
#include <optional>
#include <any>
#include <chrono>

namespace DelegateLib {

#undef max  // Prevent compiler error on next line if max is defined
constexpr auto WAIT_INFINITE = std::chrono::milliseconds::max();

/// @brief Stores all function arguments suitable for blocking asynchronous calls.
/// Argument data is not stored in the heap.
/// @tparam Args The target function arguments.
template <class...Args>
class DelegateAsyncWaitMsg : public DelegateMsg
{
public:
    /// Constructor
    /// @param[in] invoker - the invoker instance
    /// @param[in] args - a parameter pack of all target function arguments
    DelegateAsyncWaitMsg(std::shared_ptr<IDelegateInvoker> invoker, Args... args) : DelegateMsg(invoker),
        m_args(args...)
    {
    }

    virtual ~DelegateAsyncWaitMsg() {}

    /// Get all function arguments 
    /// @return A tuple of all function arguments
    std::tuple<Args...>& GetArgs() { return m_args; }

    /// Get the semaphore used to signal the sending thread that the receiving 
    /// thread has invoked the target function. 
    /// @return The semaphore reference.
    Semaphore& GetSema() { return m_sema; }

    /// Get a mutex shared between sender and receiver threads.
    /// @return The lock reference.
    std::mutex& GetLock() { return m_lock; }

    /// True if the sending thread is waiting for the receiver thread to call the function.
    /// False if the sending thread delegate timeout occurred and is not waiting.
    /// @return `true` if the sending thread is waiting for the receiving thread to complete
    /// the target function invoke.
    bool GetInvokerWaiting() { return m_invokerWaiting; }

    /// Set to true when source thread is waiting for destination thread to complete the 
    /// function call.
    /// @param[in] invokerWaiting The status of the invoker waiting flag.
    void SetInvokerWaiting(bool invokerWaiting) { m_invokerWaiting = invokerWaiting; }

private:
    /// An empty starting tuple
    std::tuple<> m_start;

    /// A tuple with each function argument element 
    std::tuple<Args...> m_args;

    /// Semaphore to signal waiting thread
    Semaphore m_sema;

    /// Lock to protect shared data 
    std::mutex m_lock;                       

    /// True if source thread is waiting for destination thread invoke to complete
    bool m_invokerWaiting = false;          
};

template <class R>
struct DelegateFreeAsyncWait; // Not defined

/// @brief `DelegateFreeAsyncWait<>` class asynchronously block invokes a free target function.
/// @tparam RetType The return type of the bound delegate function.
/// @tparam Args The argument types of the bound delegate function.
template <class RetType, class... Args>
class DelegateFreeAsyncWait<RetType(Args...)> : public DelegateFreeAsync<RetType(Args...)> {
public:
    typedef RetType(*FreeFunc)(Args...);
    using ClassType = DelegateFreeAsyncWait<RetType(Args...)>;
    using BaseType = DelegateFreeAsync<RetType(Args...)>;

    /// @brief Constructor to create a class instance.
    /// @param[in] func The target free function to store.
    /// @param[in] thread The execution thread to invoke `func`.
    /// @param[in] timeout The calling thread timeout for destination thread to
    /// invoke the target function. 
    DelegateFreeAsyncWait(FreeFunc func, DelegateThread& thread, std::chrono::milliseconds timeout) :
        BaseType(func, thread), m_timeout(timeout) {
        Bind(func, thread);
    }

    /// @brief Copy constructor that creates a copy of the given instance.
    /// @details This constructor initializes a new object as a copy of the 
    /// provided `rhs` (right-hand side) object. The `rhs` object is used to 
    /// set the state of the new instance.
    /// @param[in] rhs The object to copy from.
    DelegateFreeAsyncWait(const ClassType& rhs) :
        BaseType(rhs) {
        Assign(rhs);
    }

    DelegateFreeAsyncWait() = delete;

    /// @brief Bind a free function to the delegate.
    /// @details This method associates a free function (`func`) with the delegate. 
    /// Once the function is bound, the delegate can be used to invoke the function.
    /// @param[in] func The free function to bind to the delegate. This function must 
    /// match the signature of the delegate.
    /// @param[in] thread The execution thread to invoke `func`.
    void Bind(FreeFunc func, DelegateThread& thread) {
        BaseType::Bind(func, thread);
    }

    // <common_code>

    /// @brief Assigns the state of one object to another.
    /// @details Copy the state from the `rhs` (right-hand side) object to the
    /// current object.
    /// @param[in] rhs The object whose state is to be copied.
    void Assign(const ClassType& rhs) {
        m_timeout = rhs.m_timeout;
        BaseType::Assign(rhs);
    }

    /// @brief Creates a copy of the current object.
    /// @details Clones the current instance of the class by creating a new object
    /// and copying the state of the current object to it. 
    /// @return A pointer to a new `ClassType` instance.
    /// @post The caller is responsible for deleting the clone object.
    virtual ClassType* Clone() const override {
        return new ClassType(*this);
    }

    /// @brief Assignment operator that assigns the state of one object to another.
    /// @param[in] rhs The object whose state is to be assigned to the current object.
    /// @return A reference to the current object.
    ClassType& operator=(const ClassType& rhs) {
        if (&rhs != this) {
            BaseType::operator=(rhs);
            Assign(rhs);
        }
        return *this;
    }

    /// @brief Compares two delegate objects for equality.
    /// @param[in] rhs The `DelegateBase` object to compare with the current object.
    /// @return `true` if the two delegate objects are equal, `false` otherwise.
    virtual bool operator==(const DelegateBase& rhs) const override {
        auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
        return derivedRhs &&
            m_timeout == derivedRhs->m_timeout &&
            BaseType::operator==(rhs);
    }

    /// @brief Invoke delegate function asynchronously and block for function return value.
    /// Called by the source thread.
    /// @details Invoke delegate function asynchronously and wait for the return value.
    /// This function is called by the source thread. Dispatches the delegate data into the 
    /// destination thread message queue. `DelegateInvoke()` must be called by the destination 
    /// thread to invoke the target function.
    /// 
    /// If the destination thread invokes the function within `m_timeout`, the return 
    /// value is obtained from the destination thread function call. If `m_timeout` expires 
    /// before the destination thread processes the request, the target function is not 
    /// invoked and a default return value is returned to the caller with an undefined value. 
    /// Use `IsSuccess()` to check for success before using the return value. Alternatively, 
    /// use `AsyncInvoke()` and check the `std::optional` return value.
    /// 
    /// The `DelegateAsyncWaitMsg` does not duplicated and copy the function arguments into heap
    /// memory. The source thread waits on the destintation thread to complete, therefore argument
    /// data is shared between the source and destination threads and simultaneous access is prevented
    /// using a mutex.
    /// @param[in] args The function arguments, if any.
    /// @return The bound function return value, if any. Use `IsSuccess()` to determine if 
    /// the return value is valid before use.
    virtual RetType operator()(Args... args) override {
        // Synchronously invoke the target function?
        if (this->GetSync())
        {
            // Invoke the target function directly
            return BaseType::operator()(args...);
        }
        else
        {
            // Create a clone instance of this delegate 
            auto delegate = std::shared_ptr<ClassType>(Clone());

            // Create a new message instance for sending to the destination thread.
            auto msg = std::make_shared<DelegateAsyncWaitMsg<Args...>>(delegate, args...);
            msg->SetInvokerWaiting(true);

            // Dispatch message onto the callback destination thread. DelegateInvoke()
            // will be called by the destination thread. 
            this->GetThread().DispatchDelegate(msg);

            // Wait for destination thread to execute the delegate function and get return value
            if ((m_success = msg->GetSema().Wait(m_timeout)))
                m_retVal = delegate->m_retVal;

            // Protect data shared between source and destination threads
            const std::lock_guard<std::mutex> lock(msg->GetLock());

            // Set flag that source is not waiting anymore
            msg->SetInvokerWaiting(false);

            // Does the target function have a return value?
            if constexpr (std::is_void<RetType>::value == false)
            {
                // Is the return value valid? 
                if (m_retVal.has_value())
                {
                    // Return the destination thread target function return value
                    return std::any_cast<RetType>(m_retVal);
                }
                else
                {
                    // Return a default return value
                    return RetType();
                }
            }
        }
    }

    /// @brief Invoke delegate function asynchronously and block for function return value.
    /// Called by the source thread.
    /// @param[in] args The function arguments, if any.
    /// @return The bound function return value stored withing `std::optional`. Use  
    /// `has_value()` to check if the the return value is valid. `value()` contains 
    /// the target function return value.
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

    /// @brief Invoke the delegate function on the destination thread. Called by the 
    /// destination thread.
    /// @details Each source thread call to `operator()` generate a call to `DelegateInvoke()` 
    /// on the destination thread. A lock is used to protect source and destination thread shared 
    /// data. A semaphore is used to signal the source thread when the destination thread 
    /// completes the target function call.
    /// 
    /// If source thread timeout expires and before the destination thread invokes the 
    /// target function, the target function is not called.
    /// @param[in] msg The delegate message created and sent within `operator()(Args... args)`.
    virtual void DelegateInvoke(std::shared_ptr<DelegateMsg> msg) override {
        // Typecast the base pointer to back correct derived to instance
        auto delegateMsg = std::dynamic_pointer_cast<DelegateAsyncWaitMsg<Args...>>(msg);
        if (delegateMsg == nullptr)
            throw std::invalid_argument("Invalid DelegateAsyncWaitMsg cast");

        // Protect data shared between source and destination threads
        const std::lock_guard<std::mutex> lock(delegateMsg->GetLock());

        // Is the source thread waiting for the target function invoke to complete?
        if (delegateMsg->GetInvokerWaiting())
        {
            // Invoke the delegate function synchronously
            this->SetSync(true);

            // Does target function have a void return value?
            if constexpr (std::is_void<RetType>::value == true)
            {
                // Invoke the target function using the source thread supplied function arguments
                std::apply(&BaseType::operator(), std::tuple_cat(std::make_tuple(this), delegateMsg->GetArgs()));
            }
            else
            {
                // Invoke the target function using the source thread supplied function arguments 
                // and get the return value
                m_retVal = std::apply(&BaseType::operator(), std::tuple_cat(std::make_tuple(this), delegateMsg->GetArgs()));
            }

            // Signal the source thread that the destination thread function call is complete
            delegateMsg->GetSema().Signal();
        }
    }

    /// Returns `true` if asynchronous function successfully invoked on the target thread
    /// @return `true` if the target asynchronous function call succeeded. `false` if 
    /// the timeout expired before the target function could be invoked.
    bool IsSuccess() { return m_success; }

    /// Get the asynchronous function return value
    /// @return The destination thraed target function return value
    RetType GetRetVal() { return std::any_cast<RetType>(m_retVal); }

private:
    /// Set to `true` if async function call succeeds
    bool m_success = false;			        

    /// Time in mS to wait for async function to invoke
    std::chrono::milliseconds m_timeout;    

    /// Return value of the target invoked function
    std::any m_retVal;                      

    // </common_code>
};

template <class C, class R>
struct DelegateMemberAsyncWait; // Not defined

/// @brief `DelegateMemberAsyncWait<>` class asynchronously block invokes a class member target function.
/// @tparam TClass The class type that contains the member function.
/// @tparam RetType The return type of the bound delegate function.
/// @tparam Args The argument types of the bound delegate function.
template <class TClass, class RetType, class... Args>
class DelegateMemberAsyncWait<TClass, RetType(Args...)> : public DelegateMemberAsync<TClass, RetType(Args...)> {
public:
    typedef TClass* ObjectPtr;
    typedef RetType(TClass::*MemberFunc)(Args...);
    typedef RetType(TClass::*ConstMemberFunc)(Args...) const;
    using ClassType = DelegateMemberAsyncWait<TClass, RetType(Args...)>;
    using BaseType = DelegateMemberAsync<TClass, RetType(Args...)>;

    // Contructors take a class instance, member function, and delegate thread
    DelegateMemberAsyncWait(ObjectPtr object, MemberFunc func, DelegateThread& thread, std::chrono::milliseconds timeout) :
        BaseType(object, func, thread), m_timeout(timeout) {
        Bind(object, func, thread);
    }
    DelegateMemberAsyncWait(ObjectPtr object, ConstMemberFunc func, DelegateThread& thread, std::chrono::milliseconds timeout) :
        BaseType(object, func, thread), m_timeout(timeout) {
        Bind(object, func, thread);
    }
    DelegateMemberAsyncWait(const ClassType& rhs) :
        BaseType(rhs) {
        Assign(rhs);
    }
    DelegateMemberAsyncWait() = delete;

    /// Bind a member function to a delegate. 
    void Bind(ObjectPtr object, MemberFunc func, DelegateThread& thread) {
        BaseType::Bind(object, func, thread);
    }

    /// Bind a const member function to a delegate. 
    void Bind(ObjectPtr object, ConstMemberFunc func, DelegateThread& thread) {
        BaseType::Bind(object, func, thread);
    }

    // <common_code>

    /// @brief Assigns the state of one object to another.
    /// @details Copy the state from the `rhs` (right-hand side) object to the
    /// current object.
    /// @param[in] rhs The object whose state is to be copied.
    void Assign(const ClassType& rhs) {
        m_timeout = rhs.m_timeout;
        BaseType::Assign(rhs);
    }

    /// @brief Creates a copy of the current object.
    /// @details Clones the current instance of the class by creating a new object
    /// and copying the state of the current object to it. 
    /// @return A pointer to a new `ClassType` instance.
    /// @post The caller is responsible for deleting the clone object.
    virtual ClassType* Clone() const override {
        return new ClassType(*this);
    }

    /// @brief Assignment operator that assigns the state of one object to another.
    /// @param[in] rhs The object whose state is to be assigned to the current object.
    /// @return A reference to the current object.
    ClassType& operator=(const ClassType& rhs) {
        if (&rhs != this) {
            BaseType::operator=(rhs);
            Assign(rhs);
        }
        return *this;
    }

    /// @brief Compares two delegate objects for equality.
    /// @param[in] rhs The `DelegateBase` object to compare with the current object.
    /// @return `true` if the two delegate objects are equal, `false` otherwise.
    virtual bool operator==(const DelegateBase& rhs) const override {
        auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
        return derivedRhs &&
            m_timeout == derivedRhs->m_timeout &&
            BaseType::operator==(rhs);
    }

    /// @brief Invoke delegate function asynchronously and block for function return value. 
    /// @details Invoke delegate function asynchronously and wait for the return value.
    /// This function is called by the source thread. Dispatches the delegate data into the 
    /// destination thread message queue. `DelegateInvoke()` must be called by the destination 
    /// thread to invoke the target function.
    /// 
    /// If the destination thread invokes the function within `m_timeout`, the return 
    /// value is obtained from the destination thread function call. If `m_timeout` expires 
    /// before the destination thread processes the request, the target function is not 
    /// invoked and a default return value is returned to the caller with an undefined value. 
    /// Use `IsSuccess()` to check for success before using the return value. Alternatively, 
    /// use `AsyncInvoke()` and check the `std::optional` return value.
    /// 
    /// The `DelegateAsyncWaitMsg` does not duplicated and copy the function arguments into heap
    /// memory. The source thread waits on the destintation thread to complete, therefore argument
    /// data is shared between the source and destination threads and simultaneous access is prevented
    /// using a mutex.
    /// @param[in] args The function arguments, if any.
    /// @return The bound function return value, if any. Use `IsSuccess()` to determine if 
    /// the return value is valid before use.
    virtual RetType operator()(Args... args) override {
        // Synchronously invoke the target function?
        if (this->GetSync())
        {
            // Invoke the target function directly
            return BaseType::operator()(args...);
        }
        else
        {
            // Create a clone instance of this delegate 
            auto delegate = std::shared_ptr<ClassType>(Clone());

            // Create a new message instance for sending to the destination thread.
            auto msg = std::make_shared<DelegateAsyncWaitMsg<Args...>>(delegate, args...);
            msg->SetInvokerWaiting(true);

            // Dispatch message onto the callback destination thread. DelegateInvoke()
            // will be called by the destination thread. 
            this->GetThread().DispatchDelegate(msg);

            // Wait for destination thread to execute the delegate function and get return value
            if ((m_success = msg->GetSema().Wait(m_timeout)))
                m_retVal = delegate->m_retVal;

            // Protect data shared between source and destination threads
            const std::lock_guard<std::mutex> lock(msg->GetLock());

            // Set flag that source is not waiting anymore
            msg->SetInvokerWaiting(false);

            // Does the target function have a return value?
            if constexpr (std::is_void<RetType>::value == false)
            {
                // Is the return value valid? 
                if (m_retVal.has_value())
                {
                    // Return the destination thread target function return value
                    return std::any_cast<RetType>(m_retVal);
                }
                else
                {
                    // Return a default return value
                    return RetType();
                }
            }
        }
    }

    /// @brief Invoke delegate function asynchronously and block for function return value.
    /// Called by the source thread.
    /// @param[in] args The function arguments, if any.
    /// @return The bound function return value stored withing `std::optional`. Use  
    /// `has_value()` to check if the the return value is valid. `value()` contains 
    /// the target function return value.
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

    /// @brief Invoke the delegate function on the destination thread. 
    /// @details Each source thread call to `operator()` generate a call to `DelegateInvoke()` 
    /// on the destination thread. A lock is used to protect source and destination thread shared 
    /// data. A semaphore is used to signal the source thread when the destination thread 
    /// completes the target function call.
    /// 
    /// If source thread timeout expires and before the destination thread invokes the 
    /// target function, the target function is not called.
    /// @param[in] msg The delegate message created and sent within `operator()(Args... args)`.
    virtual void DelegateInvoke(std::shared_ptr<DelegateMsg> msg) override {
        // Typecast the base pointer to back correct derived to instance
        auto delegateMsg = std::dynamic_pointer_cast<DelegateAsyncWaitMsg<Args...>>(msg);
        if (delegateMsg == nullptr)
            throw std::invalid_argument("Invalid DelegateAsyncWaitMsg cast");

        // Protect data shared between source and destination threads
        const std::lock_guard<std::mutex> lock(delegateMsg->GetLock());

        // Is the source thread waiting for the target function invoke to complete?
        if (delegateMsg->GetInvokerWaiting())
        {
            // Invoke the delegate function synchronously
            this->SetSync(true);

            // Does target function have a void return value?
            if constexpr (std::is_void<RetType>::value == true)
            {
                // Invoke the target function using the source thread supplied function arguments
                std::apply(&BaseType::operator(), std::tuple_cat(std::make_tuple(this), delegateMsg->GetArgs()));
            }
            else
            {
                // Invoke the target function using the source thread supplied function arguments 
                // and get the return value
                m_retVal = std::apply(&BaseType::operator(), std::tuple_cat(std::make_tuple(this), delegateMsg->GetArgs()));
            }

            // Signal the source thread that the destination thread function call is complete
            delegateMsg->GetSema().Signal();
        }
    }

    /// Returns `true` if asynchronous function successfully invoked on the target thread
    /// @return `true` if the target asynchronous function call succeeded. `false` if 
    /// the timeout expired before the target function could be invoked.
    bool IsSuccess() { return m_success; }

    /// Get the asynchronous function return value
    /// @return The destination thraed target function return value
    RetType GetRetVal() { return std::any_cast<RetType>(m_retVal); }

private:
    /// Set to `true` if async function call succeeds
    bool m_success = false;			        

    /// Time in mS to wait for async function to invoke
    std::chrono::milliseconds m_timeout;    

    /// Return value of the target invoked function
    std::any m_retVal;                      

    // </common_code>
};

template <class C, class R>
struct DelegateMemberSpAsyncWait; // Not defined

/// @brief `DelegateMemberSpAsyncWait<>` class asynchronously block invokes a std::shared_ptr target function.
/// @tparam TClass The class type that contains the member function.
/// @tparam RetType The return type of the bound delegate function.
/// @tparam Args The argument types of the bound delegate function.
template <class TClass, class RetType, class... Args>
class DelegateMemberSpAsyncWait<TClass, RetType(Args...)> : public DelegateMemberSpAsync<TClass, RetType(Args...)> {
public:
    typedef std::shared_ptr<TClass> ObjectPtr;
    typedef RetType(TClass::* MemberFunc)(Args...);
    typedef RetType(TClass::* ConstMemberFunc)(Args...) const;
    using ClassType = DelegateMemberSpAsyncWait<TClass, RetType(Args...)>;
    using BaseType = DelegateMemberSpAsync<TClass, RetType(Args...)>;

    // Contructors take a class instance, member function, and delegate thread
    DelegateMemberSpAsyncWait(ObjectPtr object, MemberFunc func, DelegateThread& thread, std::chrono::milliseconds timeout) :
        BaseType(object, func, thread), m_timeout(timeout) {
        Bind(object, func, thread);
    }
    DelegateMemberSpAsyncWait(ObjectPtr object, ConstMemberFunc func, DelegateThread& thread, std::chrono::milliseconds timeout) :
        BaseType(object, func, thread), m_timeout(timeout) {
        Bind(object, func, thread);
    }
    DelegateMemberSpAsyncWait(const ClassType& rhs) :
        BaseType(rhs) {
        Assign(rhs);
    }
    DelegateMemberSpAsyncWait() = delete;

    /// Bind a member function to a delegate. 
    void Bind(ObjectPtr object, MemberFunc func, DelegateThread& thread) {
        BaseType::Bind(object, func, thread);
    }

    /// Bind a const member function to a delegate. 
    void Bind(ObjectPtr object, ConstMemberFunc func, DelegateThread& thread) {
        BaseType::Bind(object, func, thread);
    }

    // <common_code>

    /// @brief Assigns the state of one object to another.
    /// @details Copy the state from the `rhs` (right-hand side) object to the
    /// current object.
    /// @param[in] rhs The object whose state is to be copied.
    void Assign(const ClassType& rhs) {
        m_timeout = rhs.m_timeout;
        BaseType::Assign(rhs);
    }

    /// @brief Creates a copy of the current object.
    /// @details Clones the current instance of the class by creating a new object
    /// and copying the state of the current object to it. 
    /// @return A pointer to a new `ClassType` instance.
    /// @post The caller is responsible for deleting the clone object.
    virtual ClassType* Clone() const override {
        return new ClassType(*this);
    }

    /// @brief Assignment operator that assigns the state of one object to another.
    /// @param[in] rhs The object whose state is to be assigned to the current object.
    /// @return A reference to the current object.
    ClassType& operator=(const ClassType& rhs) {
        if (&rhs != this) {
            BaseType::operator=(rhs);
            Assign(rhs);
        }
        return *this;
    }

    /// @brief Compares two delegate objects for equality.
    /// @param[in] rhs The `DelegateBase` object to compare with the current object.
    /// @return `true` if the two delegate objects are equal, `false` otherwise.
    virtual bool operator==(const DelegateBase& rhs) const override {
        auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
        return derivedRhs &&
            m_timeout == derivedRhs->m_timeout &&
            BaseType::operator==(rhs);
    }

    /// @brief Invoke delegate function asynchronously and block for function return value. 
    /// @details Invoke delegate function asynchronously and wait for the return value.
    /// This function is called by the source thread. Dispatches the delegate data into the 
    /// destination thread message queue. `DelegateInvoke()` must be called by the destination 
    /// thread to invoke the target function.
    /// 
    /// If the destination thread invokes the function within `m_timeout`, the return 
    /// value is obtained from the destination thread function call. If `m_timeout` expires 
    /// before the destination thread processes the request, the target function is not 
    /// invoked and a default return value is returned to the caller with an undefined value. 
    /// Use `IsSuccess()` to check for success before using the return value. Alternatively, 
    /// use `AsyncInvoke()` and check the `std::optional` return value.
    /// 
    /// The `DelegateAsyncWaitMsg` does not duplicated and copy the function arguments into heap
    /// memory. The source thread waits on the destintation thread to complete, therefore argument
    /// data is shared between the source and destination threads and simultaneous access is prevented
    /// using a mutex.
    /// @param[in] args The function arguments, if any.
    /// @return The bound function return value, if any. Use `IsSuccess()` to determine if 
    /// the return value is valid before use.
    virtual RetType operator()(Args... args) override {
        // Synchronously invoke the target function?
        if (this->GetSync())
        {
            // Invoke the target function directly
            return BaseType::operator()(args...);
        }
        else
        {
            // Create a clone instance of this delegate 
            auto delegate = std::shared_ptr<ClassType>(Clone());

            // Create a new message instance for sending to the destination thread.
            auto msg = std::make_shared<DelegateAsyncWaitMsg<Args...>>(delegate, args...);
            msg->SetInvokerWaiting(true);

            // Dispatch message onto the callback destination thread. DelegateInvoke()
            // will be called by the destination thread. 
            this->GetThread().DispatchDelegate(msg);

            // Wait for destination thread to execute the delegate function and get return value
            if ((m_success = msg->GetSema().Wait(m_timeout)))
                m_retVal = delegate->m_retVal;

            // Protect data shared between source and destination threads
            const std::lock_guard<std::mutex> lock(msg->GetLock());

            // Set flag that source is not waiting anymore
            msg->SetInvokerWaiting(false);

            // Does the target function have a return value?
            if constexpr (std::is_void<RetType>::value == false)
            {
                // Is the return value valid? 
                if (m_retVal.has_value())
                {
                    // Return the destination thread target function return value
                    return std::any_cast<RetType>(m_retVal);
                }
                else
                {
                    // Return a default return value
                    return RetType();
                }
            }
        }
    }

    /// @brief Invoke delegate function asynchronously and block for function return value.
    /// Called by the source thread.
    /// @param[in] args The function arguments, if any.
    /// @return The bound function return value stored withing `std::optional`. Use  
    /// `has_value()` to check if the the return value is valid. `value()` contains 
    /// the target function return value.
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

    /// @brief Invoke the delegate function on the destination thread. 
    /// @details Each source thread call to `operator()` generate a call to `DelegateInvoke()` 
    /// on the destination thread. A lock is used to protect source and destination thread shared 
    /// data. A semaphore is used to signal the source thread when the destination thread 
    /// completes the target function call.
    /// 
    /// If source thread timeout expires and before the destination thread invokes the 
    /// target function, the target function is not called.
    /// @param[in] msg The delegate message created and sent within `operator()(Args... args)`.
    virtual void DelegateInvoke(std::shared_ptr<DelegateMsg> msg) override {
        // Typecast the base pointer to back correct derived to instance
        auto delegateMsg = std::dynamic_pointer_cast<DelegateAsyncWaitMsg<Args...>>(msg);
        if (delegateMsg == nullptr)
            throw std::invalid_argument("Invalid DelegateAsyncWaitMsg cast");

        // Protect data shared between source and destination threads
        const std::lock_guard<std::mutex> lock(delegateMsg->GetLock());

        // Is the source thread waiting for the target function invoke to complete?
        if (delegateMsg->GetInvokerWaiting())
        {
            // Invoke the delegate function synchronously
            this->SetSync(true);

            // Does target function have a void return value?
            if constexpr (std::is_void<RetType>::value == true)
            {
                // Invoke the target function using the source thread supplied function arguments
                std::apply(&BaseType::operator(), std::tuple_cat(std::make_tuple(this), delegateMsg->GetArgs()));
            }
            else
            {
                // Invoke the target function using the source thread supplied function arguments 
                // and get the return value
                m_retVal = std::apply(&BaseType::operator(), std::tuple_cat(std::make_tuple(this), delegateMsg->GetArgs()));
            }

            // Signal the source thread that the destination thread function call is complete
            delegateMsg->GetSema().Signal();
        }
    }

    /// Returns `true` if asynchronous function successfully invoked on the target thread
    /// @return `true` if the target asynchronous function call succeeded. `false` if 
    /// the timeout expired before the target function could be invoked.
    bool IsSuccess() { return m_success; }

    /// Get the asynchronous function return value
    /// @return The destination thraed target function return value
    RetType GetRetVal() { return std::any_cast<RetType>(m_retVal); }

private:
    /// Set to `true` if async function call succeeds
    bool m_success = false;			        

    /// Time in mS to wait for async function to invoke
    std::chrono::milliseconds m_timeout;    

    /// Return value of the target invoked function
    std::any m_retVal;                      

    // </common_code>
};

template <class R>
struct DelegateFunctionAsyncWait; // Not defined

/// @brief `DelegateFunctionAsyncWait<>` class asynchronously block invokes a std::function target function.
/// @tparam RetType The return type of the bound delegate function.
/// @tparam Args The argument types of the bound delegate function.
template <class RetType, class... Args>
class DelegateFunctionAsyncWait<RetType(Args...)> : public DelegateFunctionAsync<RetType(Args...)> {
public:
    using FunctionType = std::function<RetType(Args...)>;
    using ClassType = DelegateFunctionAsyncWait<RetType(Args...)>;
    using BaseType = DelegateFunctionAsync<RetType(Args...)>;

    // Contructors take a std::function, delegate thread and timeout
    DelegateFunctionAsyncWait(FunctionType func, DelegateThread& thread, std::chrono::milliseconds timeout) :
        BaseType(func, thread), m_timeout(timeout) {
        Bind(func, thread);
    }
    DelegateFunctionAsyncWait(const ClassType& rhs) :
        BaseType(rhs) {
        Assign(rhs);
    }
    DelegateFunctionAsyncWait() = delete;

    /// Bind a std::function to a delegate. 
    void Bind(FunctionType func, DelegateThread& thread) {
        BaseType::Bind(func, thread);
    }

    // <common_code>

    /// @brief Assigns the state of one object to another.
    /// @details Copy the state from the `rhs` (right-hand side) object to the
    /// current object.
    /// @param[in] rhs The object whose state is to be copied.
    void Assign(const ClassType& rhs) {
        m_timeout = rhs.m_timeout;
        BaseType::Assign(rhs);
    }

    /// @brief Creates a copy of the current object.
    /// @details Clones the current instance of the class by creating a new object
    /// and copying the state of the current object to it. 
    /// @return A pointer to a new `ClassType` instance.
    /// @post The caller is responsible for deleting the clone object.
    virtual ClassType* Clone() const override {
        return new ClassType(*this);
    }

    /// @brief Assignment operator that assigns the state of one object to another.
    /// @param[in] rhs The object whose state is to be assigned to the current object.
    /// @return A reference to the current object.
    ClassType& operator=(const ClassType& rhs) {
        if (&rhs != this) {
            BaseType::operator=(rhs);
            Assign(rhs);
        }
        return *this;
    }

    /// @brief Compares two delegate objects for equality.
    /// @param[in] rhs The `DelegateBase` object to compare with the current object.
    /// @return `true` if the two delegate objects are equal, `false` otherwise.
    virtual bool operator==(const DelegateBase& rhs) const override {
        auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
        return derivedRhs &&
            m_timeout == derivedRhs->m_timeout &&
            BaseType::operator==(rhs);
    }

    /// @brief Invoke delegate function asynchronously and block for function return value. 
    /// @details Invoke delegate function asynchronously and wait for the return value.
    /// This function is called by the source thread. Dispatches the delegate data into the 
    /// destination thread message queue. `DelegateInvoke()` must be called by the destination 
    /// thread to invoke the target function.
    /// 
    /// If the destination thread invokes the function within `m_timeout`, the return 
    /// value is obtained from the destination thread function call. If `m_timeout` expires 
    /// before the destination thread processes the request, the target function is not 
    /// invoked and a default return value is returned to the caller with an undefined value. 
    /// Use `IsSuccess()` to check for success before using the return value. Alternatively, 
    /// use `AsyncInvoke()` and check the `std::optional` return value.
    /// 
    /// The `DelegateAsyncWaitMsg` does not duplicated and copy the function arguments into heap
    /// memory. The source thread waits on the destintation thread to complete, therefore argument
    /// data is shared between the source and destination threads and simultaneous access is prevented
    /// using a mutex.
    /// @param[in] args The function arguments, if any.
    /// @return The bound function return value, if any. Use `IsSuccess()` to determine if 
    /// the return value is valid before use.
    virtual RetType operator()(Args... args) override {
        // Synchronously invoke the target function?
        if (this->GetSync())
        {
            // Invoke the target function directly
            return BaseType::operator()(args...);
        }
        else
        {
            // Create a clone instance of this delegate 
            auto delegate = std::shared_ptr<ClassType>(Clone());

            // Create a new message instance for sending to the destination thread.
            auto msg = std::make_shared<DelegateAsyncWaitMsg<Args...>>(delegate, args...);
            msg->SetInvokerWaiting(true);

            // Dispatch message onto the callback destination thread. DelegateInvoke()
            // will be called by the destination thread. 
            this->GetThread().DispatchDelegate(msg);

            // Wait for destination thread to execute the delegate function and get return value
            if ((m_success = msg->GetSema().Wait(m_timeout)))
                m_retVal = delegate->m_retVal;

            // Protect data shared between source and destination threads
            const std::lock_guard<std::mutex> lock(msg->GetLock());

            // Set flag that source is not waiting anymore
            msg->SetInvokerWaiting(false);

            // Does the target function have a return value?
            if constexpr (std::is_void<RetType>::value == false)
            {
                // Is the return value valid? 
                if (m_retVal.has_value())
                {
                    // Return the destination thread target function return value
                    return std::any_cast<RetType>(m_retVal);
                }
                else
                {
                    // Return a default return value
                    return RetType();
                }
            }
        }
    }

    /// @brief Invoke delegate function asynchronously and block for function return value.
    /// Called by the source thread.
    /// @param[in] args The function arguments, if any.
    /// @return The bound function return value stored withing `std::optional`. Use  
    /// `has_value()` to check if the the return value is valid. `value()` contains 
    /// the target function return value.
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

    /// @brief Invoke the delegate function on the destination thread. 
    /// @details Each source thread call to `operator()` generate a call to `DelegateInvoke()` 
    /// on the destination thread. A lock is used to protect source and destination thread shared 
    /// data. A semaphore is used to signal the source thread when the destination thread 
    /// completes the target function call.
    /// 
    /// If source thread timeout expires and before the destination thread invokes the 
    /// target function, the target function is not called.
    /// @param[in] msg The delegate message created and sent within `operator()(Args... args)`.
    virtual void DelegateInvoke(std::shared_ptr<DelegateMsg> msg) override {
        // Typecast the base pointer to back correct derived to instance
        auto delegateMsg = std::dynamic_pointer_cast<DelegateAsyncWaitMsg<Args...>>(msg);
        if (delegateMsg == nullptr)
            throw std::invalid_argument("Invalid DelegateAsyncWaitMsg cast");

        // Protect data shared between source and destination threads
        const std::lock_guard<std::mutex> lock(delegateMsg->GetLock());

        // Is the source thread waiting for the target function invoke to complete?
        if (delegateMsg->GetInvokerWaiting())
        {
            // Invoke the delegate function synchronously
            this->SetSync(true);

            // Does target function have a void return value?
            if constexpr (std::is_void<RetType>::value == true)
            {
                // Invoke the target function using the source thread supplied function arguments
                std::apply(&BaseType::operator(), std::tuple_cat(std::make_tuple(this), delegateMsg->GetArgs()));
            }
            else
            {
                // Invoke the target function using the source thread supplied function arguments 
                // and get the return value
                m_retVal = std::apply(&BaseType::operator(), std::tuple_cat(std::make_tuple(this), delegateMsg->GetArgs()));
            }

            // Signal the source thread that the destination thread function call is complete
            delegateMsg->GetSema().Signal();
        }
    }

    /// Returns `true` if asynchronous function successfully invoked on the target thread
    /// @return `true` if the target asynchronous function call succeeded. `false` if 
    /// the timeout expired before the target function could be invoked.
    bool IsSuccess() { return m_success; }

    /// Get the asynchronous function return value
    /// @return The destination thraed target function return value
    RetType GetRetVal() { return std::any_cast<RetType>(m_retVal); }

private:
    /// Set to `true` if async function call succeeds
    bool m_success = false;			        

    /// Time in mS to wait for async function to invoke
    std::chrono::milliseconds m_timeout;    

    /// Return value of the target invoked function
    std::any m_retVal;                      

    // </common_code>
};

/// @brief Creates an asynchronous delegate that binds to a free function with a wait and timeout.
/// @tparam RetType The return type of the free function.
/// @tparam Args The types of the function arguments.
/// @param[in] func A pointer to the free function to bind to the delegate.
/// @param[in] thread The `DelegateThread` on which the function will be invoked asynchronously.
/// @param[in] timeout The duration to wait for the function to complete before returning.
/// @return A `DelegateFreeAsyncWait` object bound to the specified free function, thread, and timeout.
template <class RetType, class... Args>
DelegateFreeAsyncWait<RetType(Args...)> MakeDelegate(RetType(*func)(Args... args), DelegateThread& thread, std::chrono::milliseconds timeout) {
    return DelegateFreeAsyncWait<RetType(Args...)>(func, thread, timeout);
}

/// @brief Creates an asynchronous delegate that binds to a non-const member function with a wait and timeout.
/// @tparam TClass The class type that contains the member function.
/// @tparam RetType The return type of the member function.
/// @tparam Args The types of the function arguments.
/// @param[in] object A pointer to the instance of `TClass` that will be used for the delegate.
/// @param[in] func A pointer to the non-const member function of `TClass` to bind to the delegate.
/// @param[in] thread The `DelegateThread` on which the function will be invoked asynchronously.
/// @param[in] timeout The duration to wait for the function to complete before returning.
/// @return A `DelegateMemberAsyncWait` object bound to the specified non-const member function, thread, and timeout.
template <class TClass, class RetType, class... Args>
DelegateMemberAsyncWait<TClass, RetType(Args...)> MakeDelegate(TClass* object, RetType(TClass::*func)(Args... args), DelegateThread& thread, std::chrono::milliseconds timeout) {
    return DelegateMemberAsyncWait<TClass, RetType(Args...)>(object, func, thread, timeout);
}

/// @brief Creates an asynchronous delegate that binds to a const member function with a wait and timeout.
/// @tparam TClass The class type that contains the member function.
/// @tparam RetType The return type of the member function.
/// @tparam Args The types of the function arguments.
/// @param[in] object A pointer to the instance of `TClass` that will be used for the delegate.
/// @param[in] func A pointer to the const member function of `TClass` to bind to the delegate.
/// @param[in] thread The `DelegateThread` on which the function will be invoked asynchronously.
/// @param[in] timeout The duration to wait for the function to complete before returning.
/// @return A `DelegateMemberAsyncWait` object bound to the specified const member function, thread, and timeout.
template <class TClass, class RetType, class... Args>
DelegateMemberAsyncWait<TClass, RetType(Args...)> MakeDelegate(TClass* object, RetType(TClass::*func)(Args... args) const, DelegateThread& thread, std::chrono::milliseconds timeout) {
    return DelegateMemberAsyncWait<TClass, RetType(Args...)>(object, func, thread, timeout);
}


/// @brief Creates an asynchronous delegate that binds to a non-const member function using a shared pointer, with a wait and timeout.
/// @tparam TClass The class type that contains the member function.
/// @tparam RetVal The return type of the member function.
/// @tparam Args The types of the function arguments.
/// @param[in] object A shared pointer to the instance of `TClass` that will be used for the delegate.
/// @param[in] func A pointer to the non-const member function of `TClass` to bind to the delegate.
/// @param[in] thread The `DelegateThread` on which the function will be invoked asynchronously.
/// @param[in] timeout The duration to wait for the function to complete before returning.
/// @return A `DelegateMemberSpAsyncWait` object bound to the specified non-const member function, thread, and timeout.
template <class TClass, class RetVal, class... Args>
DelegateMemberSpAsyncWait<TClass, RetVal(Args...)> MakeDelegate(std::shared_ptr<TClass> object, RetVal(TClass::* func)(Args... args), DelegateThread& thread, std::chrono::milliseconds timeout) {
    return DelegateMemberSpAsyncWait<TClass, RetVal(Args...)>(object, func, thread, timeout);
}

/// @brief Creates an asynchronous delegate that binds to a const member function using a shared pointer, with a wait and timeout.
/// @tparam TClass The class type that contains the member function.
/// @tparam RetVal The return type of the member function.
/// @tparam Args The types of the function arguments.
/// @param[in] object A shared pointer to the instance of `TClass` that will be used for the delegate.
/// @param[in] func A pointer to the const member function of `TClass` to bind to the delegate.
/// @param[in] thread The `DelegateThread` on which the function will be invoked asynchronously.
/// @param[in] timeout The duration to wait for the function to complete before returning.
/// @return A `DelegateMemberSpAsyncWait` object bound to the specified const member function, thread, and timeout.
template <class TClass, class RetVal, class... Args>
DelegateMemberSpAsyncWait<TClass, RetVal(Args...)> MakeDelegate(std::shared_ptr<TClass> object, RetVal(TClass::* func)(Args... args) const, DelegateThread& thread, std::chrono::milliseconds timeout) {
    return DelegateMemberSpAsyncWait<TClass, RetVal(Args...)>(object, func, thread, timeout);
}

/// @brief Creates an asynchronous delegate that binds to a `std::function` with a wait and timeout.
/// @tparam RetType The return type of the `std::function`.
/// @tparam Args The types of the function arguments.
/// @param[in] func The `std::function` to bind to the delegate.
/// @param[in] thread The `DelegateThread` on which the function will be invoked asynchronously.
/// @param[in] timeout The duration to wait for the function to complete before returning.
/// @return A `DelegateFunctionAsyncWait` object bound to the specified `std::function`, thread, and timeout.
template <class RetType, class... Args>
DelegateFunctionAsyncWait<RetType(Args...)> MakeDelegate(std::function<RetType(Args...)> func, DelegateThread& thread, std::chrono::milliseconds timeout) {
    return DelegateFunctionAsyncWait<RetType(Args...)>(func, thread, timeout);
}

} 

#endif