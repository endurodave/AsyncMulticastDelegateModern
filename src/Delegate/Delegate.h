#ifndef _DELEGATE_H
#define _DELEGATE_H

// Delegate.h
// @see https://github.com/endurodave/AsyncMulticastDelegateModern
// David Lafreniere, Aug 2020.

/// @file
/// @brief Delegate series of classes are used to invoke a function synchronously. 
/// @details Delgates support binding to free functions, class instance functions, class 
/// static function, and std::function targets. Lambda functions can be bound to a delegate 
/// when assigned to a `std::function`. The classes are not thread safe.

#include <functional>
#include <memory>
#include "DelegateOpt.h"

namespace DelegateLib {

/// @brief Non-template base class for all delegates.
class DelegateBase {
public:
    DelegateBase() = default;
    virtual ~DelegateBase() noexcept = default;

    /// @brief Compares two delegate objects for equality.
    /// @param rhs The delegate object to compare with the current object.
    /// @return `true` if the objects are equal, `false` otherwise.
    virtual bool operator==(const DelegateBase& rhs) const = 0;

    /// @brief Compares two delegate objects for inequality.
    /// @param rhs The delegate object to compare with the current object.
    /// @return `true` if the objects are not equal, `false` otherwise.
    virtual bool operator!=(const DelegateBase& rhs) const { return !(*this == rhs); }

    /// @brief Clone a delegate instance.
    /// @details Use Clone() to provide a deep copy using a base pointer. Covariant 
    /// overloading is used so that a Clone() method return type is a more 
    /// specific type in the derived class implementations.
    /// @return A dynamic copy of this instance created with `operator new()`. 
    /// @post The caller is responsible for deleting the clone instance. 
    virtual DelegateBase* Clone() const = 0;

    // Optional fixed block allocator for delegates created on the heap 
    // using operator new(). See USE_ALLOCATOR in DelegateOpt.h and 
    // ENABLE_ALLOCATOR in CMakeLists.txt.
    XALLOCATOR
};

template <class R>
struct Delegate; // Not defined

/// @brief Template base class for all delegates.
/// @tparam RetType The return type of the bound delegate function.
/// @tparam Args The argument types of the bound delegate function.
template <class RetType, class... Args>
class Delegate<RetType(Args...)> : public DelegateBase {
public:
    /// @brief Invoke the bound callable function stored within the 
    /// delegate instance.
    /// @param[in] args The bound function argument(s), if any.
    /// @return The bound function return value, if any.
    virtual RetType operator()(Args... args) = 0;

    /// @brief Clone an instance of a Delegate instance.
    /// @return A new Delegate instance created on the heap. 
    /// @post The caller is responsible for deleting the instance.
    virtual Delegate* Clone() const = 0;
};

template <class R>
struct DelegateFree; // Not defined

/// @brief `DelegateFree<>` class synchronously invokes a free target function.
/// @tparam RetType The return type of the bound delegate function.
/// @tparam Args The argument types of the bound delegate function.
template <class RetType, class... Args>
class DelegateFree<RetType(Args...)> : public Delegate<RetType(Args...)> {
public:
    typedef RetType(*FreeFunc)(Args...);
    using ClassType = DelegateFree<RetType(Args...)>;

    /// @brief Constructor to create a class instance.
    /// @param[in] func The target free function to store.
    DelegateFree(FreeFunc func) { Bind(func); }

    /// @brief Copy constructor that creates a copy of the given instance.
    /// @details This constructor initializes a new object as a copy of the 
    /// provided `rhs` (right-hand side) object. The `rhs` object is used to 
    /// set the state of the new instance.
    /// @param[in] rhs The object to copy from.
    DelegateFree(const ClassType& rhs) { Assign(rhs); }

    /// @brief Default constructor creates an empty delegate.
    DelegateFree() = default;

    /// @brief Bind a free function to the delegate.
    /// @details This method associates a free function (`func`) with the delegate. 
    /// Once the function is bound, the delegate can be used to invoke the function.
    /// @param[in] func The free function to bind to the delegate. This function must 
    /// match the signature of the delegate.
    void Bind(FreeFunc func) { m_func = func; }

    /// @brief Creates a copy of the current object.
    /// @details Clones the current instance of the class by creating a new object
    /// and copying the state of the current object to it. 
    /// @return A pointer to a new `ClassType` instance.
    /// @post The caller is responsible for deleting the clone object.
    virtual ClassType* Clone() const override { 
        return new ClassType(*this); 
    }

