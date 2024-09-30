#include "DelegateLib.h"
#include "SysData.h"
#include "SysDataNoLock.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <utility>
#include "WorkerThreadStd.h"
#include "Timer.h"

// main.cpp
// @see https://github.com/endurodave/AsyncMulticastDelegateModern
// David Lafreniere, Aug 2020.

#if 0 // Not reliable
#if WIN32
struct DumpLeaks
{
    ~DumpLeaks() {
        BOOL leaks = _CrtDumpMemoryLeaks();
        ASSERT_TRUE(leaks == FALSE);
    }
};
static DumpLeaks dumpLeaks;
#endif
#endif

using namespace std;
using namespace DelegateLib;

WorkerThread workerThread1("WorkerThread1");

/// @brief Test client to get callbacks from SysData::SystemModeChangedDelgate and 
/// SysDataNoLock::SystemModeChangedDelegate
class SysDataClient
{
public:
	// Constructor
	SysDataClient() :
		m_numberOfCallbacks(0)
	{
		// Register for async delegate callbacks
		SysData::GetInstance().SystemModeChangedDelegate += MakeDelegate(this, &SysDataClient::CallbackFunction, workerThread1);
		SysDataNoLock::GetInstance().SystemModeChangedDelegate += MakeDelegate(this, &SysDataClient::CallbackFunction, workerThread1);
	}

	~SysDataClient()
	{
		// Unregister the all registered delegates at once
		SysData::GetInstance().SystemModeChangedDelegate.Clear(); 

		// Alternatively unregister a single delegate
		SysDataNoLock::GetInstance().SystemModeChangedDelegate -= MakeDelegate(this, &SysDataClient::CallbackFunction, workerThread1);
	}

private:
    void CallbackFunction(const SystemModeChanged& data)
	{
		m_numberOfCallbacks++;
		cout << "CallbackFunction " << data.CurrentSystemMode << endl;
	}

	int m_numberOfCallbacks;
};

struct TestStruct
{
	int x;
    TestStruct() { x = 0; }
    TestStruct(const TestStruct& d) { x = d.x; }
    ~TestStruct() {}
};

struct TestStructNoCopy
{
	TestStructNoCopy(int _x) { x = _x;  }
	int x;

private:
	// Prevent copying objects
	TestStructNoCopy(const TestStructNoCopy&) = delete;
	TestStructNoCopy& operator=(const TestStructNoCopy&) = delete;
};

void FreeFunc()
{
	cout << "FreeFunc" << endl;
}

void FreeFuncInt(int value)
{
	cout << "FreeFuncInt " << value << endl;
}

int FreeFuncRetInt()
{
	cout << "FreeFuncRetInt " << endl;
	return 567;
}

int FreeFuncIntRetInt(int value)
{
	cout << "FreeFuncIntRetInt " << value << endl;
	return value;
}

void FreeFuncPtrTestStruct(TestStruct* value)
{
	cout << "FreeFuncTestStruct" << value->x << endl;
}

void FreeFuncPtrPtrTestStruct(TestStruct** value)
{
	cout << "FreeFuncPtrPtrTestStruct " << (*value)->x << endl;
}

void FreeFuncRefTestStruct(const TestStruct& value) 
{
	cout << "FreeFuncRefTestStruct " << value.x << endl;
}

class TestClass
{
public:
	~TestClass()
	{
	}

	void MemberFunc(TestStruct* value)
	{
		cout << "MemberFunc " << value->x << endl;
	}

    void MemberFuncThreeArgs(const TestStruct& value, float f, int** i)
	{
		cout << "MemberFuncThreeArgs " << value.x << " " << f << " " << (**i) << endl;
	}

	void MemberFuncNoCopy(std::shared_ptr<TestStructNoCopy> value)
	{
		cout << "MemberFuncNoCopy " << value->x << endl;
	}

    void MemberFuncStdString(const std::string& s, int year) 
	{
		cout << "MemberFuncStdString " << s.c_str() << " " << year << endl;
	}

