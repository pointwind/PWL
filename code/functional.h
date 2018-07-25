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

	// ���ڴ󲿷����ͣ�hash function ʲô������
	template <class Key>
	struct Hash {};

	template<class T>
	struct Hash<T*>
	{
		size_t operator()(T* p) const noexcept
		{
			return reinterpret_cast<size_t>(p);
		}
	};


	// �����������ͣ�ֻ�Ƿ���ԭֵ
#define PWL_TRIVIAL_HASH_FCN(Type)         \
template <> struct Hash<Type>                \
{                                            \
  size_t operator()(Type val) const noexcept \
  { return static_cast<size_t>(val); }       \
};											



	PWL_TRIVIAL_HASH_FCN(bool)

	PWL_TRIVIAL_HASH_FCN(char)

	PWL_TRIVIAL_HASH_FCN(signed char)

	PWL_TRIVIAL_HASH_FCN(unsigned char)

	PWL_TRIVIAL_HASH_FCN(wchar_t)

	PWL_TRIVIAL_HASH_FCN(char16_t)

	PWL_TRIVIAL_HASH_FCN(char32_t)

	PWL_TRIVIAL_HASH_FCN(short)

	PWL_TRIVIAL_HASH_FCN(unsigned short)

	PWL_TRIVIAL_HASH_FCN(int)

	PWL_TRIVIAL_HASH_FCN(unsigned int)

	PWL_TRIVIAL_HASH_FCN(long)

	PWL_TRIVIAL_HASH_FCN(unsigned long)

	PWL_TRIVIAL_HASH_FCN(long long)

	PWL_TRIVIAL_HASH_FCN(unsigned long long)

#undef MYSTL_TRIVIAL_HASH_FCN


		// ���ڸ���������λ��ϣ
		inline size_t Bitwise_Hash(const unsigned char* first, size_t count)
	{
#if (_MSC_VER && _WIN64) || ((__GNUC__ || __clang__) &&__SIZEOF_POINTER__ == 8)
		const size_t fnv_offset = 14695981039346656037ull;
		const size_t fnv_prime = 1099511628211ull;
#else
		const size_t fnv_offset = 2166136261u;
		const size_t fnv_prime = 16777619u;
#endif
		size_t result = fnv_offset;
		for (size_t i = 0; i < count; ++i)
		{
			result ^= (size_t)first[i];
			result *= fnv_prime;
		}
		return result;
	}

	template <>
	struct Hash<float>
	{
		size_t operator()(const float& val)
		{
			return val == 0.0f ? 0 : Bitwise_Hash((const unsigned char*)&val, sizeof(float));
		}
	};

	template <>
	struct Hash<double>
	{
		size_t operator()(const double& val)
		{
			return val == 0.0f ? 0 : Bitwise_Hash((const unsigned char*)&val, sizeof(double));
		}
	};

	template <>
	struct Hash<long double>
	{
		size_t operator()(const long double& val)
		{
			return val == 0.0f ? 0 : Bitwise_Hash((const unsigned char*)&val, sizeof(long double));
		}
	};
}