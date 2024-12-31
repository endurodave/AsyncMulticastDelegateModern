#ifndef _DELEGATE_ASYNC_WAIT_H
#define _DELEGATE_ASYNC_WAIT_H

// DelegateAsyncWait.h
// @see https://github.com/endurodave/cpp-async-delegate
// David Lafreniere, Aug 2020.

/// @file
/// @brief Delegate "`AsyncWait`" series of classes used to invoke a function asynchronously and
/// block waiting for completion by the target thread. 
/// 
/// @details Asynchronous delegate that invokes the target function on the specified thread of control
/// and waits for the function to be executed or a timeout occurs. Use IsSuccess() to determine if 
/// asynchronous call succeeded before using the return value and outgoing argument values.
/// 
/// Delegate "`AsyncWait`" series of classes used to invoke a function asynchronously and wait for 
/// completion by the destination target thread. Invoking a function asynchronously requires making 
/// a clone of the object to be sent to the destination thread message queue. The destination thread 
/// calls `Invoke()` to invoke the target function. The source thread blocks on a semaphore 
/// waiting for the destination thread to complete the function invoke. If the caller timeout expires, 
/// the target function is not invoked. 
/// 
/// The `m_lock` mutex is used to protect shared state data between the source and destination 
/// threads using the two thread safe functions below:
///
/// `RetType operator()(Args... args)` - called by the source thread to initiate the async
/// function call. May throw `std::bad_alloc` if dynamic storage allocation fails. Clone()
/// also may throw `std::bad_alloc`. All other delegate class functions do not throw exceptions.
///
/// `void Invoke(std::shared_ptr<DelegateMsg> msg)` - called by the destination
/// thread to invoke the target function. The destination thread must not call any other
/// delegate instance functions.
/// 
/// Limitations:
/// 
/// * Cannot use rvalue reference (T&&) as a target function argument.
/// 
/// * The target function cannot return a `std::unique_ptr` since `AsyncWait` destination 
/// target thread stores the return value (`m_retVal`) for later use by the calling source thread.
/// 
/// * Cannot insert `DelegateMemberAsyncWait` into an ordered container. e.g. `std::list` ok, 
/// `std::set` not ok.
/// 
/// * `std::function` compares the function signature type, not the underlying object instance.
/// See `DelegateFunction<>` class for more info.
///
/// Code within `<common_code>` and `</common_code>` is updated using sync_src.py. Manually update 
/// the code within the `DelegateFreeAsyncWait` `common_code` tags, then run the script to 
/// propagate to the remaining delegate classes to simplify code maintenance.
/// 
/// `python src_dup.py DelegateAsyncWait.h`

#include "Delegate.h"
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
        m_args(std::forward<Args>(args)...) {}

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
class DelegateFreeAsyncWait<RetType(Args...)> : public DelegateFree<RetType(Args...)>, public IDelegateInvoker {
public:
    typedef RetType(*FreeFunc)(Args...);
    using ClassType = DelegateFreeAsyncWait<RetType(Args...)>;
    using BaseType = DelegateFree<RetType(Args...)>;

