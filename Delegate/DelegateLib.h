#ifndef _DELEGATE_LIB_H
#define _DELEGATE_LIB_H

// DelegateLib.h is a single include for users to obtain all delegate functionality

#include "DelegateOpt.h"
#include "MulticastDelegateSafe.h"
#include "SinglecastDelegate.h"
#include "DelegateAsync.h"
#include "DelegateAsyncWait.h"
#include "DelegateRemoteSend.h"
#include "DelegateRemoteRecv.h"

#if USE_CPLUSPLUS_11
#include "DelegateSpAsync.h"
#endif

#endif