    /// @brief Assigns the state of one object to another.
    /// @details Copy the state from the `rhs` (right-hand side) object to the
    /// current object.
    /// @param[in] rhs The object whose state is to be copied.
    void Assign(const ClassType& rhs) { 
        m_func = rhs.m_func; 
    }

    /// @brief Invoke the bound delegate function synchronously. 
    /// @param[in] args - the function arguments, if any.
    /// @return The bound function return value, if any.
    virtual RetType operator()(Args... args) override {
        return std::invoke(m_func, args...);
    }

    /// @brief Assignment operator that assigns the state of one object to another.
    /// @param[in] rhs The object whose state is to be assigned to the current object.
    /// @return A reference to the current object.
    ClassType& operator=(const ClassType& rhs) {
        if (&rhs != this) {
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
            m_func == derivedRhs->m_func;
    }

    /// @brief Check if the delegate is bound to a target function.
    /// @return `true` if the delegate has a target function, `false` otherwise.
    bool Empty() const { return !m_func; }

    /// @brief Clear the target function.
    /// @post The delegate is empty.
    void Clear() { m_func = nullptr; }

    /// @brief Implicit conversion operator to `bool`.
    /// @return `true` if the object is not empty, `false` if the object is empty.
    explicit operator bool() const { return !Empty(); }

private:
    /// @brief Pointer to a free function, representing the bound target function.
    FreeFunc m_func = nullptr;		
};

template <class C, class R>
struct DelegateMember; // Not defined

/// @brief `DelegateMember<>` class synchronously invokes a class member target 
/// function using a class object pointer.
/// @tparam TClass The class type that contains the member function.
/// @tparam RetType The return type of the bound delegate function.
/// @tparam Args The argument types of the bound delegate function.
template <class TClass, class RetType, class... Args>
class DelegateMember<TClass, RetType(Args...)> : public Delegate<RetType(Args...)> {
public:
    typedef TClass* ObjectPtr;
    typedef RetType(TClass::*MemberFunc)(Args...);
    typedef RetType(TClass::*ConstMemberFunc)(Args...) const;
    using ClassType = DelegateMember<TClass, RetType(Args...)>;

    /// @brief Constructor to create a class instance.
    /// @param[in] object The target object pointer to store.
    /// @param[in] func The target member function to store.
    DelegateMember(ObjectPtr object, MemberFunc func) { Bind(object, func); }

    /// @brief Constructor to create a class instance.
    /// @param[in] object The target object pointer to store.
    /// @param[in] func The target const member function to store.
    DelegateMember(ObjectPtr object, ConstMemberFunc func) { Bind(object, func); }

    /// @brief Creates a copy of the current object.
    /// @details Clones the current instance of the class by creating a new object
    /// and copying the state of the current object to it. 
    /// @return A pointer to a new `ClassType` instance.
    /// @post The caller is responsible for deleting the clone object.
    DelegateMember(const ClassType& rhs) { Assign(rhs); }

    /// @brief Default constructor creates an empty delegate.
    DelegateMember() = default;

    /// @brief Bind a member function to the delegate.
    /// @details This method associates a member function (`func`) with the delegate. 
    /// Once the function is bound, the delegate can be used to invoke the function.
    /// @param[in] object The target object instance.
    /// @param[in] func The member function to bind to the delegate. This function must 
    /// match the signature of the delegate.
    void Bind(ObjectPtr object, MemberFunc func) {
        m_object = object;
        m_func = func;
    }

    /// @brief Bind a const member function to the delegate.
    /// @details This method associates a member function (`func`) with the delegate. 
    /// Once the function is bound, the delegate can be used to invoke the function.
    /// @param[in] object The target object instance.
    /// @param[in] func The function to bind to the delegate. The member function to 
    /// bind to the delegate. This function must match the signature of the delegate.
    void Bind(ObjectPtr object, ConstMemberFunc func) {
        m_object = object;
        m_func = reinterpret_cast<MemberFunc>(func);
    }

    /// @brief Creates a copy of the current object.
    /// @details Clones the current instance of the class by creating a new object
    /// and copying the state of the current object to it. 
    /// @return A pointer to a new `ClassType` instance.
    /// @post The caller is responsible for deleting the clone object.
    virtual ClassType* Clone() const override {
        return new ClassType(*this); 
    }

    /// @brief Assigns the state of one object to another.
    /// @details Copy the state from the `rhs` (right-hand side) object to the
    /// current object.
    /// @param[in] rhs The object whose state is to be copied.
    void Assign(const ClassType& rhs) {
        m_object = rhs.m_object;
        m_func = rhs.m_func;
    }

