#include "DelegateLib.h"
#include "UnitTestCommon.h"
#include <iostream>
#include <set>
#include "WorkerThreadStd.h"

using namespace DelegateLib;
using namespace std;
using namespace UnitTestData;

namespace Sync
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
using namespace Sync;

static void DelegateFreeTests()
{
    using Del = DelegateFree<void(int)>;

    Del delegate1(FreeFuncInt1);
    delegate1(TEST_INT);
    std::invoke(delegate1, TEST_INT);

    auto delegate2 = delegate1;
    ASSERT_TRUE(delegate1 == delegate2);
    ASSERT_TRUE(!delegate1.Empty());
    ASSERT_TRUE(!delegate2.Empty());

    Del delegate3;
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

    Del delegate6;
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

    Del del1{ &FreeFuncInt1 };
    Del del2 = del1;
    Del del3;
    del3 = &FreeFuncInt1;
    ASSERT_TRUE(!del1.Empty());
    ASSERT_TRUE(!del2.Empty());
    ASSERT_TRUE(!del3.Empty());
    ASSERT_TRUE(del1 == del2);
    ASSERT_TRUE(del2 == del3);

    DelegateFree<std::uint16_t(void)> d;
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
    DelegateFree<TestReturn()> testRet;
    ASSERT_TRUE(TestReturn::val == 0);
    testRet();
    ASSERT_TRUE(TestReturn::val == 1);

    DelegateFree<std::unique_ptr<int>(int)> delUnique;
    auto tmp = delUnique(10);
    ASSERT_TRUE(tmp == nullptr);
    delUnique.Bind(&FuncUnique);
    std::unique_ptr<int> up = delUnique(12);
    ASSERT_TRUE(*up == 12);

    auto delS1 = MakeDelegate(FreeFuncInt1);
    auto delS2 = MakeDelegate(FreeFuncInt1_2);
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

    std::function<int(int)> stdFunc = MakeDelegate(&FreeFuncIntWithReturn1);
    int stdFuncRetVal = stdFunc(TEST_INT);
    ASSERT_TRUE(stdFuncRetVal == TEST_INT);

    // ClassSingleton private constructor. Sync delegate does not make
    // copy of ClassSingleton argument; all setter functions ok.
    auto& singleton = ClassSingleton::GetInstance();
    auto delRef = MakeDelegate(&SetClassSingletonRef);
    auto delPtr = MakeDelegate(&SetClassSingletonPtr);
    auto delPtrPtr = MakeDelegate(&SetClassSingletonPtrPtr);

    // Shared pointer does not copy singleton object; no copy of shared_ptr arg.
    auto singletonSp = ClassSingleton::GetInstanceSp();
    auto delShared = MakeDelegate(&SetClassSingletonShared);
    delShared(singletonSp);

    // Test outgoing ptr argument
    StructParam sparam;
    int iparam = 100;
    sparam.val = TEST_INT;
    auto outgoingArg = MakeDelegate(&OutgoingPtrArg);
    outgoingArg(&sparam, &iparam);
    ASSERT_TRUE(sparam.val == TEST_INT + 1);
    ASSERT_TRUE(iparam == 101);

    // Test outgoing ptr-ptr argument
    StructParam* psparam = nullptr;
    sparam.val = TEST_INT;
    auto outgoingArg2 = MakeDelegate(&OutgoingPtrPtrArg);
    outgoingArg2(&psparam);
    ASSERT_TRUE(psparam->val == TEST_INT);

    // Test outgoing ref argument
    sparam.val = TEST_INT;
    auto outgoingArg3 = MakeDelegate(&OutgoingRefArg);
    outgoingArg3(sparam);
    ASSERT_TRUE(sparam.val == TEST_INT + 1);

    // Sync invoke does not copy Class object when passed to func
    Class classInstance;
    Class::m_construtorCnt = 0;
    auto cntDel = MakeDelegate(&ConstructorCnt);
    cntDel(&classInstance);
    ASSERT_TRUE(Class::m_construtorCnt == 0);

    // Test void* args
    const char* str = "Hello World!";
    void* voidPtr = (void*)str;
    auto voidPtrNotNullDel = MakeDelegate(&VoidPtrArgNotNull);
    voidPtrNotNullDel(voidPtr);
    auto voidPtrNullDel = MakeDelegate(&VoidPtrArgNull);
    voidPtrNullDel(nullptr);

    // Test void* return
    auto retVoidPtrDel = MakeDelegate(&RetVoidPtr);
    auto retVoidPtr = retVoidPtrDel();
    ASSERT_TRUE(retVoidPtr != nullptr);
    const char* retStr = (const char*)retVoidPtr;
    ASSERT_TRUE(strcmp(retStr, "Hello World!") == 0);

    // Test rvalue ref
    auto rvalueRefDel = MakeDelegate(&FuncRvalueRef);
    int rv = TEST_INT;
    rvalueRefDel(std::move(rv));
    rvalueRefDel(12345678);
}

