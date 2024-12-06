#ifndef _DELEGATE_H
#define _DELEGATE_H

// Delegate.h
// @see https://github.com/endurodave/AsyncMulticastDelegateModern
// David Lafreniere, Aug 2020.
//
// Delegate series of classes are used to invoke a function synchronously. The classes 
// are not thread safe.

#include <functional>
#include <memory>
#include "DelegateOpt.h"

namespace DelegateLib {

// Non-template base class for all delegates
class DelegateBase {
public:
    DelegateBase() = default;
    virtual ~DelegateBase() noexcept = default;

    // Derived class must implement operator== to compare objects.
    virtual bool operator==(const DelegateBase& rhs) const = 0;
    virtual bool operator!=(const DelegateBase& rhs) const { return !(*this == rhs); }

    // Use Clone to provide a deep copy using a base pointer. Covariant 
    // overloading is used so that a Clone() method return type is a
    // more specific type in the derived class implementations.
    // @return A dynamic copy of this instance created with operator new. 
    // @post The caller is responsible for deleting the clone instance. 
    virtual DelegateBase* Clone() const = 0;

    XALLOCATOR
};

template <class R>
struct Delegate; // Not defined

// Delegate template base class
template <class RetType, class... Args>
class Delegate<RetType(Args...)> : public DelegateBase {
public:
    virtual RetType operator()(Args... args) = 0;
    virtual Delegate* Clone() const = 0;
};

template <class R>
struct DelegateFree; // Not defined

// DelegateFree class synchronously invokes a free target function.
template <class RetType, class... Args>
class DelegateFree<RetType(Args...)> : public Delegate<RetType(Args...)> {
public:
    typedef RetType(*FreeFunc)(Args...);
    using ClassType = DelegateFree<RetType(Args...)>;

    ClassType(FreeFunc func) { Bind(func); }
    ClassType(const ClassType& rhs) { Assign(rhs); }
    ClassType() = default;

    // Bind a free function to the delegate.
    void Bind(FreeFunc func) { m_func = func; }

    virtual ClassType* Clone() const override { 
        return new ClassType(*this); 
    }

    void Assign(const ClassType& rhs) { 
        m_func = rhs.m_func; 
    }

    // Invoke the bound delegate function. 
    virtual RetType operator()(Args... args) override {
        return std::invoke(m_func, args...);
    }

    ClassType& operator=(const ClassType& rhs) {
        if (&rhs != this) {
            Assign(rhs);
        }
        return *this;
    }

    virtual bool operator==(const DelegateBase& rhs) const override {
        auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
        return derivedRhs &&
            m_func == derivedRhs->m_func;
    }

    bool Empty() const { return !m_func; }
    void Clear() { m_func = nullptr; }

    explicit operator bool() const { return !Empty(); }

private:
    FreeFunc m_func = nullptr;		// Pointer to a free function
};

template <class C, class R>
struct DelegateMember; // Not defined

// DelegateMember class synchronously invokes a class member target function.
template <class TClass, class RetType, class... Args>
class DelegateMember<TClass, RetType(Args...)> : public Delegate<RetType(Args...)> {
public:
    typedef TClass* ObjectPtr;
    typedef RetType(TClass::*MemberFunc)(Args...);
    typedef RetType(TClass::*ConstMemberFunc)(Args...) const;
    using ClassType = DelegateMember<TClass, RetType(Args...)>;

    ClassType(ObjectPtr object, MemberFunc func) { Bind(object, func); }
    ClassType(ObjectPtr object, ConstMemberFunc func) { Bind(object, func); }
    ClassType(const ClassType& rhs) { Assign(rhs); }
    ClassType() = default;

    // Bind a member function to a delegate. 
    void Bind(ObjectPtr object, MemberFunc func) {
        m_object = object;
        m_func = func;
    }

    // Bind a const member function to a delegate. 
    void Bind(ObjectPtr object, ConstMemberFunc func) {
        m_object = object;
        m_func = reinterpret_cast<MemberFunc>(func);
    }

    virtual ClassType* Clone() const override {
        return new ClassType(*this); 
    }

    void Assign(const ClassType& rhs) {
        m_object = rhs.m_object;
        m_func = rhs.m_func;
    }

    // Invoke the bound delegate function
    virtual RetType operator()(Args... args) override {
        return std::invoke(m_func, m_object, args...);
    }

    ClassType& operator=(const ClassType& rhs) {
        if (&rhs != this) {
            Assign(rhs);
        }
        return *this;
    }

    virtual bool operator==(const DelegateBase& rhs) const override {
        auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
        return derivedRhs &&
            m_func == derivedRhs->m_func &&
            m_object == derivedRhs->m_object;
    }

    bool Empty() const { return !(m_object && m_func); }
    void Clear() { m_object = nullptr; m_func = nullptr; }

