#include "DelegateLib.h"
#include "UnitTestCommon.h"
#include <iostream>
#include <set>
#include "WorkerThreadStd.h"

using namespace DelegateLib;
using namespace std;
using namespace UnitTestData;

static WorkerThread workerThread("DelegateAsyncWait_UT");

namespace AsyncWait
{
    struct TestReturn
    {
        ~TestReturn()
        {
            val++;
        }
        static int val;
    };

    int TestReturn::val = 0;

    class TestReturnClass
    {
    public:
        TestReturn Func() { return TestReturn{}; }
    };
}
using namespace AsyncWait;

static void DelegateFreeAsyncWaitTests()
{
    using Del = DelegateFreeAsyncWait<void(int)>;

    Del delegate1(FreeFuncInt1, workerThread);
    delegate1(TEST_INT);
    ASSERT_TRUE(delegate1.IsSuccess());
    std::invoke(delegate1, TEST_INT);
    ASSERT_TRUE(delegate1.IsSuccess());

    auto delegate2 = delegate1;
    ASSERT_TRUE(delegate1 == delegate2);
    ASSERT_TRUE(!delegate1.Empty());
    ASSERT_TRUE(!delegate2.Empty());

    Del delegate3(FreeFuncInt1, workerThread);
    delegate3 = delegate1;
    ASSERT_TRUE(delegate3 == delegate1);
    ASSERT_TRUE(delegate3);

    delegate3.Clear();
    ASSERT_TRUE(delegate3.Empty());
    ASSERT_TRUE(!delegate3);

    auto* delegate4 = delegate1.Clone();
    ASSERT_TRUE(*delegate4 == delegate1);

    auto delegate5 = std::move(delegate1);
    ASSERT_TRUE(!delegate5.Empty());
    ASSERT_TRUE(delegate1.Empty());

    Del delegate6(FreeFuncInt1, workerThread);
    delegate6 = std::move(delegate2);
    ASSERT_TRUE(!delegate6.Empty());
    ASSERT_TRUE(delegate2.Empty());
    ASSERT_TRUE(delegate6 != nullptr);
    ASSERT_TRUE(nullptr != delegate6);
    ASSERT_TRUE(delegate2 == nullptr);
    ASSERT_TRUE(nullptr == delegate2);

    DelegateFunction<void(int)> other;
    ASSERT_TRUE(!(delegate6 == other));

    Del delegate7(FreeFuncInt1, workerThread, std::chrono::milliseconds(0));
    auto success = delegate7.AsyncInvoke(TEST_INT);
    //ASSERT_TRUE(!success.has_value());

    DelegateFreeAsyncWait<std::uint16_t(void)> d;
    ASSERT_TRUE(!d);
    auto r = d();
    using ArgT = decltype(r);
    ASSERT_TRUE(std::numeric_limits<ArgT>::min() == 0);
    ASSERT_TRUE(std::numeric_limits<ArgT>::min() == 0);
    ASSERT_TRUE(std::numeric_limits<ArgT>::max() == 0xffff);
    ASSERT_TRUE(std::numeric_limits<ArgT>::is_signed == false);
    ASSERT_TRUE(std::numeric_limits<ArgT>::is_exact == true);
    ASSERT_TRUE(std::numeric_limits<ArgT>::is_integer == true);
    ASSERT_TRUE(r == 0);

    // Make sure we get a default constructed return value.
    TestReturn::val = 0;
    DelegateFreeAsyncWait<TestReturn()> testRet;
    ASSERT_TRUE(TestReturn::val == 0);
    testRet();
    ASSERT_TRUE(TestReturn::val == 1);

#if 0  // AsyncWait cannot return a unique_ptr
    DelegateFreeAsyncWait<std::unique_ptr<int>(int)> delUnique;
    auto tmp = delUnique(10);
    ASSERT_TRUE(tmp == nullptr);
    delUnique.Bind(&FuncUnique, workerThread);
    std::unique_ptr<int> up = delUnique(12);
    ASSERT_TRUE(*up == 12);
#endif

    auto delS1 = MakeDelegate(FreeFuncInt1, workerThread, WAIT_INFINITE);
    auto delS2 = MakeDelegate(FreeFuncInt1_2, workerThread, WAIT_INFINITE);
    ASSERT_TRUE(!(delS1 == delS2));

    std::set<Del> setDel;
    setDel.insert(delS1);
    setDel.insert(delS2);
    ASSERT_TRUE(setDel.size() == 2);

    delS1.Clear();
    ASSERT_TRUE(delS1.Empty());
    std::swap(delS1, delS2);
    ASSERT_TRUE(!delS1.Empty());
    ASSERT_TRUE(delS2.Empty());

    std::function<int(int)> stdFunc = MakeDelegate(&FreeFuncIntWithReturn1, workerThread, WAIT_INFINITE);
    int stdFuncRetVal = stdFunc(TEST_INT);
    ASSERT_TRUE(stdFuncRetVal == TEST_INT);

#if 0
    // ClassSingleton private constructor. Can't sent singleton as ref (&),
    // pointer (*), or pointer-to-pointer (**) since async delegate makes 
    // copy of ClassSingleton argument.
    auto& singleton = ClassSingleton::GetInstance();
    auto delRef = MakeDelegate(&SetClassSingletonRef, workerThread, WAIT_INFINITE);
    auto delPtr = MakeDelegate(&SetClassSingletonPtr, workerThread, WAIT_INFINITE);
    auto delPtrPtr = MakeDelegate(&SetClassSingletonPtrPtr, workerThread, WAIT_INFINITE);
#endif

    // Shared pointer does not copy singleton object; no copy of shared_ptr arg.
    auto singletonSp = ClassSingleton::GetInstanceSp();
    auto delShared = MakeDelegate(&SetClassSingletonShared, workerThread, WAIT_INFINITE);
    delShared(singletonSp);

    // Test outgoing ptr argument
    StructParam sparam;
    int iparam = 100;
    sparam.val = TEST_INT;
    auto outgoingArg = MakeDelegate(&OutgoingPtrArg, workerThread, WAIT_INFINITE);
    outgoingArg(&sparam, &iparam);
    ASSERT_TRUE(sparam.val == TEST_INT + 1);
    ASSERT_TRUE(iparam == 101);

    // Test outgoing ptr-ptr argument
    StructParam* psparam = nullptr;
    sparam.val = TEST_INT;
    auto outgoingArg2 = MakeDelegate(&OutgoingPtrPtrArg, workerThread, WAIT_INFINITE);
    outgoingArg2(&psparam);
    ASSERT_TRUE(psparam->val == TEST_INT);

    // Test outgoing ref argument
    sparam.val = TEST_INT;
    auto outgoingArg3 = MakeDelegate(&OutgoingRefArg, workerThread, WAIT_INFINITE);
    outgoingArg3(sparam);
    ASSERT_TRUE(sparam.val == TEST_INT + 1);

    // AsyncWait invoke does not copy Class object when passed to func
    Class classInstance;
    Class::m_construtorCnt = 0;
    auto cntDel = MakeDelegate(&ConstructorCnt, workerThread, WAIT_INFINITE);
    cntDel(&classInstance);
    ASSERT_TRUE(Class::m_construtorCnt == 0);

    // Compile error. Invalid to pass void* argument to async target function
#if 0   
    // Test void* args
    const char* str = "Hello World!";
    void* voidPtr = (void*)str;
    auto voidPtrNotNullDel = MakeDelegate(&VoidPtrArgNotNull, workerThread, WAIT_INFINITE);
    voidPtrNotNullDel(voidPtr);
    auto voidPtrNullDel = MakeDelegate(&VoidPtrArgNull, workerThread, WAIT_INFINITE);
    voidPtrNullDel(nullptr);
#endif

    // Test void* return
    auto retVoidPtrDel = MakeDelegate(&RetVoidPtr);
    auto retVoidPtr = retVoidPtrDel();
    ASSERT_TRUE(retVoidPtr != nullptr);
    const char* retStr = (const char*)retVoidPtr;
    ASSERT_TRUE(strcmp(retStr, "Hello World!") == 0);
}