    /// @brief Invoke the bound delegate function synchronously. 
    /// @param[in] args - the function arguments, if any.
    /// @return The bound function return value, if any.
    virtual RetType operator()(Args... args) override {
        return std::invoke(m_func, m_object, args...);
    }

    /// @brief Assignment operator that assigns the state of one object to another.
    /// @param[in] rhs The object whose state is to be assigned to the current object.
    /// @return A reference to the current object.
    ClassType& operator=(const ClassType& rhs) {
        if (&rhs != this) {
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
            m_func == derivedRhs->m_func &&
            m_object == derivedRhs->m_object;
    }

    /// @brief Check if the delegate is bound to a target function.
    /// @return `true` if the delegate has a target function, `false` otherwise.
    bool Empty() const { return !(m_object && m_func); }

    /// @brief Clear the target function.
    /// @post The delegate is empty.
    void Clear() { m_object = nullptr; m_func = nullptr; }

    /// @brief Implicit conversion operator to `bool`.
    /// @return `true` if the object is not empty, `false` if the object is empty.
    explicit operator bool() const { return !Empty(); }

private:
    /// Pointer to a class object, representing the bound target instance.
    ObjectPtr m_object = nullptr;

    /// Pointer to a member function, representing the bound target function.
    MemberFunc m_func = nullptr;
};

template <class C, class R>
struct DelegateMemberSp; // Not defined

/// @brief `DelegateMemberSp<>` class synchronously invokes a class member target 
/// function using a `std::shared_ptr` object.
/// @tparam TClass The class type that contains the member function.
/// @tparam RetType The return type of the bound delegate function.
/// @tparam Args The argument types of the bound delegate function.
template <class TClass, class RetType, class... Args>
class DelegateMemberSp<TClass, RetType(Args...)> : public Delegate<RetType(Args...)> {
public:
    typedef std::shared_ptr<TClass> ObjectPtr;
    typedef RetType(TClass::* MemberFunc)(Args...);
    typedef RetType(TClass::* ConstMemberFunc)(Args...) const;
    using ClassType = DelegateMemberSp<TClass, RetType(Args...)>;

    /// @brief Constructor to create a class instance.
    /// @param[in] object The target object shared pointer to store.
    /// @param[in] func The target member function to store.
    DelegateMemberSp(ObjectPtr object, MemberFunc func) { Bind(object, func); }

    /// @brief Constructor to create a class instance.
    /// @param[in] object The target shared object pointer to store.
    /// @param[in] func The target const member function to store.
    DelegateMemberSp(ObjectPtr object, ConstMemberFunc func) { Bind(object, func); }

    /// @brief Creates a copy of the current object.
    /// @details Clones the current instance of the class by creating a new object
    /// and copying the state of the current object to it. 
    /// @return A pointer to a new `ClassType` instance.
    /// @post The caller is responsible for deleting the clone object.
    DelegateMemberSp(const ClassType& rhs) { Assign(rhs); }

    /// @brief Default constructor creates an empty delegate.
    DelegateMemberSp() = default;

    /// @brief Bind a member function to the delegate.
    /// @details This method associates a member function (`func`) with the delegate. 
    /// Once the function is bound, the delegate can be used to invoke the function.
    /// @param[in] object The target shared object instance.
    /// @param[in] func The function to bind to the delegate. The member function to 
    /// bind to the delegate. This function must match the signature of the delegate.
    void Bind(ObjectPtr object, MemberFunc func) {
        m_object = object;
        m_func = func;
    }

    /// @brief Bind a const member function to the delegate.
    /// @details This method associates a member function (`func`) with the delegate. 
    /// Once the function is bound, the delegate can be used to invoke the function.
    /// @param[in] object The target shared object instance.
    /// @param[in] func The function to bind to the delegate. The function must match 
    /// the signature of the delegate.
    void Bind(ObjectPtr object, ConstMemberFunc func) {
        m_object = object;
        m_func = reinterpret_cast<MemberFunc>(func);
    }

    /// @brief Creates a copy of the current object.
    /// @details Clones the current instance of the class by creating a new object
    /// and copying the state of the current object to it. 
    /// @return A pointer to a new `ClassType` instance.
    /// @post The caller is responsible for deleting the clone object.
    virtual ClassType* Clone() const override { 
        return new ClassType(*this); 
    }

    /// @brief Assigns the state of one object to another.
    /// @details Copy the state from the `rhs` (right-hand side) object to the
    /// current object.
    /// @param[in] rhs The object whose state is to be copied.
    void Assign(const ClassType& rhs) {
        m_object = rhs.m_object;
        m_func = rhs.m_func;
    }

