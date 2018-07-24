#pragma once
#include<cstddef>
namespace PWL
{
	template<class Arg, class Result>
	struct Unarg_Function
	{
		using arguemnt_type = Arg;
		using result_type = Result;
	};

	template<class Arg1,class Arg2,class Result>
	struct Binary_Function
	{
		using first_argument_type = Arg1;
		using second_argument_type = Arg2;
		using result_type = Result;
	};


	template<class T>
	struct Plus : Binary_Function<T,T,T>
	{
		auto operator()(const T& lhs,const T& rhs)
		{
			return lhs + rhs;
		}
	};

	template <class T>
	struct Minus :public Binary_Function<T, T, T>
	{
		T operator()(const T& x, const T& y) const { return x - y; }
	};

	// �������󣺳˷�
	template <class T>
	struct Multiplies :public Binary_Function<T, T, T>
	{
		T operator()(const T& x, const T& y) const { return x * y; }
	};

	// �������󣺳���
	template <class T>
	struct Divides :public Binary_Function<T, T, T>
	{
		T operator()(const T& x, const T& y) const { return x / y; }
	};

	// ��������ģȡ
	template <class T>
	struct Modulus :public Binary_Function<T, T, T>
	{
		T operator()(const T& x, const T& y) const { return x % y; }
	};

	// �������󣺷�
	template <class T>
	struct Negate :public Unarg_Function<T, T>
	{
		T operator()(const T& x) const { return -x; }
	};

	// �ӷ���֤ͬԪ��
	template<class T>
	auto Identity_Element(Plus<T>) { return T(0); }

	// �˷���֤ͬԪ��
	template <class T>
	auto Identity_Element(Multiplies<T>) { return T(1); }

	// �������󣺵���
	template <class T>
	struct Equal_To :public Binary_Function<T, T, bool>
	{
		bool operator()(const T& x, const T& y) const { return x == y; }
	};

	// �������󣺲�����
	template <class T>
	struct Not_Equal_To :public Binary_Function<T, T, bool>
	{
		bool operator()(const T& x, const T& y) const { return x != y; }
	};

	// �������󣺴���
	template <class T>
	struct Greater :public Binary_Function<T, T, bool>
	{
		bool operator()(const T& x, const T& y) const { return x > y; }
	};

	// ��������С��
	template <class T>
	struct Less :public Binary_Function<T, T, bool>
	{
		bool operator()(const T& x, const T& y) const { return x < y; }
	};

	// �������󣺴��ڵ���
	template <class T>
	struct Greater_Equal :public Binary_Function<T, T, bool>
	{
		bool operator()(const T& x, const T& y) const { return x >= y; }
	};

	// ��������С�ڵ���
	template <class T>
	struct Less_Equal :public Binary_Function<T, T, bool>
	{
		bool operator()(const T& x, const T& y) const { return x <= y; }
	};

	// ���������߼���
	template <class T>
	struct Logical_And :public Binary_Function<T, T, bool>
	{
		bool operator()(const T& x, const T& y) const { return x && y; }
	};

	// ���������߼���
	template <class T>
	struct Logical_Or :public Binary_Function<T, T, bool>
	{
		bool operator()(const T& x, const T& y) const { return x || y; }
	};

	// ���������߼���
	template <class T>
	struct Logical_Not :public Unarg_Function<T, bool>
	{
		bool operator()(const T& x) const { return !x; }
	};

	// ֤ͬ����������ı�Ԫ�أ����ر���
	template <class T>
	struct Identity :public Unarg_Function<T, bool>
	{
		const T& operator()(const T& x) const { return x; }
	};

	// ѡ����������һ�� pair�����ص�һ��Ԫ��
	template <class Pair>
	struct Selectfirst :public Unarg_Function<Pair, typename Pair::first_type>
	{
		const typename Pair::first_type& operator()(const Pair& x) const
		{
			return x.first;
		}
	};

	// ѡ����������һ�� pair�����صڶ���Ԫ��
	template <class Pair>
	struct Selectsecond :public Unarg_Function<Pair, typename Pair::second_type>
	{
		const typename Pair::second_type& operator()(const Pair& x) const
		{
			return x.second;
		}
	};
}