static void DelegateMemberAsyncWaitTests()
{
    using Del = DelegateMemberAsyncWait<TestClass1, void(int)>;

    TestClass1 testClass1;

    Del delegate1(&testClass1, &TestClass1::MemberFuncInt1, workerThread);
    delegate1(TEST_INT);
    ASSERT_TRUE(delegate1.IsSuccess());
    std::invoke(delegate1, TEST_INT);
    ASSERT_TRUE(delegate1.IsSuccess());

    auto delegate2 = delegate1;
    ASSERT_TRUE(delegate1 == delegate2);
    ASSERT_TRUE(!delegate1.Empty());
    ASSERT_TRUE(!delegate2.Empty());

    Del delegate3(&testClass1, &TestClass1::MemberFuncInt1, workerThread);
    delegate3 = delegate1;
    ASSERT_TRUE(delegate3 == delegate1);
    ASSERT_TRUE(delegate3);

    delegate3.Clear();
    ASSERT_TRUE(delegate3.Empty());
    ASSERT_TRUE(!delegate3);

    auto* delegate4 = delegate1.Clone();
    ASSERT_TRUE(*delegate4 == delegate1);

    auto delegate5 = std::move(delegate1);
    ASSERT_TRUE(!delegate5.Empty());
    ASSERT_TRUE(delegate1.Empty());

    Del delegate6(&testClass1, &TestClass1::MemberFuncInt1, workerThread);
    delegate6 = std::move(delegate2);
    ASSERT_TRUE(!delegate6.Empty());
    ASSERT_TRUE(delegate2.Empty());
    ASSERT_TRUE(delegate6 != nullptr);
    ASSERT_TRUE(nullptr != delegate6);
    ASSERT_TRUE(delegate2 == nullptr);
    ASSERT_TRUE(nullptr == delegate2);

    DelegateFunction<void(int)> other;
    ASSERT_TRUE(!(delegate6 == other));

    delegate6 = nullptr;
    ASSERT_TRUE(delegate6.Empty());
    ASSERT_TRUE(delegate6 == nullptr);

    Del delegate7(&testClass1, &TestClass1::MemberFuncInt1, workerThread, std::chrono::milliseconds(0));
    auto success = delegate7.AsyncInvoke(TEST_INT);
    //ASSERT_TRUE(!success.has_value());

    DelegateMemberAsyncWait<Class, std::uint16_t(void)> d;
    ASSERT_TRUE(!d);
    auto r = d();
    using ArgT = decltype(r);
    ASSERT_TRUE(std::numeric_limits<ArgT>::min() == 0);
    ASSERT_TRUE(std::numeric_limits<ArgT>::min() == 0);
    ASSERT_TRUE(std::numeric_limits<ArgT>::max() == 0xffff);
    ASSERT_TRUE(std::numeric_limits<ArgT>::is_signed == false);
    ASSERT_TRUE(std::numeric_limits<ArgT>::is_exact == true);
    ASSERT_TRUE(std::numeric_limits<ArgT>::is_integer == true);
    ASSERT_TRUE(r == 0);

    // Check for const correctness
    const Class c;
    DelegateMemberAsyncWait<const Class, std::uint16_t(void)> dConstClass;
    //dConstClass.Bind(&c, &Class::Func, workerThread);     // Not OK. Should fail compile.
    dConstClass.Bind(&c, &Class::FuncConst, workerThread);  // OK
    auto rConst = dConstClass();

    // Make sure we get a default constructed return value.
    TestReturn::val = 0;
    DelegateMemberAsyncWait<TestReturnClass, TestReturn()> testRet;
    ASSERT_TRUE(TestReturn::val == 0);
    testRet();
    ASSERT_TRUE(TestReturn::val == 1);

#if 0  // AsyncWait cannot return a unique_ptr
    Class c2;
    DelegateMemberAsyncWait<Class, std::unique_ptr<int>(int)> delUnique;
    auto tmp = delUnique(10);
    ASSERT_TRUE(tmp == nullptr);
    delUnique.Bind(&c2, &Class::FuncUnique, workerThread);
    std::unique_ptr<int> up = delUnique(12);
    ASSERT_TRUE(*up == 12);
#endif

    auto delS1 = MakeDelegate(&testClass1, &TestClass1::MemberFuncInt1, workerThread, WAIT_INFINITE);
    auto delS2 = MakeDelegate(&testClass1, &TestClass1::MemberFuncInt1_2, workerThread, WAIT_INFINITE);
    ASSERT_TRUE(!(delS1 == delS2));

#if 0  // DelegateMemberAsyncWait can't be inserted into ordered container
    std::set<Del> setDel;
    setDel.insert(delS1);
    setDel.insert(delS2);
    ASSERT_TRUE(setDel.size() == 2);
#endif

    delS1.Clear();
    ASSERT_TRUE(delS1.Empty());
    std::swap(delS1, delS2);
    ASSERT_TRUE(!delS1.Empty());
    ASSERT_TRUE(delS2.Empty());

    const TestClass1 tcConst;
    auto delConstCheck = MakeDelegate(&tcConst, &TestClass1::ConstCheck, workerThread, WAIT_INFINITE);
    auto delConstCheckRetVal = delConstCheck(TEST_INT);
    ASSERT_TRUE(delConstCheckRetVal == TEST_INT);

    std::function<int(int)> stdFunc = MakeDelegate(&testClass1, &TestClass1::MemberFuncIntWithReturn1, workerThread, WAIT_INFINITE);
    int stdFuncRetVal = stdFunc(TEST_INT);
    ASSERT_TRUE(stdFuncRetVal == TEST_INT);

    SetClassSingleton setClassSingleton;
#if 0
    // ClassSingleton private constructor. Can't sent singleton as ref (&),
    // pointer (*), or pointer-to-pointer (**) since async delegate makes 
    // copy of ClassSingleton argument.
    auto& singleton = ClassSingleton::GetInstance();
    auto delRef = MakeDelegate(&setClassSingleton, &SetClassSingleton::Ref, workerThread, WAIT_INFINITE);
    auto delPtr = MakeDelegate(&setClassSingleton, &SetClassSingleton::Ptr, workerThread, WAIT_INFINITE);
    auto delPtrPtr = MakeDelegate(&setClassSingleton, &SetClassSingleton::PtrPtr, workerThread, WAIT_INFINITE);
#endif

    // Shared pointer does not copy singleton object; no copy of shared_ptr arg.
    auto singletonSp = ClassSingleton::GetInstanceSp();
    auto delShared = MakeDelegate(&setClassSingleton, &SetClassSingleton::Shared, workerThread, WAIT_INFINITE);
    delShared(singletonSp);
}

