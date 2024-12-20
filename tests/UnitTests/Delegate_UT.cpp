#include "DelegateLib.h"
#include "UnitTestCommon.h"
#include <iostream>
#include "WorkerThreadStd.h"

using namespace DelegateLib;
using namespace std;
using namespace UnitTestData;

static void DelegateFreeTests()
{
    using Del = DelegateFree<void(int)>;

    Del delegate1(FreeFuncInt1);
    delegate1(TEST_INT);

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
}

static void DelegateMemberTests()
{
    using Del = DelegateMember<TestClass1, void(int)>;

    TestClass1 testClass1;

    Del delegate1(&testClass1, &TestClass1::MemberFuncInt1);
    delegate1(TEST_INT);

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
}

static void DelegateMemberSpTests()
{
    using Del = DelegateMember<TestClass1, void(int)>;

    auto testClass1 = std::make_shared<TestClass1>();

    Del delegate1(testClass1, &TestClass1::MemberFuncInt1);
    delegate1(TEST_INT);

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
}

static void DelegateFunctionTests()
{
    using Del = DelegateFunction<void(int)>;

    Del delegate1(LambdaNoCapture);
    delegate1(TEST_INT);

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
}

void Delegate_UT()
{
    DelegateFreeTests();
    DelegateMemberTests();
    DelegateMemberSpTests();
    DelegateFunctionTests();
}