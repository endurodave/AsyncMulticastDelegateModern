#include "DelegateLib.h"
#include <iostream>
#if USE_STD_THREADS
	#include "WorkerThreadStd.h"
#elif USE_WIN32_THREADS
	#include "WorkerThreadWin.h"
#endif

using namespace DelegateLib;

WorkerThread testThread("DelegateUnitTestsThread");

static const INT TEST_INT = 12345678;

struct StructParam { INT val; };
INT FreeFuncIntWithReturn0() { return TEST_INT; }

void FreeFunc0() { }

void FreeFuncInt1(INT i) { ASSERT_TRUE(i == TEST_INT); }
INT FreeFuncIntWithReturn1(INT i) { ASSERT_TRUE(i == TEST_INT); return i; }
void FreeFuncPtrPtr1(StructParam** s) { ASSERT_TRUE((*s)->val == TEST_INT); }
void FreeFuncStruct1(StructParam s) { ASSERT_TRUE(s.val == TEST_INT); }
void FreeFuncStructPtr1(StructParam* s) { ASSERT_TRUE(s->val == TEST_INT); }
void FreeFuncStructConstPtr1(const StructParam* s) { ASSERT_TRUE(s->val == TEST_INT); }
void FreeFuncStructRef1(StructParam& s) { ASSERT_TRUE(s.val == TEST_INT); }
void FreeFuncStructConstRef1(const StructParam& s) { ASSERT_TRUE(s.val == TEST_INT); }

void FreeFuncInt2(INT i, INT i2) { ASSERT_TRUE(i == TEST_INT); ASSERT_TRUE(i2 == TEST_INT); }
INT FreeFuncIntWithReturn2(INT i, INT i2) { ASSERT_TRUE(i == TEST_INT); return i; }
void FreeFuncPtrPtr2(StructParam** s, INT i) { ASSERT_TRUE((*s)->val == TEST_INT); }
void FreeFuncStruct2(StructParam s, INT i) { ASSERT_TRUE(s.val == TEST_INT); }
void FreeFuncStructPtr2(StructParam* s, INT i) { ASSERT_TRUE(s->val == TEST_INT); }
void FreeFuncStructConstPtr2(const StructParam* s, INT i) { ASSERT_TRUE(s->val == TEST_INT); }
void FreeFuncStructRef2(StructParam& s, INT i) { ASSERT_TRUE(s.val == TEST_INT); }
void FreeFuncStructConstRef2(const StructParam& s, INT i) { ASSERT_TRUE(s.val == TEST_INT); }

void FreeFuncInt3(INT i, INT i2, INT i3) { ASSERT_TRUE(i == TEST_INT); ASSERT_TRUE(i2 == TEST_INT); ASSERT_TRUE(i3 == TEST_INT); }
INT FreeFuncIntWithReturn3(INT i, INT i2, INT i3) { ASSERT_TRUE(i == TEST_INT); return i; }
void FreeFuncPtrPtr3(StructParam** s, INT i, INT i2) { ASSERT_TRUE((*s)->val == TEST_INT); }
void FreeFuncStruct3(StructParam s, INT i, INT i2) { ASSERT_TRUE(s.val == TEST_INT); }
void FreeFuncStructPtr3(StructParam* s, INT i, INT i2) { ASSERT_TRUE(s->val == TEST_INT); }
void FreeFuncStructConstPtr3(const StructParam* s, INT i, INT i2) { ASSERT_TRUE(s->val == TEST_INT); }
void FreeFuncStructRef3(StructParam& s, INT i, INT i2) { ASSERT_TRUE(s.val == TEST_INT); }
void FreeFuncStructConstRef3(const StructParam& s, INT i, INT i2) { ASSERT_TRUE(s.val == TEST_INT); }

void FreeFuncInt4(INT i, INT i2, INT i3, INT i4) { ASSERT_TRUE(i == TEST_INT); ASSERT_TRUE(i2 == TEST_INT); ASSERT_TRUE(i3 == TEST_INT); }
INT FreeFuncIntWithReturn4(INT i, INT i2, INT i3, INT i4) { ASSERT_TRUE(i == TEST_INT); return i; }
void FreeFuncPtrPtr4(StructParam** s, INT i, INT i2, INT i3) { ASSERT_TRUE((*s)->val == TEST_INT); }
void FreeFuncStruct4(StructParam s, INT i, INT i2, INT i3) { ASSERT_TRUE(s.val == TEST_INT); }
void FreeFuncStructPtr4(StructParam* s, INT i, INT i2, INT i3) { ASSERT_TRUE(s->val == TEST_INT); }
void FreeFuncStructConstPtr4(const StructParam* s, INT i, INT i2, INT i3) { ASSERT_TRUE(s->val == TEST_INT); }
void FreeFuncStructRef4(StructParam& s, INT i, INT i2, INT i3) { ASSERT_TRUE(s.val == TEST_INT); }
void FreeFuncStructConstRef4(const StructParam& s, INT i, INT i2, INT i3) { ASSERT_TRUE(s.val == TEST_INT); }

void FreeFuncInt5(INT i, INT i2, INT i3, INT i4, INT i5) { ASSERT_TRUE(i == TEST_INT); ASSERT_TRUE(i2 == TEST_INT); ASSERT_TRUE(i3 == TEST_INT); }
INT FreeFuncIntWithReturn5(INT i, INT i2, INT i3, INT i4, INT i5) { ASSERT_TRUE(i == TEST_INT); return i; }
void FreeFuncPtrPtr5(StructParam** s, INT i, INT i2, INT i3, INT i4) { ASSERT_TRUE((*s)->val == TEST_INT); }
void FreeFuncStruct5(StructParam s, INT i, INT i2, INT i3, INT i4) { ASSERT_TRUE(s.val == TEST_INT); }
void FreeFuncStructPtr5(StructParam* s, INT i, INT i2, INT i3, INT i4) { ASSERT_TRUE(s->val == TEST_INT); }
void FreeFuncStructConstPtr5(const StructParam* s, INT i, INT i2, INT i3, INT i4) { ASSERT_TRUE(s->val == TEST_INT); }
void FreeFuncStructRef5(StructParam& s, INT i, INT i2, INT i3, INT i4) { ASSERT_TRUE(s.val == TEST_INT); }
void FreeFuncStructConstRef5(const StructParam& s, INT i, INT i2, INT i3, INT i4) { ASSERT_TRUE(s.val == TEST_INT); }

class TestClass0
{
public:
	void MemberFunc0() { }
	void MemberFunc0Const() const { }
	INT MemberFuncWithReturn0() { return TEST_INT; }

	static void StaticFunc0() { }
};

class TestClass1
{
public:
	void MemberFuncInt1(INT i) { ASSERT_TRUE(i == TEST_INT); }
	void MemberFuncInt1Const(INT i) const { ASSERT_TRUE(i == TEST_INT); }
	INT MemberFuncIntWithReturn1(INT i) { ASSERT_TRUE(i == TEST_INT); return i; }
	void MemberFuncStruct1(StructParam s) { ASSERT_TRUE(s.val == TEST_INT); }
	void MemberFuncStructPtr1(StructParam* s) { ASSERT_TRUE(s->val == TEST_INT); }
	void MemberFuncStructPtrPtr1(StructParam** s) { ASSERT_TRUE((*s)->val == TEST_INT); }
	void MemberFuncStructConstPtr1(const StructParam* s) { ASSERT_TRUE(s->val == TEST_INT); }
	void MemberFuncStructRef1(StructParam& s) { ASSERT_TRUE(s.val == TEST_INT); }
	void MemberFuncStructConstRef1(const StructParam& s) { ASSERT_TRUE(s.val == TEST_INT); }

	static void StaticFuncInt1(INT i) { ASSERT_TRUE(i == TEST_INT); }
	static void StaticFuncStruct1(StructParam s) { ASSERT_TRUE(s.val == TEST_INT); }
	static void StaticFuncStructPtr1(StructParam* s) { ASSERT_TRUE(s->val == TEST_INT); }
	static void StaticFuncStructConstPtr1(const StructParam* s) { ASSERT_TRUE(s->val == TEST_INT); }
	static void StaticFuncStructRef1(StructParam& s) { ASSERT_TRUE(s.val == TEST_INT); }
	static void StaticFuncStructConstRef1(const StructParam& s) { ASSERT_TRUE(s.val == TEST_INT); }
};

class TestClass2
{
public:
	void MemberFuncInt2(INT i, INT i2) { ASSERT_TRUE(i == TEST_INT); }
	void MemberFuncInt2Const(INT i, INT i2) const { ASSERT_TRUE(i == TEST_INT); }
	INT MemberFuncIntWithReturn2(INT i, INT i2) { ASSERT_TRUE(i == TEST_INT); return i; }
	void MemberFuncStruct2(StructParam s, INT i) { ASSERT_TRUE(s.val == TEST_INT); }
	void MemberFuncStructPtr2(StructParam* s, INT i) { ASSERT_TRUE(s->val == TEST_INT); }
	void MemberFuncStructPtrPtr2(StructParam** s, INT i) { ASSERT_TRUE((*s)->val == TEST_INT); }
	void MemberFuncStructConstPtr2(const StructParam* s, INT i) { ASSERT_TRUE(s->val == TEST_INT); }
	void MemberFuncStructRef2(StructParam& s, INT i) { ASSERT_TRUE(s.val == TEST_INT); }
	void MemberFuncStructConstRef2(const StructParam& s, INT i) { ASSERT_TRUE(s.val == TEST_INT); }

	static void StaticFuncInt2(INT i, INT i2) { ASSERT_TRUE(i == TEST_INT); }
	static void StaticFuncStruct2(StructParam s, INT i) { ASSERT_TRUE(s.val == TEST_INT); }
	static void StaticFuncStructPtr2(StructParam* s, INT i) { ASSERT_TRUE(s->val == TEST_INT); }
	static void StaticFuncStructConstPtr2(const StructParam* s, INT i) { ASSERT_TRUE(s->val == TEST_INT); }
	static void StaticFuncStructRef2(StructParam& s, INT i) { ASSERT_TRUE(s.val == TEST_INT); }
	static void StaticFuncStructConstRef2(const StructParam& s, INT i) { ASSERT_TRUE(s.val == TEST_INT); }
};

class TestClass3
{
public:
	void MemberFuncInt3(INT i, INT i2, INT i3) { ASSERT_TRUE(i == TEST_INT); }
	void MemberFuncInt3Const(INT i, INT i2, INT i3) const { ASSERT_TRUE(i == TEST_INT); }
	INT MemberFuncIntWithReturn3(INT i, INT i2, INT i3) { ASSERT_TRUE(i == TEST_INT); return i; }
	void MemberFuncStruct3(StructParam s, INT i, INT i2) { ASSERT_TRUE(s.val == TEST_INT); }
	void MemberFuncStructPtr3(StructParam* s, INT i, INT i2) { ASSERT_TRUE(s->val == TEST_INT); }
	void MemberFuncStructPtrPtr3(StructParam** s, INT i, INT i2) { ASSERT_TRUE((*s)->val == TEST_INT); }
	void MemberFuncStructConstPtr3(const StructParam* s, INT i, INT i2) { ASSERT_TRUE(s->val == TEST_INT); }
	void MemberFuncStructRef3(StructParam& s, INT i, INT i2) { ASSERT_TRUE(s.val == TEST_INT); }
	void MemberFuncStructConstRef3(const StructParam& s, INT i, INT i2) { ASSERT_TRUE(s.val == TEST_INT); }

	static void StaticFuncInt3(INT i, INT i2, INT i3) { ASSERT_TRUE(i == TEST_INT); }
	static void StaticFuncStruct3(StructParam s, INT i, INT i2) { ASSERT_TRUE(s.val == TEST_INT); }
	static void StaticFuncStructPtr3(StructParam* s, INT i, INT i2) { ASSERT_TRUE(s->val == TEST_INT); }
	static void StaticFuncStructConstPtr3(const StructParam* s, INT i, INT i2) { ASSERT_TRUE(s->val == TEST_INT); }
	static void StaticFuncStructRef3(StructParam& s, INT i, INT i2) { ASSERT_TRUE(s.val == TEST_INT); }
	static void StaticFuncStructConstRef3(const StructParam& s, INT i, INT i2) { ASSERT_TRUE(s.val == TEST_INT); }
};

class TestClass4
{
public:
	void MemberFuncInt4(INT i, INT i2, INT i3, INT i4) { ASSERT_TRUE(i == TEST_INT); }
	void MemberFuncInt4Const(INT i, INT i2, INT i3, INT i4) const { ASSERT_TRUE(i == TEST_INT); }
	INT MemberFuncIntWithReturn4(INT i, INT i2, INT i3, INT i4) { ASSERT_TRUE(i == TEST_INT); return i; }
	void MemberFuncStruct4(StructParam s, INT i, INT i2, INT i3) { ASSERT_TRUE(s.val == TEST_INT); }
	void MemberFuncStructPtr4(StructParam* s, INT i, INT i2, INT i3) { ASSERT_TRUE(s->val == TEST_INT); }
	void MemberFuncStructPtrPtr4(StructParam** s, INT i, INT i2, INT i3) { ASSERT_TRUE((*s)->val == TEST_INT); }
	void MemberFuncStructConstPtr4(const StructParam* s, INT i, INT i2, INT i3) { ASSERT_TRUE(s->val == TEST_INT); }
	void MemberFuncStructRef4(StructParam& s, INT i, INT i2, INT i3) { ASSERT_TRUE(s.val == TEST_INT); }
	void MemberFuncStructConstRef4(const StructParam& s, INT i, INT i2, INT i3) { ASSERT_TRUE(s.val == TEST_INT); }

	static void StaticFuncInt4(INT i, INT i2, INT i3, INT i4) { ASSERT_TRUE(i == TEST_INT); }
	static void StaticFuncStruct4(StructParam s, INT i, INT i2, INT i3) { ASSERT_TRUE(s.val == TEST_INT); }
	static void StaticFuncStructPtr4(StructParam* s, INT i, INT i2, INT i3) { ASSERT_TRUE(s->val == TEST_INT); }
	static void StaticFuncStructConstPtr4(const StructParam* s, INT i, INT i2, INT i3) { ASSERT_TRUE(s->val == TEST_INT); }
	static void StaticFuncStructRef4(StructParam& s, INT i, INT i2, INT i3) { ASSERT_TRUE(s.val == TEST_INT); }
	static void StaticFuncStructConstRef4(const StructParam& s, INT i, INT i2, INT i3) { ASSERT_TRUE(s.val == TEST_INT); }
};

class TestClass5
{
public:
	void MemberFuncInt5(INT i, INT i2, INT i3, INT i4, INT i5) { ASSERT_TRUE(i == TEST_INT); }
	void MemberFuncInt5Const(INT i, INT i2, INT i3, INT i4, INT i5) const { ASSERT_TRUE(i == TEST_INT); }
	INT MemberFuncIntWithReturn5(INT i, INT i2, INT i3, INT i4, INT i5) { ASSERT_TRUE(i == TEST_INT); return i; }
	void MemberFuncStruct5(StructParam s, INT i, INT i2, INT i3, INT i4) { ASSERT_TRUE(s.val == TEST_INT); }
	void MemberFuncStructPtr5(StructParam* s, INT i, INT i2, INT i3, INT i4) { ASSERT_TRUE(s->val == TEST_INT); }
	void MemberFuncStructPtrPtr5(StructParam** s, INT i, INT i2, INT i3, INT i4) { ASSERT_TRUE((*s)->val == TEST_INT); }
	void MemberFuncStructConstPtr5(const StructParam* s, INT i, INT i2, INT i3, INT i4) { ASSERT_TRUE(s->val == TEST_INT); }
	void MemberFuncStructRef5(StructParam& s, INT i, INT i2, INT i3, INT i4) { ASSERT_TRUE(s.val == TEST_INT); }
	void MemberFuncStructConstRef5(const StructParam& s, INT i, INT i2, INT i3, INT i4) { ASSERT_TRUE(s.val == TEST_INT); }

	static void StaticFuncInt5(INT i, INT i2, INT i3, INT i4, INT i5) { ASSERT_TRUE(i == TEST_INT); }
	static void StaticFuncStruct5(StructParam s, INT i, INT i2, INT i3, INT i4) { ASSERT_TRUE(s.val == TEST_INT); }
	static void StaticFuncStructPtr5(StructParam* s, INT i, INT i2, INT i3, INT i4) { ASSERT_TRUE(s->val == TEST_INT); }
	static void StaticFuncStructConstPtr5(const StructParam* s, INT i, INT i2, INT i3, INT i4) { ASSERT_TRUE(s->val == TEST_INT); }
	static void StaticFuncStructRef5(StructParam& s, INT i, INT i2, INT i3, INT i4) { ASSERT_TRUE(s.val == TEST_INT); }
	static void StaticFuncStructConstRef5(const StructParam& s, INT i, INT i2, INT i3, INT i4) { ASSERT_TRUE(s.val == TEST_INT); }
};

