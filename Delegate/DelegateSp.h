#ifndef _DELEGATE_SP_H
#define _DELEGATE_SP_H

// DelegateSp.h
// @see https://www.codeproject.com/Articles/5277036/Asynchronous-Multicast-Delegates-in-Modern-Cplusplus
// David Lafreniere, Aug 2020.

#include "Delegate.h"
#include <memory>

namespace DelegateLib {

template <class C, class R>
struct DelegateMemberSp; // Not defined

/// @brief The DelegateMemberSp delegate implemenation synchronously binds
/// and invokes class instance member functions. The std::shared_ptr<TClass> is 
/// used in lieu of a raw TClass* pointer. 
template <class TClass, class RetType, class... Args>
class DelegateMemberSp<TClass, RetType(Args...)> : public Delegate<RetType(Args...)> {
public:
    typedef std::shared_ptr<TClass> ObjectPtr;
    typedef RetType(TClass::*MemberFunc)(Args...);
    typedef RetType(TClass::*ConstMemberFunc)(Args...) const;

    DelegateMemberSp(ObjectPtr object, MemberFunc func) { Bind(object, func); }
    DelegateMemberSp(ObjectPtr object, ConstMemberFunc func) { Bind(object, func); }
    DelegateMemberSp() : m_object(nullptr), m_func(nullptr) { }

    /// Bind a member function to a delegate. 
    void Bind(ObjectPtr object, MemberFunc func) {
        m_object = object;
        m_func = func;
    }

    /// Bind a const member function to a delegate. 
    void Bind(ObjectPtr object, ConstMemberFunc func) {
        m_object = object;
        m_func = reinterpret_cast<MemberFunc>(func);
    }

    virtual DelegateMemberSp* Clone() const { return new DelegateMemberSp(*this); }

    // Invoke the bound delegate function
    virtual RetType operator()(Args... args) {
        return std::invoke(m_func, m_object, args...);
    }

    virtual bool operator==(const DelegateBase& rhs) const {
        const DelegateMemberSp<TClass, RetType(Args...)>* derivedRhs = dynamic_cast<const DelegateMemberSp<TClass, RetType(Args...)>*>(&rhs);
        return derivedRhs &&
            m_func == derivedRhs->m_func &&
            m_object == derivedRhs->m_object;
    }

    bool Empty() const { return !(m_object && m_func); }
    void Clear() { m_object = nullptr; m_func = nullptr; }

    explicit operator bool() const { return !Empty(); }

private:
    ObjectPtr m_object;		// Pointer to a class object
    MemberFunc m_func;   	// Pointer to an instance member function
};

template <class TClass, class RetType, class... Args>
DelegateMemberSp<TClass, RetType(Args...)> MakeDelegate(std::shared_ptr<TClass> object, RetType(TClass::*func)(Args... args)) {
    return DelegateMemberSp<TClass, RetType(Args...)>(object, func);
}

template <class TClass, class RetType, class... Args>
DelegateMemberSp<TClass, RetType(Args...)> MakeDelegate(std::shared_ptr<TClass> object, RetType(TClass::*func)(Args... args) const) {
    return DelegateMemberSp<TClass, RetType(Args...)>(object, func);
}

}

#endif