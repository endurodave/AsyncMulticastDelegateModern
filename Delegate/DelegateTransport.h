#ifndef _DELEGATE_TRANSPORT_H
#define _DELEGATE_TRANSPORT_H

#include <ostream>

namespace DelegateLib {

/// @brief Each platform specific implementation must inherit from IDelegateTransport
/// and provide an implementation for DispatchDelegate(). The transport can be anything
/// including Ethernet, a file, serial, data pipe, etc... Just so long as the stream 
/// of bytes arrives at the remote system (or process). 
class IDelegateTransport
{
public:
	/// Dispatch a stream of bytes to a remote system. The implementer is responsible
	/// for sending the bytes over a communication link. Once the receiver obtains the 
	/// bytes, the DelegateRemoteInvoker::DelegateInvoke() function must be called to 
    /// execute the callback on the remote system. 
	/// @param[in] s - an outgoing stream to send to the remote CPU.
    virtual void DispatchDelegate(std::iostream& s) = 0;
};

}

#endif