void SinglecastDelegateTests()
{
	StructParam structParam;
	structParam.val = TEST_INT;
	StructParam* pStructParam = &structParam;

	// N=0 Free Functions
	SinglecastDelegate0<> FreeFunc0SinglecastDelegate;
	ASSERT_TRUE(FreeFunc0SinglecastDelegate.Empty() == true);
	ASSERT_TRUE(!FreeFunc0SinglecastDelegate);
	FreeFunc0SinglecastDelegate = MakeDelegate(&FreeFunc0);
	ASSERT_TRUE(FreeFunc0SinglecastDelegate.Empty() == false);
	ASSERT_TRUE(FreeFunc0SinglecastDelegate);
	FreeFunc0SinglecastDelegate();
	FreeFunc0SinglecastDelegate.Clear();
	ASSERT_TRUE(!FreeFunc0SinglecastDelegate);

	SinglecastDelegate0<INT> FreeFuncIntWithReturn0SinglecastDelegate;
	FreeFuncIntWithReturn0SinglecastDelegate = MakeDelegate(&FreeFuncIntWithReturn0);
	ASSERT_TRUE(FreeFuncIntWithReturn0SinglecastDelegate() == TEST_INT);

	// N=0 Member Functions
	TestClass0 testClass0;

	SinglecastDelegate0<> MemberFunc0SinglecastDelegate;
	ASSERT_TRUE(MemberFunc0SinglecastDelegate.Empty() == true);
	ASSERT_TRUE(!MemberFunc0SinglecastDelegate);
	MemberFunc0SinglecastDelegate = MakeDelegate(&testClass0, &TestClass0::MemberFunc0);
	ASSERT_TRUE(MemberFunc0SinglecastDelegate.Empty() == false);
	ASSERT_TRUE(MemberFunc0SinglecastDelegate);
	MemberFunc0SinglecastDelegate();
	MemberFunc0SinglecastDelegate.Clear();
	ASSERT_TRUE(!MemberFunc0SinglecastDelegate);

	SinglecastDelegate0<INT> MemberFuncIntWithReturn0SinglecastDelegate;
	MemberFuncIntWithReturn0SinglecastDelegate = MakeDelegate(&testClass0, &TestClass0::MemberFuncWithReturn0);
	ASSERT_TRUE(MemberFuncIntWithReturn0SinglecastDelegate() == TEST_INT);

	// N=0 Static Functions
	SinglecastDelegate0<> StaticFunc0SinglecastDelegate;
	StaticFunc0SinglecastDelegate = MakeDelegate(&TestClass0::StaticFunc0);
	StaticFunc0SinglecastDelegate();

	// N=1 Free Functions
	SinglecastDelegate1<INT> FreeFuncInt1SinglecastDelegate;
	ASSERT_TRUE(FreeFuncInt1SinglecastDelegate.Empty() == true);
	ASSERT_TRUE(!FreeFuncInt1SinglecastDelegate);
	FreeFuncInt1SinglecastDelegate = MakeDelegate(&FreeFuncInt1);
	ASSERT_TRUE(FreeFuncInt1SinglecastDelegate.Empty() == false);
	ASSERT_TRUE(FreeFuncInt1SinglecastDelegate);
	FreeFuncInt1SinglecastDelegate(TEST_INT);
	FreeFuncInt1SinglecastDelegate.Clear();
	ASSERT_TRUE(!FreeFuncInt1SinglecastDelegate);

	SinglecastDelegate1<INT, INT> FreeFuncIntWithReturn1SinglecastDelegate;
	FreeFuncIntWithReturn1SinglecastDelegate = MakeDelegate(&FreeFuncIntWithReturn1);
	ASSERT_TRUE(FreeFuncIntWithReturn1SinglecastDelegate(TEST_INT) == TEST_INT);

	SinglecastDelegate1<StructParam**> FreeFuncPtrPtr1SinglecastDelegate;
	FreeFuncPtrPtr1SinglecastDelegate = MakeDelegate(&FreeFuncPtrPtr1);
	FreeFuncPtrPtr1SinglecastDelegate(&pStructParam);

	SinglecastDelegate1<StructParam> FreeFuncStruct1SinglecastDelegate;
	FreeFuncStruct1SinglecastDelegate = MakeDelegate(&FreeFuncStruct1);
	FreeFuncStruct1SinglecastDelegate(structParam);

	SinglecastDelegate1<StructParam*> FreeFuncStructPtr1SinglecastDelegate;
	FreeFuncStructPtr1SinglecastDelegate = MakeDelegate(&FreeFuncStructPtr1);
	FreeFuncStructPtr1SinglecastDelegate(&structParam);

	SinglecastDelegate1<const StructParam*> FreeFuncStructConstPtr1SinglecastDelegate;
	FreeFuncStructConstPtr1SinglecastDelegate = MakeDelegate(&FreeFuncStructConstPtr1);
	FreeFuncStructConstPtr1SinglecastDelegate(&structParam);

	SinglecastDelegate1<StructParam&> FreeFuncStructRef1SinglecastDelegate;
	FreeFuncStructRef1SinglecastDelegate = MakeDelegate(&FreeFuncStructRef1);
	FreeFuncStructRef1SinglecastDelegate(structParam);

	SinglecastDelegate1<const StructParam&> FreeFuncStructConstRef1SinglecastDelegate;
	FreeFuncStructConstRef1SinglecastDelegate = MakeDelegate(&FreeFuncStructConstRef1);
	FreeFuncStructConstRef1SinglecastDelegate(structParam);

	// N=1 Member Functions
	TestClass1 testClass1;

	SinglecastDelegate1<INT> MemberFuncInt1SinglecastDelegate;
	ASSERT_TRUE(MemberFuncInt1SinglecastDelegate.Empty() == true);
	ASSERT_TRUE(!MemberFuncInt1SinglecastDelegate);
	MemberFuncInt1SinglecastDelegate = MakeDelegate(&testClass1, &TestClass1::MemberFuncInt1);
	ASSERT_TRUE(MemberFuncInt1SinglecastDelegate.Empty() == false);
	ASSERT_TRUE(MemberFuncInt1SinglecastDelegate);
	MemberFuncInt1SinglecastDelegate(TEST_INT);
	MemberFuncInt1SinglecastDelegate.Clear();
	ASSERT_TRUE(!MemberFuncInt1SinglecastDelegate);

	SinglecastDelegate1<INT, INT> MemberFuncIntWithReturn1SinglecastDelegate;
	MemberFuncIntWithReturn1SinglecastDelegate = MakeDelegate(&testClass1, &TestClass1::MemberFuncIntWithReturn1);
	ASSERT_TRUE(MemberFuncIntWithReturn1SinglecastDelegate(TEST_INT) == TEST_INT);

	SinglecastDelegate1<StructParam> MemberFuncStruct1SinglecastDelegate;
	MemberFuncStruct1SinglecastDelegate = MakeDelegate(&testClass1, &TestClass1::MemberFuncStruct1);
	MemberFuncStruct1SinglecastDelegate(structParam);

	SinglecastDelegate1<StructParam*> MemberFuncStructPtr1SinglecastDelegate;
	MemberFuncStructPtr1SinglecastDelegate = MakeDelegate(&testClass1, &TestClass1::MemberFuncStructPtr1);
	MemberFuncStructPtr1SinglecastDelegate(&structParam);

	SinglecastDelegate1<const StructParam*> MemberFuncStructConstPtr1SinglecastDelegate;
	MemberFuncStructConstPtr1SinglecastDelegate = MakeDelegate(&testClass1, &TestClass1::MemberFuncStructConstPtr1);
	MemberFuncStructConstPtr1SinglecastDelegate(&structParam);

	SinglecastDelegate1<StructParam&> MemberFuncStructRef1SinglecastDelegate;
	MemberFuncStructRef1SinglecastDelegate = MakeDelegate(&testClass1, &TestClass1::MemberFuncStructRef1);
	MemberFuncStructRef1SinglecastDelegate(structParam);

	SinglecastDelegate1<const StructParam&> MemberFuncStructConstRef1SinglecastDelegate;
	MemberFuncStructConstRef1SinglecastDelegate = MakeDelegate(&testClass1, &TestClass1::MemberFuncStructConstRef1);
	MemberFuncStructConstRef1SinglecastDelegate(structParam);

	// N=1 Static Functions
	SinglecastDelegate1<INT> StaticFuncInt1SinglecastDelegate;
	StaticFuncInt1SinglecastDelegate = MakeDelegate(&TestClass1::StaticFuncInt1);
	StaticFuncInt1SinglecastDelegate(TEST_INT);

	SinglecastDelegate1<StructParam> StaticFuncStruct1SinglecastDelegate;
	StaticFuncStruct1SinglecastDelegate = MakeDelegate(&TestClass1::StaticFuncStruct1);
	StaticFuncStruct1SinglecastDelegate(structParam);

	SinglecastDelegate1<StructParam*> StaticFuncStructPtr1SinglecastDelegate;
	StaticFuncStructPtr1SinglecastDelegate = MakeDelegate(&TestClass1::StaticFuncStructPtr1);
	StaticFuncStructPtr1SinglecastDelegate(&structParam);

	SinglecastDelegate1<const StructParam*> StaticFuncStructConstPtr1SinglecastDelegate;
	StaticFuncStructConstPtr1SinglecastDelegate = MakeDelegate(&TestClass1::StaticFuncStructConstPtr1);
	StaticFuncStructConstPtr1SinglecastDelegate(&structParam);

	SinglecastDelegate1<StructParam&> StaticFuncStructRef1SinglecastDelegate;
	StaticFuncStructRef1SinglecastDelegate = MakeDelegate(&TestClass1::StaticFuncStructRef1);
	StaticFuncStructRef1SinglecastDelegate(structParam);

	SinglecastDelegate1<const StructParam&> StaticFuncStructConstRef1SinglecastDelegate;
	StaticFuncStructConstRef1SinglecastDelegate = MakeDelegate(&TestClass1::StaticFuncStructConstRef1);
	StaticFuncStructConstRef1SinglecastDelegate(structParam);

	// N=2 Free Functions
	SinglecastDelegate2<INT, INT> FreeFuncInt2SinglecastDelegate;
	ASSERT_TRUE(FreeFuncInt2SinglecastDelegate.Empty() == true);
	ASSERT_TRUE(!FreeFuncInt2SinglecastDelegate);
	FreeFuncInt2SinglecastDelegate = MakeDelegate(&FreeFuncInt2);
	ASSERT_TRUE(FreeFuncInt2SinglecastDelegate.Empty() == false);
	ASSERT_TRUE(FreeFuncInt2SinglecastDelegate);
	FreeFuncInt2SinglecastDelegate(TEST_INT, TEST_INT);
	FreeFuncInt2SinglecastDelegate.Clear();
	ASSERT_TRUE(!FreeFuncInt2SinglecastDelegate);

	SinglecastDelegate2<INT, INT, INT> FreeFuncIntWithReturn2SinglecastDelegate;
	FreeFuncIntWithReturn2SinglecastDelegate = MakeDelegate(&FreeFuncIntWithReturn2);
	ASSERT_TRUE(FreeFuncIntWithReturn2SinglecastDelegate(TEST_INT, TEST_INT) == TEST_INT);

	SinglecastDelegate2<StructParam**, INT> FreeFuncPtrPtr2SinglecastDelegate;
	FreeFuncPtrPtr2SinglecastDelegate = MakeDelegate(&FreeFuncPtrPtr2);
	FreeFuncPtrPtr2SinglecastDelegate(&pStructParam, TEST_INT);

	SinglecastDelegate2<StructParam, INT> FreeFuncStruct2SinglecastDelegate;
	FreeFuncStruct2SinglecastDelegate = MakeDelegate(&FreeFuncStruct2);
	FreeFuncStruct2SinglecastDelegate(structParam, TEST_INT);

	SinglecastDelegate2<StructParam*, INT> FreeFuncStructPtr2SinglecastDelegate;
	FreeFuncStructPtr2SinglecastDelegate = MakeDelegate(&FreeFuncStructPtr2);
	FreeFuncStructPtr2SinglecastDelegate(&structParam, TEST_INT);

	SinglecastDelegate2<const StructParam*, INT> FreeFuncStructConstPtr2SinglecastDelegate;
	FreeFuncStructConstPtr2SinglecastDelegate = MakeDelegate(&FreeFuncStructConstPtr2);
	FreeFuncStructConstPtr2SinglecastDelegate(&structParam, TEST_INT);

	SinglecastDelegate2<StructParam&, INT> FreeFuncStructRef2SinglecastDelegate;
	FreeFuncStructRef2SinglecastDelegate = MakeDelegate(&FreeFuncStructRef2);
	FreeFuncStructRef2SinglecastDelegate(structParam, TEST_INT);

	SinglecastDelegate2<const StructParam&, INT> FreeFuncStructConstRef2SinglecastDelegate;
	FreeFuncStructConstRef2SinglecastDelegate = MakeDelegate(&FreeFuncStructConstRef2);
	FreeFuncStructConstRef2SinglecastDelegate(structParam, TEST_INT);

	// N=2 Member Functions
	TestClass2 testClass2;

	SinglecastDelegate2<INT, INT> MemberFuncInt2SinglecastDelegate;
	ASSERT_TRUE(MemberFuncInt2SinglecastDelegate.Empty() == true);
	ASSERT_TRUE(!MemberFuncInt2SinglecastDelegate);
	MemberFuncInt2SinglecastDelegate = MakeDelegate(&testClass2, &TestClass2::MemberFuncInt2);
	ASSERT_TRUE(MemberFuncInt2SinglecastDelegate.Empty() == false);
	ASSERT_TRUE(MemberFuncInt2SinglecastDelegate);
	MemberFuncInt2SinglecastDelegate(TEST_INT, TEST_INT);
	MemberFuncInt2SinglecastDelegate.Clear();
	ASSERT_TRUE(!MemberFuncInt2SinglecastDelegate);

	SinglecastDelegate2<INT, INT, INT> MemberFuncIntWithReturn2SinglecastDelegate;
	MemberFuncIntWithReturn2SinglecastDelegate = MakeDelegate(&testClass2, &TestClass2::MemberFuncIntWithReturn2);
	ASSERT_TRUE(MemberFuncIntWithReturn2SinglecastDelegate(TEST_INT, TEST_INT) == TEST_INT);

	SinglecastDelegate2<StructParam, INT> MemberFuncStruct2SinglecastDelegate;
	MemberFuncStruct2SinglecastDelegate = MakeDelegate(&testClass2, &TestClass2::MemberFuncStruct2);
	MemberFuncStruct2SinglecastDelegate(structParam, TEST_INT);

	SinglecastDelegate2<StructParam*, INT> MemberFuncStructPtr2SinglecastDelegate;
	MemberFuncStructPtr2SinglecastDelegate = MakeDelegate(&testClass2, &TestClass2::MemberFuncStructPtr2);
	MemberFuncStructPtr2SinglecastDelegate(&structParam, TEST_INT);

	SinglecastDelegate2<const StructParam*, INT> MemberFuncStructConstPtr2SinglecastDelegate;
	MemberFuncStructConstPtr2SinglecastDelegate = MakeDelegate(&testClass2, &TestClass2::MemberFuncStructConstPtr2);
	MemberFuncStructConstPtr2SinglecastDelegate(&structParam, TEST_INT);

	SinglecastDelegate2<StructParam&, INT> MemberFuncStructRef2SinglecastDelegate;
	MemberFuncStructRef2SinglecastDelegate = MakeDelegate(&testClass2, &TestClass2::MemberFuncStructRef2);
	MemberFuncStructRef2SinglecastDelegate(structParam, TEST_INT);

	SinglecastDelegate2<const StructParam&, INT> MemberFuncStructConstRef2SinglecastDelegate;
	MemberFuncStructConstRef2SinglecastDelegate = MakeDelegate(&testClass2, &TestClass2::MemberFuncStructConstRef2);
	MemberFuncStructConstRef2SinglecastDelegate(structParam, TEST_INT);

	// N=2 Static Functions
	SinglecastDelegate2<INT, INT> StaticFuncInt2SinglecastDelegate;
	StaticFuncInt2SinglecastDelegate = MakeDelegate(&TestClass2::StaticFuncInt2);
	StaticFuncInt2SinglecastDelegate(TEST_INT, TEST_INT);

	SinglecastDelegate2<StructParam, INT> StaticFuncStruct2SinglecastDelegate;
	StaticFuncStruct2SinglecastDelegate = MakeDelegate(&TestClass2::StaticFuncStruct2);
	StaticFuncStruct2SinglecastDelegate(structParam, TEST_INT);

	SinglecastDelegate2<StructParam*, INT> StaticFuncStructPtr2SinglecastDelegate;
	StaticFuncStructPtr2SinglecastDelegate = MakeDelegate(&TestClass2::StaticFuncStructPtr2);
	StaticFuncStructPtr2SinglecastDelegate(&structParam, TEST_INT);

	SinglecastDelegate2<const StructParam*, INT> StaticFuncStructConstPtr2SinglecastDelegate;
	StaticFuncStructConstPtr2SinglecastDelegate = MakeDelegate(&TestClass2::StaticFuncStructConstPtr2);
	StaticFuncStructConstPtr2SinglecastDelegate(&structParam, TEST_INT);

	SinglecastDelegate2<StructParam&, INT> StaticFuncStructRef2SinglecastDelegate;
	StaticFuncStructRef2SinglecastDelegate = MakeDelegate(&TestClass2::StaticFuncStructRef2);
	StaticFuncStructRef2SinglecastDelegate(structParam, TEST_INT);

	SinglecastDelegate2<const StructParam&, INT> StaticFuncStructConstRef2SinglecastDelegate;
	StaticFuncStructConstRef2SinglecastDelegate = MakeDelegate(&TestClass2::StaticFuncStructConstRef2);
	StaticFuncStructConstRef2SinglecastDelegate(structParam, TEST_INT);

	// N=3 Free Functions
	SinglecastDelegate3<INT, INT, INT> FreeFuncInt3SinglecastDelegate;
	ASSERT_TRUE(FreeFuncInt3SinglecastDelegate.Empty() == true);
	ASSERT_TRUE(!FreeFuncInt3SinglecastDelegate);
	FreeFuncInt3SinglecastDelegate = MakeDelegate(&FreeFuncInt3);
	ASSERT_TRUE(FreeFuncInt3SinglecastDelegate.Empty() == false);
	ASSERT_TRUE(FreeFuncInt3SinglecastDelegate);
	FreeFuncInt3SinglecastDelegate(TEST_INT, TEST_INT, TEST_INT);
	FreeFuncInt3SinglecastDelegate.Clear();
	ASSERT_TRUE(!FreeFuncInt3SinglecastDelegate);

	SinglecastDelegate3<INT, INT, INT, INT> FreeFuncIntWithReturn3SinglecastDelegate;
	FreeFuncIntWithReturn3SinglecastDelegate = MakeDelegate(&FreeFuncIntWithReturn3);
	ASSERT_TRUE(FreeFuncIntWithReturn3SinglecastDelegate(TEST_INT, TEST_INT, TEST_INT) == TEST_INT);

	SinglecastDelegate3<StructParam, INT, INT> FreeFuncStruct3SinglecastDelegate;
	FreeFuncStruct3SinglecastDelegate = MakeDelegate(&FreeFuncStruct3);
	FreeFuncStruct3SinglecastDelegate(structParam, TEST_INT, TEST_INT);

	SinglecastDelegate3<StructParam**, INT, INT> FreeFuncPtrPtr3SinglecastDelegate;
	FreeFuncPtrPtr3SinglecastDelegate = MakeDelegate(&FreeFuncPtrPtr3);
	FreeFuncPtrPtr3SinglecastDelegate(&pStructParam, TEST_INT, TEST_INT);

	SinglecastDelegate3<StructParam*, INT, INT> FreeFuncStructPtr3SinglecastDelegate;
	FreeFuncStructPtr3SinglecastDelegate = MakeDelegate(&FreeFuncStructPtr3);
	FreeFuncStructPtr3SinglecastDelegate(&structParam, TEST_INT, TEST_INT);

	SinglecastDelegate3<const StructParam*, INT, INT> FreeFuncStructConstPtr3SinglecastDelegate;
	FreeFuncStructConstPtr3SinglecastDelegate = MakeDelegate(&FreeFuncStructConstPtr3);
	FreeFuncStructConstPtr3SinglecastDelegate(&structParam, TEST_INT, TEST_INT);

	SinglecastDelegate3<StructParam&, INT, INT> FreeFuncStructRef3SinglecastDelegate;
	FreeFuncStructRef3SinglecastDelegate = MakeDelegate(&FreeFuncStructRef3);
	FreeFuncStructRef3SinglecastDelegate(structParam, TEST_INT, TEST_INT);

	SinglecastDelegate3<const StructParam&, INT, INT> FreeFuncStructConstRef3SinglecastDelegate;
	FreeFuncStructConstRef3SinglecastDelegate = MakeDelegate(&FreeFuncStructConstRef3);
	FreeFuncStructConstRef3SinglecastDelegate(structParam, TEST_INT, TEST_INT);

	// N=3 Member Functions
	TestClass3 testClass3;

	SinglecastDelegate3<INT, INT, INT> MemberFuncInt3SinglecastDelegate;
	ASSERT_TRUE(MemberFuncInt3SinglecastDelegate.Empty() == true);
	ASSERT_TRUE(!MemberFuncInt3SinglecastDelegate);
	MemberFuncInt3SinglecastDelegate = MakeDelegate(&testClass3, &TestClass3::MemberFuncInt3);
	ASSERT_TRUE(MemberFuncInt3SinglecastDelegate.Empty() == false);
	ASSERT_TRUE(MemberFuncInt3SinglecastDelegate);
	MemberFuncInt3SinglecastDelegate(TEST_INT, TEST_INT, TEST_INT);
	MemberFuncInt3SinglecastDelegate.Clear();
	ASSERT_TRUE(!MemberFuncInt3SinglecastDelegate);

	SinglecastDelegate3<INT, INT, INT, INT> MemberFuncIntWithReturn3SinglecastDelegate;
	MemberFuncIntWithReturn3SinglecastDelegate = MakeDelegate(&testClass3, &TestClass3::MemberFuncIntWithReturn3);
	ASSERT_TRUE(MemberFuncIntWithReturn3SinglecastDelegate(TEST_INT, TEST_INT, TEST_INT) == TEST_INT);

	SinglecastDelegate3<StructParam, INT, INT> MemberFuncStruct3SinglecastDelegate;
	MemberFuncStruct3SinglecastDelegate = MakeDelegate(&testClass3, &TestClass3::MemberFuncStruct3);
	MemberFuncStruct3SinglecastDelegate(structParam, TEST_INT, TEST_INT);

	SinglecastDelegate3<StructParam*, INT, INT> MemberFuncStructPtr3SinglecastDelegate;
	MemberFuncStructPtr3SinglecastDelegate = MakeDelegate(&testClass3, &TestClass3::MemberFuncStructPtr3);
	MemberFuncStructPtr3SinglecastDelegate(&structParam, TEST_INT, TEST_INT);

	SinglecastDelegate3<const StructParam*, INT, INT> MemberFuncStructConstPtr3SinglecastDelegate;
	MemberFuncStructConstPtr3SinglecastDelegate = MakeDelegate(&testClass3, &TestClass3::MemberFuncStructConstPtr3);
	MemberFuncStructConstPtr3SinglecastDelegate(&structParam, TEST_INT, TEST_INT);

	SinglecastDelegate3<StructParam&, INT, INT> MemberFuncStructRef3SinglecastDelegate;
	MemberFuncStructRef3SinglecastDelegate = MakeDelegate(&testClass3, &TestClass3::MemberFuncStructRef3);
	MemberFuncStructRef3SinglecastDelegate(structParam, TEST_INT, TEST_INT);

	SinglecastDelegate3<const StructParam&, INT, INT> MemberFuncStructConstRef3SinglecastDelegate;
	MemberFuncStructConstRef3SinglecastDelegate = MakeDelegate(&testClass3, &TestClass3::MemberFuncStructConstRef3);
	MemberFuncStructConstRef3SinglecastDelegate(structParam, TEST_INT, TEST_INT);

	// N=3 Static Functions
	SinglecastDelegate3<INT, INT, INT> StaticFuncInt3SinglecastDelegate;
	StaticFuncInt3SinglecastDelegate = MakeDelegate(&TestClass3::StaticFuncInt3);
	StaticFuncInt3SinglecastDelegate(TEST_INT, TEST_INT, TEST_INT);

	SinglecastDelegate3<StructParam, INT, INT> StaticFuncStruct3SinglecastDelegate;
	StaticFuncStruct3SinglecastDelegate = MakeDelegate(&TestClass3::StaticFuncStruct3);
	StaticFuncStruct3SinglecastDelegate(structParam, TEST_INT, TEST_INT);

	SinglecastDelegate3<StructParam*, INT, INT> StaticFuncStructPtr3SinglecastDelegate;
	StaticFuncStructPtr3SinglecastDelegate = MakeDelegate(&TestClass3::StaticFuncStructPtr3);
	StaticFuncStructPtr3SinglecastDelegate(&structParam, TEST_INT, TEST_INT);

	SinglecastDelegate3<const StructParam*, INT, INT> StaticFuncStructConstPtr3SinglecastDelegate;
	StaticFuncStructConstPtr3SinglecastDelegate = MakeDelegate(&TestClass3::StaticFuncStructConstPtr3);
	StaticFuncStructConstPtr3SinglecastDelegate(&structParam, TEST_INT, TEST_INT);

	SinglecastDelegate3<StructParam&, INT, INT> StaticFuncStructRef3SinglecastDelegate;
	StaticFuncStructRef3SinglecastDelegate = MakeDelegate(&TestClass3::StaticFuncStructRef3);
	StaticFuncStructRef3SinglecastDelegate(structParam, TEST_INT, TEST_INT);

	SinglecastDelegate3<const StructParam&, INT, INT> StaticFuncStructConstRef3SinglecastDelegate;
	StaticFuncStructConstRef3SinglecastDelegate = MakeDelegate(&TestClass3::StaticFuncStructConstRef3);
	StaticFuncStructConstRef3SinglecastDelegate(structParam, TEST_INT, TEST_INT);

	// N=4 Free Functions
	SinglecastDelegate4<INT, INT, INT, INT> FreeFuncInt4SinglecastDelegate;
	ASSERT_TRUE(FreeFuncInt4SinglecastDelegate.Empty() == true);
	ASSERT_TRUE(!FreeFuncInt4SinglecastDelegate);
	FreeFuncInt4SinglecastDelegate = MakeDelegate(&FreeFuncInt4);
	ASSERT_TRUE(FreeFuncInt4SinglecastDelegate.Empty() == false);
	ASSERT_TRUE(FreeFuncInt4SinglecastDelegate);
	FreeFuncInt4SinglecastDelegate(TEST_INT, TEST_INT, TEST_INT, TEST_INT);
	FreeFuncInt4SinglecastDelegate.Clear();
	ASSERT_TRUE(!FreeFuncInt4SinglecastDelegate);

	SinglecastDelegate4<INT, INT, INT, INT, INT> FreeFuncIntWithReturn4SinglecastDelegate;
	FreeFuncIntWithReturn4SinglecastDelegate = MakeDelegate(&FreeFuncIntWithReturn4);
	ASSERT_TRUE(FreeFuncIntWithReturn4SinglecastDelegate(TEST_INT, TEST_INT, TEST_INT, TEST_INT) == TEST_INT);

	SinglecastDelegate4<StructParam, INT, INT, INT> FreeFuncStruct4SinglecastDelegate;
	FreeFuncStruct4SinglecastDelegate = MakeDelegate(&FreeFuncStruct4);
	FreeFuncStruct4SinglecastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT);

	SinglecastDelegate4<StructParam**, INT, INT, INT> FreeFuncPtrPtr4SinglecastDelegate;
	FreeFuncPtrPtr4SinglecastDelegate = MakeDelegate(&FreeFuncPtrPtr4);
	FreeFuncPtrPtr4SinglecastDelegate(&pStructParam, TEST_INT, TEST_INT, TEST_INT);

	SinglecastDelegate4<StructParam*, INT, INT, INT> FreeFuncStructPtr4SinglecastDelegate;
	FreeFuncStructPtr4SinglecastDelegate = MakeDelegate(&FreeFuncStructPtr4);
	FreeFuncStructPtr4SinglecastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT);

	SinglecastDelegate4<const StructParam*, INT, INT, INT> FreeFuncStructConstPtr4SinglecastDelegate;
	FreeFuncStructConstPtr4SinglecastDelegate = MakeDelegate(&FreeFuncStructConstPtr4);
	FreeFuncStructConstPtr4SinglecastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT);

	SinglecastDelegate4<StructParam&, INT, INT, INT> FreeFuncStructRef4SinglecastDelegate;
	FreeFuncStructRef4SinglecastDelegate = MakeDelegate(&FreeFuncStructRef4);
	FreeFuncStructRef4SinglecastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT);

	SinglecastDelegate4<const StructParam&, INT, INT, INT> FreeFuncStructConstRef4SinglecastDelegate;
	FreeFuncStructConstRef4SinglecastDelegate = MakeDelegate(&FreeFuncStructConstRef4);
	FreeFuncStructConstRef4SinglecastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT);

	// N=4 Member Functions
	TestClass4 testClass4;

	SinglecastDelegate4<INT, INT, INT, INT> MemberFuncInt4SinglecastDelegate;
	ASSERT_TRUE(MemberFuncInt4SinglecastDelegate.Empty() == true);
	ASSERT_TRUE(!MemberFuncInt4SinglecastDelegate);
	MemberFuncInt4SinglecastDelegate = MakeDelegate(&testClass4, &TestClass4::MemberFuncInt4);
	ASSERT_TRUE(MemberFuncInt4SinglecastDelegate.Empty() == false);
	ASSERT_TRUE(MemberFuncInt4SinglecastDelegate);
	MemberFuncInt4SinglecastDelegate(TEST_INT, TEST_INT, TEST_INT, TEST_INT);
	MemberFuncInt4SinglecastDelegate.Clear();
	ASSERT_TRUE(!MemberFuncInt4SinglecastDelegate);

	SinglecastDelegate4<INT, INT, INT, INT, INT> MemberFuncIntWithReturn4SinglecastDelegate;
	MemberFuncIntWithReturn4SinglecastDelegate = MakeDelegate(&testClass4, &TestClass4::MemberFuncIntWithReturn4);
	ASSERT_TRUE(MemberFuncIntWithReturn4SinglecastDelegate(TEST_INT, TEST_INT, TEST_INT, TEST_INT) == TEST_INT);

	SinglecastDelegate4<StructParam, INT, INT, INT> MemberFuncStruct4SinglecastDelegate;
	MemberFuncStruct4SinglecastDelegate = MakeDelegate(&testClass4, &TestClass4::MemberFuncStruct4);
	MemberFuncStruct4SinglecastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT);

	SinglecastDelegate4<StructParam*, INT, INT, INT> MemberFuncStructPtr4SinglecastDelegate;
	MemberFuncStructPtr4SinglecastDelegate = MakeDelegate(&testClass4, &TestClass4::MemberFuncStructPtr4);
	MemberFuncStructPtr4SinglecastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT);

	SinglecastDelegate4<const StructParam*, INT, INT, INT> MemberFuncStructConstPtr4SinglecastDelegate;
	MemberFuncStructConstPtr4SinglecastDelegate = MakeDelegate(&testClass4, &TestClass4::MemberFuncStructConstPtr4);
	MemberFuncStructConstPtr4SinglecastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT);

	SinglecastDelegate4<StructParam&, INT, INT, INT> MemberFuncStructRef4SinglecastDelegate;
	MemberFuncStructRef4SinglecastDelegate = MakeDelegate(&testClass4, &TestClass4::MemberFuncStructRef4);
	MemberFuncStructRef4SinglecastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT);

	SinglecastDelegate4<const StructParam&, INT, INT, INT> MemberFuncStructConstRef4SinglecastDelegate;
	MemberFuncStructConstRef4SinglecastDelegate = MakeDelegate(&testClass4, &TestClass4::MemberFuncStructConstRef4);
	MemberFuncStructConstRef4SinglecastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT);

	// N=4 Static Functions
	SinglecastDelegate4<INT, INT, INT, INT> StaticFuncInt4SinglecastDelegate;
	StaticFuncInt4SinglecastDelegate = MakeDelegate(&TestClass4::StaticFuncInt4);
	StaticFuncInt4SinglecastDelegate(TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	SinglecastDelegate4<StructParam, INT, INT, INT> StaticFuncStruct4SinglecastDelegate;
	StaticFuncStruct4SinglecastDelegate = MakeDelegate(&TestClass4::StaticFuncStruct4);
	StaticFuncStruct4SinglecastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT);

	SinglecastDelegate4<StructParam*, INT, INT, INT> StaticFuncStructPtr4SinglecastDelegate;
	StaticFuncStructPtr4SinglecastDelegate = MakeDelegate(&TestClass4::StaticFuncStructPtr4);
	StaticFuncStructPtr4SinglecastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT);

	SinglecastDelegate4<const StructParam*, INT, INT, INT> StaticFuncStructConstPtr4SinglecastDelegate;
	StaticFuncStructConstPtr4SinglecastDelegate = MakeDelegate(&TestClass4::StaticFuncStructConstPtr4);
	StaticFuncStructConstPtr4SinglecastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT);

	SinglecastDelegate4<StructParam&, INT, INT, INT> StaticFuncStructRef4SinglecastDelegate;
	StaticFuncStructRef4SinglecastDelegate = MakeDelegate(&TestClass4::StaticFuncStructRef4);
	StaticFuncStructRef4SinglecastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT);

	SinglecastDelegate4<const StructParam&, INT, INT, INT> StaticFuncStructConstRef4SinglecastDelegate;
	StaticFuncStructConstRef4SinglecastDelegate = MakeDelegate(&TestClass4::StaticFuncStructConstRef4);
	StaticFuncStructConstRef4SinglecastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT);

	// N=5 Free Functions
	SinglecastDelegate5<INT, INT, INT, INT, INT> FreeFuncInt5SinglecastDelegate;
	ASSERT_TRUE(FreeFuncInt5SinglecastDelegate.Empty() == true);
	ASSERT_TRUE(!FreeFuncInt5SinglecastDelegate);
	FreeFuncInt5SinglecastDelegate = MakeDelegate(&FreeFuncInt5);
	ASSERT_TRUE(FreeFuncInt5SinglecastDelegate.Empty() == false);
	ASSERT_TRUE(FreeFuncInt5SinglecastDelegate);
	FreeFuncInt5SinglecastDelegate(TEST_INT, TEST_INT, TEST_INT, TEST_INT, TEST_INT);
	FreeFuncInt5SinglecastDelegate.Clear();
	ASSERT_TRUE(!FreeFuncInt5SinglecastDelegate);

	SinglecastDelegate5<INT, INT, INT, INT, INT, INT> FreeFuncIntWithReturn5SinglecastDelegate;
	FreeFuncIntWithReturn5SinglecastDelegate = MakeDelegate(&FreeFuncIntWithReturn5);
	ASSERT_TRUE(FreeFuncIntWithReturn5SinglecastDelegate(TEST_INT, TEST_INT, TEST_INT, TEST_INT, TEST_INT) == TEST_INT);

	SinglecastDelegate5<StructParam, INT, INT, INT, INT> FreeFuncStruct5SinglecastDelegate;
	FreeFuncStruct5SinglecastDelegate = MakeDelegate(&FreeFuncStruct5);
	FreeFuncStruct5SinglecastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	SinglecastDelegate5<StructParam**, INT, INT, INT, INT> FreeFuncPtrPtr5SinglecastDelegate;
	FreeFuncPtrPtr5SinglecastDelegate = MakeDelegate(&FreeFuncPtrPtr5);
	FreeFuncPtrPtr5SinglecastDelegate(&pStructParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	SinglecastDelegate5<StructParam*, INT, INT, INT, INT> FreeFuncStructPtr5SinglecastDelegate;
	FreeFuncStructPtr5SinglecastDelegate = MakeDelegate(&FreeFuncStructPtr5);
	FreeFuncStructPtr5SinglecastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	SinglecastDelegate5<const StructParam*, INT, INT, INT, INT> FreeFuncStructConstPtr5SinglecastDelegate;
	FreeFuncStructConstPtr5SinglecastDelegate = MakeDelegate(&FreeFuncStructConstPtr5);
	FreeFuncStructConstPtr5SinglecastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	SinglecastDelegate5<StructParam&, INT, INT, INT, INT> FreeFuncStructRef5SinglecastDelegate;
	FreeFuncStructRef5SinglecastDelegate = MakeDelegate(&FreeFuncStructRef5);
	FreeFuncStructRef5SinglecastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	SinglecastDelegate5<const StructParam&, INT, INT, INT, INT> FreeFuncStructConstRef5SinglecastDelegate;
	FreeFuncStructConstRef5SinglecastDelegate = MakeDelegate(&FreeFuncStructConstRef5);
	FreeFuncStructConstRef5SinglecastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	// N=5 Member Functions
	TestClass5 testClass5;

	SinglecastDelegate5<INT, INT, INT, INT, INT> MemberFuncInt5SinglecastDelegate;
	ASSERT_TRUE(MemberFuncInt5SinglecastDelegate.Empty() == true);
	ASSERT_TRUE(!MemberFuncInt5SinglecastDelegate);
	MemberFuncInt5SinglecastDelegate = MakeDelegate(&testClass5, &TestClass5::MemberFuncInt5);
	ASSERT_TRUE(MemberFuncInt5SinglecastDelegate.Empty() == false);
	ASSERT_TRUE(MemberFuncInt5SinglecastDelegate);
	MemberFuncInt5SinglecastDelegate(TEST_INT, TEST_INT, TEST_INT, TEST_INT, TEST_INT);
	MemberFuncInt5SinglecastDelegate.Clear();
	ASSERT_TRUE(!MemberFuncInt5SinglecastDelegate);

	SinglecastDelegate5<INT, INT, INT, INT, INT, INT> MemberFuncIntWithReturn5SinglecastDelegate;
	MemberFuncIntWithReturn5SinglecastDelegate = MakeDelegate(&testClass5, &TestClass5::MemberFuncIntWithReturn5);
	ASSERT_TRUE(MemberFuncIntWithReturn5SinglecastDelegate(TEST_INT, TEST_INT, TEST_INT, TEST_INT, TEST_INT) == TEST_INT);

	SinglecastDelegate5<StructParam, INT, INT, INT, INT> MemberFuncStruct5SinglecastDelegate;
	MemberFuncStruct5SinglecastDelegate = MakeDelegate(&testClass5, &TestClass5::MemberFuncStruct5);
	MemberFuncStruct5SinglecastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	SinglecastDelegate5<StructParam*, INT, INT, INT, INT> MemberFuncStructPtr5SinglecastDelegate;
	MemberFuncStructPtr5SinglecastDelegate = MakeDelegate(&testClass5, &TestClass5::MemberFuncStructPtr5);
	MemberFuncStructPtr5SinglecastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	SinglecastDelegate5<const StructParam*, INT, INT, INT, INT> MemberFuncStructConstPtr5SinglecastDelegate;
	MemberFuncStructConstPtr5SinglecastDelegate = MakeDelegate(&testClass5, &TestClass5::MemberFuncStructConstPtr5);
	MemberFuncStructConstPtr5SinglecastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	SinglecastDelegate5<StructParam&, INT, INT, INT, INT> MemberFuncStructRef5SinglecastDelegate;
	MemberFuncStructRef5SinglecastDelegate = MakeDelegate(&testClass5, &TestClass5::MemberFuncStructRef5);
	MemberFuncStructRef5SinglecastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	SinglecastDelegate5<const StructParam&, INT, INT, INT, INT> MemberFuncStructConstRef5SinglecastDelegate;
	MemberFuncStructConstRef5SinglecastDelegate = MakeDelegate(&testClass5, &TestClass5::MemberFuncStructConstRef5);
	MemberFuncStructConstRef5SinglecastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	// N=5 Static Functions
	SinglecastDelegate5<INT, INT, INT, INT, INT> StaticFuncInt5SinglecastDelegate;
	StaticFuncInt5SinglecastDelegate = MakeDelegate(&TestClass5::StaticFuncInt5);
	StaticFuncInt5SinglecastDelegate(TEST_INT, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	SinglecastDelegate5<StructParam, INT, INT, INT, INT> StaticFuncStruct5SinglecastDelegate;
	StaticFuncStruct5SinglecastDelegate = MakeDelegate(&TestClass5::StaticFuncStruct5);
	StaticFuncStruct5SinglecastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	SinglecastDelegate5<StructParam*, INT, INT, INT, INT> StaticFuncStructPtr5SinglecastDelegate;
	StaticFuncStructPtr5SinglecastDelegate = MakeDelegate(&TestClass5::StaticFuncStructPtr5);
	StaticFuncStructPtr5SinglecastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	SinglecastDelegate5<const StructParam*, INT, INT, INT, INT> StaticFuncStructConstPtr5SinglecastDelegate;
	StaticFuncStructConstPtr5SinglecastDelegate = MakeDelegate(&TestClass5::StaticFuncStructConstPtr5);
	StaticFuncStructConstPtr5SinglecastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	SinglecastDelegate5<StructParam&, INT, INT, INT, INT> StaticFuncStructRef5SinglecastDelegate;
	StaticFuncStructRef5SinglecastDelegate = MakeDelegate(&TestClass5::StaticFuncStructRef5);
	StaticFuncStructRef5SinglecastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	SinglecastDelegate5<const StructParam&, INT, INT, INT, INT> StaticFuncStructConstRef5SinglecastDelegate;
	StaticFuncStructConstRef5SinglecastDelegate = MakeDelegate(&TestClass5::StaticFuncStructConstRef5);
	StaticFuncStructConstRef5SinglecastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);
}