static void DelegateMemberTests()
{
    using Del = DelegateMember<TestClass1, void(int)>;

    TestClass1 testClass1;

    Del delegate1(&testClass1, &TestClass1::MemberFuncInt1);
    delegate1(TEST_INT);
    std::invoke(delegate1, TEST_INT);

    auto delegate2 = delegate1;
    ASSERT_TRUE(delegate1 == delegate2);
    ASSERT_TRUE(!delegate1.Empty());
    ASSERT_TRUE(!delegate2.Empty());

    Del delegate3;
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

    Del delegate6;
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

    Base* base = new Derive;
    DelegateMember<Base, int(void)> delegate7;
    delegate7 = MakeDelegate(base, &Base::Func);
    ASSERT_TRUE(delegate7() == TEST_INT);

    DelegateMember<Class, std::uint16_t(void)> d;
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
    DelegateMember<const Class, std::uint16_t(void)> dConstClass;
    //dConstClass.Bind(&c, &Class::Func);     // Not OK. Should fail compile.
    dConstClass.Bind(&c, &Class::FuncConst);  // OK
    auto rConst = dConstClass();

    // Make sure we get a default constructed return value.
    TestReturn::val = 0;
    DelegateMember<TestReturnClass, TestReturn()> testRet;
    ASSERT_TRUE(TestReturn::val == 0);
    testRet();
    ASSERT_TRUE(TestReturn::val == 1);

    Class c2;
    DelegateMember<Class, std::unique_ptr<int>(int)> delUnique;
    auto tmp = delUnique(10);
    ASSERT_TRUE(tmp == nullptr);
    delUnique.Bind(&c2, &Class::FuncUnique);
    std::unique_ptr<int> up = delUnique(12);
    ASSERT_TRUE(*up == 12);

    auto delS1 = MakeDelegate(&testClass1, &TestClass1::MemberFuncInt1);
    auto delS2 = MakeDelegate(&testClass1, &TestClass1::MemberFuncInt1_2);
    ASSERT_TRUE(!(delS1 == delS2));

#if 0  // DelegateMember can't be inserted into ordered container
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
    auto delConstCheck = MakeDelegate(&tcConst, &TestClass1::ConstCheck);
    auto delConstCheckRetVal = delConstCheck(TEST_INT);
    ASSERT_TRUE(delConstCheckRetVal == TEST_INT);

    std::function<int(int)> stdFunc = MakeDelegate(&testClass1, &TestClass1::MemberFuncIntWithReturn1);
    int stdFuncRetVal = stdFunc(TEST_INT);
    ASSERT_TRUE(stdFuncRetVal == TEST_INT);

    SetClassSingleton setClassSingleton;
    // ClassSingleton private constructor. Sync delegate does not make
    // copy of ClassSingleton argument; all setter functions ok.
    auto& singleton = ClassSingleton::GetInstance();
    auto delRef = MakeDelegate(&setClassSingleton, &SetClassSingleton::Ref);
    auto delPtr = MakeDelegate(&setClassSingleton, &SetClassSingleton::Ptr);
    auto delPtrPtr = MakeDelegate(&setClassSingleton, &SetClassSingleton::PtrPtr);

    // Shared pointer does not copy singleton object; no copy of shared_ptr arg.
    auto singletonSp = ClassSingleton::GetInstanceSp();
    auto delShared = MakeDelegate(&setClassSingleton, &SetClassSingleton::Shared);
    delShared(singletonSp);

    // Test rvalue ref
    auto rvalueRefDel = MakeDelegate(&testClass1, &TestClass1::FuncRvalueRef);
    int rv = TEST_INT;
    rvalueRefDel(std::move(rv));
    rvalueRefDel(12345678);
}