    /// @brief Invoke the bound delegate function synchronously. 
    /// @param[in] args - the function arguments, if any.
    /// @return The bound function return value, if any.
    virtual RetType operator()(Args... args) override {
        return std::invoke(m_func, m_object, args...);
    }

    /// @brief Assignment operator that assigns the state of one object to another.
    /// @param[in] rhs The object whose state is to be assigned to the current object.
    /// @return A reference to the current object.
    ClassType& operator=(const ClassType& rhs) {
        if (&rhs != this) {
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
            m_func == derivedRhs->m_func &&
            m_object == derivedRhs->m_object;
    }

    /// @brief Check if the delegate is bound to a target function.
    /// @return `true` if the delegate has a target function, `false` otherwise.
    bool Empty() const { return !(m_object && m_func); }

    /// @brief Clear the target function.
    /// @post The delegate is empty.
    void Clear() { m_object = nullptr; m_func = nullptr; }

    /// @brief Implicit conversion operator to `bool`.
    /// @return `true` if the object is not empty, `false` if the object is empty.
    explicit operator bool() const { return !Empty(); }

private:
    /// Shared pointer to a class object, representing the bound target instance.
    ObjectPtr m_object = nullptr;

    /// Pointer to a member function, representing the bound target function.
    MemberFunc m_func = nullptr;
};

template <class R>
class DelegateFunction; // Not defined

/// @brief `DelegateFunction<>` class synchronously invokes a `std::function` target function.
/// @tparam RetType The return type of the bound delegate function.
/// @tparam Args The argument types of the bound delegate function.
template <class RetType, class... Args>
class DelegateFunction<RetType(Args...)> : public Delegate<RetType(Args...)> {
public:
    using FunctionType = std::function<RetType(Args...)>;
    using ClassType = DelegateFunction<RetType(Args...)>;

    /// @brief Constructor to create a class instance.
    /// @param[in] func The target `std::function` to store.
    DelegateFunction(FunctionType func) { Bind(func); }

    /// @brief Creates a copy of the current object.
    /// @details Clones the current instance of the class by creating a new object
    /// and copying the state of the current object to it. 
    /// @return A pointer to a new `ClassType` instance.
    /// @post The caller is responsible for deleting the clone object.
    DelegateFunction(const ClassType& rhs) { Assign(rhs); }

    /// @brief Default constructor creates an empty delegate.
    DelegateFunction() = default;

    /// @brief Bind a member function to the delegate.
    /// @details This method associates a member function (`func`) with the delegate. 
    /// Once the function is bound, the delegate can be used to invoke the function.
    /// @param[in] func The `std::function` to bind to the delegate. This function must 
    /// match the signature of the delegate.
    void Bind(FunctionType func) {
        m_func = func;
    }

    /// @brief Creates a copy of the current object.
    /// @details Clones the current instance of the class by creating a new object
    /// and copying the state of the current object to it. 
    /// @return A pointer to a new `ClassType` instance.
    /// @post The caller is responsible for deleting the clone object.
    virtual ClassType* Clone() const override { 
        return new ClassType(*this); 
    }

    /// @brief Assigns the state of one object to another.
    /// @details Copy the state from the `rhs` (right-hand side) object to the
    /// current object.
    /// @param[in] rhs The object whose state is to be copied.
    void Assign(const ClassType& rhs) {
        m_func = rhs.m_func;
    }

    /// @brief Invoke the bound delegate function synchronously. 
    /// @param[in] args - the function arguments, if any.
    /// @return The bound function return value, if any.
    virtual RetType operator()(Args... args) override {
        return m_func(std::forward<Args>(args)...);
    }

    /// @brief Assignment operator that assigns the state of one object to another.
    /// @param[in] rhs The object whose state is to be assigned to the current object.
    /// @return A reference to the current object.
    ClassType& operator=(const ClassType& rhs) {
        if (&rhs != this) {
            Assign(rhs);
        }
        return *this;
    }

    /// @brief Compares two delegate objects for equality.
    /// @param[in] rhs The `DelegateBase` object to compare with the current object.
    /// @return `true` if the two delegate objects are equal, `false` otherwise.
    virtual bool operator==(const DelegateBase& rhs) const override {
        auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
        if (derivedRhs) {
            // If both delegates are empty, they are equal
            if (Empty() && derivedRhs->Empty())
                return true;

            if (m_func && derivedRhs->m_func)
                return m_func.target_type() == derivedRhs->m_func.target_type();

            return false;
        }

        return false;  // Return false if dynamic cast failed
    }