void MulticastDelegateTests()
{
	StructParam structParam;
	structParam.val = TEST_INT;

	// N=0 Free Functions
	MulticastDelegate0 FreeFunc0MulticastDelegate;
	ASSERT_TRUE(FreeFunc0MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!FreeFunc0MulticastDelegate);
	FreeFunc0MulticastDelegate += MakeDelegate(&FreeFunc0);
	FreeFunc0MulticastDelegate += MakeDelegate(&FreeFunc0);
	ASSERT_TRUE(FreeFunc0MulticastDelegate.Empty() == false);
	ASSERT_TRUE(FreeFunc0MulticastDelegate);
	FreeFunc0MulticastDelegate();
	FreeFunc0MulticastDelegate -= MakeDelegate(&FreeFunc0);
	ASSERT_TRUE(FreeFunc0MulticastDelegate);
	FreeFunc0MulticastDelegate.Clear();
	ASSERT_TRUE(!FreeFunc0MulticastDelegate);

	// N=0 Member Functions
	TestClass0 testClass0;

	MulticastDelegate0 MemberFunc0MulticastDelegate;
	ASSERT_TRUE(MemberFunc0MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!MemberFunc0MulticastDelegate);
	MemberFunc0MulticastDelegate += MakeDelegate(&testClass0, &TestClass0::MemberFunc0);
	ASSERT_TRUE(MemberFunc0MulticastDelegate.Empty() == false);
	ASSERT_TRUE(MemberFunc0MulticastDelegate);
	MemberFunc0MulticastDelegate();
	MemberFunc0MulticastDelegate.Clear();
	ASSERT_TRUE(!MemberFunc0MulticastDelegate);

	// N=0 Static Functions
	MulticastDelegate0 StaticFunc0MulticastDelegate;
	StaticFunc0MulticastDelegate += MakeDelegate(&TestClass0::StaticFunc0);
	StaticFunc0MulticastDelegate();

	// N=1 Free Functions
	MulticastDelegate1<INT> FreeFuncInt1MulticastDelegate;
	ASSERT_TRUE(FreeFuncInt1MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!FreeFuncInt1MulticastDelegate);
	FreeFuncInt1MulticastDelegate += MakeDelegate(&FreeFuncInt1);
	FreeFuncInt1MulticastDelegate += MakeDelegate(&FreeFuncInt1);
	ASSERT_TRUE(FreeFuncInt1MulticastDelegate.Empty() == false);
	ASSERT_TRUE(FreeFuncInt1MulticastDelegate);
	FreeFuncInt1MulticastDelegate(TEST_INT);
	FreeFuncInt1MulticastDelegate -= MakeDelegate(&FreeFuncInt1);
	ASSERT_TRUE(FreeFuncInt1MulticastDelegate);
	FreeFuncInt1MulticastDelegate.Clear();
	ASSERT_TRUE(!FreeFuncInt1MulticastDelegate);

	MulticastDelegate1<StructParam> FreeFuncStruct1MulticastDelegate;
	FreeFuncStruct1MulticastDelegate += MakeDelegate(&FreeFuncStruct1);
	FreeFuncStruct1MulticastDelegate(structParam);

	MulticastDelegate1<StructParam*> FreeFuncStructPtr1MulticastDelegate;
	FreeFuncStructPtr1MulticastDelegate += MakeDelegate(&FreeFuncStructPtr1);
	FreeFuncStructPtr1MulticastDelegate(&structParam);

	MulticastDelegate1<const StructParam*> FreeFuncStructConstPtr1MulticastDelegate;
	FreeFuncStructConstPtr1MulticastDelegate += MakeDelegate(&FreeFuncStructConstPtr1);
	FreeFuncStructConstPtr1MulticastDelegate(&structParam);

	MulticastDelegate1<StructParam&> FreeFuncStructRef1MulticastDelegate;
	FreeFuncStructRef1MulticastDelegate += MakeDelegate(&FreeFuncStructRef1);
	FreeFuncStructRef1MulticastDelegate(structParam);

	MulticastDelegate1<const StructParam&> FreeFuncStructConstRef1MulticastDelegate;
	FreeFuncStructConstRef1MulticastDelegate += MakeDelegate(&FreeFuncStructConstRef1);
	FreeFuncStructConstRef1MulticastDelegate(structParam);

	// N=1 Member Functions
	TestClass1 testClass1;

	MulticastDelegate1<INT> MemberFuncInt1MulticastDelegate;
	ASSERT_TRUE(MemberFuncInt1MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!MemberFuncInt1MulticastDelegate);
	MemberFuncInt1MulticastDelegate += MakeDelegate(&testClass1, &TestClass1::MemberFuncInt1);
	ASSERT_TRUE(MemberFuncInt1MulticastDelegate.Empty() == false);
	ASSERT_TRUE(MemberFuncInt1MulticastDelegate);
	MemberFuncInt1MulticastDelegate(TEST_INT);
	MemberFuncInt1MulticastDelegate.Clear();
	ASSERT_TRUE(!MemberFuncInt1MulticastDelegate);

	MulticastDelegate1<StructParam> MemberFuncStruct1MulticastDelegate;
	MemberFuncStruct1MulticastDelegate += MakeDelegate(&testClass1, &TestClass1::MemberFuncStruct1);
	MemberFuncStruct1MulticastDelegate(structParam);

	MulticastDelegate1<StructParam*> MemberFuncStructPtr1MulticastDelegate;
	MemberFuncStructPtr1MulticastDelegate += MakeDelegate(&testClass1, &TestClass1::MemberFuncStructPtr1);
	MemberFuncStructPtr1MulticastDelegate(&structParam);

	MulticastDelegate1<const StructParam*> MemberFuncStructConstPtr1MulticastDelegate;
	MemberFuncStructConstPtr1MulticastDelegate += MakeDelegate(&testClass1, &TestClass1::MemberFuncStructConstPtr1);
	MemberFuncStructConstPtr1MulticastDelegate(&structParam);

	MulticastDelegate1<StructParam&> MemberFuncStructRef1MulticastDelegate;
	MemberFuncStructRef1MulticastDelegate += MakeDelegate(&testClass1, &TestClass1::MemberFuncStructRef1);
	MemberFuncStructRef1MulticastDelegate(structParam);

	MulticastDelegate1<const StructParam&> MemberFuncStructConstRef1MulticastDelegate;
	MemberFuncStructConstRef1MulticastDelegate += MakeDelegate(&testClass1, &TestClass1::MemberFuncStructConstRef1);
	MemberFuncStructConstRef1MulticastDelegate(structParam);

	// N=1 Static Functions
	MulticastDelegate1<INT> StaticFuncInt1MulticastDelegate;
	StaticFuncInt1MulticastDelegate += MakeDelegate(&TestClass1::StaticFuncInt1);
	StaticFuncInt1MulticastDelegate(TEST_INT);

	MulticastDelegate1<StructParam> StaticFuncStruct1MulticastDelegate;
	StaticFuncStruct1MulticastDelegate += MakeDelegate(&TestClass1::StaticFuncStruct1);
	StaticFuncStruct1MulticastDelegate(structParam);

	MulticastDelegate1<StructParam*> StaticFuncStructPtr1MulticastDelegate;
	StaticFuncStructPtr1MulticastDelegate += MakeDelegate(&TestClass1::StaticFuncStructPtr1);
	StaticFuncStructPtr1MulticastDelegate(&structParam);

	MulticastDelegate1<const StructParam*> StaticFuncStructConstPtr1MulticastDelegate;
	StaticFuncStructConstPtr1MulticastDelegate += MakeDelegate(&TestClass1::StaticFuncStructConstPtr1);
	StaticFuncStructConstPtr1MulticastDelegate(&structParam);

	MulticastDelegate1<StructParam&> StaticFuncStructRef1MulticastDelegate;
	StaticFuncStructRef1MulticastDelegate += MakeDelegate(&TestClass1::StaticFuncStructRef1);
	StaticFuncStructRef1MulticastDelegate(structParam);

	MulticastDelegate1<const StructParam&> StaticFuncStructConstRef1MulticastDelegate;
	StaticFuncStructConstRef1MulticastDelegate += MakeDelegate(&TestClass1::StaticFuncStructConstRef1);
	StaticFuncStructConstRef1MulticastDelegate(structParam);

	// N=2 Free Functions
	MulticastDelegate2<INT, INT> FreeFuncInt2MulticastDelegate;
	ASSERT_TRUE(FreeFuncInt2MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!FreeFuncInt2MulticastDelegate);
	FreeFuncInt2MulticastDelegate += MakeDelegate(&FreeFuncInt2);
	FreeFuncInt2MulticastDelegate += MakeDelegate(&FreeFuncInt2);
	ASSERT_TRUE(FreeFuncInt2MulticastDelegate.Empty() == false);
	ASSERT_TRUE(FreeFuncInt2MulticastDelegate);
	FreeFuncInt2MulticastDelegate(TEST_INT, TEST_INT);
	FreeFuncInt2MulticastDelegate -= MakeDelegate(&FreeFuncInt2);
	ASSERT_TRUE(FreeFuncInt2MulticastDelegate);
	FreeFuncInt2MulticastDelegate.Clear();
	ASSERT_TRUE(!FreeFuncInt2MulticastDelegate);

	MulticastDelegate2<StructParam, INT> FreeFuncStruct2MulticastDelegate;
	FreeFuncStruct2MulticastDelegate += MakeDelegate(&FreeFuncStruct2);
	FreeFuncStruct2MulticastDelegate(structParam, TEST_INT);

	MulticastDelegate2<StructParam*, INT> FreeFuncStructPtr2MulticastDelegate;
	FreeFuncStructPtr2MulticastDelegate += MakeDelegate(&FreeFuncStructPtr2);
	FreeFuncStructPtr2MulticastDelegate(&structParam, TEST_INT);

	MulticastDelegate2<const StructParam*, INT> FreeFuncStructConstPtr2MulticastDelegate;
	FreeFuncStructConstPtr2MulticastDelegate += MakeDelegate(&FreeFuncStructConstPtr2);
	FreeFuncStructConstPtr2MulticastDelegate(&structParam, TEST_INT);

	MulticastDelegate2<StructParam&, INT> FreeFuncStructRef2MulticastDelegate;
	FreeFuncStructRef2MulticastDelegate += MakeDelegate(&FreeFuncStructRef2);
	FreeFuncStructRef2MulticastDelegate(structParam, TEST_INT);

	MulticastDelegate2<const StructParam&, INT> FreeFuncStructConstRef2MulticastDelegate;
	FreeFuncStructConstRef2MulticastDelegate += MakeDelegate(&FreeFuncStructConstRef2);
	FreeFuncStructConstRef2MulticastDelegate(structParam, TEST_INT);

	// N=2 Member Functions
	TestClass2 testClass2;

	MulticastDelegate2<INT, INT> MemberFuncInt2MulticastDelegate;
	ASSERT_TRUE(MemberFuncInt2MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!MemberFuncInt2MulticastDelegate);
	MemberFuncInt2MulticastDelegate += MakeDelegate(&testClass2, &TestClass2::MemberFuncInt2);
	ASSERT_TRUE(MemberFuncInt2MulticastDelegate.Empty() == false);
	ASSERT_TRUE(MemberFuncInt2MulticastDelegate);
	MemberFuncInt2MulticastDelegate(TEST_INT, TEST_INT);
	MemberFuncInt2MulticastDelegate.Clear();
	ASSERT_TRUE(!MemberFuncInt2MulticastDelegate);

	MulticastDelegate2<StructParam, INT> MemberFuncStruct2MulticastDelegate;
	MemberFuncStruct2MulticastDelegate += MakeDelegate(&testClass2, &TestClass2::MemberFuncStruct2);
	MemberFuncStruct2MulticastDelegate(structParam, TEST_INT);

	MulticastDelegate2<StructParam*, INT> MemberFuncStructPtr2MulticastDelegate;
	MemberFuncStructPtr2MulticastDelegate += MakeDelegate(&testClass2, &TestClass2::MemberFuncStructPtr2);
	MemberFuncStructPtr2MulticastDelegate(&structParam, TEST_INT);

	MulticastDelegate2<const StructParam*, INT> MemberFuncStructConstPtr2MulticastDelegate;
	MemberFuncStructConstPtr2MulticastDelegate += MakeDelegate(&testClass2, &TestClass2::MemberFuncStructConstPtr2);
	MemberFuncStructConstPtr2MulticastDelegate(&structParam, TEST_INT);

	MulticastDelegate2<StructParam&, INT> MemberFuncStructRef2MulticastDelegate;
	MemberFuncStructRef2MulticastDelegate += MakeDelegate(&testClass2, &TestClass2::MemberFuncStructRef2);
	MemberFuncStructRef2MulticastDelegate(structParam, TEST_INT);

	MulticastDelegate2<const StructParam&, INT> MemberFuncStructConstRef2MulticastDelegate;
	MemberFuncStructConstRef2MulticastDelegate += MakeDelegate(&testClass2, &TestClass2::MemberFuncStructConstRef2);
	MemberFuncStructConstRef2MulticastDelegate(structParam, TEST_INT);

	// N=2 Static Functions
	MulticastDelegate2<INT, INT> StaticFuncInt2MulticastDelegate;
	StaticFuncInt2MulticastDelegate += MakeDelegate(&TestClass2::StaticFuncInt2);
	StaticFuncInt2MulticastDelegate(TEST_INT, TEST_INT);

	MulticastDelegate2<StructParam, INT> StaticFuncStruct2MulticastDelegate;
	StaticFuncStruct2MulticastDelegate += MakeDelegate(&TestClass2::StaticFuncStruct2);
	StaticFuncStruct2MulticastDelegate(structParam, TEST_INT);

	MulticastDelegate2<StructParam*, INT> StaticFuncStructPtr2MulticastDelegate;
	StaticFuncStructPtr2MulticastDelegate += MakeDelegate(&TestClass2::StaticFuncStructPtr2);
	StaticFuncStructPtr2MulticastDelegate(&structParam, TEST_INT);

	MulticastDelegate2<const StructParam*, INT> StaticFuncStructConstPtr2MulticastDelegate;
	StaticFuncStructConstPtr2MulticastDelegate += MakeDelegate(&TestClass2::StaticFuncStructConstPtr2);
	StaticFuncStructConstPtr2MulticastDelegate(&structParam, TEST_INT);

	MulticastDelegate2<StructParam&, INT> StaticFuncStructRef2MulticastDelegate;
	StaticFuncStructRef2MulticastDelegate += MakeDelegate(&TestClass2::StaticFuncStructRef2);
	StaticFuncStructRef2MulticastDelegate(structParam, TEST_INT);

	MulticastDelegate2<const StructParam&, INT> StaticFuncStructConstRef2MulticastDelegate;
	StaticFuncStructConstRef2MulticastDelegate += MakeDelegate(&TestClass2::StaticFuncStructConstRef2);
	StaticFuncStructConstRef2MulticastDelegate(structParam, TEST_INT);

	// N=3 Free Functions
	MulticastDelegate3<INT, INT, INT> FreeFuncInt3MulticastDelegate;
	ASSERT_TRUE(FreeFuncInt3MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!FreeFuncInt3MulticastDelegate);
	FreeFuncInt3MulticastDelegate += MakeDelegate(&FreeFuncInt3);
	FreeFuncInt3MulticastDelegate += MakeDelegate(&FreeFuncInt3);
	ASSERT_TRUE(FreeFuncInt3MulticastDelegate.Empty() == false);
	ASSERT_TRUE(FreeFuncInt3MulticastDelegate);
	FreeFuncInt3MulticastDelegate(TEST_INT, TEST_INT, TEST_INT);
	FreeFuncInt3MulticastDelegate -= MakeDelegate(&FreeFuncInt3);
	ASSERT_TRUE(FreeFuncInt3MulticastDelegate);
	FreeFuncInt3MulticastDelegate.Clear();
	ASSERT_TRUE(!FreeFuncInt3MulticastDelegate);

	MulticastDelegate3<StructParam, INT, INT> FreeFuncStruct3MulticastDelegate;
	FreeFuncStruct3MulticastDelegate += MakeDelegate(&FreeFuncStruct3);
	FreeFuncStruct3MulticastDelegate(structParam, TEST_INT, TEST_INT);

	MulticastDelegate3<StructParam*, INT, INT> FreeFuncStructPtr3MulticastDelegate;
	FreeFuncStructPtr3MulticastDelegate += MakeDelegate(&FreeFuncStructPtr3);
	FreeFuncStructPtr3MulticastDelegate(&structParam, TEST_INT, TEST_INT);

	MulticastDelegate3<const StructParam*, INT, INT> FreeFuncStructConstPtr3MulticastDelegate;
	FreeFuncStructConstPtr3MulticastDelegate += MakeDelegate(&FreeFuncStructConstPtr3);
	FreeFuncStructConstPtr3MulticastDelegate(&structParam, TEST_INT, TEST_INT);

	MulticastDelegate3<StructParam&, INT, INT> FreeFuncStructRef3MulticastDelegate;
	FreeFuncStructRef3MulticastDelegate += MakeDelegate(&FreeFuncStructRef3);
	FreeFuncStructRef3MulticastDelegate(structParam, TEST_INT, TEST_INT);

	MulticastDelegate3<const StructParam&, INT, INT> FreeFuncStructConstRef3MulticastDelegate;
	FreeFuncStructConstRef3MulticastDelegate += MakeDelegate(&FreeFuncStructConstRef3);
	FreeFuncStructConstRef3MulticastDelegate(structParam, TEST_INT, TEST_INT);

	// N=3 Member Functions
	TestClass3 testClass3;

	MulticastDelegate3<INT, INT, INT> MemberFuncInt3MulticastDelegate;
	ASSERT_TRUE(MemberFuncInt3MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!MemberFuncInt3MulticastDelegate);
	MemberFuncInt3MulticastDelegate += MakeDelegate(&testClass3, &TestClass3::MemberFuncInt3);
	ASSERT_TRUE(MemberFuncInt3MulticastDelegate.Empty() == false);
	ASSERT_TRUE(MemberFuncInt3MulticastDelegate);
	MemberFuncInt3MulticastDelegate(TEST_INT, TEST_INT, TEST_INT);
	MemberFuncInt3MulticastDelegate.Clear();
	ASSERT_TRUE(!MemberFuncInt3MulticastDelegate);

	MulticastDelegate3<StructParam, INT, INT> MemberFuncStruct3MulticastDelegate;
	MemberFuncStruct3MulticastDelegate += MakeDelegate(&testClass3, &TestClass3::MemberFuncStruct3);
	MemberFuncStruct3MulticastDelegate(structParam, TEST_INT, TEST_INT);

	MulticastDelegate3<StructParam*, INT, INT> MemberFuncStructPtr3MulticastDelegate;
	MemberFuncStructPtr3MulticastDelegate += MakeDelegate(&testClass3, &TestClass3::MemberFuncStructPtr3);
	MemberFuncStructPtr3MulticastDelegate(&structParam, TEST_INT, TEST_INT);

	MulticastDelegate3<const StructParam*, INT, INT> MemberFuncStructConstPtr3MulticastDelegate;
	MemberFuncStructConstPtr3MulticastDelegate += MakeDelegate(&testClass3, &TestClass3::MemberFuncStructConstPtr3);
	MemberFuncStructConstPtr3MulticastDelegate(&structParam, TEST_INT, TEST_INT);

	MulticastDelegate3<StructParam&, INT, INT> MemberFuncStructRef3MulticastDelegate;
	MemberFuncStructRef3MulticastDelegate += MakeDelegate(&testClass3, &TestClass3::MemberFuncStructRef3);
	MemberFuncStructRef3MulticastDelegate(structParam, TEST_INT, TEST_INT);

	MulticastDelegate3<const StructParam&, INT, INT> MemberFuncStructConstRef3MulticastDelegate;
	MemberFuncStructConstRef3MulticastDelegate += MakeDelegate(&testClass3, &TestClass3::MemberFuncStructConstRef3);
	MemberFuncStructConstRef3MulticastDelegate(structParam, TEST_INT, TEST_INT);

	// N=3 Static Functions
	MulticastDelegate3<INT, INT, INT> StaticFuncInt3MulticastDelegate;
	StaticFuncInt3MulticastDelegate += MakeDelegate(&TestClass3::StaticFuncInt3);
	StaticFuncInt3MulticastDelegate(TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegate3<StructParam, INT, INT> StaticFuncStruct3MulticastDelegate;
	StaticFuncStruct3MulticastDelegate += MakeDelegate(&TestClass3::StaticFuncStruct3);
	StaticFuncStruct3MulticastDelegate(structParam, TEST_INT, TEST_INT);

	MulticastDelegate3<StructParam*, INT, INT> StaticFuncStructPtr3MulticastDelegate;
	StaticFuncStructPtr3MulticastDelegate += MakeDelegate(&TestClass3::StaticFuncStructPtr3);
	StaticFuncStructPtr3MulticastDelegate(&structParam, TEST_INT, TEST_INT);

	MulticastDelegate3<const StructParam*, INT, INT> StaticFuncStructConstPtr3MulticastDelegate;
	StaticFuncStructConstPtr3MulticastDelegate += MakeDelegate(&TestClass3::StaticFuncStructConstPtr3);
	StaticFuncStructConstPtr3MulticastDelegate(&structParam, TEST_INT, TEST_INT);

	MulticastDelegate3<StructParam&, INT, INT> StaticFuncStructRef3MulticastDelegate;
	StaticFuncStructRef3MulticastDelegate += MakeDelegate(&TestClass3::StaticFuncStructRef3);
	StaticFuncStructRef3MulticastDelegate(structParam, TEST_INT, TEST_INT);

	MulticastDelegate3<const StructParam&, INT, INT> StaticFuncStructConstRef3MulticastDelegate;
	StaticFuncStructConstRef3MulticastDelegate += MakeDelegate(&TestClass3::StaticFuncStructConstRef3);
	StaticFuncStructConstRef3MulticastDelegate(structParam, TEST_INT, TEST_INT);

	// N=4 Free Functions
	MulticastDelegate4<INT, INT, INT, INT> FreeFuncInt4MulticastDelegate;
	ASSERT_TRUE(FreeFuncInt4MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!FreeFuncInt4MulticastDelegate);
	FreeFuncInt4MulticastDelegate += MakeDelegate(&FreeFuncInt4);
	FreeFuncInt4MulticastDelegate += MakeDelegate(&FreeFuncInt4);
	ASSERT_TRUE(FreeFuncInt4MulticastDelegate.Empty() == false);
	ASSERT_TRUE(FreeFuncInt4MulticastDelegate);
	FreeFuncInt4MulticastDelegate(TEST_INT, TEST_INT, TEST_INT, TEST_INT);
	FreeFuncInt4MulticastDelegate -= MakeDelegate(&FreeFuncInt4);
	ASSERT_TRUE(FreeFuncInt4MulticastDelegate);
	FreeFuncInt4MulticastDelegate.Clear();
	ASSERT_TRUE(!FreeFuncInt4MulticastDelegate);

	MulticastDelegate4<StructParam, INT, INT, INT> FreeFuncStruct4MulticastDelegate;
	FreeFuncStruct4MulticastDelegate += MakeDelegate(&FreeFuncStruct4);
	FreeFuncStruct4MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegate4<StructParam*, INT, INT, INT> FreeFuncStructPtr4MulticastDelegate;
	FreeFuncStructPtr4MulticastDelegate += MakeDelegate(&FreeFuncStructPtr4);
	FreeFuncStructPtr4MulticastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegate4<const StructParam*, INT, INT, INT> FreeFuncStructConstPtr4MulticastDelegate;
	FreeFuncStructConstPtr4MulticastDelegate += MakeDelegate(&FreeFuncStructConstPtr4);
	FreeFuncStructConstPtr4MulticastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegate4<StructParam&, INT, INT, INT> FreeFuncStructRef4MulticastDelegate;
	FreeFuncStructRef4MulticastDelegate += MakeDelegate(&FreeFuncStructRef4);
	FreeFuncStructRef4MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegate4<const StructParam&, INT, INT, INT> FreeFuncStructConstRef4MulticastDelegate;
	FreeFuncStructConstRef4MulticastDelegate += MakeDelegate(&FreeFuncStructConstRef4);
	FreeFuncStructConstRef4MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT);

	// N=4 Member Functions
	TestClass4 testClass4;

	MulticastDelegate4<INT, INT, INT, INT> MemberFuncInt4MulticastDelegate;
	ASSERT_TRUE(MemberFuncInt4MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!MemberFuncInt4MulticastDelegate);
	MemberFuncInt4MulticastDelegate += MakeDelegate(&testClass4, &TestClass4::MemberFuncInt4);
	ASSERT_TRUE(MemberFuncInt4MulticastDelegate.Empty() == false);
	ASSERT_TRUE(MemberFuncInt4MulticastDelegate);
	MemberFuncInt4MulticastDelegate(TEST_INT, TEST_INT, TEST_INT, TEST_INT);
	MemberFuncInt4MulticastDelegate.Clear();
	ASSERT_TRUE(!MemberFuncInt4MulticastDelegate);

	MulticastDelegate4<StructParam, INT, INT, INT> MemberFuncStruct4MulticastDelegate;
	MemberFuncStruct4MulticastDelegate += MakeDelegate(&testClass4, &TestClass4::MemberFuncStruct4);
	MemberFuncStruct4MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegate4<StructParam*, INT, INT, INT> MemberFuncStructPtr4MulticastDelegate;
	MemberFuncStructPtr4MulticastDelegate += MakeDelegate(&testClass4, &TestClass4::MemberFuncStructPtr4);
	MemberFuncStructPtr4MulticastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegate4<const StructParam*, INT, INT, INT> MemberFuncStructConstPtr4MulticastDelegate;
	MemberFuncStructConstPtr4MulticastDelegate += MakeDelegate(&testClass4, &TestClass4::MemberFuncStructConstPtr4);
	MemberFuncStructConstPtr4MulticastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegate4<StructParam&, INT, INT, INT> MemberFuncStructRef4MulticastDelegate;
	MemberFuncStructRef4MulticastDelegate += MakeDelegate(&testClass4, &TestClass4::MemberFuncStructRef4);
	MemberFuncStructRef4MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegate4<const StructParam&, INT, INT, INT> MemberFuncStructConstRef4MulticastDelegate;
	MemberFuncStructConstRef4MulticastDelegate += MakeDelegate(&testClass4, &TestClass4::MemberFuncStructConstRef4);
	MemberFuncStructConstRef4MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT);

	// N=4 Static Functions
	MulticastDelegate4<INT, INT, INT, INT> StaticFuncInt4MulticastDelegate;
	StaticFuncInt4MulticastDelegate += MakeDelegate(&TestClass4::StaticFuncInt4);
	StaticFuncInt4MulticastDelegate(TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegate4<StructParam, INT, INT, INT> StaticFuncStruct4MulticastDelegate;
	StaticFuncStruct4MulticastDelegate += MakeDelegate(&TestClass4::StaticFuncStruct4);
	StaticFuncStruct4MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegate4<StructParam*, INT, INT, INT> StaticFuncStructPtr4MulticastDelegate;
	StaticFuncStructPtr4MulticastDelegate += MakeDelegate(&TestClass4::StaticFuncStructPtr4);
	StaticFuncStructPtr4MulticastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegate4<const StructParam*, INT, INT, INT> StaticFuncStructConstPtr4MulticastDelegate;
	StaticFuncStructConstPtr4MulticastDelegate += MakeDelegate(&TestClass4::StaticFuncStructConstPtr4);
	StaticFuncStructConstPtr4MulticastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegate4<StructParam&, INT, INT, INT> StaticFuncStructRef4MulticastDelegate;
	StaticFuncStructRef4MulticastDelegate += MakeDelegate(&TestClass4::StaticFuncStructRef4);
	StaticFuncStructRef4MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegate4<const StructParam&, INT, INT, INT> StaticFuncStructConstRef4MulticastDelegate;
	StaticFuncStructConstRef4MulticastDelegate += MakeDelegate(&TestClass4::StaticFuncStructConstRef4);
	StaticFuncStructConstRef4MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT);

	// N=5 Free Functions
	MulticastDelegate5<INT, INT, INT, INT, INT> FreeFuncInt5MulticastDelegate;
	ASSERT_TRUE(FreeFuncInt5MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!FreeFuncInt5MulticastDelegate);
	FreeFuncInt5MulticastDelegate += MakeDelegate(&FreeFuncInt5);
	FreeFuncInt5MulticastDelegate += MakeDelegate(&FreeFuncInt5);
	ASSERT_TRUE(FreeFuncInt5MulticastDelegate.Empty() == false);
	ASSERT_TRUE(FreeFuncInt5MulticastDelegate);
	FreeFuncInt5MulticastDelegate(TEST_INT, TEST_INT, TEST_INT, TEST_INT, TEST_INT);
	FreeFuncInt5MulticastDelegate -= MakeDelegate(&FreeFuncInt5);
	ASSERT_TRUE(FreeFuncInt5MulticastDelegate);
	FreeFuncInt5MulticastDelegate.Clear();
	ASSERT_TRUE(!FreeFuncInt5MulticastDelegate);

	MulticastDelegate5<StructParam, INT, INT, INT, INT> FreeFuncStruct5MulticastDelegate;
	FreeFuncStruct5MulticastDelegate += MakeDelegate(&FreeFuncStruct5);
	FreeFuncStruct5MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegate5<StructParam*, INT, INT, INT, INT> FreeFuncStructPtr5MulticastDelegate;
	FreeFuncStructPtr5MulticastDelegate += MakeDelegate(&FreeFuncStructPtr5);
	FreeFuncStructPtr5MulticastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegate5<const StructParam*, INT, INT, INT, INT> FreeFuncStructConstPtr5MulticastDelegate;
	FreeFuncStructConstPtr5MulticastDelegate += MakeDelegate(&FreeFuncStructConstPtr5);
	FreeFuncStructConstPtr5MulticastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegate5<StructParam&, INT, INT, INT, INT> FreeFuncStructRef5MulticastDelegate;
	FreeFuncStructRef5MulticastDelegate += MakeDelegate(&FreeFuncStructRef5);
	FreeFuncStructRef5MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegate5<const StructParam&, INT, INT, INT, INT> FreeFuncStructConstRef5MulticastDelegate;
	FreeFuncStructConstRef5MulticastDelegate += MakeDelegate(&FreeFuncStructConstRef5);
	FreeFuncStructConstRef5MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	// N=5 Member Functions
	TestClass5 testClass5;

	MulticastDelegate5<INT, INT, INT, INT, INT> MemberFuncInt5MulticastDelegate;
	ASSERT_TRUE(MemberFuncInt5MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!MemberFuncInt5MulticastDelegate);
	MemberFuncInt5MulticastDelegate += MakeDelegate(&testClass5, &TestClass5::MemberFuncInt5);
	ASSERT_TRUE(MemberFuncInt5MulticastDelegate.Empty() == false);
	ASSERT_TRUE(MemberFuncInt5MulticastDelegate);
	MemberFuncInt5MulticastDelegate(TEST_INT, TEST_INT, TEST_INT, TEST_INT, TEST_INT);
	MemberFuncInt5MulticastDelegate.Clear();
	ASSERT_TRUE(!MemberFuncInt5MulticastDelegate);

	MulticastDelegate5<StructParam, INT, INT, INT, INT> MemberFuncStruct5MulticastDelegate;
	MemberFuncStruct5MulticastDelegate += MakeDelegate(&testClass5, &TestClass5::MemberFuncStruct5);
	MemberFuncStruct5MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegate5<StructParam*, INT, INT, INT, INT> MemberFuncStructPtr5MulticastDelegate;
	MemberFuncStructPtr5MulticastDelegate += MakeDelegate(&testClass5, &TestClass5::MemberFuncStructPtr5);
	MemberFuncStructPtr5MulticastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegate5<const StructParam*, INT, INT, INT, INT> MemberFuncStructConstPtr5MulticastDelegate;
	MemberFuncStructConstPtr5MulticastDelegate += MakeDelegate(&testClass5, &TestClass5::MemberFuncStructConstPtr5);
	MemberFuncStructConstPtr5MulticastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegate5<StructParam&, INT, INT, INT, INT> MemberFuncStructRef5MulticastDelegate;
	MemberFuncStructRef5MulticastDelegate += MakeDelegate(&testClass5, &TestClass5::MemberFuncStructRef5);
	MemberFuncStructRef5MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegate5<const StructParam&, INT, INT, INT, INT> MemberFuncStructConstRef5MulticastDelegate;
	MemberFuncStructConstRef5MulticastDelegate += MakeDelegate(&testClass5, &TestClass5::MemberFuncStructConstRef5);
	MemberFuncStructConstRef5MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	// N=5 Static Functions
	MulticastDelegate5<INT, INT, INT, INT, INT> StaticFuncInt5MulticastDelegate;
	StaticFuncInt5MulticastDelegate += MakeDelegate(&TestClass5::StaticFuncInt5);
	StaticFuncInt5MulticastDelegate(TEST_INT, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegate5<StructParam, INT, INT, INT, INT> StaticFuncStruct5MulticastDelegate;
	StaticFuncStruct5MulticastDelegate += MakeDelegate(&TestClass5::StaticFuncStruct5);
	StaticFuncStruct5MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegate5<StructParam*, INT, INT, INT, INT> StaticFuncStructPtr5MulticastDelegate;
	StaticFuncStructPtr5MulticastDelegate += MakeDelegate(&TestClass5::StaticFuncStructPtr5);
	StaticFuncStructPtr5MulticastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegate5<const StructParam*, INT, INT, INT, INT> StaticFuncStructConstPtr5MulticastDelegate;
	StaticFuncStructConstPtr5MulticastDelegate += MakeDelegate(&TestClass5::StaticFuncStructConstPtr5);
	StaticFuncStructConstPtr5MulticastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegate5<StructParam&, INT, INT, INT, INT> StaticFuncStructRef5MulticastDelegate;
	StaticFuncStructRef5MulticastDelegate += MakeDelegate(&TestClass5::StaticFuncStructRef5);
	StaticFuncStructRef5MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegate5<const StructParam&, INT, INT, INT, INT> StaticFuncStructConstRef5MulticastDelegate;
	StaticFuncStructConstRef5MulticastDelegate += MakeDelegate(&TestClass5::StaticFuncStructConstRef5);
	StaticFuncStructConstRef5MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);
}

