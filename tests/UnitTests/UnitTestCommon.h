#pragma once

#include <iostream>
#include <functional>

namespace UnitTestData
{
	static int callCnt = 0;

	static const int TEST_INT = 12345678;

	static std::function<void(int)> LambdaNoCapture = [](int i) {
	};

	static std::function<void(int)> LambdaForcedCapture = +[](int i) {
	};

	struct StructParam { int val; };
	static int FreeFuncIntWithReturn0() { return TEST_INT; }

	static void FreeFunc0() { }

	static void FreeFuncInt1(int i) { ASSERT_TRUE(i == TEST_INT); }
	static int FreeFuncIntWithReturn1(int i) { ASSERT_TRUE(i == TEST_INT); return i; }
	static void FreeFuncPtrPtr1(StructParam** s) { ASSERT_TRUE((*s)->val == TEST_INT); }
	static void FreeFuncStruct1(StructParam s) { ASSERT_TRUE(s.val == TEST_INT); }
	static void FreeFuncStructPtr1(StructParam* s) { ASSERT_TRUE(s->val == TEST_INT); }
	static void FreeFuncStructConstPtr1(const StructParam* s) { ASSERT_TRUE(s->val == TEST_INT); }
	static void FreeFuncStructRef1(StructParam& s) { ASSERT_TRUE(s.val == TEST_INT); }
	static void FreeFuncStructConstRef1(const StructParam& s) { ASSERT_TRUE(s.val == TEST_INT); }

	static void FreeFuncInt2(int i, int i2) { ASSERT_TRUE(i == TEST_INT); ASSERT_TRUE(i2 == TEST_INT); }
	static int FreeFuncIntWithReturn2(int i, int i2) { ASSERT_TRUE(i == TEST_INT); return i; }
	static void FreeFuncPtrPtr2(StructParam** s, int i) { ASSERT_TRUE((*s)->val == TEST_INT); }
	static void FreeFuncStruct2(StructParam s, int i) { ASSERT_TRUE(s.val == TEST_INT); }
	static void FreeFuncStructPtr2(StructParam* s, int i) { ASSERT_TRUE(s->val == TEST_INT); }
	static void FreeFuncStructConstPtr2(const StructParam* s, int i) { ASSERT_TRUE(s->val == TEST_INT); }
	static void FreeFuncStructRef2(StructParam& s, int i) { ASSERT_TRUE(s.val == TEST_INT); }
	static void FreeFuncStructConstRef2(const StructParam& s, int i) { ASSERT_TRUE(s.val == TEST_INT); }

	class TestClass0
	{
	public:
		void MemberFunc0() { }
		void MemberFunc0Const() const { }
		int MemberFuncWithReturn0() { return TEST_INT; }

		static void StaticFunc0() { }
	};

	class TestClass1
	{
	public:
		void MemberFuncInt1(int i) { ASSERT_TRUE(i == TEST_INT); }
		void MemberFuncInt1Const(int i) const { ASSERT_TRUE(i == TEST_INT); }
		int MemberFuncIntWithReturn1(int i) { ASSERT_TRUE(i == TEST_INT); return i; }
		void MemberFuncStruct1(StructParam s) { ASSERT_TRUE(s.val == TEST_INT); }
		void MemberFuncStructPtr1(StructParam* s) { ASSERT_TRUE(s->val == TEST_INT); }
		void MemberFuncStructPtrPtr1(StructParam** s) { ASSERT_TRUE((*s)->val == TEST_INT); }
		void MemberFuncStructConstPtr1(const StructParam* s) { ASSERT_TRUE(s->val == TEST_INT); }
		void MemberFuncStructRef1(StructParam& s) { ASSERT_TRUE(s.val == TEST_INT); }
		void MemberFuncStructConstRef1(const StructParam& s) { ASSERT_TRUE(s.val == TEST_INT); }

		static void StaticFuncInt1(int i) { ASSERT_TRUE(i == TEST_INT); }
		static void StaticFuncStruct1(StructParam s) { ASSERT_TRUE(s.val == TEST_INT); }
		static void StaticFuncStructPtr1(StructParam* s) { ASSERT_TRUE(s->val == TEST_INT); }
		static void StaticFuncStructConstPtr1(const StructParam* s) { ASSERT_TRUE(s->val == TEST_INT); }
		static void StaticFuncStructRef1(StructParam& s) { ASSERT_TRUE(s.val == TEST_INT); }
		static void StaticFuncStructConstRef1(const StructParam& s) { ASSERT_TRUE(s.val == TEST_INT); }
	};

	class TestClass2
	{
	public:
		void MemberFuncInt2(int i, int i2) { ASSERT_TRUE(i == TEST_INT); }
		void MemberFuncInt2Const(int i, int i2) const { ASSERT_TRUE(i == TEST_INT); }
		int MemberFuncIntWithReturn2(int i, int i2) { ASSERT_TRUE(i == TEST_INT); return i; }
		void MemberFuncStruct2(StructParam s, int i) { ASSERT_TRUE(s.val == TEST_INT); }
		void MemberFuncStructPtr2(StructParam* s, int i) { ASSERT_TRUE(s->val == TEST_INT); }
		void MemberFuncStructPtrPtr2(StructParam** s, int i) { ASSERT_TRUE((*s)->val == TEST_INT); }
		void MemberFuncStructConstPtr2(const StructParam* s, int i) { ASSERT_TRUE(s->val == TEST_INT); }
		void MemberFuncStructRef2(StructParam& s, int i) { ASSERT_TRUE(s.val == TEST_INT); }
		void MemberFuncStructConstRef2(const StructParam& s, int i) { ASSERT_TRUE(s.val == TEST_INT); }

		static void StaticFuncInt2(int i, int i2) { ASSERT_TRUE(i == TEST_INT); }
		static void StaticFuncStruct2(StructParam s, int i) { ASSERT_TRUE(s.val == TEST_INT); }
		static void StaticFuncStructPtr2(StructParam* s, int i) { ASSERT_TRUE(s->val == TEST_INT); }
		static void StaticFuncStructConstPtr2(const StructParam* s, int i) { ASSERT_TRUE(s->val == TEST_INT); }
		static void StaticFuncStructRef2(StructParam& s, int i) { ASSERT_TRUE(s.val == TEST_INT); }
		static void StaticFuncStructConstRef2(const StructParam& s, int i) { ASSERT_TRUE(s.val == TEST_INT); }
	};
}

