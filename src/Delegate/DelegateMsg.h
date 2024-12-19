#ifndef _DELEGATE_MSG_H
#define _DELEGATE_MSG_H

/// @file
/// @brief Delegate inter-thread message base class. 

#include "Fault.h"
#include "DelegateInvoker.h"
#include "DelegateOpt.h"
#include "Semaphore.h"
#include "make_tuple_heap.h"
#include <tuple>
#include <list>
#include <memory>
#include <mutex>
#include <stdexcept>

namespace DelegateLib {

/// @brief Base class for all delegate inter-thread messages
class DelegateMsg
{
public:
	/// Constructor
	/// @param[in] invoker - the invoker instance the delegate is registered with.
	DelegateMsg(std::shared_ptr<IDelegateInvoker> invoker) :
		m_invoker(invoker)
	{
	}

	virtual ~DelegateMsg() = default;

	/// Get the delegate invoker instance the delegate is registered with.
	/// @return The invoker instance. 
	std::shared_ptr<IDelegateInvoker> GetDelegateInvoker() const { return m_invoker; }

private:
	/// The IDelegateInvoker instance used to invoke the target function 
    /// on the destination thread of control
	std::shared_ptr<IDelegateInvoker> m_invoker;
};

}

#endif