    /// @brief Constructor to create a class instance.
    /// @param[in] func The target free function to store.
    /// @param[in] thread The execution thread to invoke `func`.
    /// @param[in] timeout The calling thread timeout for destination thread to
    /// invoke the target function. 
    DelegateFreeAsyncWait(FreeFunc func, DelegateThread& thread, std::chrono::milliseconds timeout = WAIT_INFINITE) :
        BaseType(func), m_thread(&thread), m_timeout(timeout) {
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

    /// @brief Move constructor that transfers ownership of resources.
    /// @param[in] rhs The object to move from.
    DelegateFreeAsyncWait(ClassType&& rhs) noexcept :
        BaseType(rhs), m_thread(rhs.m_thread), m_timeout(rhs.m_timeout), m_success(rhs.m_success), m_retVal(rhs.m_retVal) {
        rhs.Clear();
    }

    DelegateFreeAsyncWait() = default;

    /// @brief Bind a free function to the delegate.
    /// @details This method associates a free function (`func`) with the delegate. 
    /// Once the function is bound, the delegate can be used to invoke the function.
    /// @param[in] func The free function to bind to the delegate. This function must 
    /// match the signature of the delegate.
    /// @param[in] thread The execution thread to invoke `func`.
    /// @param[in] timeout The calling thread timeout for destination thread to
    /// invoke the target function. 
    void Bind(FreeFunc func, DelegateThread& thread, std::chrono::milliseconds timeout = WAIT_INFINITE) {
        m_thread = &thread;
        m_timeout = timeout;
        BaseType::Bind(func);
    }

    // <common_code>

    /// @brief Assigns the state of one object to another.
    /// @details Copy the state from the `rhs` (right-hand side) object to the
    /// current object.
    /// @param[in] rhs The object whose state is to be copied.
    void Assign(const ClassType& rhs) {
        m_thread = rhs.m_thread;
        m_timeout = rhs.m_timeout;
        m_success = rhs.m_success;
        m_retVal = rhs.m_retVal;
        BaseType::Assign(rhs);
    }

    /// @brief Creates a copy of the current object.
    /// @details Clones the current instance of the class by creating a new object
    /// and copying the state of the current object to it. 
    /// @return A pointer to a new `ClassType` instance.
    /// @post The caller is responsible for deleting the clone object.
    /// @throws std::bad_alloc If dynamic memory allocation fails and USE_ASSERTS not defined.
    virtual ClassType* Clone() const override {
        return new(std::nothrow) ClassType(*this);
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

    /// @brief Move assignment operator that transfers ownership of resources.
    /// @param[in] rhs The object to move from.
    /// @return A reference to the current object.
    ClassType& operator=(ClassType&& rhs) noexcept {
        if (&rhs != this) {
            BaseType::operator=(std::move(rhs));
            m_thread = rhs.m_thread;    // Use the resource
            m_timeout = rhs.m_timeout;    
            m_success = rhs.m_success;
            m_retVal = rhs.m_retVal;
        }
        return *this;
    }

    /// @brief Clear the target function.
    virtual void operator=(std::nullptr_t) noexcept override {
        return this->Clear();
    }

    /// @brief Compares two delegate objects for equality.
    /// @param[in] rhs The `DelegateBase` object to compare with the current object.
    /// @return `true` if the two delegate objects are equal, `false` otherwise.
    virtual bool Equal(const DelegateBase& rhs) const override {
        auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
        return derivedRhs &&
            m_thread == derivedRhs->m_thread &&
            m_timeout == derivedRhs->m_timeout &&
            BaseType::Equal(rhs);
    }

    /// Compares two delegate objects for equality.
    /// @return `true` if the objects are equal, `false` otherwise.
    bool operator==(const ClassType& rhs) const noexcept { return Equal(rhs); }

    /// Overload operator== to compare the delegate to nullptr
    /// @return `true` if delegate is null.
    virtual bool operator==(std::nullptr_t) const noexcept override {
        return this->Empty();
    }

    /// Overload operator!= to compare the delegate to nullptr
    /// @return `true` if delegate is not null.
    virtual bool operator!=(std::nullptr_t) const noexcept override {
        return !this->Empty();
    }

    /// Overload operator== to compare the delegate to nullptr
    /// @return `true` if delegate is null.
    friend bool operator==(std::nullptr_t, const ClassType& rhs) noexcept {
        return rhs.Empty();
    }

    /// Overload operator!= to compare the delegate to nullptr
    /// @return `true` if delegate is not null.
    friend bool operator!=(std::nullptr_t, const ClassType& rhs) noexcept {
        return !rhs.Empty();
    }

    /// @brief Invoke delegate function asynchronously and block for function return value.
    /// Called by the source thread.
    /// @details Invoke delegate function asynchronously and wait for the return value.
    /// This function is called by the source thread. Dispatches the delegate data into the 
    /// destination thread message queue. `Invoke()` must be called by the destination 
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
        if (this->Empty())
            return RetType();

        // Synchronously invoke the target function?
        if (m_sync) {
            // Invoke the target function directly
            return BaseType::operator()(std::forward<Args>(args)...);
        } else {
            // Create a clone instance of this delegate 
            auto delegate = std::shared_ptr<ClassType>(Clone());
            if (!delegate)
                BAD_ALLOC();

            // Create a new message instance for sending to the destination thread.
            auto msg = std::make_shared<DelegateAsyncWaitMsg<Args...>>(delegate, std::forward<Args>(args)...);
            if (!msg)
                BAD_ALLOC();
            msg->SetInvokerWaiting(true);

            auto thread = this->GetThread();
            if (thread) {
                // Dispatch message onto the callback destination thread. Invoke()
                // will be called by the destination thread. 
                thread->DispatchDelegate(msg);

                // Wait for destination thread to execute the delegate function and get return value
                if ((m_success = msg->GetSema().Wait(m_timeout)))
                    m_retVal = delegate->m_retVal;
            }

            // Protect data shared between source and destination threads
            const std::lock_guard<std::mutex> lock(msg->GetLock());

            // Set flag that source is not waiting anymore
            msg->SetInvokerWaiting(false);

            // Does the target function have a return value?
            if constexpr (std::is_void<RetType>::value == false) {
                // Is the return value valid? 
                if (m_retVal.has_value()) {
                    // Return the destination thread target function return value
                    return GetRetVal();
                } else {
                    // Return a default return value
                    return RetType{};
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
        if constexpr (std::is_void<RetType>::value == true) {
            operator()(args...);
            return IsSuccess() ? std::optional<bool>(true) : std::optional<bool>();
        } else {
            auto retVal = operator()(args...);
            return IsSuccess() ? std::optional<RetType>(retVal) : std::optional<RetType>();
        }
    }

    /// @brief Invoke the delegate function on the destination thread. Called by the 
    /// destination thread.
    /// @details Each source thread call to `operator()` generate a call to `Invoke()` 
    /// on the destination thread. A lock is used to protect source and destination thread shared 
    /// data. A semaphore is used to signal the source thread when the destination thread 
    /// completes the target function call.
    /// 
    /// If source thread timeout expires and before the destination thread invokes the 
    /// target function, the target function is not called.
    /// @param[in] msg The delegate message created and sent within `operator()(Args... args)`.
    /// @return `true` if target function invoked or timeout expired; `false` if error. 
    virtual bool Invoke(std::shared_ptr<DelegateMsg> msg) override {
        static_assert(!(is_unique_ptr<RetType>::value), "std::unique_ptr return value not allowed");

        // Typecast the base pointer to back correct derived to instance
        auto delegateMsg = std::dynamic_pointer_cast<DelegateAsyncWaitMsg<Args...>>(msg);
        if (delegateMsg == nullptr)
            return false;

        // Protect data shared between source and destination threads
        const std::lock_guard<std::mutex> lock(delegateMsg->GetLock());

        // Is the source thread waiting for the target function invoke to complete?
        if (delegateMsg->GetInvokerWaiting()) {
            // Invoke the delegate function synchronously
            m_sync = true;

            // Does target function have a void return value?
            if constexpr (std::is_void<RetType>::value == true) {
                // Invoke the target function using the source thread supplied function arguments
                std::apply(&BaseType::operator(), std::tuple_cat(std::make_tuple(this), delegateMsg->GetArgs()));
            } else {
                // Invoke the target function using the source thread supplied function arguments 
                // and get the return value
                m_retVal = std::apply(&BaseType::operator(), std::tuple_cat(std::make_tuple(this), delegateMsg->GetArgs()));
            }

            // Signal the source thread that the destination thread function call is complete
            delegateMsg->GetSema().Signal();
        }
        return true;
    }

    /// Returns `true` if asynchronous function successfully invoked on the target thread
    /// @return `true` if the target asynchronous function call succeeded. `false` if 
    /// the timeout expired before the target function could be invoked.
    bool IsSuccess() noexcept { return m_success; }

    /// Get the asynchronous function return value
    /// @return The destination thraed target function return value
    RetType GetRetVal() noexcept {
        try {
            return std::any_cast<RetType>(m_retVal);
        }
        catch (const std::bad_any_cast&) {
            return RetType{};  // Return a default value if error
        }
    }

    ///@brief Get the destination thread that the target function is invoked on.
    // @return The target thread.
    DelegateThread* GetThread() noexcept { return m_thread; }

private:
    /// The target thread to invoke the delegate function.
    DelegateThread* m_thread = nullptr;

    /// Flag to control synchronous vs asynchronous target invoke behavior.
    bool m_sync = false;

    /// Set to `true` if async function call succeeds
    bool m_success = false;			        

    /// Time in mS to wait for async function to invoke
    std::chrono::milliseconds m_timeout = WAIT_INFINITE;    

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
class DelegateMemberAsyncWait<TClass, RetType(Args...)> : public DelegateMember<TClass, RetType(Args...)>, public IDelegateInvoker {
public:
    typedef TClass* ObjectPtr;
    typedef std::shared_ptr<TClass> SharedPtr;
    typedef RetType(TClass::* MemberFunc)(Args...);
    typedef RetType(TClass::* ConstMemberFunc)(Args...) const;
    using ClassType = DelegateMemberAsyncWait<TClass, RetType(Args...)>;
    using BaseType = DelegateMember<TClass, RetType(Args...)>;

    /// @brief Constructor to create a class instance.
    /// @param[in] object The target object pointer to store.
    /// @param[in] func The target member function to store.
    /// @param[in] thread The execution thread to invoke `func`.
    /// @param[in] timeout The calling thread timeout for destination thread to
    /// invoke the target function. 
    DelegateMemberAsyncWait(SharedPtr object, MemberFunc func, DelegateThread& thread, std::chrono::milliseconds timeout = WAIT_INFINITE) :
        BaseType(object, func), m_thread(&thread), m_timeout(timeout) {
        Bind(object, func, thread);
    }

    /// @brief Constructor to create a class instance.
    /// @param[in] object The target object pointer to store.
    /// @param[in] func The target const member function to store.
    /// @param[in] thread The execution thread to invoke `func`.
    /// @param[in] timeout The calling thread timeout for destination thread to
    /// invoke the target function. 
    DelegateMemberAsyncWait(SharedPtr object, ConstMemberFunc func, DelegateThread& thread, std::chrono::milliseconds timeout) :
        BaseType(object, func), m_thread(&thread), m_timeout(timeout) {
        Bind(object, func, thread);
    }

    /// @brief Constructor to create a class instance.
    /// @param[in] object The target object pointer to store.
    /// @param[in] func The target member function to store.
    /// @param[in] thread The execution thread to invoke `func`.
    /// @param[in] timeout The calling thread timeout for destination thread to
    /// invoke the target function. 
    DelegateMemberAsyncWait(ObjectPtr object, MemberFunc func, DelegateThread& thread, std::chrono::milliseconds timeout = WAIT_INFINITE) :
        BaseType(object, func), m_thread(&thread), m_timeout(timeout) {
        Bind(object, func, thread);
    }

    /// @brief Constructor to create a class instance.
    /// @param[in] object The target object pointer to store.
    /// @param[in] func The target const member function to store.
    /// @param[in] thread The execution thread to invoke `func`.
    /// @param[in] timeout The calling thread timeout for destination thread to
    /// invoke the target function. 
    DelegateMemberAsyncWait(ObjectPtr object, ConstMemberFunc func, DelegateThread& thread, std::chrono::milliseconds timeout) :
        BaseType(object, func), m_thread(&thread), m_timeout(timeout) {
        Bind(object, func, thread);
    }

    /// @brief Copy constructor that creates a copy of the given instance.
    /// @details This constructor initializes a new object as a copy of the 
    /// provided `rhs` (right-hand side) object. The `rhs` object is used to 
    /// set the state of the new instance.
    /// @param[in] rhs The object to copy from.
    DelegateMemberAsyncWait(const ClassType& rhs) :
        BaseType(rhs) {
        Assign(rhs);
    }

    /// @brief Move constructor that transfers ownership of resources.
    /// @param[in] rhs The object to move from.
    DelegateMemberAsyncWait(ClassType&& rhs) noexcept :
        BaseType(rhs), m_thread(rhs.m_thread), m_timeout(rhs.m_timeout), m_success(rhs.m_success), m_retVal(rhs.m_retVal) {
        rhs.Clear();
    }

    DelegateMemberAsyncWait() = default;

    /// @brief Bind a const member function to the delegate.
    /// @details This method associates a member function (`func`) with the delegate. 
    /// Once the function is bound, the delegate can be used to invoke the function.
    /// @param[in] object The target object instance.
    /// @param[in] func The function to bind to the delegate. This function must match 
    /// the signature of the delegate.
    /// @param[in] thread The execution thread to invoke `func`.
    /// @param[in] timeout The calling thread timeout for destination thread to
    /// invoke the target function. 
    void Bind(SharedPtr object, MemberFunc func, DelegateThread& thread, std::chrono::milliseconds timeout = WAIT_INFINITE) {
        m_thread = &thread;
        m_timeout = timeout;
        BaseType::Bind(object, func);
    }

    /// @brief Bind a member function to the delegate.
    /// @details This method associates a member function (`func`) with the delegate. 
    /// Once the function is bound, the delegate can be used to invoke the function.
    /// @param[in] object The target object instance.
    /// @param[in] func The member function to bind to the delegate. This function must 
    /// match the signature of the delegate.
    /// @param[in] thread The execution thread to invoke `func`.
    /// @param[in] timeout The calling thread timeout for destination thread to
    /// invoke the target function. 
    void Bind(SharedPtr object, ConstMemberFunc func, DelegateThread& thread, std::chrono::milliseconds timeout = WAIT_INFINITE) {
        m_thread = &thread;
        m_timeout = timeout;
        BaseType::Bind(object, func);
    }

    /// @brief Bind a const member function to the delegate.
    /// @details This method associates a member function (`func`) with the delegate. 
    /// Once the function is bound, the delegate can be used to invoke the function.
    /// @param[in] object The target object instance.
    /// @param[in] func The function to bind to the delegate. This function must match 
    /// the signature of the delegate.
    /// @param[in] thread The execution thread to invoke `func`.
    /// @param[in] timeout The calling thread timeout for destination thread to
    /// invoke the target function. 
    void Bind(ObjectPtr object, MemberFunc func, DelegateThread& thread, std::chrono::milliseconds timeout = WAIT_INFINITE) {
        m_thread = &thread;
        m_timeout = timeout;
        BaseType::Bind(object, func);
    }

    /// @brief Bind a member function to the delegate.
    /// @details This method associates a member function (`func`) with the delegate. 
    /// Once the function is bound, the delegate can be used to invoke the function.
    /// @param[in] object The target object instance.
    /// @param[in] func The member function to bind to the delegate. This function must 
    /// match the signature of the delegate.
    /// @param[in] thread The execution thread to invoke `func`.
    /// @param[in] timeout The calling thread timeout for destination thread to
    /// invoke the target function. 
    void Bind(ObjectPtr object, ConstMemberFunc func, DelegateThread& thread, std::chrono::milliseconds timeout = WAIT_INFINITE) {
        m_thread = &thread;
        m_timeout = timeout;
        BaseType::Bind(object, func);
    }

    // <common_code>

    /// @brief Assigns the state of one object to another.
    /// @details Copy the state from the `rhs` (right-hand side) object to the
    /// current object.
    /// @param[in] rhs The object whose state is to be copied.
    void Assign(const ClassType& rhs) {
        m_thread = rhs.m_thread;
        m_timeout = rhs.m_timeout;
        m_success = rhs.m_success;
        m_retVal = rhs.m_retVal;
        BaseType::Assign(rhs);
    }

    /// @brief Creates a copy of the current object.
    /// @details Clones the current instance of the class by creating a new object
    /// and copying the state of the current object to it. 
    /// @return A pointer to a new `ClassType` instance.
    /// @post The caller is responsible for deleting the clone object.
    /// @throws std::bad_alloc If dynamic memory allocation fails and USE_ASSERTS not defined.
    virtual ClassType* Clone() const override {
        return new(std::nothrow) ClassType(*this);
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

    /// @brief Move assignment operator that transfers ownership of resources.
    /// @param[in] rhs The object to move from.
    /// @return A reference to the current object.
    ClassType& operator=(ClassType&& rhs) noexcept {
        if (&rhs != this) {
            BaseType::operator=(std::move(rhs));
            m_thread = rhs.m_thread;    // Use the resource
            m_timeout = rhs.m_timeout;    
            m_success = rhs.m_success;
            m_retVal = rhs.m_retVal;
        }
        return *this;
    }

    /// @brief Clear the target function.
    virtual void operator=(std::nullptr_t) noexcept override {
        return this->Clear();
    }

    /// @brief Compares two delegate objects for equality.
    /// @param[in] rhs The `DelegateBase` object to compare with the current object.
    /// @return `true` if the two delegate objects are equal, `false` otherwise.
    virtual bool Equal(const DelegateBase& rhs) const override {
        auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
        return derivedRhs &&
            m_thread == derivedRhs->m_thread &&
            m_timeout == derivedRhs->m_timeout &&
            BaseType::Equal(rhs);
    }

    /// Compares two delegate objects for equality.
    /// @return `true` if the objects are equal, `false` otherwise.
    bool operator==(const ClassType& rhs) const noexcept { return Equal(rhs); }

    /// Overload operator== to compare the delegate to nullptr
    /// @return `true` if delegate is null.
    virtual bool operator==(std::nullptr_t) const noexcept override {
        return this->Empty();
    }

    /// Overload operator!= to compare the delegate to nullptr
    /// @return `true` if delegate is not null.
    virtual bool operator!=(std::nullptr_t) const noexcept override {
        return !this->Empty();
    }

    /// Overload operator== to compare the delegate to nullptr
    /// @return `true` if delegate is null.
    friend bool operator==(std::nullptr_t, const ClassType& rhs) noexcept {
        return rhs.Empty();
    }

    /// Overload operator!= to compare the delegate to nullptr
    /// @return `true` if delegate is not null.
    friend bool operator!=(std::nullptr_t, const ClassType& rhs) noexcept {
        return !rhs.Empty();
    }

    /// @brief Invoke delegate function asynchronously and block for function return value.
    /// Called by the source thread.
    /// @details Invoke delegate function asynchronously and wait for the return value.
    /// This function is called by the source thread. Dispatches the delegate data into the 
    /// destination thread message queue. `Invoke()` must be called by the destination 
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
        if (this->Empty())
            return RetType();

        // Synchronously invoke the target function?
        if (m_sync) {
            // Invoke the target function directly
            return BaseType::operator()(std::forward<Args>(args)...);
        } else {
            // Create a clone instance of this delegate 
            auto delegate = std::shared_ptr<ClassType>(Clone());
            if (!delegate)
                BAD_ALLOC();

            // Create a new message instance for sending to the destination thread.
            auto msg = std::make_shared<DelegateAsyncWaitMsg<Args...>>(delegate, std::forward<Args>(args)...);
            if (!msg)
                BAD_ALLOC();
            msg->SetInvokerWaiting(true);

            auto thread = this->GetThread();
            if (thread) {
                // Dispatch message onto the callback destination thread. Invoke()
                // will be called by the destination thread. 
                thread->DispatchDelegate(msg);

                // Wait for destination thread to execute the delegate function and get return value
                if ((m_success = msg->GetSema().Wait(m_timeout)))
                    m_retVal = delegate->m_retVal;
            }

            // Protect data shared between source and destination threads
            const std::lock_guard<std::mutex> lock(msg->GetLock());

            // Set flag that source is not waiting anymore
            msg->SetInvokerWaiting(false);

            // Does the target function have a return value?
            if constexpr (std::is_void<RetType>::value == false) {
                // Is the return value valid? 
                if (m_retVal.has_value()) {
                    // Return the destination thread target function return value
                    return GetRetVal();
                } else {
                    // Return a default return value
                    return RetType{};
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
        if constexpr (std::is_void<RetType>::value == true) {
            operator()(args...);
            return IsSuccess() ? std::optional<bool>(true) : std::optional<bool>();
        } else {
            auto retVal = operator()(args...);
            return IsSuccess() ? std::optional<RetType>(retVal) : std::optional<RetType>();
        }
    }

    /// @brief Invoke the delegate function on the destination thread. Called by the 
    /// destination thread.
    /// @details Each source thread call to `operator()` generate a call to `Invoke()` 
    /// on the destination thread. A lock is used to protect source and destination thread shared 
    /// data. A semaphore is used to signal the source thread when the destination thread 
    /// completes the target function call.
    /// 
    /// If source thread timeout expires and before the destination thread invokes the 
    /// target function, the target function is not called.
    /// @param[in] msg The delegate message created and sent within `operator()(Args... args)`.
    /// @return `true` if target function invoked or timeout expired; `false` if error. 
    virtual bool Invoke(std::shared_ptr<DelegateMsg> msg) override {
        static_assert(!(is_unique_ptr<RetType>::value), "std::unique_ptr return value not allowed");

        // Typecast the base pointer to back correct derived to instance
        auto delegateMsg = std::dynamic_pointer_cast<DelegateAsyncWaitMsg<Args...>>(msg);
        if (delegateMsg == nullptr)
            return false;

        // Protect data shared between source and destination threads
        const std::lock_guard<std::mutex> lock(delegateMsg->GetLock());

        // Is the source thread waiting for the target function invoke to complete?
        if (delegateMsg->GetInvokerWaiting()) {
            // Invoke the delegate function synchronously
            m_sync = true;

            // Does target function have a void return value?
            if constexpr (std::is_void<RetType>::value == true) {
                // Invoke the target function using the source thread supplied function arguments
                std::apply(&BaseType::operator(), std::tuple_cat(std::make_tuple(this), delegateMsg->GetArgs()));
            } else {
                // Invoke the target function using the source thread supplied function arguments 
                // and get the return value
                m_retVal = std::apply(&BaseType::operator(), std::tuple_cat(std::make_tuple(this), delegateMsg->GetArgs()));
            }

            // Signal the source thread that the destination thread function call is complete
            delegateMsg->GetSema().Signal();
        }
        return true;
    }

    /// Returns `true` if asynchronous function successfully invoked on the target thread
    /// @return `true` if the target asynchronous function call succeeded. `false` if 
    /// the timeout expired before the target function could be invoked.
    bool IsSuccess() noexcept { return m_success; }

    /// Get the asynchronous function return value
    /// @return The destination thraed target function return value
    RetType GetRetVal() noexcept {
        try {
            return std::any_cast<RetType>(m_retVal);
        }
        catch (const std::bad_any_cast&) {
            return RetType{};  // Return a default value if error
        }
    }

    ///@brief Get the destination thread that the target function is invoked on.
    // @return The target thread.
    DelegateThread* GetThread() noexcept { return m_thread; }

private:
    /// The target thread to invoke the delegate function.
    DelegateThread* m_thread = nullptr;

    /// Flag to control synchronous vs asynchronous target invoke behavior.
    bool m_sync = false;

    /// Set to `true` if async function call succeeds
    bool m_success = false;			        

    /// Time in mS to wait for async function to invoke
    std::chrono::milliseconds m_timeout = WAIT_INFINITE;    

    /// Return value of the target invoked function
    std::any m_retVal;                      

    // </common_code>
};

template <class R>
struct DelegateFunctionAsyncWait; // Not defined

/// @brief `DelegateFunctionAsyncWait<>` class asynchronously block invokes a std::function target function.
/// 
/// See `DelegateFunction<>` base class for important usage limitations.
/// 
/// @tparam RetType The return type of the bound delegate function.
/// @tparam Args The argument types of the bound delegate function.
template <class RetType, class... Args>
class DelegateFunctionAsyncWait<RetType(Args...)> : public DelegateFunction<RetType(Args...)>, public IDelegateInvoker {
public:
    using FunctionType = std::function<RetType(Args...)>;
    using ClassType = DelegateFunctionAsyncWait<RetType(Args...)>;
    using BaseType = DelegateFunction<RetType(Args...)>;

    /// @brief Constructor to create a class instance.
    /// @param[in] func The target `std::function` to store.
    /// @param[in] thread The execution thread to invoke `func`.
    /// @param[in] timeout The calling thread timeout for destination thread to
    /// invoke the target function. 
    DelegateFunctionAsyncWait(FunctionType func, DelegateThread& thread, std::chrono::milliseconds timeout = WAIT_INFINITE) :
        BaseType(func), m_thread(&thread), m_timeout(timeout) {
        Bind(func, thread);
    }

    /// @brief Copy constructor that creates a copy of the given instance.
    /// @details This constructor initializes a new object as a copy of the 
    /// provided `rhs` (right-hand side) object. The `rhs` object is used to 
    /// set the state of the new instance.
    /// @param[in] rhs The object to copy from.
    DelegateFunctionAsyncWait(const ClassType& rhs) :
        BaseType(rhs) {
        Assign(rhs);
    }

    /// @brief Move constructor that transfers ownership of resources.
    /// @param[in] rhs The object to move from.
    DelegateFunctionAsyncWait(ClassType&& rhs) noexcept :
        BaseType(rhs), m_thread(rhs.m_thread), m_timeout(rhs.m_timeout), m_success(rhs.m_success), m_retVal(rhs.m_retVal) {
        rhs.Clear();
    }

    DelegateFunctionAsyncWait() = default;

    /// @brief Bind a `std::function` to the delegate.
    /// @details This method associates a member function (`func`) with the delegate. 
    /// Once the function is bound, the delegate can be used to invoke the function.
    /// @param[in] func The `std::function` to bind to the delegate. This function must match 
    /// the signature of the delegate.
    /// @param[in] thread The execution thread to invoke `func`.
    /// @param[in] timeout The calling thread timeout for destination thread to
    /// invoke the target function. 
    void Bind(FunctionType func, DelegateThread& thread, std::chrono::milliseconds timeout = WAIT_INFINITE) {
        m_thread = &thread;
        m_timeout = timeout;
        BaseType::Bind(func);
    }

    // <common_code>

    /// @brief Assigns the state of one object to another.
    /// @details Copy the state from the `rhs` (right-hand side) object to the
    /// current object.
    /// @param[in] rhs The object whose state is to be copied.
    void Assign(const ClassType& rhs) {
        m_thread = rhs.m_thread;
        m_timeout = rhs.m_timeout;
        m_success = rhs.m_success;
        m_retVal = rhs.m_retVal;
        BaseType::Assign(rhs);
    }

    /// @brief Creates a copy of the current object.
    /// @details Clones the current instance of the class by creating a new object
    /// and copying the state of the current object to it. 
    /// @return A pointer to a new `ClassType` instance.
    /// @post The caller is responsible for deleting the clone object.
    /// @throws std::bad_alloc If dynamic memory allocation fails and USE_ASSERTS not defined.
    virtual ClassType* Clone() const override {
        return new(std::nothrow) ClassType(*this);
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

    /// @brief Move assignment operator that transfers ownership of resources.
    /// @param[in] rhs The object to move from.
    /// @return A reference to the current object.
    ClassType& operator=(ClassType&& rhs) noexcept {
        if (&rhs != this) {
            BaseType::operator=(std::move(rhs));
            m_thread = rhs.m_thread;    // Use the resource
            m_timeout = rhs.m_timeout;    
            m_success = rhs.m_success;
            m_retVal = rhs.m_retVal;
        }
        return *this;
    }

    /// @brief Clear the target function.
    virtual void operator=(std::nullptr_t) noexcept override {
        return this->Clear();
    }

    /// @brief Compares two delegate objects for equality.
    /// @param[in] rhs The `DelegateBase` object to compare with the current object.
    /// @return `true` if the two delegate objects are equal, `false` otherwise.
    virtual bool Equal(const DelegateBase& rhs) const override {
        auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
        return derivedRhs &&
            m_thread == derivedRhs->m_thread &&
            m_timeout == derivedRhs->m_timeout &&
            BaseType::Equal(rhs);
    }

    /// Compares two delegate objects for equality.
    /// @return `true` if the objects are equal, `false` otherwise.
    bool operator==(const ClassType& rhs) const noexcept { return Equal(rhs); }

    /// Overload operator== to compare the delegate to nullptr
    /// @return `true` if delegate is null.
    virtual bool operator==(std::nullptr_t) const noexcept override {
        return this->Empty();
    }

    /// Overload operator!= to compare the delegate to nullptr
    /// @return `true` if delegate is not null.
    virtual bool operator!=(std::nullptr_t) const noexcept override {
        return !this->Empty();
    }

    /// Overload operator== to compare the delegate to nullptr
    /// @return `true` if delegate is null.
    friend bool operator==(std::nullptr_t, const ClassType& rhs) noexcept {
        return rhs.Empty();
    }

    /// Overload operator!= to compare the delegate to nullptr
    /// @return `true` if delegate is not null.
    friend bool operator!=(std::nullptr_t, const ClassType& rhs) noexcept {
        return !rhs.Empty();
    }

    /// @brief Invoke delegate function asynchronously and block for function return value.
    /// Called by the source thread.
    /// @details Invoke delegate function asynchronously and wait for the return value.
    /// This function is called by the source thread. Dispatches the delegate data into the 
    /// destination thread message queue. `Invoke()` must be called by the destination 
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
        if (this->Empty())
            return RetType();

        // Synchronously invoke the target function?
        if (m_sync) {
            // Invoke the target function directly
            return BaseType::operator()(std::forward<Args>(args)...);
        } else {
            // Create a clone instance of this delegate 
            auto delegate = std::shared_ptr<ClassType>(Clone());
            if (!delegate)
                BAD_ALLOC();

            // Create a new message instance for sending to the destination thread.
            auto msg = std::make_shared<DelegateAsyncWaitMsg<Args...>>(delegate, std::forward<Args>(args)...);
            if (!msg)
                BAD_ALLOC();
            msg->SetInvokerWaiting(true);

            auto thread = this->GetThread();
            if (thread) {
                // Dispatch message onto the callback destination thread. Invoke()
                // will be called by the destination thread. 
                thread->DispatchDelegate(msg);

                // Wait for destination thread to execute the delegate function and get return value
                if ((m_success = msg->GetSema().Wait(m_timeout)))
                    m_retVal = delegate->m_retVal;
            }

            // Protect data shared between source and destination threads
            const std::lock_guard<std::mutex> lock(msg->GetLock());

            // Set flag that source is not waiting anymore
            msg->SetInvokerWaiting(false);

            // Does the target function have a return value?
            if constexpr (std::is_void<RetType>::value == false) {
                // Is the return value valid? 
                if (m_retVal.has_value()) {
                    // Return the destination thread target function return value
                    return GetRetVal();
                } else {
                    // Return a default return value
                    return RetType{};
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
        if constexpr (std::is_void<RetType>::value == true) {
            operator()(args...);
            return IsSuccess() ? std::optional<bool>(true) : std::optional<bool>();
        } else {
            auto retVal = operator()(args...);
            return IsSuccess() ? std::optional<RetType>(retVal) : std::optional<RetType>();
        }
    }

    /// @brief Invoke the delegate function on the destination thread. Called by the 
    /// destination thread.
    /// @details Each source thread call to `operator()` generate a call to `Invoke()` 
    /// on the destination thread. A lock is used to protect source and destination thread shared 
    /// data. A semaphore is used to signal the source thread when the destination thread 
    /// completes the target function call.
    /// 
    /// If source thread timeout expires and before the destination thread invokes the 
    /// target function, the target function is not called.
    /// @param[in] msg The delegate message created and sent within `operator()(Args... args)`.
    /// @return `true` if target function invoked or timeout expired; `false` if error. 
    virtual bool Invoke(std::shared_ptr<DelegateMsg> msg) override {
        static_assert(!(is_unique_ptr<RetType>::value), "std::unique_ptr return value not allowed");

        // Typecast the base pointer to back correct derived to instance
        auto delegateMsg = std::dynamic_pointer_cast<DelegateAsyncWaitMsg<Args...>>(msg);
        if (delegateMsg == nullptr)
            return false;

        // Protect data shared between source and destination threads
        const std::lock_guard<std::mutex> lock(delegateMsg->GetLock());

        // Is the source thread waiting for the target function invoke to complete?
        if (delegateMsg->GetInvokerWaiting()) {
            // Invoke the delegate function synchronously
            m_sync = true;

            // Does target function have a void return value?
            if constexpr (std::is_void<RetType>::value == true) {
                // Invoke the target function using the source thread supplied function arguments
                std::apply(&BaseType::operator(), std::tuple_cat(std::make_tuple(this), delegateMsg->GetArgs()));
            } else {
                // Invoke the target function using the source thread supplied function arguments 
                // and get the return value
                m_retVal = std::apply(&BaseType::operator(), std::tuple_cat(std::make_tuple(this), delegateMsg->GetArgs()));
            }

            // Signal the source thread that the destination thread function call is complete
            delegateMsg->GetSema().Signal();
        }
        return true;
    }

    /// Returns `true` if asynchronous function successfully invoked on the target thread
    /// @return `true` if the target asynchronous function call succeeded. `false` if 
    /// the timeout expired before the target function could be invoked.
    bool IsSuccess() noexcept { return m_success; }

    /// Get the asynchronous function return value
    /// @return The destination thraed target function return value
    RetType GetRetVal() noexcept {
        try {
            return std::any_cast<RetType>(m_retVal);
        }
        catch (const std::bad_any_cast&) {
            return RetType{};  // Return a default value if error
        }
    }

    ///@brief Get the destination thread that the target function is invoked on.
    // @return The target thread.
    DelegateThread* GetThread() noexcept { return m_thread; }

private:
    /// The target thread to invoke the delegate function.
    DelegateThread* m_thread = nullptr;

    /// Flag to control synchronous vs asynchronous target invoke behavior.
    bool m_sync = false;

    /// Set to `true` if async function call succeeds
    bool m_success = false;			        

    /// Time in mS to wait for async function to invoke
    std::chrono::milliseconds m_timeout = WAIT_INFINITE;    

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
auto MakeDelegate(RetType(*func)(Args... args), DelegateThread& thread, std::chrono::milliseconds timeout) {
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
auto MakeDelegate(TClass* object, RetType(TClass::*func)(Args... args), DelegateThread& thread, std::chrono::milliseconds timeout) {
    return DelegateMemberAsyncWait<TClass, RetType(Args...)>(object, func, thread, timeout);
}

/// @brief Creates an asynchronous delegate that binds to a const member function with a wait and timeout.
/// @tparam TClass The class type that contains the const member function.
/// @tparam RetType The return type of the member function.
/// @tparam Args The types of the function arguments.
/// @param[in] object A pointer to the instance of `TClass` that will be used for the delegate.
/// @param[in] func A pointer to the const member function of `TClass` to bind to the delegate.
/// @param[in] thread The `DelegateThread` on which the function will be invoked asynchronously.
/// @param[in] timeout The duration to wait for the function to complete before returning.
/// @return A `DelegateMemberAsyncWait` object bound to the specified const member function, thread, and timeout.
template <class TClass, class RetType, class... Args>
auto MakeDelegate(TClass* object, RetType(TClass::*func)(Args... args) const, DelegateThread& thread, std::chrono::milliseconds timeout) {
    return DelegateMemberAsyncWait<TClass, RetType(Args...)>(object, func, thread, timeout);
}

/// @brief Creates a delegate that binds to a const member function.
/// @tparam TClass The const class type that contains the const member function.
/// @tparam RetType The return type of the member function.
/// @tparam Args The types of the function arguments.
/// @param[in] object A pointer to the instance of `TClass` that will be used for the delegate.
/// @param[in] func A pointer to the non-const member function of `TClass` to bind to the delegate.
/// @param[in] thread The `DelegateThread` on which the function will be invoked asynchronously.
/// @param[in] timeout The duration to wait for the function to complete before returning.
/// @return A `DelegateMemberAsyncWait` object bound to the specified non-const member function.
template <class TClass, class RetType, class... Args>
auto MakeDelegate(const TClass* object, RetType(TClass::* func)(Args... args) const, DelegateThread& thread, std::chrono::milliseconds timeout) {
    return DelegateMemberAsyncWait<const TClass, RetType(Args...)>(object, func, thread, timeout);
}


/// @brief Creates an asynchronous delegate that binds to a non-const member function using a shared pointer, with a wait and timeout.
/// @tparam TClass The class type that contains the member function.
/// @tparam RetVal The return type of the member function.
/// @tparam Args The types of the function arguments.
/// @param[in] object A shared pointer to the instance of `TClass` that will be used for the delegate.
/// @param[in] func A pointer to the non-const member function of `TClass` to bind to the delegate.
/// @param[in] thread The `DelegateThread` on which the function will be invoked asynchronously.
/// @param[in] timeout The duration to wait for the function to complete before returning.
/// @return A `DelegateMemberAsyncWait` shared pointer bound to the specified non-const member function, thread, and timeout.
template <class TClass, class RetVal, class... Args>
auto MakeDelegate(std::shared_ptr<TClass> object, RetVal(TClass::* func)(Args... args), DelegateThread& thread, std::chrono::milliseconds timeout) {
    return DelegateMemberAsyncWait<TClass, RetVal(Args...)>(object, func, thread, timeout);
}

/// @brief Creates an asynchronous delegate that binds to a const member function using a shared pointer, with a wait and timeout.
/// @tparam TClass The class type that contains the member function.
/// @tparam RetVal The return type of the member function.
/// @tparam Args The types of the function arguments.
/// @param[in] object A shared pointer to the instance of `TClass` that will be used for the delegate.
/// @param[in] func A pointer to the const member function of `TClass` to bind to the delegate.
/// @param[in] thread The `DelegateThread` on which the function will be invoked asynchronously.
/// @param[in] timeout The duration to wait for the function to complete before returning.
/// @return A `DelegateMemberAsyncWait` shared pointer bound to the specified const member function, thread, and timeout.
template <class TClass, class RetVal, class... Args>
auto MakeDelegate(std::shared_ptr<TClass> object, RetVal(TClass::* func)(Args... args) const, DelegateThread& thread, std::chrono::milliseconds timeout) {
    return DelegateMemberAsyncWait<TClass, RetVal(Args...)>(object, func, thread, timeout);
}

/// @brief Creates an asynchronous delegate that binds to a `std::function` with a wait and timeout.
/// @tparam RetType The return type of the `std::function`.
/// @tparam Args The types of the function arguments.
/// @param[in] func The `std::function` to bind to the delegate.
/// @param[in] thread The `DelegateThread` on which the function will be invoked asynchronously.
/// @param[in] timeout The duration to wait for the function to complete before returning.
/// @return A `DelegateFunctionAsyncWait` object bound to the specified `std::function`, thread, and timeout.
template <class RetType, class... Args>
auto MakeDelegate(std::function<RetType(Args...)> func, DelegateThread& thread, std::chrono::milliseconds timeout) {
    return DelegateFunctionAsyncWait<RetType(Args...)>(func, thread, timeout);
}

} 

#endif