    explicit operator bool() const { return !Empty(); }

private:
    ObjectPtr m_object = nullptr;	// Pointer to a class object
    MemberFunc m_func = nullptr;   	// Pointer to an instance member function
};

template <class C, class R>
struct DelegateMemberSp; // Not defined

// DelegateMemberSp class synchronously invokes a std::shared_ptr target function.
template <class TClass, class RetType, class... Args>
class DelegateMemberSp<TClass, RetType(Args...)> : public Delegate<RetType(Args...)> {
public:
    typedef std::shared_ptr<TClass> ObjectPtr;
    typedef RetType(TClass::* MemberFunc)(Args...);
    typedef RetType(TClass::* ConstMemberFunc)(Args...) const;
    using ClassType = DelegateMemberSp<TClass, RetType(Args...)>;

    ClassType(ObjectPtr object, MemberFunc func) { Bind(object, func); }
    ClassType(ObjectPtr object, ConstMemberFunc func) { Bind(object, func); }
    ClassType(const ClassType& rhs) { Assign(rhs); }
    ClassType() = default;

    // Bind a member function to a delegate. 
    void Bind(ObjectPtr object, MemberFunc func) {
        m_object = object;
        m_func = func;
    }

    // Bind a const member function to a delegate. 
    void Bind(ObjectPtr object, ConstMemberFunc func) {
        m_object = object;
        m_func = reinterpret_cast<MemberFunc>(func);
    }

    virtual ClassType* Clone() const override { 
        return new ClassType(*this); 
    }

    void Assign(const ClassType& rhs) {
        m_object = rhs.m_object;
        m_func = rhs.m_func;
    }

    // Invoke the bound delegate function
    virtual RetType operator()(Args... args) override {
        return std::invoke(m_func, m_object, args...);
    }

    ClassType& operator=(const ClassType& rhs) {
        if (&rhs != this) {
            Assign(rhs);
        }
        return *this;
    }

    virtual bool operator==(const DelegateBase& rhs) const override {
        auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
        return derivedRhs &&
            m_func == derivedRhs->m_func &&
            m_object == derivedRhs->m_object;
    }

    bool Empty() const { return !(m_object && m_func); }
    void Clear() { m_object = nullptr; m_func = nullptr; }

    explicit operator bool() const { return !Empty(); }

private:
    ObjectPtr m_object = nullptr;	// Pointer to a class object
    MemberFunc m_func = nullptr;   	// Pointer to an instance member function
};

template <class R>
class DelegateFunction; // Not defined

// DelegateFunction class synchronously invokes a std::function target function.
template <class RetType, class... Args>
class DelegateFunction<RetType(Args...)> : public Delegate<RetType(Args...)> {
public:
    using FunctionType = std::function<RetType(Args...)>;
    using ClassType = DelegateFunction<RetType(Args...)>;

    ClassType(FunctionType func) { Bind(func); }
    ClassType(const ClassType& rhs) { Assign(rhs); }
    ClassType() = default;

    void Bind(FunctionType func) {
        m_func = func;
    }

    // Invoke the bound function
    virtual RetType operator()(Args... args) override {
        return m_func(std::forward<Args>(args)...);
    }

    // Clone the delegate
    virtual ClassType* Clone() const override { 
        return new ClassType(*this); 
    }

    // Assign state data
    void Assign(const ClassType& rhs) {
        m_func = rhs.m_func;
    }

    ClassType& operator=(const ClassType& rhs) {
        if (&rhs != this) {
            Assign(rhs);
        }
        return *this;
    }

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

    bool Empty() const { return !m_func; }
    void Clear() { m_func = nullptr; }

    explicit operator bool() const { return !Empty(); }

private:
    FunctionType m_func;        // Stores any std::function target
};

template <class RetType, class... Args>
DelegateFree<RetType(Args...)> MakeDelegate(RetType(*func)(Args... args)) {
    return DelegateFree<RetType(Args...)>(func);
}

template <class TClass, class RetType, class... Args>
DelegateMember<TClass, RetType(Args...)> MakeDelegate(TClass* object, RetType(TClass::*func)(Args... args)) {
    return DelegateMember<TClass, RetType(Args...)>(object, func);
}

template <class TClass, class RetType, class... Args>
DelegateMember<TClass, RetType(Args...)> MakeDelegate(TClass* object, RetType(TClass::*func)(Args... args) const) {
    return DelegateMember<TClass, RetType(Args...)>(object, func);
}

template <class TClass, class RetType, class... Args>
DelegateMemberSp<TClass, RetType(Args...)> MakeDelegate(std::shared_ptr<TClass> object, RetType(TClass::* func)(Args... args)) {
    return DelegateMemberSp<TClass, RetType(Args...)>(object, func);
}

template <class TClass, class RetType, class... Args>
DelegateMemberSp<TClass, RetType(Args...)> MakeDelegate(std::shared_ptr<TClass> object, RetType(TClass::* func)(Args... args) const) {
    return DelegateMemberSp<TClass, RetType(Args...)>(object, func);
}

template <class RetType, class... Args>
DelegateFunction<RetType(Args...)> MakeDelegate(std::function<RetType(Args...)> func) {
    return DelegateFunction<RetType(Args...)>(func);
}

}

#endif