static void DelegateMemberSpAsyncWaitTests()
{
    using Del = DelegateMemberAsyncWait<TestClass1, void(int)>;

    auto testClass1 = std::make_shared<TestClass1>();

    Del delegate1(testClass1, &TestClass1::MemberFuncInt1, workerThread);
    delegate1(TEST_INT);
    ASSERT_TRUE(delegate1.IsSuccess());
    std::invoke(delegate1, TEST_INT);
    ASSERT_TRUE(delegate1.IsSuccess());

    auto delegate2 = delegate1;
    ASSERT_TRUE(delegate1 == delegate2);
    ASSERT_TRUE(!delegate1.Empty());
    ASSERT_TRUE(!delegate2.Empty());

    Del delegate3(testClass1, &TestClass1::MemberFuncInt1, workerThread);
    delegate3 = delegate1;
    ASSERT_TRUE(delegate3 == delegate1);
    ASSERT_TRUE(delegate3);

    delegate3.Clear();
    ASSERT_TRUE(delegate3.Empty());
    ASSERT_TRUE(!delegate3);

    auto* delegate4 = delegate1.Clone();
    ASSERT_TRUE(*delegate4 == delegate1);

    auto delegate5 = std::move(delegate1);
    ASSERT_TRUE(!delegate5.Empty());
    ASSERT_TRUE(delegate1.Empty());

    Del delegate6(testClass1, &TestClass1::MemberFuncInt1, workerThread);
    delegate6 = std::move(delegate2);
    ASSERT_TRUE(!delegate6.Empty());
    ASSERT_TRUE(delegate2.Empty());
    ASSERT_TRUE(delegate6 != nullptr);
    ASSERT_TRUE(nullptr != delegate6);
    ASSERT_TRUE(delegate2 == nullptr);
    ASSERT_TRUE(nullptr == delegate2);

    DelegateFunction<void(int)> other;
    ASSERT_TRUE(!(delegate6 == other));

    delegate6 = nullptr;
    ASSERT_TRUE(delegate6.Empty());
    ASSERT_TRUE(delegate6 == nullptr);

    Del delegate7(testClass1, &TestClass1::MemberFuncInt1, workerThread, std::chrono::milliseconds(0));
    auto success = delegate7.AsyncInvoke(TEST_INT);
    //ASSERT_TRUE(!success.has_value());

    DelegateMemberAsyncWait<Class, std::uint16_t(void)> d;
    ASSERT_TRUE(!d);
    auto r = d();
    using ArgT = decltype(r);
    ASSERT_TRUE(std::numeric_limits<ArgT>::min() == 0);
    ASSERT_TRUE(std::numeric_limits<ArgT>::min() == 0);
    ASSERT_TRUE(std::numeric_limits<ArgT>::max() == 0xffff);
    ASSERT_TRUE(std::numeric_limits<ArgT>::is_signed == false);
    ASSERT_TRUE(std::numeric_limits<ArgT>::is_exact == true);
    ASSERT_TRUE(std::numeric_limits<ArgT>::is_integer == true);
    ASSERT_TRUE(r == 0);

    // Check for const correctness
    auto c = std::make_shared<const Class>();
    DelegateMemberAsyncWait<const Class, std::uint16_t(void)> dConstClass;
    //dConstClass.Bind(c, &Class::Func, workerThread);     // Not OK. Should fail compile.
    dConstClass.Bind(c, &Class::FuncConst, workerThread);  // OK
    auto rConst = dConstClass();

    // Make sure we get a default constructed return value.
    TestReturn::val = 0;
    DelegateMemberAsyncWait<TestReturnClass, TestReturn()> testRet;
    ASSERT_TRUE(TestReturn::val == 0);
    testRet();
    ASSERT_TRUE(TestReturn::val == 1);

#if 0  // AsyncWait cannot return a unique_ptr
    Class c2;
    DelegateMemberAsyncWait<Class, std::unique_ptr<int>(int)> delUnique;
    auto tmp = delUnique(10);
    ASSERT_TRUE(tmp == nullptr);
    delUnique.Bind(&c2, &Class::FuncUnique, workerThread);
    std::unique_ptr<int> up = delUnique(12);
    ASSERT_TRUE(*up == 12);
#endif

    auto delS1 = MakeDelegate(testClass1, &TestClass1::MemberFuncInt1, workerThread, WAIT_INFINITE);
    auto delS2 = MakeDelegate(testClass1, &TestClass1::MemberFuncInt1_2, workerThread, WAIT_INFINITE);
    ASSERT_TRUE(!(delS1 == delS2));

#if 0  // DelegateMemberAsyncWait can't be inserted into ordered container
    std::set<Del> setDel;
    setDel.insert(delS1);
    setDel.insert(delS2);
    ASSERT_TRUE(setDel.size() == 2);
#endif

    delS1.Clear();
    ASSERT_TRUE(delS1.Empty());
    std::swap(delS1, delS2);
    ASSERT_TRUE(!delS1.Empty());
    ASSERT_TRUE(delS2.Empty());

    std::function<int(int)> stdFunc = MakeDelegate(testClass1, &TestClass1::MemberFuncIntWithReturn1, workerThread, WAIT_INFINITE);
    int stdFuncRetVal = stdFunc(TEST_INT);
    ASSERT_TRUE(stdFuncRetVal == TEST_INT);
}

