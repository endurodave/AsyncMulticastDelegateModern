#include "DelegateLib.h"
#include "UnitTestCommon.h"
#include <iostream>
#include "WorkerThreadStd.h"

using namespace DelegateLib;
using namespace std;
using namespace UnitTestData;

static void DelegateFreeTests()
{
    DelegateFree<void(int)> delegate1(FreeFuncInt1);
    delegate1(TEST_INT);

    auto delegate2 = delegate1;
    ASSERT_TRUE(delegate1 == delegate2);
    ASSERT_TRUE(!delegate1.Empty());
    ASSERT_TRUE(!delegate2.Empty());

    DelegateFree<void(int)> delegate3;
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

    DelegateFree<void(int)> delegate6;
    delegate6 = std::move(delegate2);
    ASSERT_TRUE(!delegate6.Empty());
    ASSERT_TRUE(delegate2.Empty());

    DelegateFunction<void(int)> other;
    ASSERT_TRUE(!(delegate6 == other));
}

static void DelegateMemberTests()
{
    TestClass1 testClass1;

    DelegateMember<TestClass1, void(int)> delegate1(&testClass1, &TestClass1::MemberFuncInt1);
    delegate1(TEST_INT);

    auto delegate2 = delegate1;
    ASSERT_TRUE(delegate1 == delegate2);
    ASSERT_TRUE(!delegate1.Empty());
    ASSERT_TRUE(!delegate2.Empty());

    DelegateMember<TestClass1, void(int)> delegate3;
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

    DelegateMember<TestClass1, void(int)> delegate6;
    delegate6 = std::move(delegate2);
    ASSERT_TRUE(!delegate6.Empty());
    ASSERT_TRUE(delegate2.Empty());

    DelegateFunction<void(int)> other;
    ASSERT_TRUE(!(delegate6 == other));
}

static void DelegateMemberSpTests()
{
    auto testClass1 = std::make_shared<TestClass1>();

    DelegateMember<TestClass1, void(int)> delegate1(testClass1, &TestClass1::MemberFuncInt1);
    delegate1(TEST_INT);

    auto delegate2 = delegate1;
    ASSERT_TRUE(delegate1 == delegate2);
    ASSERT_TRUE(!delegate1.Empty());
    ASSERT_TRUE(!delegate2.Empty());

    DelegateMember<TestClass1, void(int)> delegate3;
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

    DelegateMember<TestClass1, void(int)> delegate6;
    delegate6 = std::move(delegate2);
    ASSERT_TRUE(!delegate6.Empty());
    ASSERT_TRUE(delegate2.Empty());

    DelegateFunction<void(int)> other;
    ASSERT_TRUE(!(delegate6 == other));
}

static void DelegateFunctionTests()
{
    DelegateFunction<void(int)> delegate1(LambdaNoCapture);
    delegate1(TEST_INT);

    auto delegate2 = delegate1;
    ASSERT_TRUE(delegate1 == delegate2);
    ASSERT_TRUE(!delegate1.Empty());
    ASSERT_TRUE(!delegate2.Empty());

    DelegateFunction<void(int)> delegate3;
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

    DelegateFunction<void(int)> delegate6;
    delegate6 = std::move(delegate2);
    ASSERT_TRUE(!delegate6.Empty());
    ASSERT_TRUE(delegate2.Empty());

    DelegateFree<void(int)> other;
    ASSERT_TRUE(!(delegate6 == other));
}

void Delegate_UT()
{
    DelegateFreeTests();
    DelegateMemberTests();
    DelegateMemberSpTests();
    DelegateFunctionTests();
}