// Synchronous test of MulticastDelegateSafe<>
void MulticastDelegateSafeTests()
{
	StructParam structParam;
	structParam.val = TEST_INT;

	// N=0 Member Functions
	TestClass0 testClass0;

	MulticastDelegateSafe0 MemberFunc0MulticastDelegate;
	ASSERT_TRUE(MemberFunc0MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!MemberFunc0MulticastDelegate);
	MemberFunc0MulticastDelegate += MakeDelegate(&testClass0, &TestClass0::MemberFunc0);
	ASSERT_TRUE(MemberFunc0MulticastDelegate.Empty() == false);
	ASSERT_TRUE(MemberFunc0MulticastDelegate);
	MemberFunc0MulticastDelegate();
	MemberFunc0MulticastDelegate.Clear();
	ASSERT_TRUE(!MemberFunc0MulticastDelegate);

	// N=1 Member Functions
	TestClass1 testClass1;

	MulticastDelegateSafe1<INT> MemberFuncInt1MulticastDelegate;
	ASSERT_TRUE(MemberFuncInt1MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!MemberFuncInt1MulticastDelegate);
	MemberFuncInt1MulticastDelegate += MakeDelegate(&testClass1, &TestClass1::MemberFuncInt1);
	ASSERT_TRUE(MemberFuncInt1MulticastDelegate.Empty() == false);
	ASSERT_TRUE(MemberFuncInt1MulticastDelegate);
	MemberFuncInt1MulticastDelegate(TEST_INT);
	MemberFuncInt1MulticastDelegate.Clear();
	ASSERT_TRUE(!MemberFuncInt1MulticastDelegate);

	// N=2 Member Functions
	TestClass2 testClass2;

	MulticastDelegateSafe2<INT, INT> MemberFuncInt2MulticastDelegate;
	ASSERT_TRUE(MemberFuncInt2MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!MemberFuncInt2MulticastDelegate);
	MemberFuncInt2MulticastDelegate += MakeDelegate(&testClass2, &TestClass2::MemberFuncInt2);
	ASSERT_TRUE(MemberFuncInt2MulticastDelegate.Empty() == false);
	ASSERT_TRUE(MemberFuncInt2MulticastDelegate);
	MemberFuncInt2MulticastDelegate(TEST_INT, TEST_INT);
	MemberFuncInt2MulticastDelegate.Clear();
	ASSERT_TRUE(!MemberFuncInt2MulticastDelegate);

	// N=3 Member Functions
	TestClass3 testClass3;

	MulticastDelegateSafe3<INT, INT, INT> MemberFuncInt3MulticastDelegate;
	ASSERT_TRUE(MemberFuncInt3MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!MemberFuncInt3MulticastDelegate);
	MemberFuncInt3MulticastDelegate += MakeDelegate(&testClass3, &TestClass3::MemberFuncInt3);
	ASSERT_TRUE(MemberFuncInt3MulticastDelegate.Empty() == false);
	ASSERT_TRUE(MemberFuncInt3MulticastDelegate);
	MemberFuncInt3MulticastDelegate(TEST_INT, TEST_INT, TEST_INT);
	MemberFuncInt3MulticastDelegate.Clear();
	ASSERT_TRUE(!MemberFuncInt3MulticastDelegate);

	// N=4 Member Functions
	TestClass4 testClass4;

	MulticastDelegateSafe4<INT, INT, INT, INT> MemberFuncInt4MulticastDelegate;
	ASSERT_TRUE(MemberFuncInt4MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!MemberFuncInt4MulticastDelegate);
	MemberFuncInt4MulticastDelegate += MakeDelegate(&testClass4, &TestClass4::MemberFuncInt4);
	ASSERT_TRUE(MemberFuncInt4MulticastDelegate.Empty() == false);
	ASSERT_TRUE(MemberFuncInt4MulticastDelegate);
	MemberFuncInt4MulticastDelegate(TEST_INT, TEST_INT, TEST_INT, TEST_INT);
	MemberFuncInt4MulticastDelegate.Clear();
	ASSERT_TRUE(!MemberFuncInt4MulticastDelegate);

	// N=5 Member Functions
	TestClass5 testClass5;

	MulticastDelegateSafe5<INT, INT, INT, INT, INT> MemberFuncInt5MulticastDelegate;
	ASSERT_TRUE(MemberFuncInt5MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!MemberFuncInt5MulticastDelegate);
	MemberFuncInt5MulticastDelegate += MakeDelegate(&testClass5, &TestClass5::MemberFuncInt5);
	ASSERT_TRUE(MemberFuncInt5MulticastDelegate.Empty() == false);
	ASSERT_TRUE(MemberFuncInt5MulticastDelegate);
	MemberFuncInt5MulticastDelegate(TEST_INT, TEST_INT, TEST_INT, TEST_INT, TEST_INT);
	MemberFuncInt5MulticastDelegate.Clear();
	ASSERT_TRUE(!MemberFuncInt5MulticastDelegate);
}

