#ifndef _DELEGATE_REMOTE_INVOKER_H
#define _DELEGATE_REMOTE_INVOKER_H

#include "LockGuard.h"
#include <istream>
#include <map>

namespace DelegateLib {

typedef int DelegateIdType;

/// @brief An abstract base class used to invoke a delegate on a remote system. 
class DelegateRemoteInvoker
{
public:
    /// Constructor
    /// @param[in] id - an id shared by both remote systems.
    DelegateRemoteInvoker(DelegateIdType id);

    /// Destructor
    ~DelegateRemoteInvoker();

    /// Invoke a remote delegate
    /// @param[in] s - the incoming remote message stream. 
    static bool Invoke(std::istream& s);

protected:
    /// Called to invoke the callback by the remote system. 
    /// @param[in] s - the incoming remote message stream. 
    virtual void DelegateInvoke(std::istream& s) = 0;

private:
    DelegateIdType m_id;

    static std::map<DelegateIdType, DelegateRemoteInvoker*>& GetMap()
    {
        static std::map<DelegateIdType, DelegateRemoteInvoker*> map;
        return map;
    }

    static LOCK* GetLock()
    {
        static LOCK lock;
        static LockCreateDestroy lockCreateDestroy(lock);
        return &lock;
    }
};

}

#endif