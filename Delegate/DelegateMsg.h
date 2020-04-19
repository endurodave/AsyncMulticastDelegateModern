#ifndef _DELEGATE_MSG_H
#define _DELEGATE_MSG_H

#include "Fault.h"
#include "DelegateInvoker.h"
#include "make_tuple_heap.h"
#include <tuple>
#include <list>
#include <memory>

extern int newDeleteCnt;    // TODO REMOVE

namespace DelegateLib {

class DelegateMsgBase
{
public:
	/// Constructor
	/// @param[in] invoker - the invoker instance the delegate is registered with.
	/// @param[in] delegate - the delegate instance. 
	DelegateMsgBase(std::shared_ptr<IDelegateInvoker> invoker) :
		m_invoker(invoker)
	{
		ASSERT_TRUE(m_invoker != 0);
	}

    virtual ~DelegateMsgBase() {}

	/// Get the delegate invoker instance the delegate is registered with.
	/// @return The invoker instance. 
	std::shared_ptr<IDelegateInvoker> GetDelegateInvoker() const { return m_invoker; }
	
private:
	/// The IDelegateInvoker instance 
	std::shared_ptr<IDelegateInvoker> m_invoker;
};

/// @brief A class storing all function arguments within the heap suitable 
/// for sending between tasks or threads.
template <class...Args>
class DelegateMsg : public DelegateMsgBase
{
public:
    DelegateMsg(std::shared_ptr<IDelegateInvoker> invoker, Args... args) : DelegateMsgBase(invoker), 
        m_heapArgs(make_tuple_heap(m_heapMem, m_start, args...))
    { 
    }

    virtual ~DelegateMsg()
    {
    }

    std::tuple<Args...>& GetArgs() { return m_heapArgs; }

private:
    /// A list of heap allocated argument memory blocks
    std::list<std::shared_ptr<heap_arg_deleter_base>> m_heapMem;

    /// An empty starting tuple
    std::tuple<> m_start;

    /// A tuple with each element stored within the heap
    std::tuple<Args...> m_heapArgs;
};

}

#endif