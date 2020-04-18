#ifndef _DELEGATE_REMOTE_SEND_H
#define _DELEGATE_REMOTE_SEND_H

// DelegateRemoteSend.h
// @see https://www.codeproject.com/Articles/5262271/Remote-Procedure-Calls-using-Cplusplus-Delegates
// David Lafreniere, Mar 2020.

#include "Delegate.h"
#include "DelegateTransport.h"
#include "DelegateRemoteInvoker.h"

namespace DelegateLib {

template <class R>
struct DelegateRemoteSend; // Not defined

/// @brief Send a delegate to invoke a function on a remote system. 
template <class... Args>
class DelegateRemoteSend<void(Args...)> : public Delegate<void(Args...)> {
public:
    DelegateRemoteSend(IDelegateTransport& transport, std::iostream& stream, DelegateIdType id) :
        m_transport(transport), m_stream(stream), m_id(id) { }

    virtual DelegateRemoteSend* Clone() const { return new DelegateRemoteSend(*this); }

    /// Invoke the bound delegate function. 
    virtual void operator()(Args... args) {
        m_stream << m_id << std::ends;
        //m_stream << p1 << std::ends;      // TODO FIX - unroll args like DelegateArg::New() does
        m_transport.DispatchDelegate(m_stream);
    }

    virtual bool operator==(const DelegateBase& rhs) const {
        const DelegateRemoteSend<void(Args...)>* derivedRhs = dynamic_cast<const DelegateRemoteSend<void(Args...)>*>(&rhs);
        return derivedRhs &&
            m_id == derivedRhs->m_id &&
            &m_transport == &derivedRhs->m_transport;
    }

private:
    IDelegateTransport & m_transport;   // Object sends data to remote
    std::iostream& m_stream;            // Storage for remote message 
    DelegateIdType m_id;                // Remote delegate identifier
};

template <class... Args>
DelegateRemoteSend<void(Args...)> MakeDelegate(IDelegateTransport& transport, std::iostream& stream, DelegateIdType id) {
    return DelegateRemoteSend<void(Args...)>(transport, stream, id);
}

}

#endif