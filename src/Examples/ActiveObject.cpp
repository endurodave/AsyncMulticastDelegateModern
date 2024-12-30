/// @file
/// @brief Implement a active object pattern using delegates. 
/// An example of asynchronous method invocation.

#include "ActiveObject.h"
#include "DelegateLib.h"
#include "WorkerThreadStd.h"
#include <iostream>

using namespace DelegateLib;
using namespace std;

class ActiveObject
{
public:
    ActiveObject() : m_thread("ActiveObject") {
        m_thread.CreateThread();
    }

    void SetValue(double v) {
        // Is caller executing on m_thread?
        if (m_thread.GetThreadId() != WorkerThread::GetCurrentThreadId())
        {
            // Create an asynchronous delegate and re-invoke the function call on m_thread
            MakeDelegate(this, &ActiveObject::SetValue, m_thread).AsyncInvoke(v);
            return;
        }

        // m_value is always updated on m_thread
        m_value = v;
    }

    ~ActiveObject() {
        m_thread.ExitThread();
    }

private:
    double m_value = 0.0;

    // Object has an internal thread
    WorkerThread m_thread;
};


void ActiveObjectExample()
{
    ActiveObject activeObject;
    activeObject.SetValue(1.0);
    activeObject.SetValue(2.0);
}
