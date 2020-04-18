#ifndef _DELEGATE_REMOTE_RECV_H
#define _DELEGATE_REMOTE_RECV_H

// DelegateRemoteRecv.h
// @see https://www.codeproject.com/Articles/5262271/Remote-Procedure-Calls-using-Cplusplus-Delegates
// David Lafreniere, Mar 2020.

#include "Delegate.h"
#include "DelegateTransport.h"
#include "DelegateRemoteInvoker.h"

namespace DelegateLib {

template <class Param>
class RemoteParam
{
public:
    Param& Get() { return m_param; }
private:
    Param m_param;
};

template <class Param>
class RemoteParam<Param*>
{
public:
    Param* Get() { return &m_param; }
private:
    Param m_param;
};

template <class Param>
class RemoteParam<Param**>
{
public:
    RemoteParam() { m_pParam = &m_param; }
    Param ** Get() { return &m_pParam; }
private:
    Param m_param;
    Param* m_pParam;
};

template <class Param>
class RemoteParam<Param&>
{
public:
    Param & Get() { return m_param; }
private:
    Param m_param;
};

template <class R>
struct DelegateFreeRemoteRecv; // Not defined

template <class RetType, class... Args> // TODO void RetType
class DelegateFreeRemoteRecv<RetType(Args...)> : public DelegateFree<RetType(Args...)>, public DelegateRemoteInvoker {
public:
    typedef void(*FreeFunc)(Args...);

    // Contructors take a free function and delegete id 
    DelegateFreeRemoteRecv(FreeFunc func, DelegateIdType id) : DelegateRemoteInvoker(id) { Bind(func, id); }

    /// Bind a free function to the delegate.
    void Bind(FreeFunc func, DelegateIdType id) {
        m_id = id;
        DelegateFree<RetType(Args...)>::Bind(func);
    }

    virtual DelegateFreeRemoteRecv* Clone() const { return new DelegateFreeRemoteRecv(*this); }

    /// Called by the remote system to invoke the delegate function
    virtual void DelegateInvoke(std::istream& stream) {
#if 0
        RemoteParam<Param1> param1;

        Param1 p1 = param1.Get();

        stream >> m_id;
        stream.seekg(stream.tellg() + std::streampos(1));
        stream >> p1;
        stream.seekg(stream.tellg() + std::streampos(1));

        DelegateFree1<Param1>::operator()(p1);
#endif
    }

    virtual bool operator==(const DelegateBase& rhs) const {
        const DelegateFreeRemoteRecv<RetType(Args...)>* derivedRhs = dynamic_cast<const DelegateFreeRemoteRecv<RetType(Args...)>*>(&rhs);
        return derivedRhs &&
            m_id == derivedRhs->m_id &&
            DelegateFree<RetType(Args...)>::operator == (rhs);
    }

private:
    DelegateIdType m_id;               // Remote delegate identifier
};

template <class C, class R>
struct DelegateMemberRemoteRecv; // Not defined

/// @brief Receive a delegate from a remote system and invoke the bound function. 
template <class TClass, class RetType, class... Args>
class DelegateMemberRemoteRecv<TClass, RetType(Args...)> : public DelegateMember<TClass, RetType(Args...)>, public DelegateRemoteInvoker {
public:
    typedef TClass* ObjectPtr;
    typedef RetType(TClass::*MemberFunc)(Args...);
    typedef RetType(TClass::*ConstMemberFunc)(Args...) const;

    // Contructors take a class instance, member function, and delegete id 
    DelegateMemberRemoteRecv(ObjectPtr object, MemberFunc func, DelegateIdType id) :
        DelegateRemoteInvoker(id) {
        Bind(object, func, id);
    }
    DelegateMemberRemoteRecv(ObjectPtr object, ConstMemberFunc func, DelegateIdType id) :
        DelegateRemoteInvoker(id) {
        Bind(object, func, id);
    }

    /// Bind a member function to a delegate. 
    void Bind(ObjectPtr object, MemberFunc func, DelegateIdType id) {
        m_id = id;
        DelegateMember<TClass, RetType(Args...)>::Bind(object, func);
    }

    /// Bind a const member function to a delegate. 
    void Bind(ObjectPtr object, ConstMemberFunc func, DelegateIdType id) {
        m_id = id;
        DelegateMember<TClass, RetType(Args...)>::Bind(object, func);
    }

    virtual DelegateMemberRemoteRecv<TClass, RetType(Args...)>* Clone() const {
        return new DelegateMemberRemoteRecv<TClass, RetType(Args...)>(*this);
    }

    /// Called by the remote system to invoke the delegate function
    virtual void DelegateInvoke(std::istream& stream) {
#if 0
        RemoteParam<Param1> param1;

        Param1 p1 = param1.Get();

        stream >> m_id;
        stream.seekg(stream.tellg() + std::streampos(1));
        stream >> p1;
        stream.seekg(stream.tellg() + std::streampos(1));

        DelegateMember1<TClass, RetType(Args...)>::operator()(p1);
#endif
    }

    virtual bool operator==(const DelegateBase& rhs) const {
        const DelegateMemberRemoteRecv<TClass, RetType(Args...)>* derivedRhs = dynamic_cast<const DelegateMemberRemoteRecv<TClass, RetType(Args...)>*>(&rhs);
        return derivedRhs &&
            m_id == derivedRhs->m_id &&
            DelegateMember<TClass, RetType(Args...)>::operator == (rhs);
    }

private:
    DelegateIdType m_id;               // Remote delegate identifier
};

template <class TClass, class RetType, class... Args>
DelegateMemberRemoteRecv<TClass, RetType(Args...)> MakeDelegate(TClass* object, void (TClass::*func)(Args... args), DelegateIdType id) {
    return DelegateMemberRemoteRecv<TClass, RetType(Args...)>(object, func, id);
}

template <class TClass, class RetType, class... Args>
DelegateMemberRemoteRecv<TClass, RetType(Args...)> MakeDelegate(TClass* object, void (TClass::*func)(Args... args) const, DelegateIdType id) {
    return DelegateMemberRemoteRecv<TClass, RetType(Args...)>(object, func, id);
}

template <class RetType, class... Args>
DelegateFreeRemoteRecv<RetType(Args...)> MakeDelegate(void(*func)(Args... args), DelegateIdType id) {
    return DelegateFreeRemoteRecv<RetType(Args...)>(func, id);
}

}

#endif