// Asynchronous test of MulticastDelegateSafe<>
void MulticastDelegateSafeAsyncTests()
{
	StructParam structParam;
	structParam.val = TEST_INT;

	// N=0 Free Functions
	MulticastDelegateSafe0 FreeFunc0MulticastDelegate;
	ASSERT_TRUE(FreeFunc0MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!FreeFunc0MulticastDelegate);
	FreeFunc0MulticastDelegate += MakeDelegate(&FreeFunc0, &testThread);
	FreeFunc0MulticastDelegate += MakeDelegate(&FreeFunc0, &testThread);
	ASSERT_TRUE(FreeFunc0MulticastDelegate.Empty() == false);
	ASSERT_TRUE(FreeFunc0MulticastDelegate);
	FreeFunc0MulticastDelegate();
	FreeFunc0MulticastDelegate -= MakeDelegate(&FreeFunc0, &testThread);
	ASSERT_TRUE(FreeFunc0MulticastDelegate);
	FreeFunc0MulticastDelegate.Clear();
	ASSERT_TRUE(!FreeFunc0MulticastDelegate);

	// N=0 Member Functions
	TestClass0 testClass0;

	MulticastDelegateSafe0 MemberFunc0MulticastDelegate;
	ASSERT_TRUE(MemberFunc0MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!MemberFunc0MulticastDelegate);
	MemberFunc0MulticastDelegate += MakeDelegate(&testClass0, &TestClass0::MemberFunc0, &testThread);
	MemberFunc0MulticastDelegate += MakeDelegate(&testClass0, &TestClass0::MemberFunc0Const, &testThread);
	ASSERT_TRUE(MemberFunc0MulticastDelegate.Empty() == false);
	ASSERT_TRUE(MemberFunc0MulticastDelegate);
	if (MemberFunc0MulticastDelegate)
		MemberFunc0MulticastDelegate();
	MemberFunc0MulticastDelegate.Clear();
	ASSERT_TRUE(!MemberFunc0MulticastDelegate);

	// N=0 Static Functions
	MulticastDelegateSafe0 StaticFunc0MulticastDelegate;
	StaticFunc0MulticastDelegate += MakeDelegate(&TestClass0::StaticFunc0, &testThread);
	StaticFunc0MulticastDelegate();

	// N=1 Free Functions
	MulticastDelegateSafe1<INT> FreeFuncInt1MulticastDelegate;
	ASSERT_TRUE(FreeFuncInt1MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!FreeFuncInt1MulticastDelegate);
	FreeFuncInt1MulticastDelegate += MakeDelegate(&FreeFuncInt1, &testThread);
	FreeFuncInt1MulticastDelegate += MakeDelegate(&FreeFuncInt1, &testThread);
	ASSERT_TRUE(FreeFuncInt1MulticastDelegate.Empty() == false);
	ASSERT_TRUE(FreeFuncInt1MulticastDelegate);
	FreeFuncInt1MulticastDelegate(TEST_INT);
	FreeFuncInt1MulticastDelegate -= MakeDelegate(&FreeFuncInt1, &testThread);
	ASSERT_TRUE(FreeFuncInt1MulticastDelegate);
	FreeFuncInt1MulticastDelegate.Clear();
	ASSERT_TRUE(!FreeFuncInt1MulticastDelegate);

	MulticastDelegateSafe1<StructParam> FreeFuncStruct1MulticastDelegate;
	FreeFuncStruct1MulticastDelegate += MakeDelegate(&FreeFuncStruct1);
	FreeFuncStruct1MulticastDelegate(structParam);

	MulticastDelegateSafe1<StructParam*> FreeFuncStructPtr1MulticastDelegate;
	FreeFuncStructPtr1MulticastDelegate += MakeDelegate(&FreeFuncStructPtr1, &testThread);
	FreeFuncStructPtr1MulticastDelegate(&structParam);

	MulticastDelegateSafe1<const StructParam*> FreeFuncStructConstPtr1MulticastDelegate;
	FreeFuncStructConstPtr1MulticastDelegate += MakeDelegate(&FreeFuncStructConstPtr1, &testThread);
	FreeFuncStructConstPtr1MulticastDelegate(&structParam);

	MulticastDelegateSafe1<StructParam&> FreeFuncStructRef1MulticastDelegate;
	FreeFuncStructRef1MulticastDelegate += MakeDelegate(&FreeFuncStructRef1, &testThread);
	FreeFuncStructRef1MulticastDelegate(structParam);

	MulticastDelegateSafe1<const StructParam&> FreeFuncStructConstRef1MulticastDelegate;
	FreeFuncStructConstRef1MulticastDelegate += MakeDelegate(&FreeFuncStructConstRef1, &testThread);
	FreeFuncStructConstRef1MulticastDelegate(structParam);

	// N=1 Member Functions
	TestClass1 testClass1;

	MulticastDelegateSafe1<INT> MemberFuncInt1MulticastDelegate;
	ASSERT_TRUE(MemberFuncInt1MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!MemberFuncInt1MulticastDelegate);
	MemberFuncInt1MulticastDelegate += MakeDelegate(&testClass1, &TestClass1::MemberFuncInt1, &testThread);
	MemberFuncInt1MulticastDelegate += MakeDelegate(&testClass1, &TestClass1::MemberFuncInt1Const, &testThread);
	ASSERT_TRUE(MemberFuncInt1MulticastDelegate.Empty() == false);
	ASSERT_TRUE(MemberFuncInt1MulticastDelegate);
	MemberFuncInt1MulticastDelegate(TEST_INT);
	MemberFuncInt1MulticastDelegate.Clear();
	ASSERT_TRUE(!MemberFuncInt1MulticastDelegate);

	MulticastDelegateSafe1<StructParam> MemberFuncStruct1MulticastDelegate;
	MemberFuncStruct1MulticastDelegate += MakeDelegate(&testClass1, &TestClass1::MemberFuncStruct1, &testThread);
	if (MemberFuncStruct1MulticastDelegate)
		MemberFuncStruct1MulticastDelegate(structParam);

	MulticastDelegateSafe1<StructParam*> MemberFuncStructPtr1MulticastDelegate;
	MemberFuncStructPtr1MulticastDelegate += MakeDelegate(&testClass1, &TestClass1::MemberFuncStructPtr1, &testThread);
	MemberFuncStructPtr1MulticastDelegate(&structParam);

	MulticastDelegateSafe1<const StructParam*> MemberFuncStructConstPtr1MulticastDelegate;
	MemberFuncStructConstPtr1MulticastDelegate += MakeDelegate(&testClass1, &TestClass1::MemberFuncStructConstPtr1, &testThread);
	MemberFuncStructConstPtr1MulticastDelegate(&structParam);

	MulticastDelegateSafe1<StructParam&> MemberFuncStructRef1MulticastDelegate;
	MemberFuncStructRef1MulticastDelegate += MakeDelegate(&testClass1, &TestClass1::MemberFuncStructRef1, &testThread);
	MemberFuncStructRef1MulticastDelegate(structParam);

	MulticastDelegateSafe1<const StructParam&> MemberFuncStructConstRef1MulticastDelegate;
	MemberFuncStructConstRef1MulticastDelegate += MakeDelegate(&testClass1, &TestClass1::MemberFuncStructConstRef1, &testThread);
	MemberFuncStructConstRef1MulticastDelegate(structParam);

	// N=1 Static Functions
	MulticastDelegateSafe1<INT> StaticFuncInt1MulticastDelegate;
	StaticFuncInt1MulticastDelegate += MakeDelegate(&TestClass1::StaticFuncInt1, &testThread);
	StaticFuncInt1MulticastDelegate(TEST_INT);

	MulticastDelegateSafe1<StructParam> StaticFuncStruct1MulticastDelegate;
	StaticFuncStruct1MulticastDelegate += MakeDelegate(&TestClass1::StaticFuncStruct1, &testThread);
	StaticFuncStruct1MulticastDelegate(structParam);

	MulticastDelegateSafe1<StructParam*> StaticFuncStructPtr1MulticastDelegate;
	StaticFuncStructPtr1MulticastDelegate += MakeDelegate(&TestClass1::StaticFuncStructPtr1, &testThread);
	StaticFuncStructPtr1MulticastDelegate(&structParam);

	MulticastDelegateSafe1<const StructParam*> StaticFuncStructConstPtr1MulticastDelegate;
	StaticFuncStructConstPtr1MulticastDelegate += MakeDelegate(&TestClass1::StaticFuncStructConstPtr1, &testThread);
	StaticFuncStructConstPtr1MulticastDelegate(&structParam);

	MulticastDelegateSafe1<StructParam&> StaticFuncStructRef1MulticastDelegate;
	StaticFuncStructRef1MulticastDelegate += MakeDelegate(&TestClass1::StaticFuncStructRef1, &testThread);
	StaticFuncStructRef1MulticastDelegate(structParam);

	MulticastDelegateSafe1<const StructParam&> StaticFuncStructConstRef1MulticastDelegate;
	StaticFuncStructConstRef1MulticastDelegate += MakeDelegate(&TestClass1::StaticFuncStructConstRef1, &testThread);
	StaticFuncStructConstRef1MulticastDelegate(structParam);

	// N=2 Free Functions
	MulticastDelegateSafe2<INT, INT> FreeFuncInt2MulticastDelegate;
	ASSERT_TRUE(FreeFuncInt2MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!FreeFuncInt2MulticastDelegate);
	FreeFuncInt2MulticastDelegate += MakeDelegate(&FreeFuncInt2, &testThread);
	FreeFuncInt2MulticastDelegate += MakeDelegate(&FreeFuncInt2, &testThread);
	ASSERT_TRUE(FreeFuncInt2MulticastDelegate.Empty() == false);
	ASSERT_TRUE(FreeFuncInt2MulticastDelegate);
	FreeFuncInt2MulticastDelegate(TEST_INT, TEST_INT);
	FreeFuncInt2MulticastDelegate -= MakeDelegate(&FreeFuncInt2, &testThread);
	ASSERT_TRUE(FreeFuncInt2MulticastDelegate);
	FreeFuncInt2MulticastDelegate.Clear();
	ASSERT_TRUE(!FreeFuncInt2MulticastDelegate);

	MulticastDelegateSafe2<StructParam, INT> FreeFuncStruct2MulticastDelegate;
	FreeFuncStruct2MulticastDelegate += MakeDelegate(&FreeFuncStruct2, &testThread);
	FreeFuncStruct2MulticastDelegate(structParam, TEST_INT);

	MulticastDelegateSafe2<StructParam*, INT> FreeFuncStructPtr2MulticastDelegate;
	FreeFuncStructPtr2MulticastDelegate += MakeDelegate(&FreeFuncStructPtr2, &testThread);
	FreeFuncStructPtr2MulticastDelegate(&structParam, TEST_INT);

	MulticastDelegateSafe2<const StructParam*, INT> FreeFuncStructConstPtr2MulticastDelegate;
	FreeFuncStructConstPtr2MulticastDelegate += MakeDelegate(&FreeFuncStructConstPtr2, &testThread);
	FreeFuncStructConstPtr2MulticastDelegate(&structParam, TEST_INT);

	MulticastDelegateSafe2<StructParam&, INT> FreeFuncStructRef2MulticastDelegate;
	FreeFuncStructRef2MulticastDelegate += MakeDelegate(&FreeFuncStructRef2, &testThread);
	FreeFuncStructRef2MulticastDelegate(structParam, TEST_INT);

	MulticastDelegateSafe2<const StructParam&, INT> FreeFuncStructConstRef2MulticastDelegate;
	FreeFuncStructConstRef2MulticastDelegate += MakeDelegate(&FreeFuncStructConstRef2, &testThread);
	FreeFuncStructConstRef2MulticastDelegate(structParam, TEST_INT);

	// N=2 Member Functions
	TestClass2 testClass2;

	MulticastDelegateSafe2<INT, INT> MemberFuncInt2MulticastDelegate;
	ASSERT_TRUE(MemberFuncInt2MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!MemberFuncInt2MulticastDelegate);
	MemberFuncInt2MulticastDelegate += MakeDelegate(&testClass2, &TestClass2::MemberFuncInt2, &testThread);
	MemberFuncInt2MulticastDelegate += MakeDelegate(&testClass2, &TestClass2::MemberFuncInt2Const, &testThread);
	ASSERT_TRUE(MemberFuncInt2MulticastDelegate.Empty() == false);
	ASSERT_TRUE(MemberFuncInt2MulticastDelegate);
	MemberFuncInt2MulticastDelegate(TEST_INT, TEST_INT);
	MemberFuncInt2MulticastDelegate.Clear();
	ASSERT_TRUE(!MemberFuncInt2MulticastDelegate);

	MulticastDelegateSafe2<StructParam, INT> MemberFuncStruct2MulticastDelegate;
	MemberFuncStruct2MulticastDelegate += MakeDelegate(&testClass2, &TestClass2::MemberFuncStruct2, &testThread);
	if (MemberFuncStruct2MulticastDelegate)
		MemberFuncStruct2MulticastDelegate(structParam, TEST_INT);

	MulticastDelegateSafe2<StructParam*, INT> MemberFuncStructPtr2MulticastDelegate;
	MemberFuncStructPtr2MulticastDelegate += MakeDelegate(&testClass2, &TestClass2::MemberFuncStructPtr2, &testThread);
	MemberFuncStructPtr2MulticastDelegate(&structParam, TEST_INT);

	MulticastDelegateSafe2<const StructParam*, INT> MemberFuncStructConstPtr2MulticastDelegate;
	MemberFuncStructConstPtr2MulticastDelegate += MakeDelegate(&testClass2, &TestClass2::MemberFuncStructConstPtr2, &testThread);
	MemberFuncStructConstPtr2MulticastDelegate(&structParam, TEST_INT);

	MulticastDelegateSafe2<StructParam&, INT> MemberFuncStructRef2MulticastDelegate;
	MemberFuncStructRef2MulticastDelegate += MakeDelegate(&testClass2, &TestClass2::MemberFuncStructRef2, &testThread);
	MemberFuncStructRef2MulticastDelegate(structParam, TEST_INT);

	MulticastDelegateSafe2<const StructParam&, INT> MemberFuncStructConstRef2MulticastDelegate;
	MemberFuncStructConstRef2MulticastDelegate += MakeDelegate(&testClass2, &TestClass2::MemberFuncStructConstRef2, &testThread);
	MemberFuncStructConstRef2MulticastDelegate(structParam, TEST_INT);

	// N=2 Static Functions
	MulticastDelegateSafe2<INT, INT> StaticFuncInt2MulticastDelegate;
	StaticFuncInt2MulticastDelegate += MakeDelegate(&TestClass2::StaticFuncInt2, &testThread);
	StaticFuncInt2MulticastDelegate(TEST_INT, TEST_INT);

	MulticastDelegateSafe2<StructParam, INT> StaticFuncStruct2MulticastDelegate;
	StaticFuncStruct2MulticastDelegate += MakeDelegate(&TestClass2::StaticFuncStruct2, &testThread);
	StaticFuncStruct2MulticastDelegate(structParam, TEST_INT);

	MulticastDelegateSafe2<StructParam*, INT> StaticFuncStructPtr2MulticastDelegate;
	StaticFuncStructPtr2MulticastDelegate += MakeDelegate(&TestClass2::StaticFuncStructPtr2, &testThread);
	StaticFuncStructPtr2MulticastDelegate(&structParam, TEST_INT);

	MulticastDelegateSafe2<const StructParam*, INT> StaticFuncStructConstPtr2MulticastDelegate;
	StaticFuncStructConstPtr2MulticastDelegate += MakeDelegate(&TestClass2::StaticFuncStructConstPtr2, &testThread);
	StaticFuncStructConstPtr2MulticastDelegate(&structParam, TEST_INT);

	MulticastDelegateSafe2<StructParam&, INT> StaticFuncStructRef2MulticastDelegate;
	StaticFuncStructRef2MulticastDelegate += MakeDelegate(&TestClass2::StaticFuncStructRef2, &testThread);
	StaticFuncStructRef2MulticastDelegate(structParam, TEST_INT);

	MulticastDelegateSafe2<const StructParam&, INT> StaticFuncStructConstRef2MulticastDelegate;
	StaticFuncStructConstRef2MulticastDelegate += MakeDelegate(&TestClass2::StaticFuncStructConstRef2, &testThread);
	StaticFuncStructConstRef2MulticastDelegate(structParam, TEST_INT);

	// N=3 Free Functions
	MulticastDelegateSafe3<INT, INT, INT> FreeFuncInt3MulticastDelegate;
	ASSERT_TRUE(FreeFuncInt3MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!FreeFuncInt3MulticastDelegate);
	FreeFuncInt3MulticastDelegate += MakeDelegate(&FreeFuncInt3, &testThread);
	FreeFuncInt3MulticastDelegate += MakeDelegate(&FreeFuncInt3, &testThread);
	ASSERT_TRUE(FreeFuncInt3MulticastDelegate.Empty() == false);
	ASSERT_TRUE(FreeFuncInt3MulticastDelegate);
	FreeFuncInt3MulticastDelegate(TEST_INT, TEST_INT, TEST_INT);
	FreeFuncInt3MulticastDelegate -= MakeDelegate(&FreeFuncInt3, &testThread);
	ASSERT_TRUE(FreeFuncInt3MulticastDelegate);
	FreeFuncInt3MulticastDelegate.Clear();
	ASSERT_TRUE(!FreeFuncInt3MulticastDelegate);

	MulticastDelegateSafe3<StructParam, INT, INT> FreeFuncStruct3MulticastDelegate;
	FreeFuncStruct3MulticastDelegate += MakeDelegate(&FreeFuncStruct3, &testThread);
	FreeFuncStruct3MulticastDelegate(structParam, TEST_INT, TEST_INT);

	MulticastDelegateSafe3<StructParam*, INT, INT> FreeFuncStructPtr3MulticastDelegate;
	FreeFuncStructPtr3MulticastDelegate += MakeDelegate(&FreeFuncStructPtr3, &testThread);
	FreeFuncStructPtr3MulticastDelegate(&structParam, TEST_INT, TEST_INT);

	MulticastDelegateSafe3<const StructParam*, INT, INT> FreeFuncStructConstPtr3MulticastDelegate;
	FreeFuncStructConstPtr3MulticastDelegate += MakeDelegate(&FreeFuncStructConstPtr3, &testThread);
	FreeFuncStructConstPtr3MulticastDelegate(&structParam, TEST_INT, TEST_INT);

	MulticastDelegateSafe3<StructParam&, INT, INT> FreeFuncStructRef3MulticastDelegate;
	FreeFuncStructRef3MulticastDelegate += MakeDelegate(&FreeFuncStructRef3, &testThread);
	FreeFuncStructRef3MulticastDelegate(structParam, TEST_INT, TEST_INT);

	MulticastDelegateSafe3<const StructParam&, INT, INT> FreeFuncStructConstRef3MulticastDelegate;
	FreeFuncStructConstRef3MulticastDelegate += MakeDelegate(&FreeFuncStructConstRef3, &testThread);
	FreeFuncStructConstRef3MulticastDelegate(structParam, TEST_INT, TEST_INT);

	// N=3 Member Functions
	TestClass3 testClass3;

	MulticastDelegateSafe3<INT, INT, INT> MemberFuncInt3MulticastDelegate;
	ASSERT_TRUE(MemberFuncInt3MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!MemberFuncInt3MulticastDelegate);
	MemberFuncInt3MulticastDelegate += MakeDelegate(&testClass3, &TestClass3::MemberFuncInt3, &testThread);
	MemberFuncInt3MulticastDelegate += MakeDelegate(&testClass3, &TestClass3::MemberFuncInt3Const, &testThread);
	ASSERT_TRUE(MemberFuncInt3MulticastDelegate.Empty() == false);
	ASSERT_TRUE(MemberFuncInt3MulticastDelegate);
	MemberFuncInt3MulticastDelegate(TEST_INT, TEST_INT, TEST_INT);
	MemberFuncInt3MulticastDelegate.Clear();
	ASSERT_TRUE(!MemberFuncInt3MulticastDelegate);

	MulticastDelegateSafe3<StructParam, INT, INT> MemberFuncStruct3MulticastDelegate;
	MemberFuncStruct3MulticastDelegate += MakeDelegate(&testClass3, &TestClass3::MemberFuncStruct3, &testThread);
	if (MemberFuncStruct3MulticastDelegate)
		MemberFuncStruct3MulticastDelegate(structParam, TEST_INT, TEST_INT);

	MulticastDelegateSafe3<StructParam*, INT, INT> MemberFuncStructPtr3MulticastDelegate;
	MemberFuncStructPtr3MulticastDelegate += MakeDelegate(&testClass3, &TestClass3::MemberFuncStructPtr3, &testThread);
	MemberFuncStructPtr3MulticastDelegate(&structParam, TEST_INT, TEST_INT);

	MulticastDelegateSafe3<const StructParam*, INT, INT> MemberFuncStructConstPtr3MulticastDelegate;
	MemberFuncStructConstPtr3MulticastDelegate += MakeDelegate(&testClass3, &TestClass3::MemberFuncStructConstPtr3, &testThread);
	MemberFuncStructConstPtr3MulticastDelegate(&structParam, TEST_INT, TEST_INT);

	MulticastDelegateSafe3<StructParam&, INT, INT> MemberFuncStructRef3MulticastDelegate;
	MemberFuncStructRef3MulticastDelegate += MakeDelegate(&testClass3, &TestClass3::MemberFuncStructRef3, &testThread);
	MemberFuncStructRef3MulticastDelegate(structParam, TEST_INT, TEST_INT);

	MulticastDelegateSafe3<const StructParam&, INT, INT> MemberFuncStructConstRef3MulticastDelegate;
	MemberFuncStructConstRef3MulticastDelegate += MakeDelegate(&testClass3, &TestClass3::MemberFuncStructConstRef3, &testThread);
	MemberFuncStructConstRef3MulticastDelegate(structParam, TEST_INT, TEST_INT);

	// N=3 Static Functions
	MulticastDelegateSafe3<INT, INT, INT> StaticFuncInt3MulticastDelegate;
	StaticFuncInt3MulticastDelegate += MakeDelegate(&TestClass3::StaticFuncInt3, &testThread);
	StaticFuncInt3MulticastDelegate(TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe3<StructParam, INT, INT> StaticFuncStruct3MulticastDelegate;
	StaticFuncStruct3MulticastDelegate += MakeDelegate(&TestClass3::StaticFuncStruct3, &testThread);
	StaticFuncStruct3MulticastDelegate(structParam, TEST_INT, TEST_INT);

	MulticastDelegateSafe3<StructParam*, INT, INT> StaticFuncStructPtr3MulticastDelegate;
	StaticFuncStructPtr3MulticastDelegate += MakeDelegate(&TestClass3::StaticFuncStructPtr3, &testThread);
	StaticFuncStructPtr3MulticastDelegate(&structParam, TEST_INT, TEST_INT);

	MulticastDelegateSafe3<const StructParam*, INT, INT> StaticFuncStructConstPtr3MulticastDelegate;
	StaticFuncStructConstPtr3MulticastDelegate += MakeDelegate(&TestClass3::StaticFuncStructConstPtr3, &testThread);
	StaticFuncStructConstPtr3MulticastDelegate(&structParam, TEST_INT, TEST_INT);

	MulticastDelegateSafe3<StructParam&, INT, INT> StaticFuncStructRef3MulticastDelegate;
	StaticFuncStructRef3MulticastDelegate += MakeDelegate(&TestClass3::StaticFuncStructRef3, &testThread);
	StaticFuncStructRef3MulticastDelegate(structParam, TEST_INT, TEST_INT);

	MulticastDelegateSafe3<const StructParam&, INT, INT> StaticFuncStructConstRef3MulticastDelegate;
	StaticFuncStructConstRef3MulticastDelegate += MakeDelegate(&TestClass3::StaticFuncStructConstRef3, &testThread);
	StaticFuncStructConstRef3MulticastDelegate(structParam, TEST_INT, TEST_INT);

	// N=4 Free Functions
	MulticastDelegateSafe4<INT, INT, INT, INT> FreeFuncInt4MulticastDelegate;
	ASSERT_TRUE(FreeFuncInt4MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!FreeFuncInt4MulticastDelegate);
	FreeFuncInt4MulticastDelegate += MakeDelegate(&FreeFuncInt4, &testThread);
	FreeFuncInt4MulticastDelegate += MakeDelegate(&FreeFuncInt4, &testThread);
	ASSERT_TRUE(FreeFuncInt4MulticastDelegate.Empty() == false);
	ASSERT_TRUE(FreeFuncInt4MulticastDelegate);
	FreeFuncInt4MulticastDelegate(TEST_INT, TEST_INT, TEST_INT, TEST_INT);
	FreeFuncInt4MulticastDelegate -= MakeDelegate(&FreeFuncInt4, &testThread);
	ASSERT_TRUE(FreeFuncInt4MulticastDelegate);
	FreeFuncInt4MulticastDelegate.Clear();
	ASSERT_TRUE(!FreeFuncInt4MulticastDelegate);

	MulticastDelegateSafe4<StructParam, INT, INT, INT> FreeFuncStruct4MulticastDelegate;
	FreeFuncStruct4MulticastDelegate += MakeDelegate(&FreeFuncStruct4, &testThread);
	FreeFuncStruct4MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe4<StructParam*, INT, INT, INT> FreeFuncStructPtr4MulticastDelegate;
	FreeFuncStructPtr4MulticastDelegate += MakeDelegate(&FreeFuncStructPtr4, &testThread);
	FreeFuncStructPtr4MulticastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe4<const StructParam*, INT, INT, INT> FreeFuncStructConstPtr4MulticastDelegate;
	FreeFuncStructConstPtr4MulticastDelegate += MakeDelegate(&FreeFuncStructConstPtr4, &testThread);
	FreeFuncStructConstPtr4MulticastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe4<StructParam&, INT, INT, INT> FreeFuncStructRef4MulticastDelegate;
	FreeFuncStructRef4MulticastDelegate += MakeDelegate(&FreeFuncStructRef4, &testThread);
	FreeFuncStructRef4MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe4<const StructParam&, INT, INT, INT> FreeFuncStructConstRef4MulticastDelegate;
	FreeFuncStructConstRef4MulticastDelegate += MakeDelegate(&FreeFuncStructConstRef4, &testThread);
	FreeFuncStructConstRef4MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT);

	// N=4 Member Functions
	TestClass4 testClass4;

	MulticastDelegateSafe4<INT, INT, INT, INT> MemberFuncInt4MulticastDelegate;
	ASSERT_TRUE(MemberFuncInt4MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!MemberFuncInt4MulticastDelegate);
	MemberFuncInt4MulticastDelegate += MakeDelegate(&testClass4, &TestClass4::MemberFuncInt4, &testThread);
	MemberFuncInt4MulticastDelegate += MakeDelegate(&testClass4, &TestClass4::MemberFuncInt4Const, &testThread);
	ASSERT_TRUE(MemberFuncInt4MulticastDelegate.Empty() == false);
	ASSERT_TRUE(MemberFuncInt4MulticastDelegate);
	MemberFuncInt4MulticastDelegate(TEST_INT, TEST_INT, TEST_INT, TEST_INT);
	MemberFuncInt4MulticastDelegate.Clear();
	ASSERT_TRUE(!MemberFuncInt4MulticastDelegate);

	MulticastDelegateSafe4<StructParam, INT, INT, INT> MemberFuncStruct4MulticastDelegate;
	MemberFuncStruct4MulticastDelegate += MakeDelegate(&testClass4, &TestClass4::MemberFuncStruct4, &testThread);
	if (MemberFuncStruct4MulticastDelegate)
		MemberFuncStruct4MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe4<StructParam*, INT, INT, INT> MemberFuncStructPtr4MulticastDelegate;
	MemberFuncStructPtr4MulticastDelegate += MakeDelegate(&testClass4, &TestClass4::MemberFuncStructPtr4, &testThread);
	MemberFuncStructPtr4MulticastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe4<const StructParam*, INT, INT, INT> MemberFuncStructConstPtr4MulticastDelegate;
	MemberFuncStructConstPtr4MulticastDelegate += MakeDelegate(&testClass4, &TestClass4::MemberFuncStructConstPtr4, &testThread);
	MemberFuncStructConstPtr4MulticastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe4<StructParam&, INT, INT, INT> MemberFuncStructRef4MulticastDelegate;
	MemberFuncStructRef4MulticastDelegate += MakeDelegate(&testClass4, &TestClass4::MemberFuncStructRef4, &testThread);
	MemberFuncStructRef4MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe4<const StructParam&, INT, INT, INT> MemberFuncStructConstRef4MulticastDelegate;
	MemberFuncStructConstRef4MulticastDelegate += MakeDelegate(&testClass4, &TestClass4::MemberFuncStructConstRef4, &testThread);
	MemberFuncStructConstRef4MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT);

	// N=4 Static Functions
	MulticastDelegateSafe4<INT, INT, INT, INT> StaticFuncInt4MulticastDelegate;
	StaticFuncInt4MulticastDelegate += MakeDelegate(&TestClass4::StaticFuncInt4, &testThread);
	StaticFuncInt4MulticastDelegate(TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe4<StructParam, INT, INT, INT> StaticFuncStruct4MulticastDelegate;
	StaticFuncStruct4MulticastDelegate += MakeDelegate(&TestClass4::StaticFuncStruct4, &testThread);
	StaticFuncStruct4MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe4<StructParam*, INT, INT, INT> StaticFuncStructPtr4MulticastDelegate;
	StaticFuncStructPtr4MulticastDelegate += MakeDelegate(&TestClass4::StaticFuncStructPtr4, &testThread);
	StaticFuncStructPtr4MulticastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe4<const StructParam*, INT, INT, INT> StaticFuncStructConstPtr4MulticastDelegate;
	StaticFuncStructConstPtr4MulticastDelegate += MakeDelegate(&TestClass4::StaticFuncStructConstPtr4, &testThread);
	StaticFuncStructConstPtr4MulticastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe4<StructParam&, INT, INT, INT> StaticFuncStructRef4MulticastDelegate;
	StaticFuncStructRef4MulticastDelegate += MakeDelegate(&TestClass4::StaticFuncStructRef4, &testThread);
	StaticFuncStructRef4MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe4<const StructParam&, INT, INT, INT> StaticFuncStructConstRef4MulticastDelegate;
	StaticFuncStructConstRef4MulticastDelegate += MakeDelegate(&TestClass4::StaticFuncStructConstRef4, &testThread);
	StaticFuncStructConstRef4MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT);

	// N=5 Free Functions
	MulticastDelegateSafe5<INT, INT, INT, INT, INT> FreeFuncInt5MulticastDelegate;
	ASSERT_TRUE(FreeFuncInt5MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!FreeFuncInt5MulticastDelegate);
	FreeFuncInt5MulticastDelegate += MakeDelegate(&FreeFuncInt5, &testThread);
	FreeFuncInt5MulticastDelegate += MakeDelegate(&FreeFuncInt5, &testThread);
	ASSERT_TRUE(FreeFuncInt5MulticastDelegate.Empty() == false);
	ASSERT_TRUE(FreeFuncInt5MulticastDelegate);
	FreeFuncInt5MulticastDelegate(TEST_INT, TEST_INT, TEST_INT, TEST_INT, TEST_INT);
	FreeFuncInt5MulticastDelegate -= MakeDelegate(&FreeFuncInt5, &testThread);
	ASSERT_TRUE(FreeFuncInt5MulticastDelegate);
	FreeFuncInt5MulticastDelegate.Clear();
	ASSERT_TRUE(!FreeFuncInt5MulticastDelegate);

	MulticastDelegateSafe5<StructParam, INT, INT, INT, INT> FreeFuncStruct5MulticastDelegate;
	FreeFuncStruct5MulticastDelegate += MakeDelegate(&FreeFuncStruct5, &testThread);
	FreeFuncStruct5MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe5<StructParam*, INT, INT, INT, INT> FreeFuncStructPtr5MulticastDelegate;
	FreeFuncStructPtr5MulticastDelegate += MakeDelegate(&FreeFuncStructPtr5, &testThread);
	FreeFuncStructPtr5MulticastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe5<const StructParam*, INT, INT, INT, INT> FreeFuncStructConstPtr5MulticastDelegate;
	FreeFuncStructConstPtr5MulticastDelegate += MakeDelegate(&FreeFuncStructConstPtr5, &testThread);
	FreeFuncStructConstPtr5MulticastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe5<StructParam&, INT, INT, INT, INT> FreeFuncStructRef5MulticastDelegate;
	FreeFuncStructRef5MulticastDelegate += MakeDelegate(&FreeFuncStructRef5, &testThread);
	FreeFuncStructRef5MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe5<const StructParam&, INT, INT, INT, INT> FreeFuncStructConstRef5MulticastDelegate;
	FreeFuncStructConstRef5MulticastDelegate += MakeDelegate(&FreeFuncStructConstRef5, &testThread);
	FreeFuncStructConstRef5MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	// N=5 Member Functions
	TestClass5 testClass5;

	MulticastDelegateSafe5<INT, INT, INT, INT, INT> MemberFuncInt5MulticastDelegate;
	ASSERT_TRUE(MemberFuncInt5MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!MemberFuncInt5MulticastDelegate);
	MemberFuncInt5MulticastDelegate += MakeDelegate(&testClass5, &TestClass5::MemberFuncInt5, &testThread);
	MemberFuncInt5MulticastDelegate += MakeDelegate(&testClass5, &TestClass5::MemberFuncInt5Const, &testThread);
	ASSERT_TRUE(MemberFuncInt5MulticastDelegate.Empty() == false);
	ASSERT_TRUE(MemberFuncInt5MulticastDelegate);
	MemberFuncInt5MulticastDelegate(TEST_INT, TEST_INT, TEST_INT, TEST_INT, TEST_INT);
	MemberFuncInt5MulticastDelegate.Clear();
	ASSERT_TRUE(!MemberFuncInt5MulticastDelegate);

	MulticastDelegateSafe5<StructParam, INT, INT, INT, INT> MemberFuncStruct5MulticastDelegate;
	MemberFuncStruct5MulticastDelegate += MakeDelegate(&testClass5, &TestClass5::MemberFuncStruct5, &testThread);
	if (MemberFuncStruct5MulticastDelegate)
		MemberFuncStruct5MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe5<StructParam*, INT, INT, INT, INT> MemberFuncStructPtr5MulticastDelegate;
	MemberFuncStructPtr5MulticastDelegate += MakeDelegate(&testClass5, &TestClass5::MemberFuncStructPtr5, &testThread);
	MemberFuncStructPtr5MulticastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe5<const StructParam*, INT, INT, INT, INT> MemberFuncStructConstPtr5MulticastDelegate;
	MemberFuncStructConstPtr5MulticastDelegate += MakeDelegate(&testClass5, &TestClass5::MemberFuncStructConstPtr5, &testThread);
	MemberFuncStructConstPtr5MulticastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe5<StructParam&, INT, INT, INT, INT> MemberFuncStructRef5MulticastDelegate;
	MemberFuncStructRef5MulticastDelegate += MakeDelegate(&testClass5, &TestClass5::MemberFuncStructRef5, &testThread);
	MemberFuncStructRef5MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe5<const StructParam&, INT, INT, INT, INT> MemberFuncStructConstRef5MulticastDelegate;
	MemberFuncStructConstRef5MulticastDelegate += MakeDelegate(&testClass5, &TestClass5::MemberFuncStructConstRef5, &testThread);
	MemberFuncStructConstRef5MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	// N=5 Static Functions
	MulticastDelegateSafe5<INT, INT, INT, INT, INT> StaticFuncInt5MulticastDelegate;
	StaticFuncInt5MulticastDelegate += MakeDelegate(&TestClass5::StaticFuncInt5, &testThread);
	StaticFuncInt5MulticastDelegate(TEST_INT, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe5<StructParam, INT, INT, INT, INT> StaticFuncStruct5MulticastDelegate;
	StaticFuncStruct5MulticastDelegate += MakeDelegate(&TestClass5::StaticFuncStruct5, &testThread);
	StaticFuncStruct5MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe5<StructParam*, INT, INT, INT, INT> StaticFuncStructPtr5MulticastDelegate;
	StaticFuncStructPtr5MulticastDelegate += MakeDelegate(&TestClass5::StaticFuncStructPtr5, &testThread);
	StaticFuncStructPtr5MulticastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe5<const StructParam*, INT, INT, INT, INT> StaticFuncStructConstPtr5MulticastDelegate;
	StaticFuncStructConstPtr5MulticastDelegate += MakeDelegate(&TestClass5::StaticFuncStructConstPtr5, &testThread);
	StaticFuncStructConstPtr5MulticastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe5<StructParam&, INT, INT, INT, INT> StaticFuncStructRef5MulticastDelegate;
	StaticFuncStructRef5MulticastDelegate += MakeDelegate(&TestClass5::StaticFuncStructRef5, &testThread);
	StaticFuncStructRef5MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe5<const StructParam&, INT, INT, INT, INT> StaticFuncStructConstRef5MulticastDelegate;
	StaticFuncStructConstRef5MulticastDelegate += MakeDelegate(&TestClass5::StaticFuncStructConstRef5, &testThread);
	StaticFuncStructConstRef5MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);
}

