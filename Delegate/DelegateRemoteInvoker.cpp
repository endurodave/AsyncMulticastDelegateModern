#include "DelegateRemoteInvoker.h"
#include "Fault.h"

namespace DelegateLib 
{
    DelegateRemoteInvoker::DelegateRemoteInvoker(DelegateIdType id) : m_id(id)
    {
        LockGuard lockGuard(GetLock());

        // Don't allow duplicate entries
        std::map<DelegateIdType, DelegateRemoteInvoker*>::iterator it = GetMap().find(m_id);
        ASSERT_TRUE(it == GetMap().end());

        GetMap()[m_id] = this;
    }

    DelegateRemoteInvoker::~DelegateRemoteInvoker()
    {
        LockGuard lockGuard(GetLock());
        GetMap().erase(m_id);
    }
    
    bool DelegateRemoteInvoker::Invoke(std::istream& s)
    {
        // Get id from stream
        DelegateIdType id;
        s >> id;
        s.seekg(0);

        // Find invoker instance matching the id
        std::map<DelegateIdType, DelegateRemoteInvoker*>::iterator it;
        {
            LockGuard lockGuard(GetLock());
            it = GetMap().find(id);
        }
        if (it != GetMap().end())
        {
            // Invoke the delegate instance
            (*it).second->DelegateInvoke(s);
            return true;
        }
        else
        {
            // No delegate found
            return false;
        }
    }
}