	int MemberFuncStdStringRetInt(std::string& s)
	{
		s = "Hello world";
		return 2020;
	}

	static void StaticFunc(TestStruct* value)
	{
		cout << "StaticFunc " << value->x << endl;
	}

	int TestFunc()
	{
		cout << "TestFunc " << endl;
		return 987;
	}
	void TestFuncNoRet()
	{
		cout << "TestFuncNoRet " << endl;
	}

    TestStruct TestFuncUserTypeRet()
    {
        TestStruct t;
        t.x = 777;
        return t;
    }
};

void TimerExpiredCb(void)
{
    static int count = 0;
    cout << "TimerExpiredCb " << count++ << endl;
}

class Coordinates
{
public:
    int x = 0;
    int y = 0;
};

class CoordinatesHandler
{
public:
    static MulticastDelegateSafe<void(const std::shared_ptr<const Coordinates>)> CoordinatesChanged;

    void SetData(const Coordinates& data)
    {
        m_data = data;
        CoordinatesChanged(std::make_shared<const Coordinates>(m_data));
    }

private:
    Coordinates m_data;
};

MulticastDelegateSafe<void(const std::shared_ptr<const Coordinates>)> CoordinatesHandler::CoordinatesChanged;

void CoordinatesChangedCallback(const std::shared_ptr<const Coordinates> c)
{
    cout << "New coordinates " << c->x << " " << c->y << endl;
}

// Do not allow shared_ptr references. Causes compile error if used with Async delegates.
void CoordinatesChangedCallbackError(std::shared_ptr<const Coordinates>& c) {}
void CoordinatesChangedCallbackError2(const std::shared_ptr<const Coordinates>& c) {}
void CoordinatesChangedCallbackError3(std::shared_ptr<const Coordinates>* c) {}
void CoordinatesChangedCallbackError4(const std::shared_ptr<const Coordinates>* c) {}

extern void DelegateUnitTests();