    /// @brief Check if the delegate is bound to a target function.
    /// @return `true` if the delegate has a target function, `false` otherwise.
    bool Empty() const { return !m_func; }

    /// @brief Clear the target function.
    /// @post The delegate is empty.
    void Clear() { m_func = nullptr; }

    /// @brief Implicit conversion operator to `bool`.
    /// @return `true` if the object is not empty, `false` if the object is empty.
    explicit operator bool() const { return !Empty(); }

private:
    /// A std::function instance, representing the bound target function.
    FunctionType m_func;
};

/// @brief Creates a delegate that binds to a free function.
/// @tparam RetType The return type of the free function.
/// @tparam Args The types of the function arguments.
/// @param[in] func A pointer to the free function to bind to the delegate.
/// @return A `DelegateFree` object bound to the specified free function.
template <class RetType, class... Args>
DelegateFree<RetType(Args...)> MakeDelegate(RetType(*func)(Args... args)) {
    return DelegateFree<RetType(Args...)>(func);
}

/// @brief Creates a delegate that binds to a non-const member function.
/// @tparam TClass The class type that contains the member function.
/// @tparam RetType The return type of the member function.
/// @tparam Args The types of the function arguments.
/// @param[in] object A pointer to the instance of `TClass` that will be used for the delegate.
/// @param[in] func A pointer to the non-const member function of `TClass` to bind to the delegate.
/// @return A `DelegateMember` object bound to the specified non-const member function.
template <class TClass, class RetType, class... Args>
DelegateMember<TClass, RetType(Args...)> MakeDelegate(TClass* object, RetType(TClass::*func)(Args... args)) {
    return DelegateMember<TClass, RetType(Args...)>(object, func);
}

/// @brief Creates a delegate that binds to a const member function.
/// @tparam TClass The class type that contains the member function.
/// @tparam RetType The return type of the member function.
/// @tparam Args The types of the function arguments.
/// @param[in] object A pointer to the instance of `TClass` that will be used for the delegate.
/// @param[in] func A pointer to the const member function of `TClass` to bind to the delegate.
/// @return A `DelegateMember` object bound to the specified const member function.
template <class TClass, class RetType, class... Args>
DelegateMember<TClass, RetType(Args...)> MakeDelegate(TClass* object, RetType(TClass::*func)(Args... args) const) {
    return DelegateMember<TClass, RetType(Args...)>(object, func);
}

/// @brief Creates a delegate that binds to a non-const member function with a shared pointer to the object.
/// @tparam TClass The class type that contains the member function.
/// @tparam RetType The return type of the member function.
/// @tparam Args The types of the function arguments.
/// @param[in] object A shared pointer to the instance of `TClass` that will be used for the delegate.
/// @param[in] func A pointer to the non-const member function of `TClass` to bind to the delegate.
/// @return A `DelegateMemberSp` object bound to the specified non-const member function.
template <class TClass, class RetType, class... Args>
DelegateMemberSp<TClass, RetType(Args...)> MakeDelegate(std::shared_ptr<TClass> object, RetType(TClass::* func)(Args... args)) {
    return DelegateMemberSp<TClass, RetType(Args...)>(object, func);
}

/// @brief Creates a delegate that binds to a const member function with a shared pointer to the object.
/// @tparam TClass The class type that contains the member function.
/// @tparam RetType The return type of the member function.
/// @tparam Args The types of the function arguments.
/// @param[in] object A shared pointer to the instance of `TClass` that will be used for the delegate.
/// @param[in] func A pointer to the const member function of `TClass` to bind to the delegate.
/// @return A `DelegateMemberSp` object bound to the specified const member function.
template <class TClass, class RetType, class... Args>
DelegateMemberSp<TClass, RetType(Args...)> MakeDelegate(std::shared_ptr<TClass> object, RetType(TClass::* func)(Args... args) const) {
    return DelegateMemberSp<TClass, RetType(Args...)>(object, func);
}

/// @brief Creates a delegate that binds to a `std::function`.
/// @tparam RetType The return type of the `std::function`.
/// @tparam Args The types of the function arguments.
/// @param[in] func The `std::function` to bind to the delegate.
/// @return A `DelegateFunction` object bound to the specified `std::function`.
template <class RetType, class... Args>
DelegateFunction<RetType(Args...)> MakeDelegate(std::function<RetType(Args...)> func) {
    return DelegateFunction<RetType(Args...)>(func);
}

}

#endif