static void DelegateFunctionAsyncWaitTests()
{
    using Del = DelegateFunctionAsyncWait<void(int)>;

    Del delegate1(LambdaNoCapture, workerThread);
    delegate1(TEST_INT);
    ASSERT_TRUE(delegate1.IsSuccess());
    std::invoke(delegate1, TEST_INT);
    ASSERT_TRUE(delegate1.IsSuccess());

    auto delegate2 = delegate1;
    ASSERT_TRUE(delegate1 == delegate2);
    ASSERT_TRUE(!delegate1.Empty());
    ASSERT_TRUE(!delegate2.Empty());

    DelegateFunctionAsyncWait<void(int)> delegate3(LambdaNoCapture, workerThread);
    delegate3 = delegate1;
    ASSERT_TRUE(delegate3 == delegate1);
    ASSERT_TRUE(delegate3);

    delegate3.Clear();
    ASSERT_TRUE(delegate3.Empty());
    ASSERT_TRUE(!delegate3);

    auto* delegate4 = delegate1.Clone();
    ASSERT_TRUE(*delegate4 == delegate1);

    auto delegate5 = std::move(delegate1);
    ASSERT_TRUE(!delegate5.Empty());
    ASSERT_TRUE(delegate1.Empty());

    DelegateFunctionAsyncWait<void(int)> delegate6(LambdaNoCapture, workerThread);
    delegate6 = std::move(delegate2);
    ASSERT_TRUE(!delegate6.Empty());
    ASSERT_TRUE(delegate2.Empty());
    ASSERT_TRUE(delegate6 != nullptr);
    ASSERT_TRUE(nullptr != delegate6);
    ASSERT_TRUE(delegate2 == nullptr);
    ASSERT_TRUE(nullptr == delegate2);

    DelegateFree<void(int)> other;
    ASSERT_TRUE(!(delegate6 == other));

    delegate6 = nullptr;
    ASSERT_TRUE(delegate6.Empty());
    ASSERT_TRUE(delegate6 == nullptr);

    Del delegate7(LambdaNoCapture, workerThread, std::chrono::milliseconds(0));
    auto success = delegate7.AsyncInvoke(TEST_INT);
    //ASSERT_TRUE(!success.has_value());

    auto delegate8{ delegate5 };
    ASSERT_TRUE(delegate8 == delegate5);
    ASSERT_TRUE(!delegate8.Empty());

    auto l = []() -> std::uint16_t { return 0; };
    DelegateFunctionAsyncWait<std::uint16_t(void)> d;
    ASSERT_TRUE(!d);
    auto r = d();
    using ArgT = decltype(r);
    ASSERT_TRUE(std::numeric_limits<ArgT>::min() == 0);
    ASSERT_TRUE(std::numeric_limits<ArgT>::min() == 0);
    ASSERT_TRUE(std::numeric_limits<ArgT>::max() == 0xffff);
    ASSERT_TRUE(std::numeric_limits<ArgT>::is_signed == false);
    ASSERT_TRUE(std::numeric_limits<ArgT>::is_exact == true);
    ASSERT_TRUE(std::numeric_limits<ArgT>::is_integer == true);
    ASSERT_TRUE(r == 0);

    // Make sure we get a default constructed return value.
    TestReturn::val = 0;
    DelegateFunctionAsyncWait<TestReturn()> testRet;
    ASSERT_TRUE(TestReturn::val == 0);
    testRet();
    ASSERT_TRUE(TestReturn::val == 1);

#if 0  // AsyncWait cannot return a unique_ptr
    auto c2 = std::make_shared<Class>();
    DelegateMemberAsyncWait<Class, std::unique_ptr<int>(int)> delUnique;
    auto tmp = delUnique(10);
    ASSERT_TRUE(tmp == nullptr);
    delUnique.Bind(c2, &Class::FuncUnique, workerThread);
    std::unique_ptr<int> up = delUnique(12);
    ASSERT_TRUE(*up == 12);
#endif

    auto delS1 = MakeDelegate(LambdaNoCapture, workerThread, WAIT_INFINITE);
    auto delS2 = MakeDelegate(LambdaNoCapture2, workerThread, WAIT_INFINITE);
    //ASSERT_TRUE(!(delS1 == delS2));  // std::function can't distriguish difference

    std::set<Del> setDel;
    setDel.insert(delS1);
    setDel.insert(delS2);
    ASSERT_TRUE(setDel.size() == 1);

    delS1.Clear();
    ASSERT_TRUE(delS1.Empty());
    std::swap(delS1, delS2);
    ASSERT_TRUE(!delS1.Empty());
    ASSERT_TRUE(delS2.Empty());
}

void DelegateAsyncWait_UT()
{
    workerThread.CreateThread();

    DelegateFreeAsyncWaitTests();
    DelegateMemberAsyncWaitTests();
    DelegateMemberSpAsyncWaitTests();
    DelegateFunctionAsyncWaitTests();

    workerThread.ExitThread();
}