static void DelegateMemberSpTests()
{
    using Del = DelegateMember<TestClass1, void(int)>;

    auto testClass1 = std::make_shared<TestClass1>();

    Del delegate1(testClass1, &TestClass1::MemberFuncInt1);
    delegate1(TEST_INT);
    std::invoke(delegate1, TEST_INT);

    auto delegate2 = delegate1;
    ASSERT_TRUE(delegate1 == delegate2);
    ASSERT_TRUE(!delegate1.Empty());
    ASSERT_TRUE(!delegate2.Empty());

    Del delegate3;
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

    Del delegate6;
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

    std::shared_ptr<Base> base = std::make_shared<Derive>();
    DelegateMember<Base, int(void)> delegate7;
    delegate7 = MakeDelegate(base, &Base::Func);
    ASSERT_TRUE(delegate7() == TEST_INT);

    DelegateMember<Class, std::uint16_t(void)> d;
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
    DelegateMember<const Class, std::uint16_t(void)> dConstClass;
    //dConstClass.Bind(c, &Class::Func);     // Not OK. Should fail compile.
    dConstClass.Bind(c, &Class::FuncConst);  // OK
    auto rConst = dConstClass();

    // Make sure we get a default constructed return value.
    TestReturn::val = 0;
    DelegateMember<TestReturnClass, TestReturn()> testRet;
    ASSERT_TRUE(TestReturn::val == 0);
    testRet();
    ASSERT_TRUE(TestReturn::val == 1);

    auto c2 = std::make_shared<Class>();
    DelegateMember<Class, std::unique_ptr<int>(int)> delUnique;
    auto tmp = delUnique(10);
    ASSERT_TRUE(tmp == nullptr);
    delUnique.Bind(c2, &Class::FuncUnique);
    std::unique_ptr<int> up = delUnique(12);
    ASSERT_TRUE(*up == 12);

    auto delS1 = MakeDelegate(testClass1, &TestClass1::MemberFuncInt1);
    auto delS2 = MakeDelegate(testClass1, &TestClass1::MemberFuncInt1_2);
    ASSERT_TRUE(!(delS1 == delS2));

#if 0  // DelegateMember can't be inserted into ordered container
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

    std::function<int(int)> stdFunc = MakeDelegate(testClass1, &TestClass1::MemberFuncIntWithReturn1);
    int stdFuncRetVal = stdFunc(TEST_INT);
    ASSERT_TRUE(stdFuncRetVal == TEST_INT);
}

static void DelegateFunctionTests()
{
    using Del = DelegateFunction<void(int)>;

    Del delegate1(LambdaNoCapture);
    delegate1(TEST_INT);
    std::invoke(delegate1, TEST_INT);

    auto delegate2 = delegate1;
    ASSERT_TRUE(delegate1 == delegate2);
    ASSERT_TRUE(!delegate1.Empty());
    ASSERT_TRUE(!delegate2.Empty());

    Del delegate3;
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

    Del delegate6;
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
    ASSERT_TRUE(nullptr == delegate6);
    ASSERT_TRUE(!(delegate6 != nullptr));

    Del del1{ LambdaNoCapture };
    Del del2 = del1;
    Del del3;
    del3 = LambdaNoCapture;
    ASSERT_TRUE(!del1.Empty());
    ASSERT_TRUE(!del2.Empty());
    ASSERT_TRUE(!del3.Empty());
    ASSERT_TRUE(del1 == del2);
    ASSERT_TRUE(del2 == del3);

    DelegateFunction<std::uint16_t(void)> d;
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
    DelegateFunction<TestReturn()> testRet;
    ASSERT_TRUE(TestReturn::val == 0);
    testRet();
    ASSERT_TRUE(TestReturn::val == 1);

    auto c2 = std::make_shared<Class>();
    DelegateFunction<std::unique_ptr<int>(int)> delUnique;
    auto tmp = delUnique(10);
    ASSERT_TRUE(tmp == nullptr);
    delUnique.Bind(LambdaUnqiue);
    std::unique_ptr<int> up = delUnique(12);
    ASSERT_TRUE(*up == 12);

    auto delS1 = MakeDelegate(LambdaNoCapture);
    auto delS2 = MakeDelegate(LambdaNoCapture2);
    //ASSERT_TRUE(!(delS1 == delS2));  // std::function can't distriguish difference

    delS1.Clear();
    ASSERT_TRUE(delS1.Empty());
    std::swap(delS1, delS2);
    ASSERT_TRUE(!delS1.Empty());
    ASSERT_TRUE(delS2.Empty());

    std::set<Del> setDel;
    setDel.insert(delS1);
    setDel.insert(delS2);
    ASSERT_TRUE(setDel.size() == 1);
}

void Delegate_UT()
{
    DelegateFreeTests();
    DelegateMemberTests();
    DelegateMemberSpTests();
    DelegateFunctionTests();
}