//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------
int main(void)
{
	TestStruct testStruct;
	testStruct.x = 123;
	TestStruct* pTestStruct = &testStruct;

	TestClass testClass;

	// Create the worker threads
	workerThread1.CreateThread();
	SysDataNoLock::GetInstance();

    // Create a timer that expires every 250mS and calls 
    // TimerExpiredCb on workerThread1 upon expiration
    Timer timer;
    timer.Expired = MakeDelegate(&TimerExpiredCb, workerThread1);
    timer.Start(std::chrono::milliseconds(250));

	// Run all unit tests (uncomment to run unit tests)
#ifdef DELEGATE_UNIT_TESTS
	DelegateUnitTests();
#endif

    // Create a delegate bound to a free function then invoke
    DelegateFree<void(int)> delegateFree = MakeDelegate(&FreeFuncInt);
    delegateFree(123);

    // Create a delegate bound to a member function then invoke
    DelegateMember<TestClass, void(TestStruct*)> delegateMember = MakeDelegate(&testClass, &TestClass::MemberFunc);
    delegateMember(&testStruct);

    // Create a delegate bound to a member function. Assign and invoke from
    // a base reference. 
    DelegateMember<TestClass, void(TestStruct*)> delegateMember2 = MakeDelegate(&testClass, &TestClass::MemberFunc);
    delegateMember2(&testStruct);

    // Create a multicast delegate container that accepts Delegate<void(int)> delegates.
    // Any function with the signature "void Func(int)".
    MulticastDelegate<void(int)> delegateA;

    // Add a DelegateFree1<int> delegate to the container 
    delegateA += MakeDelegate(&FreeFuncInt);

    // Invoke the delegate target free function FreeFuncInt()
    if (delegateA)
        delegateA(123);

    // Remove the delegate from the container
    delegateA -= MakeDelegate(&FreeFuncInt);

    // Create a multicast delegate container that accepts Delegate<void (TestStruct*)> delegates
    // Any function with the signature "void Func(TestStruct*)".
    MulticastDelegate<void(TestStruct*)> delegateB;

    // Add a DelegateMember1<TestStruct*> delegate to the container
    delegateB += MakeDelegate(&testClass, &TestClass::MemberFunc);

    // Invoke the delegate target member function TestClass::MemberFunc()
    if (delegateB)
        delegateB(&testStruct);

    // Remove the delegate from the container
    delegateB -= MakeDelegate(&testClass, &TestClass::MemberFunc);

    // Create a thread-safe multicast delegate container that accepts Delegate<void (TestStruct*)> delegates
    // Any function with the signature "void Func(TestStruct*)".
    MulticastDelegateSafe<void(TestStruct*)> delegateC;

    // Add a DelegateMember1<TestStruct*> delegate to the container that will invoke on workerThread1
    delegateC += MakeDelegate(&testClass, &TestClass::MemberFunc, workerThread1);

    // Asynchronously invoke the delegate target member function TestClass::MemberFunc()
    if (delegateC)
        delegateC(&testStruct);

    // Remove the delegate from the container
    delegateC -= MakeDelegate(&testClass, &TestClass::MemberFunc, workerThread1);

    // Create a thread-safe multicast delegate container that accepts Delegate<void (TestStruct&, float, int**)> delegates
    // Any function with the signature "void Func(const TestStruct&, float, int**)".
    MulticastDelegateSafe<void(const TestStruct&, float, int**)> delegateD;

    // Add a delegate to the container that will invoke on workerThread1
    delegateD += MakeDelegate(&testClass, &TestClass::MemberFuncThreeArgs, workerThread1);

    // Asynchronously invoke the delegate target member function TestClass::MemberFuncThreeArgs()
    if (delegateD)
    {
        int i = 555;
        int* pI = &i;
        delegateD(testStruct, 1.23f, &pI);
    }

    // Remove the delegate from the container
    delegateD -= MakeDelegate(&testClass, &TestClass::MemberFuncThreeArgs, workerThread1);

    // Create a singlecast delegate container that accepts Delegate<int (int)> delegates.
    // Any function with the signature "int Func(int)".
    SinglecastDelegate<int(int)> delegateF;

    // Add a DelegateFree<int(int)> delegate to the container 
    delegateF = MakeDelegate(&FreeFuncIntRetInt);

    // Invoke the delegate target free function FreeFuncInt()
    int retVal = 0;
    if (delegateF)
        retVal = delegateF(123);

    // Remove the delegate from the container
    delegateF.Clear();

    // Create a singlecast delegate container that accepts delegates with 
    // the signature "void Func(TestStruct**)"
    SinglecastDelegate<void(TestStruct**)> delegateG;

    // Make a delegate that points to a free function 
    delegateG = MakeDelegate(&FreeFuncPtrPtrTestStruct);

    // Invoke the delegate target function FreeFuncPtrPtrTestStruct()
    delegateG(&pTestStruct);

    // Remove the delegate from the container
    delegateG = 0;

    // Create delegate with std::string and int arguments then asynchronously 
    // invoke on a member function
    MulticastDelegateSafe<void(const std::string&, int)> delegateH;
    delegateH += MakeDelegate(&testClass, &TestClass::MemberFuncStdString, workerThread1);
    delegateH("Hello world", 2020);
    delegateH.Clear();

    // Create a asynchronous blocking delegate and invoke. This thread will block until the 
    // msg and year stack values are set by MemberFuncStdStringRetInt on workerThread1.
    auto delegateI = MakeDelegate(&testClass, &TestClass::MemberFuncStdStringRetInt, workerThread1, WAIT_INFINITE);
    std::string msg;
    int year = delegateI(msg);
    if (delegateI.IsSuccess())
    {
        auto year2 = delegateI.GetRetVal();
        cout << msg.c_str() << " " << year << endl;
    }

    // Alternate means to invoke a function asynchronousy using AsyncInvoke. This thread will block until the 
    // msg and year stack values are set by MemberFuncStdStringRetInt on workerThread1.
    std::string msg2;
    auto asyncInvokeRetVal = MakeDelegate(&testClass, &TestClass::MemberFuncStdStringRetInt, workerThread1, std::chrono::milliseconds(100)).AsyncInvoke(msg2);
    if (asyncInvokeRetVal.has_value())
        cout << msg.c_str() << " " << asyncInvokeRetVal.value() << endl;
    else
        cout << "Asynchronous call to MemberFuncStdStringRetInt failed to invoke within specified timeout!";

    // Invoke function asynchronously with user defined return type
    auto testStructRet = MakeDelegate(&testClass, &TestClass::TestFuncUserTypeRet, workerThread1, WAIT_INFINITE).AsyncInvoke();

    // Invoke functions asynchronously with no return value
    auto noRetValRet = MakeDelegate(&testClass, &TestClass::TestFuncNoRet, workerThread1, std::chrono::milliseconds(10)).AsyncInvoke();
    auto noRetValRet2 = MakeDelegate(&FreeFuncInt, workerThread1, std::chrono::milliseconds(10)).AsyncInvoke(123);
    if (noRetValRet.has_value() && noRetValRet2.has_value())
        cout << "Asynchronous calls with no return value succeeded!" << endl;

    // Create a shared_ptr, create a delegate, then synchronously invoke delegate function
    std::shared_ptr<TestClass> spObject(new TestClass());
    auto delegateMemberSp = MakeDelegate(spObject, &TestClass::MemberFuncStdString);
    delegateMemberSp("Hello world using shared_ptr", 2020);

    // Example of a bug where the testClassHeap is deleted before the asychronous delegate 
    // is invoked on the workerThread1. In other words, by the time workerThread1 calls
    // the bound delegate function the testClassHeap instance is deleted and no longer valid.
    TestClass* testClassHeap = new TestClass();
    auto delegateMemberAsync = MakeDelegate(testClassHeap, &TestClass::MemberFuncStdString, workerThread1);
    delegateMemberAsync("Function async invoked on deleted object. Bug!", 2020);
    delegateMemberAsync.Clear();
    delete testClassHeap;

    // Example of the smart pointer function version of the delegate. The testClassSp instance 
    // is only deleted after workerThread1 invokes the callback function thus solving the bug.
    std::shared_ptr<TestClass> testClassSp(new TestClass());
    auto delegateMemberSpAsync = MakeDelegate(testClassSp, &TestClass::MemberFuncStdString, workerThread1);
    delegateMemberSpAsync("Function async invoked using smart pointer. Bug solved!", 2020);
    delegateMemberSpAsync.Clear();
    testClassSp.reset();

    {
        // Example of a shared_ptr argument that does not copy the function
        // argument data. 
        auto delegateJ = MakeDelegate(&testClass, &TestClass::MemberFuncNoCopy, workerThread1);
        std::shared_ptr<TestStructNoCopy> testStructNoCopy = std::make_shared<TestStructNoCopy>(987);
        delegateJ(testStructNoCopy);
    }


    // Example of using std::shared_ptr function arguments with asynchrononous delegate. Using a 
    // shared_ptr<T> argument ensures that the argument T is not copied for each registered client.
    // Could be helpful if T is very large and two or more clients register to receive asynchronous
    // callbacks.
    CoordinatesHandler coordinatesHandler;
    CoordinatesHandler::CoordinatesChanged += MakeDelegate(&CoordinatesChangedCallback, workerThread1);

    Coordinates coordinates;
    coordinates.x = 11;
    coordinates.y = 99;
    coordinatesHandler.SetData(coordinates);

#if 0
    // Causes compiler error. shared_ptr references not allowed; undefined behavior 
    // in multithreaded system.
    auto errorDel = MakeDelegate(&CoordinatesChangedCallbackError, workerThread1);
    auto errorDel2 = MakeDelegate(&CoordinatesChangedCallbackError2, workerThread1);
    auto errorDel3 = MakeDelegate(&CoordinatesChangedCallbackError3, workerThread1);
    auto errorDel4 = MakeDelegate(&CoordinatesChangedCallbackError4, workerThread1);
#endif

    // Begin lambda examples. Lambda captures not allowed if delegates used to invoke.
    auto LambdaFunc1 = +[](int i) -> int
    {
        cout << "Called LambdaFunc1 " << i << std::endl;
        return ++i;
    };

    // Asynchronously invoke lambda on workerThread1 and wait for the return value
    auto lambdaDelegate1 = MakeDelegate(LambdaFunc1, workerThread1, WAIT_INFINITE);
    int lambdaRetVal2 = lambdaDelegate1(123);

    auto LambdaFunc2 = +[](const TestStruct& s, bool b)
    {
        cout << "Called LambdaFunc2 " << s.x << " " << b << std::endl;
    };

    // Invoke lambda via function pointer without delegates
    int lambdaRetVal1 = LambdaFunc1(876);

    TestStruct lambdaArg;
    lambdaArg.x = 4321;

    // Asynchronously invoke lambda on workerThread1 without waiting
    auto lambdaDelegate2 = MakeDelegate(LambdaFunc2, workerThread1);
    lambdaDelegate2(lambdaArg, true);

    // Asynchronously invoke lambda on workerThread1 using AsyncInvoke
    auto lambdaRet = MakeDelegate(LambdaFunc1, workerThread1, std::chrono::milliseconds(100)).AsyncInvoke(543);
    if (lambdaRet.has_value())
        cout << "LambdaFunc1 success! " << lambdaRet.value() << endl;

    std::vector<int> v{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    // Synchronous lambda example
    const auto valResult = std::count_if(v.begin(), v.end(),
        [](int v) { return v > 2 && v <= 6; });
    cout << "Synchronous lambda result: " << valResult << endl;

    // Asychronous lambda example (pass delegate to algorithm)
    auto CountLambda = +[](int v) -> int
    {
        return v > 2 && v <= 6;
    };
    auto countLambdaDelegate = MakeDelegate(CountLambda, workerThread1, WAIT_INFINITE);

    // Alternate syntax
    //auto countLambdaDelegate = MakeDelegate(
    //    +[](int v) { return v > 2 && v <= 6; },
    //	workerThread1, 
    //	WAIT_INFINITE);

    const auto valAsyncResult = std::count_if(v.begin(), v.end(),
        countLambdaDelegate);
    cout << "Asynchronous lambda result: " << valAsyncResult << endl;
    // End lambda examples

    // Create a SysDataClient instance on the stack
    SysDataClient sysDataClient;

    // Set new SystemMode values. Each call will invoke callbacks to all 
    // registered client subscribers.
    SysData::GetInstance().SetSystemMode(SystemMode::STARTING);
    SysData::GetInstance().SetSystemMode(SystemMode::NORMAL);

    // Set new SystemMode values for SysDataNoLock.
    SysDataNoLock::GetInstance().SetSystemMode(SystemMode::SERVICE);
    SysDataNoLock::GetInstance().SetSystemMode(SystemMode::SYS_INOP);

    // Set new SystemMode values for SysDataNoLock using async API
    SysDataNoLock::GetInstance().SetSystemModeAsyncAPI(SystemMode::SERVICE);
    SysDataNoLock::GetInstance().SetSystemModeAsyncAPI(SystemMode::SYS_INOP);

    // Set new SystemMode values for SysDataNoLock using async wait API
    SystemMode::Type previousMode;
    previousMode = SysDataNoLock::GetInstance().SetSystemModeAsyncWaitAPI(SystemMode::STARTING);
    previousMode = SysDataNoLock::GetInstance().SetSystemModeAsyncWaitAPI(SystemMode::NORMAL);

    timer.Stop();
    timer.Expired.Clear();

    std::this_thread::sleep_for(std::chrono::seconds(1));

   	workerThread1.ExitThread();

    std::this_thread::sleep_for(std::chrono::seconds(1));

	return 0;
}