void DelegateMemberSpTests()
{
#if USE_CPLUSPLUS_11
	std::shared_ptr<TestClass0> testClass0(new TestClass0());
	auto DelegateMemberSp0 = MakeDelegate(testClass0, &TestClass0::MemberFunc0);
	DelegateMemberSp0();

	std::shared_ptr<TestClass1> testClass1(new TestClass1());
	auto DelegateMemberSp1 = MakeDelegate(testClass1, &TestClass1::MemberFuncInt1);
	DelegateMemberSp1(TEST_INT);

	std::shared_ptr<TestClass2> testClass2(new TestClass2());
	auto DelegateMemberSp2 = MakeDelegate(testClass2, &TestClass2::MemberFuncInt2);
	DelegateMemberSp2(TEST_INT, TEST_INT);

	std::shared_ptr<TestClass3> testClass3(new TestClass3());
	auto DelegateMemberSp3 = MakeDelegate(testClass3, &TestClass3::MemberFuncInt3);
	DelegateMemberSp3(TEST_INT, TEST_INT, TEST_INT);

	std::shared_ptr<TestClass4> testClass4(new TestClass4());
	auto DelegateMemberSp4 = MakeDelegate(testClass4, &TestClass4::MemberFuncInt4);
	DelegateMemberSp4(TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	std::shared_ptr<TestClass5> testClass5(new TestClass5());
	auto DelegateMemberSp5 = MakeDelegate(testClass5, &TestClass5::MemberFuncInt5);
	DelegateMemberSp5(TEST_INT, TEST_INT, TEST_INT, TEST_INT, TEST_INT);
#endif
}

void DelegateMemberAsyncSpTests()
{
#if USE_CPLUSPLUS_11
	std::shared_ptr<TestClass0> testClass0(new TestClass0());
	auto DelegateMemberAsyncSp0 = MakeDelegate(testClass0, &TestClass0::MemberFunc0, &testThread);
	DelegateMemberAsyncSp0();

	std::shared_ptr<TestClass1> testClass1(new TestClass1());
	auto DelegateMemberAsyncSp1 = MakeDelegate(testClass1, &TestClass1::MemberFuncInt1, &testThread);
	DelegateMemberAsyncSp1(TEST_INT);

	std::shared_ptr<TestClass2> testClass2(new TestClass2());
	auto DelegateMemberAsyncSp2 = MakeDelegate(testClass2, &TestClass2::MemberFuncInt2, &testThread);
	DelegateMemberAsyncSp2(TEST_INT, TEST_INT);

	std::shared_ptr<TestClass3> testClass3(new TestClass3());
	auto DelegateMemberAsyncSp3 = MakeDelegate(testClass3, &TestClass3::MemberFuncInt3, &testThread);
	DelegateMemberAsyncSp3(TEST_INT, TEST_INT, TEST_INT);

	std::shared_ptr<TestClass4> testClass4(new TestClass4());
	auto DelegateMemberAsyncSp4 = MakeDelegate(testClass4, &TestClass4::MemberFuncInt4, &testThread);
	DelegateMemberAsyncSp4(TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	std::shared_ptr<TestClass5> testClass5(new TestClass5());
	auto DelegateMemberAsyncSp5 = MakeDelegate(testClass5, &TestClass5::MemberFuncInt5, &testThread);
	DelegateMemberAsyncSp5(TEST_INT, TEST_INT, TEST_INT, TEST_INT, TEST_INT);
#endif
}

void DelegateMemberAsyncWaitTests()
{
#if USE_CPLUSPLUS_11
	const int LOOP_CNT = 100;
	StructParam structParam;
	structParam.val = TEST_INT;
	StructParam* pStructParam = &structParam;

	// N=0 Free Functions
	MulticastDelegateSafe0 FreeFunc0MulticastDelegate;
	ASSERT_TRUE(FreeFunc0MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!FreeFunc0MulticastDelegate);
	FreeFunc0MulticastDelegate += MakeDelegate(&FreeFunc0, &testThread, WAIT_INFINITE);
	FreeFunc0MulticastDelegate += MakeDelegate(&FreeFunc0, &testThread, WAIT_INFINITE);
	ASSERT_TRUE(FreeFunc0MulticastDelegate.Empty() == false);
	ASSERT_TRUE(FreeFunc0MulticastDelegate);
	FreeFunc0MulticastDelegate();
	FreeFunc0MulticastDelegate -= MakeDelegate(&FreeFunc0, &testThread, WAIT_INFINITE);
	ASSERT_TRUE(FreeFunc0MulticastDelegate);
	FreeFunc0MulticastDelegate.Clear();
	ASSERT_TRUE(!FreeFunc0MulticastDelegate);

	// N=0 Member Functions
	TestClass0 testClass0;

	MulticastDelegateSafe0 MemberFunc0MulticastDelegate;
	ASSERT_TRUE(MemberFunc0MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!MemberFunc0MulticastDelegate);
	//auto d1 = MakeDelegate(&testClass0, &TestClass0::MemberFunc0, &testThread, WAIT_INFINITE);
	//MemberFunc0MulticastDelegate += d1;
	MemberFunc0MulticastDelegate += MakeDelegate(&testClass0, &TestClass0::MemberFunc0, &testThread, WAIT_INFINITE);
	MemberFunc0MulticastDelegate += MakeDelegate(&testClass0, &TestClass0::MemberFunc0Const, &testThread, WAIT_INFINITE);
	ASSERT_TRUE(MemberFunc0MulticastDelegate.Empty() == false);
	ASSERT_TRUE(MemberFunc0MulticastDelegate);
	if (MemberFunc0MulticastDelegate)
		MemberFunc0MulticastDelegate();
	MemberFunc0MulticastDelegate.Clear();
	ASSERT_TRUE(!MemberFunc0MulticastDelegate);

	// N=0 Static Functions
	MulticastDelegateSafe0 StaticFunc0MulticastDelegate;
	StaticFunc0MulticastDelegate += MakeDelegate(&TestClass0::StaticFunc0, &testThread, WAIT_INFINITE);
	StaticFunc0MulticastDelegate();

	// N=0 Free/Member Functions with Return
	auto FreeFuncIntWithReturn0Delegate = MakeDelegate(&FreeFuncIntWithReturn0, &testThread, WAIT_INFINITE);
	ASSERT_TRUE(FreeFuncIntWithReturn0Delegate);
	if (FreeFuncIntWithReturn0Delegate) {
		ASSERT_TRUE(FreeFuncIntWithReturn0Delegate() == TEST_INT);
		ASSERT_TRUE(FreeFuncIntWithReturn0Delegate.IsSuccess() == true);
		FreeFuncIntWithReturn0Delegate.Clear();
	}

	auto MemberFuncIntWithReturn0Delegate = MakeDelegate(&testClass0, &TestClass0::MemberFuncWithReturn0, &testThread, WAIT_INFINITE);
	ASSERT_TRUE(MemberFuncIntWithReturn0Delegate);
	if (MemberFuncIntWithReturn0Delegate) {
		ASSERT_TRUE(MemberFuncIntWithReturn0Delegate() == TEST_INT);
		ASSERT_TRUE(MemberFuncIntWithReturn0Delegate.IsSuccess() == true);
		MemberFuncIntWithReturn0Delegate.Clear();
	}

	FreeFuncIntWithReturn0Delegate = MakeDelegate(&FreeFuncIntWithReturn0, &testThread, 1);
	for (int i = 0; i < LOOP_CNT; i++)
		int ret = FreeFuncIntWithReturn0Delegate();

	MemberFuncIntWithReturn0Delegate = MakeDelegate(&testClass0, &TestClass0::MemberFuncWithReturn0, &testThread, 1);
	for (int i = 0; i < LOOP_CNT; i++)
		int ret = MemberFuncIntWithReturn0Delegate();

	// N=1 Free Functions
	MulticastDelegateSafe1<INT> FreeFuncInt1MulticastDelegate;
	ASSERT_TRUE(FreeFuncInt1MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!FreeFuncInt1MulticastDelegate);
	FreeFuncInt1MulticastDelegate += MakeDelegate(&FreeFuncInt1, &testThread, WAIT_INFINITE);
	FreeFuncInt1MulticastDelegate += MakeDelegate(&FreeFuncInt1, &testThread, WAIT_INFINITE);
	ASSERT_TRUE(FreeFuncInt1MulticastDelegate.Empty() == false);
	ASSERT_TRUE(FreeFuncInt1MulticastDelegate);
	FreeFuncInt1MulticastDelegate(TEST_INT);
	FreeFuncInt1MulticastDelegate -= MakeDelegate(&FreeFuncInt1, &testThread, WAIT_INFINITE);
	ASSERT_TRUE(FreeFuncInt1MulticastDelegate);
	FreeFuncInt1MulticastDelegate.Clear();
	ASSERT_TRUE(!FreeFuncInt1MulticastDelegate);

	MulticastDelegateSafe1<StructParam> FreeFuncStruct1MulticastDelegate;
	FreeFuncStruct1MulticastDelegate += MakeDelegate(&FreeFuncStruct1);
	FreeFuncStruct1MulticastDelegate(structParam);

	MulticastDelegateSafe1<StructParam*> FreeFuncStructPtr1MulticastDelegate;
	FreeFuncStructPtr1MulticastDelegate += MakeDelegate(&FreeFuncStructPtr1, &testThread, WAIT_INFINITE);
	FreeFuncStructPtr1MulticastDelegate(&structParam);

	MulticastDelegateSafe1<const StructParam*> FreeFuncStructConstPtr1MulticastDelegate;
	FreeFuncStructConstPtr1MulticastDelegate += MakeDelegate(&FreeFuncStructConstPtr1, &testThread, WAIT_INFINITE);
	FreeFuncStructConstPtr1MulticastDelegate(&structParam);

	MulticastDelegateSafe1<StructParam&> FreeFuncStructRef1MulticastDelegate;
	FreeFuncStructRef1MulticastDelegate += MakeDelegate(&FreeFuncStructRef1, &testThread, WAIT_INFINITE);
	FreeFuncStructRef1MulticastDelegate(structParam);

	MulticastDelegateSafe1<const StructParam&> FreeFuncStructConstRef1MulticastDelegate;
	FreeFuncStructConstRef1MulticastDelegate += MakeDelegate(&FreeFuncStructConstRef1, &testThread, WAIT_INFINITE);
	FreeFuncStructConstRef1MulticastDelegate(structParam);

	// N=1 Member Functions
	TestClass1 testClass1;

	MulticastDelegateSafe1<INT> MemberFuncInt1MulticastDelegate;
	ASSERT_TRUE(MemberFuncInt1MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!MemberFuncInt1MulticastDelegate);
	MemberFuncInt1MulticastDelegate += MakeDelegate(&testClass1, &TestClass1::MemberFuncInt1, &testThread, WAIT_INFINITE);
	MemberFuncInt1MulticastDelegate += MakeDelegate(&testClass1, &TestClass1::MemberFuncInt1Const, &testThread, WAIT_INFINITE);
	ASSERT_TRUE(MemberFuncInt1MulticastDelegate.Empty() == false);
	ASSERT_TRUE(MemberFuncInt1MulticastDelegate);
	MemberFuncInt1MulticastDelegate(TEST_INT);
	MemberFuncInt1MulticastDelegate.Clear();
	ASSERT_TRUE(!MemberFuncInt1MulticastDelegate);

	MulticastDelegateSafe1<StructParam> MemberFuncStruct1MulticastDelegate;
	MemberFuncStruct1MulticastDelegate += MakeDelegate(&testClass1, &TestClass1::MemberFuncStruct1, &testThread, WAIT_INFINITE);
	if (MemberFuncStruct1MulticastDelegate)
		MemberFuncStruct1MulticastDelegate(structParam);

	MulticastDelegateSafe1<StructParam*> MemberFuncStructPtr1MulticastDelegate;
	MemberFuncStructPtr1MulticastDelegate += MakeDelegate(&testClass1, &TestClass1::MemberFuncStructPtr1, &testThread, WAIT_INFINITE);
	MemberFuncStructPtr1MulticastDelegate(&structParam);

	MulticastDelegateSafe1<StructParam**> MemberFuncStructPtrPtr1MulticastDelegate;
	MemberFuncStructPtrPtr1MulticastDelegate += MakeDelegate(&testClass1, &TestClass1::MemberFuncStructPtrPtr1, &testThread, WAIT_INFINITE);
	MemberFuncStructPtrPtr1MulticastDelegate(&pStructParam);

	MulticastDelegateSafe1<const StructParam*> MemberFuncStructConstPtr1MulticastDelegate;
	MemberFuncStructConstPtr1MulticastDelegate += MakeDelegate(&testClass1, &TestClass1::MemberFuncStructConstPtr1, &testThread, WAIT_INFINITE);
	MemberFuncStructConstPtr1MulticastDelegate(&structParam);

	MulticastDelegateSafe1<StructParam&> MemberFuncStructRef1MulticastDelegate;
	MemberFuncStructRef1MulticastDelegate += MakeDelegate(&testClass1, &TestClass1::MemberFuncStructRef1, &testThread, WAIT_INFINITE);
	MemberFuncStructRef1MulticastDelegate(structParam);

	MulticastDelegateSafe1<const StructParam&> MemberFuncStructConstRef1MulticastDelegate;
	MemberFuncStructConstRef1MulticastDelegate += MakeDelegate(&testClass1, &TestClass1::MemberFuncStructConstRef1, &testThread, WAIT_INFINITE);
	MemberFuncStructConstRef1MulticastDelegate(structParam);

	// N=1 Static Functions
	MulticastDelegateSafe1<INT> StaticFuncInt1MulticastDelegate;
	StaticFuncInt1MulticastDelegate += MakeDelegate(&TestClass1::StaticFuncInt1, &testThread, WAIT_INFINITE);
	StaticFuncInt1MulticastDelegate(TEST_INT);

	MulticastDelegateSafe1<StructParam> StaticFuncStruct1MulticastDelegate;
	StaticFuncStruct1MulticastDelegate += MakeDelegate(&TestClass1::StaticFuncStruct1, &testThread, WAIT_INFINITE);
	StaticFuncStruct1MulticastDelegate(structParam);

	MulticastDelegateSafe1<StructParam*> StaticFuncStructPtr1MulticastDelegate;
	StaticFuncStructPtr1MulticastDelegate += MakeDelegate(&TestClass1::StaticFuncStructPtr1, &testThread, WAIT_INFINITE);
	StaticFuncStructPtr1MulticastDelegate(&structParam);

	MulticastDelegateSafe1<const StructParam*> StaticFuncStructConstPtr1MulticastDelegate;
	StaticFuncStructConstPtr1MulticastDelegate += MakeDelegate(&TestClass1::StaticFuncStructConstPtr1, &testThread, WAIT_INFINITE);
	StaticFuncStructConstPtr1MulticastDelegate(&structParam);

	MulticastDelegateSafe1<StructParam&> StaticFuncStructRef1MulticastDelegate;
	StaticFuncStructRef1MulticastDelegate += MakeDelegate(&TestClass1::StaticFuncStructRef1, &testThread, WAIT_INFINITE);
	StaticFuncStructRef1MulticastDelegate(structParam);

	MulticastDelegateSafe1<const StructParam&> StaticFuncStructConstRef1MulticastDelegate;
	StaticFuncStructConstRef1MulticastDelegate += MakeDelegate(&TestClass1::StaticFuncStructConstRef1, &testThread, WAIT_INFINITE);
	StaticFuncStructConstRef1MulticastDelegate(structParam);

	// N=1 Free/Member Functions with Return
	auto FreeFuncIntWithReturn1Delegate = MakeDelegate(&FreeFuncIntWithReturn1, &testThread, WAIT_INFINITE);
	ASSERT_TRUE(FreeFuncIntWithReturn1Delegate);
	if (FreeFuncIntWithReturn1Delegate) {
		ASSERT_TRUE(FreeFuncIntWithReturn1Delegate(TEST_INT) == TEST_INT);
		ASSERT_TRUE(FreeFuncIntWithReturn1Delegate.IsSuccess() == true);
		FreeFuncIntWithReturn1Delegate.Clear();
	}

	auto MemberFuncIntWithReturn1Delegate = MakeDelegate(&testClass1, &TestClass1::MemberFuncIntWithReturn1, &testThread, WAIT_INFINITE);
	ASSERT_TRUE(MemberFuncIntWithReturn1Delegate);
	if (MemberFuncIntWithReturn1Delegate) {
		ASSERT_TRUE(MemberFuncIntWithReturn1Delegate(TEST_INT) == TEST_INT);
		ASSERT_TRUE(MemberFuncIntWithReturn1Delegate.IsSuccess() == true);
		MemberFuncIntWithReturn1Delegate.Clear();
	}

	FreeFuncIntWithReturn1Delegate = MakeDelegate(&FreeFuncIntWithReturn1, &testThread, 1);
	for (int i = 0; i < LOOP_CNT; i++)
		int ret = FreeFuncIntWithReturn1Delegate(TEST_INT);

	MemberFuncIntWithReturn1Delegate = MakeDelegate(&testClass1, &TestClass1::MemberFuncIntWithReturn1, &testThread, 1);
	for (int i = 0; i < LOOP_CNT; i++)
		int ret = MemberFuncIntWithReturn1Delegate(TEST_INT);

	// N=2 Free Functions
	MulticastDelegateSafe2<INT, INT> FreeFuncInt2MulticastDelegate;
	ASSERT_TRUE(FreeFuncInt2MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!FreeFuncInt2MulticastDelegate);
	FreeFuncInt2MulticastDelegate += MakeDelegate(&FreeFuncInt2, &testThread, WAIT_INFINITE);
	FreeFuncInt2MulticastDelegate += MakeDelegate(&FreeFuncInt2, &testThread, WAIT_INFINITE);
	ASSERT_TRUE(FreeFuncInt2MulticastDelegate.Empty() == false);
	ASSERT_TRUE(FreeFuncInt2MulticastDelegate);
	FreeFuncInt2MulticastDelegate(TEST_INT, TEST_INT);
	FreeFuncInt2MulticastDelegate -= MakeDelegate(&FreeFuncInt2, &testThread, WAIT_INFINITE);
	ASSERT_TRUE(FreeFuncInt2MulticastDelegate);
	FreeFuncInt2MulticastDelegate.Clear();
	ASSERT_TRUE(!FreeFuncInt2MulticastDelegate);

	MulticastDelegateSafe2<StructParam, INT> FreeFuncStruct2MulticastDelegate;
	FreeFuncStruct2MulticastDelegate += MakeDelegate(&FreeFuncStruct2, &testThread, WAIT_INFINITE);
	FreeFuncStruct2MulticastDelegate(structParam, TEST_INT);

	MulticastDelegateSafe2<StructParam*, INT> FreeFuncStructPtr2MulticastDelegate;
	FreeFuncStructPtr2MulticastDelegate += MakeDelegate(&FreeFuncStructPtr2, &testThread, WAIT_INFINITE);
	FreeFuncStructPtr2MulticastDelegate(&structParam, TEST_INT);

	MulticastDelegateSafe2<const StructParam*, INT> FreeFuncStructConstPtr2MulticastDelegate;
	FreeFuncStructConstPtr2MulticastDelegate += MakeDelegate(&FreeFuncStructConstPtr2, &testThread, WAIT_INFINITE);
	FreeFuncStructConstPtr2MulticastDelegate(&structParam, TEST_INT);

	MulticastDelegateSafe2<StructParam&, INT> FreeFuncStructRef2MulticastDelegate;
	FreeFuncStructRef2MulticastDelegate += MakeDelegate(&FreeFuncStructRef2, &testThread, WAIT_INFINITE);
	FreeFuncStructRef2MulticastDelegate(structParam, TEST_INT);

	MulticastDelegateSafe2<const StructParam&, INT> FreeFuncStructConstRef2MulticastDelegate;
	FreeFuncStructConstRef2MulticastDelegate += MakeDelegate(&FreeFuncStructConstRef2, &testThread, WAIT_INFINITE);
	FreeFuncStructConstRef2MulticastDelegate(structParam, TEST_INT);

	// N=2 Member Functions
	TestClass2 testClass2;

	MulticastDelegateSafe2<INT, INT> MemberFuncInt2MulticastDelegate;
	ASSERT_TRUE(MemberFuncInt2MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!MemberFuncInt2MulticastDelegate);
	MemberFuncInt2MulticastDelegate += MakeDelegate(&testClass2, &TestClass2::MemberFuncInt2, &testThread, WAIT_INFINITE);
	MemberFuncInt2MulticastDelegate += MakeDelegate(&testClass2, &TestClass2::MemberFuncInt2Const, &testThread, WAIT_INFINITE);
	ASSERT_TRUE(MemberFuncInt2MulticastDelegate.Empty() == false);
	ASSERT_TRUE(MemberFuncInt2MulticastDelegate);
	MemberFuncInt2MulticastDelegate(TEST_INT, TEST_INT);
	MemberFuncInt2MulticastDelegate.Clear();
	ASSERT_TRUE(!MemberFuncInt2MulticastDelegate);

	MulticastDelegateSafe2<StructParam, INT> MemberFuncStruct2MulticastDelegate;
	MemberFuncStruct2MulticastDelegate += MakeDelegate(&testClass2, &TestClass2::MemberFuncStruct2, &testThread, WAIT_INFINITE);
	if (MemberFuncStruct2MulticastDelegate)
		MemberFuncStruct2MulticastDelegate(structParam, TEST_INT);

	MulticastDelegateSafe2<StructParam*, INT> MemberFuncStructPtr2MulticastDelegate;
	MemberFuncStructPtr2MulticastDelegate += MakeDelegate(&testClass2, &TestClass2::MemberFuncStructPtr2, &testThread, WAIT_INFINITE);
	MemberFuncStructPtr2MulticastDelegate(&structParam, TEST_INT);

	MulticastDelegateSafe2<const StructParam*, INT> MemberFuncStructConstPtr2MulticastDelegate;
	MemberFuncStructConstPtr2MulticastDelegate += MakeDelegate(&testClass2, &TestClass2::MemberFuncStructConstPtr2, &testThread, WAIT_INFINITE);
	MemberFuncStructConstPtr2MulticastDelegate(&structParam, TEST_INT);

	MulticastDelegateSafe2<StructParam&, INT> MemberFuncStructRef2MulticastDelegate;
	MemberFuncStructRef2MulticastDelegate += MakeDelegate(&testClass2, &TestClass2::MemberFuncStructRef2, &testThread, WAIT_INFINITE);
	MemberFuncStructRef2MulticastDelegate(structParam, TEST_INT);

	MulticastDelegateSafe2<const StructParam&, INT> MemberFuncStructConstRef2MulticastDelegate;
	MemberFuncStructConstRef2MulticastDelegate += MakeDelegate(&testClass2, &TestClass2::MemberFuncStructConstRef2, &testThread, WAIT_INFINITE);
	MemberFuncStructConstRef2MulticastDelegate(structParam, TEST_INT);

	// N=2 Static Functions
	MulticastDelegateSafe2<INT, INT> StaticFuncInt2MulticastDelegate;
	StaticFuncInt2MulticastDelegate += MakeDelegate(&TestClass2::StaticFuncInt2, &testThread, WAIT_INFINITE);
	StaticFuncInt2MulticastDelegate(TEST_INT, TEST_INT);

	MulticastDelegateSafe2<StructParam, INT> StaticFuncStruct2MulticastDelegate;
	StaticFuncStruct2MulticastDelegate += MakeDelegate(&TestClass2::StaticFuncStruct2, &testThread, WAIT_INFINITE);
	StaticFuncStruct2MulticastDelegate(structParam, TEST_INT);

	MulticastDelegateSafe2<StructParam*, INT> StaticFuncStructPtr2MulticastDelegate;
	StaticFuncStructPtr2MulticastDelegate += MakeDelegate(&TestClass2::StaticFuncStructPtr2, &testThread, WAIT_INFINITE);
	StaticFuncStructPtr2MulticastDelegate(&structParam, TEST_INT);

	MulticastDelegateSafe2<const StructParam*, INT> StaticFuncStructConstPtr2MulticastDelegate;
	StaticFuncStructConstPtr2MulticastDelegate += MakeDelegate(&TestClass2::StaticFuncStructConstPtr2, &testThread, WAIT_INFINITE);
	StaticFuncStructConstPtr2MulticastDelegate(&structParam, TEST_INT);

	MulticastDelegateSafe2<StructParam&, INT> StaticFuncStructRef2MulticastDelegate;
	StaticFuncStructRef2MulticastDelegate += MakeDelegate(&TestClass2::StaticFuncStructRef2, &testThread, WAIT_INFINITE);
	StaticFuncStructRef2MulticastDelegate(structParam, TEST_INT);

	MulticastDelegateSafe2<const StructParam&, INT> StaticFuncStructConstRef2MulticastDelegate;
	StaticFuncStructConstRef2MulticastDelegate += MakeDelegate(&TestClass2::StaticFuncStructConstRef2, &testThread, WAIT_INFINITE);
	StaticFuncStructConstRef2MulticastDelegate(structParam, TEST_INT);

	// N=2 Free/Member Functions with Return
	auto FreeFuncIntWithReturn2Delegate = MakeDelegate(&FreeFuncIntWithReturn2, &testThread, WAIT_INFINITE);
	ASSERT_TRUE(FreeFuncIntWithReturn2Delegate);
	if (FreeFuncIntWithReturn2Delegate) {
		ASSERT_TRUE(FreeFuncIntWithReturn2Delegate(TEST_INT, TEST_INT) == TEST_INT);
		ASSERT_TRUE(FreeFuncIntWithReturn2Delegate.IsSuccess() == true);
		FreeFuncIntWithReturn2Delegate.Clear();
	}

	auto MemberFuncIntWithReturn2Delegate = MakeDelegate(&testClass2, &TestClass2::MemberFuncIntWithReturn2, &testThread, WAIT_INFINITE);
	ASSERT_TRUE(MemberFuncIntWithReturn2Delegate);
	if (MemberFuncIntWithReturn2Delegate) {
		ASSERT_TRUE(MemberFuncIntWithReturn2Delegate(TEST_INT, TEST_INT) == TEST_INT);
		ASSERT_TRUE(MemberFuncIntWithReturn2Delegate.IsSuccess() == true);
		MemberFuncIntWithReturn2Delegate.Clear();
	}

	FreeFuncIntWithReturn2Delegate = MakeDelegate(&FreeFuncIntWithReturn2, &testThread, 1);
	for (int i = 0; i < LOOP_CNT; i++)
		int ret = FreeFuncIntWithReturn2Delegate(TEST_INT, TEST_INT);

	MemberFuncIntWithReturn2Delegate = MakeDelegate(&testClass2, &TestClass2::MemberFuncIntWithReturn2, &testThread, 1);
	for (int i = 0; i < LOOP_CNT; i++)
		int ret = MemberFuncIntWithReturn2Delegate(TEST_INT, TEST_INT);

	// N=3 Free Functions
	MulticastDelegateSafe3<INT, INT, INT> FreeFuncInt3MulticastDelegate;
	ASSERT_TRUE(FreeFuncInt3MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!FreeFuncInt3MulticastDelegate);
	FreeFuncInt3MulticastDelegate += MakeDelegate(&FreeFuncInt3, &testThread, WAIT_INFINITE);
	FreeFuncInt3MulticastDelegate += MakeDelegate(&FreeFuncInt3, &testThread, WAIT_INFINITE);
	ASSERT_TRUE(FreeFuncInt3MulticastDelegate.Empty() == false);
	ASSERT_TRUE(FreeFuncInt3MulticastDelegate);
	FreeFuncInt3MulticastDelegate(TEST_INT, TEST_INT, TEST_INT);
	FreeFuncInt3MulticastDelegate -= MakeDelegate(&FreeFuncInt3, &testThread, WAIT_INFINITE);
	ASSERT_TRUE(FreeFuncInt3MulticastDelegate);
	FreeFuncInt3MulticastDelegate.Clear();
	ASSERT_TRUE(!FreeFuncInt3MulticastDelegate);

	MulticastDelegateSafe3<StructParam, INT, INT> FreeFuncStruct3MulticastDelegate;
	FreeFuncStruct3MulticastDelegate += MakeDelegate(&FreeFuncStruct3, &testThread, WAIT_INFINITE);
	FreeFuncStruct3MulticastDelegate(structParam, TEST_INT, TEST_INT);

	MulticastDelegateSafe3<StructParam*, INT, INT> FreeFuncStructPtr3MulticastDelegate;
	FreeFuncStructPtr3MulticastDelegate += MakeDelegate(&FreeFuncStructPtr3, &testThread, WAIT_INFINITE);
	FreeFuncStructPtr3MulticastDelegate(&structParam, TEST_INT, TEST_INT);

	MulticastDelegateSafe3<const StructParam*, INT, INT> FreeFuncStructConstPtr3MulticastDelegate;
	FreeFuncStructConstPtr3MulticastDelegate += MakeDelegate(&FreeFuncStructConstPtr3, &testThread, WAIT_INFINITE);
	FreeFuncStructConstPtr3MulticastDelegate(&structParam, TEST_INT, TEST_INT);

	MulticastDelegateSafe3<StructParam&, INT, INT> FreeFuncStructRef3MulticastDelegate;
	FreeFuncStructRef3MulticastDelegate += MakeDelegate(&FreeFuncStructRef3, &testThread, WAIT_INFINITE);
	FreeFuncStructRef3MulticastDelegate(structParam, TEST_INT, TEST_INT);

	MulticastDelegateSafe3<const StructParam&, INT, INT> FreeFuncStructConstRef3MulticastDelegate;
	FreeFuncStructConstRef3MulticastDelegate += MakeDelegate(&FreeFuncStructConstRef3, &testThread, WAIT_INFINITE);
	FreeFuncStructConstRef3MulticastDelegate(structParam, TEST_INT, TEST_INT);

	// N=3 Member Functions
	TestClass3 testClass3;

	MulticastDelegateSafe3<INT, INT, INT> MemberFuncInt3MulticastDelegate;
	ASSERT_TRUE(MemberFuncInt3MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!MemberFuncInt3MulticastDelegate);
	MemberFuncInt3MulticastDelegate += MakeDelegate(&testClass3, &TestClass3::MemberFuncInt3, &testThread, WAIT_INFINITE);
	MemberFuncInt3MulticastDelegate += MakeDelegate(&testClass3, &TestClass3::MemberFuncInt3Const, &testThread, WAIT_INFINITE);
	ASSERT_TRUE(MemberFuncInt3MulticastDelegate.Empty() == false);
	ASSERT_TRUE(MemberFuncInt3MulticastDelegate);
	MemberFuncInt3MulticastDelegate(TEST_INT, TEST_INT, TEST_INT);
	MemberFuncInt3MulticastDelegate.Clear();
	ASSERT_TRUE(!MemberFuncInt3MulticastDelegate);

	MulticastDelegateSafe3<StructParam, INT, INT> MemberFuncStruct3MulticastDelegate;
	MemberFuncStruct3MulticastDelegate += MakeDelegate(&testClass3, &TestClass3::MemberFuncStruct3, &testThread, WAIT_INFINITE);
	if (MemberFuncStruct3MulticastDelegate)
		MemberFuncStruct3MulticastDelegate(structParam, TEST_INT, TEST_INT);

	MulticastDelegateSafe3<StructParam*, INT, INT> MemberFuncStructPtr3MulticastDelegate;
	MemberFuncStructPtr3MulticastDelegate += MakeDelegate(&testClass3, &TestClass3::MemberFuncStructPtr3, &testThread, WAIT_INFINITE);
	MemberFuncStructPtr3MulticastDelegate(&structParam, TEST_INT, TEST_INT);

	MulticastDelegateSafe3<const StructParam*, INT, INT> MemberFuncStructConstPtr3MulticastDelegate;
	MemberFuncStructConstPtr3MulticastDelegate += MakeDelegate(&testClass3, &TestClass3::MemberFuncStructConstPtr3, &testThread, WAIT_INFINITE);
	MemberFuncStructConstPtr3MulticastDelegate(&structParam, TEST_INT, TEST_INT);

	MulticastDelegateSafe3<StructParam&, INT, INT> MemberFuncStructRef3MulticastDelegate;
	MemberFuncStructRef3MulticastDelegate += MakeDelegate(&testClass3, &TestClass3::MemberFuncStructRef3, &testThread, WAIT_INFINITE);
	MemberFuncStructRef3MulticastDelegate(structParam, TEST_INT, TEST_INT);

	MulticastDelegateSafe3<const StructParam&, INT, INT> MemberFuncStructConstRef3MulticastDelegate;
	MemberFuncStructConstRef3MulticastDelegate += MakeDelegate(&testClass3, &TestClass3::MemberFuncStructConstRef3, &testThread, WAIT_INFINITE);
	MemberFuncStructConstRef3MulticastDelegate(structParam, TEST_INT, TEST_INT);

	// N=3 Static Functions
	MulticastDelegateSafe3<INT, INT, INT> StaticFuncInt3MulticastDelegate;
	StaticFuncInt3MulticastDelegate += MakeDelegate(&TestClass3::StaticFuncInt3, &testThread, WAIT_INFINITE);
	StaticFuncInt3MulticastDelegate(TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe3<StructParam, INT, INT> StaticFuncStruct3MulticastDelegate;
	StaticFuncStruct3MulticastDelegate += MakeDelegate(&TestClass3::StaticFuncStruct3, &testThread, WAIT_INFINITE);
	StaticFuncStruct3MulticastDelegate(structParam, TEST_INT, TEST_INT);

	MulticastDelegateSafe3<StructParam*, INT, INT> StaticFuncStructPtr3MulticastDelegate;
	StaticFuncStructPtr3MulticastDelegate += MakeDelegate(&TestClass3::StaticFuncStructPtr3, &testThread, WAIT_INFINITE);
	StaticFuncStructPtr3MulticastDelegate(&structParam, TEST_INT, TEST_INT);

	MulticastDelegateSafe3<const StructParam*, INT, INT> StaticFuncStructConstPtr3MulticastDelegate;
	StaticFuncStructConstPtr3MulticastDelegate += MakeDelegate(&TestClass3::StaticFuncStructConstPtr3, &testThread, WAIT_INFINITE);
	StaticFuncStructConstPtr3MulticastDelegate(&structParam, TEST_INT, TEST_INT);

	MulticastDelegateSafe3<StructParam&, INT, INT> StaticFuncStructRef3MulticastDelegate;
	StaticFuncStructRef3MulticastDelegate += MakeDelegate(&TestClass3::StaticFuncStructRef3, &testThread, WAIT_INFINITE);
	StaticFuncStructRef3MulticastDelegate(structParam, TEST_INT, TEST_INT);

	MulticastDelegateSafe3<const StructParam&, INT, INT> StaticFuncStructConstRef3MulticastDelegate;
	StaticFuncStructConstRef3MulticastDelegate += MakeDelegate(&TestClass3::StaticFuncStructConstRef3, &testThread, WAIT_INFINITE);
	StaticFuncStructConstRef3MulticastDelegate(structParam, TEST_INT, TEST_INT);

	// N=3 Free/Member Functions with Return
	auto FreeFuncIntWithReturn3Delegate = MakeDelegate(&FreeFuncIntWithReturn3, &testThread, WAIT_INFINITE);
	ASSERT_TRUE(FreeFuncIntWithReturn3Delegate);
	if (FreeFuncIntWithReturn3Delegate) {
		ASSERT_TRUE(FreeFuncIntWithReturn3Delegate(TEST_INT, TEST_INT, TEST_INT) == TEST_INT);
		ASSERT_TRUE(FreeFuncIntWithReturn3Delegate.IsSuccess() == true);
		FreeFuncIntWithReturn3Delegate.Clear();
	}

	auto MemberFuncIntWithReturn3Delegate = MakeDelegate(&testClass3, &TestClass3::MemberFuncIntWithReturn3, &testThread, WAIT_INFINITE);
	ASSERT_TRUE(MemberFuncIntWithReturn3Delegate);
	if (MemberFuncIntWithReturn3Delegate) {
		ASSERT_TRUE(MemberFuncIntWithReturn3Delegate(TEST_INT, TEST_INT, TEST_INT) == TEST_INT);
		ASSERT_TRUE(MemberFuncIntWithReturn3Delegate.IsSuccess() == true);
		MemberFuncIntWithReturn3Delegate.Clear();
	}

	FreeFuncIntWithReturn3Delegate = MakeDelegate(&FreeFuncIntWithReturn3, &testThread, 1);
	for (int i = 0; i < LOOP_CNT; i++)
		int ret = FreeFuncIntWithReturn3Delegate(TEST_INT, TEST_INT, TEST_INT);

	MemberFuncIntWithReturn3Delegate = MakeDelegate(&testClass3, &TestClass3::MemberFuncIntWithReturn3, &testThread, 1);
	for (int i = 0; i < LOOP_CNT; i++)
		int ret = MemberFuncIntWithReturn3Delegate(TEST_INT, TEST_INT, TEST_INT);

	// N=4 Free Functions
	MulticastDelegateSafe4<INT, INT, INT, INT> FreeFuncInt4MulticastDelegate;
	ASSERT_TRUE(FreeFuncInt4MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!FreeFuncInt4MulticastDelegate);
	FreeFuncInt4MulticastDelegate += MakeDelegate(&FreeFuncInt4, &testThread, WAIT_INFINITE);
	FreeFuncInt4MulticastDelegate += MakeDelegate(&FreeFuncInt4, &testThread, WAIT_INFINITE);
	ASSERT_TRUE(FreeFuncInt4MulticastDelegate.Empty() == false);
	ASSERT_TRUE(FreeFuncInt4MulticastDelegate);
	FreeFuncInt4MulticastDelegate(TEST_INT, TEST_INT, TEST_INT, TEST_INT);
	FreeFuncInt4MulticastDelegate -= MakeDelegate(&FreeFuncInt4, &testThread, WAIT_INFINITE);
	ASSERT_TRUE(FreeFuncInt4MulticastDelegate);
	FreeFuncInt4MulticastDelegate.Clear();
	ASSERT_TRUE(!FreeFuncInt4MulticastDelegate);

	MulticastDelegateSafe4<StructParam, INT, INT, INT> FreeFuncStruct4MulticastDelegate;
	FreeFuncStruct4MulticastDelegate += MakeDelegate(&FreeFuncStruct4, &testThread, WAIT_INFINITE);
	FreeFuncStruct4MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe4<StructParam*, INT, INT, INT> FreeFuncStructPtr4MulticastDelegate;
	FreeFuncStructPtr4MulticastDelegate += MakeDelegate(&FreeFuncStructPtr4, &testThread, WAIT_INFINITE);
	FreeFuncStructPtr4MulticastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe4<const StructParam*, INT, INT, INT> FreeFuncStructConstPtr4MulticastDelegate;
	FreeFuncStructConstPtr4MulticastDelegate += MakeDelegate(&FreeFuncStructConstPtr4, &testThread, WAIT_INFINITE);
	FreeFuncStructConstPtr4MulticastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe4<StructParam&, INT, INT, INT> FreeFuncStructRef4MulticastDelegate;
	FreeFuncStructRef4MulticastDelegate += MakeDelegate(&FreeFuncStructRef4, &testThread, WAIT_INFINITE);
	FreeFuncStructRef4MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe4<const StructParam&, INT, INT, INT> FreeFuncStructConstRef4MulticastDelegate;
	FreeFuncStructConstRef4MulticastDelegate += MakeDelegate(&FreeFuncStructConstRef4, &testThread, WAIT_INFINITE);
	FreeFuncStructConstRef4MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT);

	// N=4 Member Functions
	TestClass4 testClass4;

	MulticastDelegateSafe4<INT, INT, INT, INT> MemberFuncInt4MulticastDelegate;
	ASSERT_TRUE(MemberFuncInt4MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!MemberFuncInt4MulticastDelegate);
	MemberFuncInt4MulticastDelegate += MakeDelegate(&testClass4, &TestClass4::MemberFuncInt4, &testThread, WAIT_INFINITE);
	MemberFuncInt4MulticastDelegate += MakeDelegate(&testClass4, &TestClass4::MemberFuncInt4Const, &testThread, WAIT_INFINITE);
	ASSERT_TRUE(MemberFuncInt4MulticastDelegate.Empty() == false);
	ASSERT_TRUE(MemberFuncInt4MulticastDelegate);
	MemberFuncInt4MulticastDelegate(TEST_INT, TEST_INT, TEST_INT, TEST_INT);
	MemberFuncInt4MulticastDelegate.Clear();
	ASSERT_TRUE(!MemberFuncInt4MulticastDelegate);

	MulticastDelegateSafe4<StructParam, INT, INT, INT> MemberFuncStruct4MulticastDelegate;
	MemberFuncStruct4MulticastDelegate += MakeDelegate(&testClass4, &TestClass4::MemberFuncStruct4, &testThread, WAIT_INFINITE);
	if (MemberFuncStruct4MulticastDelegate)
		MemberFuncStruct4MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe4<StructParam*, INT, INT, INT> MemberFuncStructPtr4MulticastDelegate;
	MemberFuncStructPtr4MulticastDelegate += MakeDelegate(&testClass4, &TestClass4::MemberFuncStructPtr4, &testThread, WAIT_INFINITE);
	MemberFuncStructPtr4MulticastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe4<const StructParam*, INT, INT, INT> MemberFuncStructConstPtr4MulticastDelegate;
	MemberFuncStructConstPtr4MulticastDelegate += MakeDelegate(&testClass4, &TestClass4::MemberFuncStructConstPtr4, &testThread, WAIT_INFINITE);
	MemberFuncStructConstPtr4MulticastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe4<StructParam&, INT, INT, INT> MemberFuncStructRef4MulticastDelegate;
	MemberFuncStructRef4MulticastDelegate += MakeDelegate(&testClass4, &TestClass4::MemberFuncStructRef4, &testThread, WAIT_INFINITE);
	MemberFuncStructRef4MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe4<const StructParam&, INT, INT, INT> MemberFuncStructConstRef4MulticastDelegate;
	MemberFuncStructConstRef4MulticastDelegate += MakeDelegate(&testClass4, &TestClass4::MemberFuncStructConstRef4, &testThread, WAIT_INFINITE);
	MemberFuncStructConstRef4MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT);

	// N=4 Static Functions
	MulticastDelegateSafe4<INT, INT, INT, INT> StaticFuncInt4MulticastDelegate;
	StaticFuncInt4MulticastDelegate += MakeDelegate(&TestClass4::StaticFuncInt4, &testThread, WAIT_INFINITE);
	StaticFuncInt4MulticastDelegate(TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe4<StructParam, INT, INT, INT> StaticFuncStruct4MulticastDelegate;
	StaticFuncStruct4MulticastDelegate += MakeDelegate(&TestClass4::StaticFuncStruct4, &testThread, WAIT_INFINITE);
	StaticFuncStruct4MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe4<StructParam*, INT, INT, INT> StaticFuncStructPtr4MulticastDelegate;
	StaticFuncStructPtr4MulticastDelegate += MakeDelegate(&TestClass4::StaticFuncStructPtr4, &testThread, WAIT_INFINITE);
	StaticFuncStructPtr4MulticastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe4<const StructParam*, INT, INT, INT> StaticFuncStructConstPtr4MulticastDelegate;
	StaticFuncStructConstPtr4MulticastDelegate += MakeDelegate(&TestClass4::StaticFuncStructConstPtr4, &testThread, WAIT_INFINITE);
	StaticFuncStructConstPtr4MulticastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe4<StructParam&, INT, INT, INT> StaticFuncStructRef4MulticastDelegate;
	StaticFuncStructRef4MulticastDelegate += MakeDelegate(&TestClass4::StaticFuncStructRef4, &testThread, WAIT_INFINITE);
	StaticFuncStructRef4MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe4<const StructParam&, INT, INT, INT> StaticFuncStructConstRef4MulticastDelegate;
	StaticFuncStructConstRef4MulticastDelegate += MakeDelegate(&TestClass4::StaticFuncStructConstRef4, &testThread, WAIT_INFINITE);
	StaticFuncStructConstRef4MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT);

	// N=4 Free/Member Functions with Return
	auto FreeFuncIntWithReturn4Delegate = MakeDelegate(&FreeFuncIntWithReturn4, &testThread, WAIT_INFINITE);
	ASSERT_TRUE(FreeFuncIntWithReturn4Delegate);
	if (FreeFuncIntWithReturn4Delegate) {
		ASSERT_TRUE(FreeFuncIntWithReturn4Delegate(TEST_INT, TEST_INT, TEST_INT, TEST_INT) == TEST_INT);
		ASSERT_TRUE(FreeFuncIntWithReturn4Delegate.IsSuccess() == true);
		FreeFuncIntWithReturn4Delegate.Clear();
	}

	auto MemberFuncIntWithReturn4Delegate = MakeDelegate(&testClass4, &TestClass4::MemberFuncIntWithReturn4, &testThread, WAIT_INFINITE);
	ASSERT_TRUE(MemberFuncIntWithReturn4Delegate);
	if (MemberFuncIntWithReturn4Delegate) {
		ASSERT_TRUE(MemberFuncIntWithReturn4Delegate(TEST_INT, TEST_INT, TEST_INT, TEST_INT) == TEST_INT);
		ASSERT_TRUE(MemberFuncIntWithReturn4Delegate.IsSuccess() == true);
		MemberFuncIntWithReturn4Delegate.Clear();
	}

	FreeFuncIntWithReturn4Delegate = MakeDelegate(&FreeFuncIntWithReturn4, &testThread, 1);
	for (int i = 0; i < LOOP_CNT; i++)
		int ret = FreeFuncIntWithReturn4Delegate(TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MemberFuncIntWithReturn4Delegate = MakeDelegate(&testClass4, &TestClass4::MemberFuncIntWithReturn4, &testThread, 1);
	for (int i = 0; i < LOOP_CNT; i++)
		int ret = MemberFuncIntWithReturn4Delegate(TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	// N=5 Free Functions
	MulticastDelegateSafe5<INT, INT, INT, INT, INT> FreeFuncInt5MulticastDelegate;
	ASSERT_TRUE(FreeFuncInt5MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!FreeFuncInt5MulticastDelegate);
	FreeFuncInt5MulticastDelegate += MakeDelegate(&FreeFuncInt5, &testThread, WAIT_INFINITE);
	FreeFuncInt5MulticastDelegate += MakeDelegate(&FreeFuncInt5, &testThread, WAIT_INFINITE);
	ASSERT_TRUE(FreeFuncInt5MulticastDelegate.Empty() == false);
	ASSERT_TRUE(FreeFuncInt5MulticastDelegate);
	FreeFuncInt5MulticastDelegate(TEST_INT, TEST_INT, TEST_INT, TEST_INT, TEST_INT);
	FreeFuncInt5MulticastDelegate -= MakeDelegate(&FreeFuncInt5, &testThread, WAIT_INFINITE);
	ASSERT_TRUE(FreeFuncInt5MulticastDelegate);
	FreeFuncInt5MulticastDelegate.Clear();
	ASSERT_TRUE(!FreeFuncInt5MulticastDelegate);

	MulticastDelegateSafe5<StructParam, INT, INT, INT, INT> FreeFuncStruct5MulticastDelegate;
	FreeFuncStruct5MulticastDelegate += MakeDelegate(&FreeFuncStruct5, &testThread, WAIT_INFINITE);
	FreeFuncStruct5MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe5<StructParam*, INT, INT, INT, INT> FreeFuncStructPtr5MulticastDelegate;
	FreeFuncStructPtr5MulticastDelegate += MakeDelegate(&FreeFuncStructPtr5, &testThread, WAIT_INFINITE);
	FreeFuncStructPtr5MulticastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe5<const StructParam*, INT, INT, INT, INT> FreeFuncStructConstPtr5MulticastDelegate;
	FreeFuncStructConstPtr5MulticastDelegate += MakeDelegate(&FreeFuncStructConstPtr5, &testThread, WAIT_INFINITE);
	FreeFuncStructConstPtr5MulticastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe5<StructParam&, INT, INT, INT, INT> FreeFuncStructRef5MulticastDelegate;
	FreeFuncStructRef5MulticastDelegate += MakeDelegate(&FreeFuncStructRef5, &testThread, WAIT_INFINITE);
	FreeFuncStructRef5MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe5<const StructParam&, INT, INT, INT, INT> FreeFuncStructConstRef5MulticastDelegate;
	FreeFuncStructConstRef5MulticastDelegate += MakeDelegate(&FreeFuncStructConstRef5, &testThread, WAIT_INFINITE);
	FreeFuncStructConstRef5MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	// N=5 Member Functions
	TestClass5 testClass5;

	MulticastDelegateSafe5<INT, INT, INT, INT, INT> MemberFuncInt5MulticastDelegate;
	ASSERT_TRUE(MemberFuncInt5MulticastDelegate.Empty() == true);
	ASSERT_TRUE(!MemberFuncInt5MulticastDelegate);
	MemberFuncInt5MulticastDelegate += MakeDelegate(&testClass5, &TestClass5::MemberFuncInt5, &testThread, WAIT_INFINITE);
	MemberFuncInt5MulticastDelegate += MakeDelegate(&testClass5, &TestClass5::MemberFuncInt5Const, &testThread, WAIT_INFINITE);
	ASSERT_TRUE(MemberFuncInt5MulticastDelegate.Empty() == false);
	ASSERT_TRUE(MemberFuncInt5MulticastDelegate);
	MemberFuncInt5MulticastDelegate(TEST_INT, TEST_INT, TEST_INT, TEST_INT, TEST_INT);
	MemberFuncInt5MulticastDelegate.Clear();
	ASSERT_TRUE(!MemberFuncInt5MulticastDelegate);

	MulticastDelegateSafe5<StructParam, INT, INT, INT, INT> MemberFuncStruct5MulticastDelegate;
	MemberFuncStruct5MulticastDelegate += MakeDelegate(&testClass5, &TestClass5::MemberFuncStruct5, &testThread, WAIT_INFINITE);
	if (MemberFuncStruct5MulticastDelegate)
		MemberFuncStruct5MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe5<StructParam*, INT, INT, INT, INT> MemberFuncStructPtr5MulticastDelegate;
	MemberFuncStructPtr5MulticastDelegate += MakeDelegate(&testClass5, &TestClass5::MemberFuncStructPtr5, &testThread, WAIT_INFINITE);
	MemberFuncStructPtr5MulticastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe5<const StructParam*, INT, INT, INT, INT> MemberFuncStructConstPtr5MulticastDelegate;
	MemberFuncStructConstPtr5MulticastDelegate += MakeDelegate(&testClass5, &TestClass5::MemberFuncStructConstPtr5, &testThread, WAIT_INFINITE);
	MemberFuncStructConstPtr5MulticastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe5<StructParam&, INT, INT, INT, INT> MemberFuncStructRef5MulticastDelegate;
	MemberFuncStructRef5MulticastDelegate += MakeDelegate(&testClass5, &TestClass5::MemberFuncStructRef5, &testThread, WAIT_INFINITE);
	MemberFuncStructRef5MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe5<const StructParam&, INT, INT, INT, INT> MemberFuncStructConstRef5MulticastDelegate;
	MemberFuncStructConstRef5MulticastDelegate += MakeDelegate(&testClass5, &TestClass5::MemberFuncStructConstRef5, &testThread, WAIT_INFINITE);
	MemberFuncStructConstRef5MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	// N=5 Static Functions
	MulticastDelegateSafe5<INT, INT, INT, INT, INT> StaticFuncInt5MulticastDelegate;
	StaticFuncInt5MulticastDelegate += MakeDelegate(&TestClass5::StaticFuncInt5, &testThread, WAIT_INFINITE);
	StaticFuncInt5MulticastDelegate(TEST_INT, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe5<StructParam, INT, INT, INT, INT> StaticFuncStruct5MulticastDelegate;
	StaticFuncStruct5MulticastDelegate += MakeDelegate(&TestClass5::StaticFuncStruct5, &testThread, WAIT_INFINITE);
	StaticFuncStruct5MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe5<StructParam*, INT, INT, INT, INT> StaticFuncStructPtr5MulticastDelegate;
	StaticFuncStructPtr5MulticastDelegate += MakeDelegate(&TestClass5::StaticFuncStructPtr5, &testThread, WAIT_INFINITE);
	StaticFuncStructPtr5MulticastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe5<const StructParam*, INT, INT, INT, INT> StaticFuncStructConstPtr5MulticastDelegate;
	StaticFuncStructConstPtr5MulticastDelegate += MakeDelegate(&TestClass5::StaticFuncStructConstPtr5, &testThread, WAIT_INFINITE);
	StaticFuncStructConstPtr5MulticastDelegate(&structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe5<StructParam&, INT, INT, INT, INT> StaticFuncStructRef5MulticastDelegate;
	StaticFuncStructRef5MulticastDelegate += MakeDelegate(&TestClass5::StaticFuncStructRef5, &testThread, WAIT_INFINITE);
	StaticFuncStructRef5MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MulticastDelegateSafe5<const StructParam&, INT, INT, INT, INT> StaticFuncStructConstRef5MulticastDelegate;
	StaticFuncStructConstRef5MulticastDelegate += MakeDelegate(&TestClass5::StaticFuncStructConstRef5, &testThread, WAIT_INFINITE);
	StaticFuncStructConstRef5MulticastDelegate(structParam, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	// N=5 Free/Member Functions with Return
	auto FreeFuncIntWithReturn5Delegate = MakeDelegate(&FreeFuncIntWithReturn5, &testThread, WAIT_INFINITE);
	ASSERT_TRUE(FreeFuncIntWithReturn5Delegate);
	if (FreeFuncIntWithReturn5Delegate) {
		ASSERT_TRUE(FreeFuncIntWithReturn5Delegate(TEST_INT, TEST_INT, TEST_INT, TEST_INT, TEST_INT) == TEST_INT);
		ASSERT_TRUE(FreeFuncIntWithReturn5Delegate.IsSuccess() == true);
		FreeFuncIntWithReturn5Delegate.Clear();
	}

	auto MemberFuncIntWithReturn5Delegate = MakeDelegate(&testClass5, &TestClass5::MemberFuncIntWithReturn5, &testThread, WAIT_INFINITE);
	ASSERT_TRUE(MemberFuncIntWithReturn5Delegate);
	if (MemberFuncIntWithReturn5Delegate) {
		ASSERT_TRUE(MemberFuncIntWithReturn5Delegate(TEST_INT, TEST_INT, TEST_INT, TEST_INT, TEST_INT) == TEST_INT);
		ASSERT_TRUE(MemberFuncIntWithReturn5Delegate.IsSuccess() == true);
		MemberFuncIntWithReturn5Delegate.Clear();
	}

	FreeFuncIntWithReturn5Delegate = MakeDelegate(&FreeFuncIntWithReturn5, &testThread, 1);
	for (int i = 0; i < LOOP_CNT; i++)
		int ret = FreeFuncIntWithReturn5Delegate(TEST_INT, TEST_INT, TEST_INT, TEST_INT, TEST_INT);

	MemberFuncIntWithReturn5Delegate = MakeDelegate(&testClass5, &TestClass5::MemberFuncIntWithReturn5, &testThread, 1);
	for (int i = 0; i < LOOP_CNT; i++)
		int ret = MemberFuncIntWithReturn5Delegate(TEST_INT, TEST_INT, TEST_INT, TEST_INT, TEST_INT);
#endif
}

void DelegateUnitTests()
{
	testThread.CreateThread();

#ifdef WIN32
	LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds, TotalElapsedMicroseconds = { 0 };
	LARGE_INTEGER Frequency;

	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&StartingTime);
#endif

	// Run unit tests repeatedly to expose problems (e.g. deadlocks, memory leaks) 
	// with async delegates.
	for (int i = 0; i < 100; i++)
	{
		SinglecastDelegateTests();
		MulticastDelegateTests();
		MulticastDelegateSafeTests();
		MulticastDelegateSafeAsyncTests();
		DelegateMemberAsyncWaitTests();

#if USE_CPLUSPLUS_11
		DelegateMemberSpTests();
		DelegateMemberAsyncSpTests();
#endif
	}

#ifdef WIN32
	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
	ElapsedMicroseconds.QuadPart *= 1000000;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;
	std::cout << "Elapsed Time: " << (float)ElapsedMicroseconds.QuadPart / 1000000.0f << " seconds" << std::endl;
#endif

	testThread.ExitThread();
}
