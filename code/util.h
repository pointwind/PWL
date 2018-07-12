#pragma once
#include"Iterator.h"
#include<cstddef>
namespace PWL
{
	template<class T>typename std::remove_reference<T>::type&& move(T&& arg)noexcept
	{
		return static_cast<typename std::remove_reference<T>::type&&>(arg);
	}
	template<class T>constexpr T&& forward(typename std::remove_reference<T>::type& arg)
	{
		return static_cast<T&&>(arg);
	}
	template<class T>constexpr T&& forward(typename std::remove_reference<T>::type&& arg)//rvalue can't forward to lvalue
	{
		static_assert(!std::is_lvalue_reference<T>::value, "bad forward");
		return static_cast<T&&>(arg);
	}
	template<class T>void swap(T& lhs,T& rhs) noexcept
	{
		auto temp = move(lhs);
		lhs = move(rhs);
		rhs = move(temp);
	}
	template<class ForwardIter1,class ForwardIter2>ForwardIter2 swap_range(ForwardIter1 first1,ForwardIter1 last1,ForwardIter2 first2)
	{
		for(;first1!=last1;++first1,(void)++first2)
		{
			swap(*first1, *first2);
		}
		return first2;
	}
	template<class T,size_t N>void swap(T (&a)[N],T (&b)[N]) noexcept//T(&a)[N] declare an array
	{
		swap_range(a, a + N, b);
	}


	template<class T1,class T2> struct pair
	{
		using first_type = T1;
		using second_type = T2;

		first_type first;
		second_type second;

		template<class Other1 = T1,class Other2 = T2,
			typename = typename std::enable_if<
			std::is_default_constructible<Other1>::value &&
			std::is_default_constructible<Other2>::value>::value>
			constexpr pair():first(),second(){}

		//implicit
		template<class U1 = T1, class U2 = T2,
			typename std::enable_if<std::is_copy_constructible<U1>::value &&
			std::is_copy_constructible<U2>::value &&
			std::is_convertible<U1&, T1>::value &&
			std::is_convertible<U2&, T2>::value>::type* = nullptr>
			 constexpr pair(const T1& a, const T2& b) : first(a), second(b) {}

		//explicit
		template<class U1 = T1,class U2 = T2,
			typename std::enable_if<std::is_copy_constructible<U1>::value &&
			std::is_copy_constructible<U2>::value &&
			(!std::is_convertible<U1&,T1>::value || !std::is_convertible<U2&,T2>::value)>::type* = nullptr>
			explicit constexpr pair(const T1& a,const T2& b)
			:first(a),second(b){}

		pair(const pair& rhs) = default;
		pair(pair&& rhs) = default;

		template<class Other1,class Other2,
			typename std::enable_if<std::is_constructible<T1,Other1>::value 
			&& std::is_constructible<T2,Other2>::value &&
			std::is_convertible<Other1&&,T1>::value &&
			std::is_convertible<Other2&&,T2>::value,int>::type* = nullptr>
			constexpr pair(Other1&& a,Other2&& b)
			:first(PWL::forward<Other1>(a)),second(PWL::forward<Other2>(b)){}

		template<class Other1, class Other2,
			typename std::enable_if<std::is_constructible<T1, Other1>::value
			&& std::is_constructible<T2, Other2>::value &&
			(!std::is_convertible<Other1, T1>::value ||
			!std::is_convertible<Other2, T2>::value), int>::type* = nullptr>
			explicit constexpr pair(Other1&& a, Other2&& b)
			:first(PWL::forward<Other1>(a)), second(PWL::forward<Other2>(b)) {}

		template<class Other1,class Other2,
			typename std::enable_if<std::is_constructible<T1,const Other1&>::value &&
			std::is_convertible<T2,const Other2&>::value &&
			std::is_convertible<Other1&,T1>::value &&
			std::is_convertible<Other1&,T2>::value>::type* = nullptr>
			constexpr pair(const pair<Other1,Other2>& other)
			:first(other.first),second(other.second){}

		template<class Other1, class Other2,
			typename std::enable_if<std::is_constructible<T1, const Other1&>::value &&
			std::is_convertible<T2, const Other2&>::value &&
			(!std::is_convertible<Other1&, T1>::value ||
			!std::is_convertible<Other1&, T2>::value)>::type* = nullptr>
			explicit constexpr pair(const pair<Other1, Other2>& other) 
			:first(other.first), second(other.second) {}

		template<class Other1, class Other2,
			typename std::enable_if<std::is_constructible<T1, const Other1&>::value &&
			std::is_convertible<T2, const Other2&>::value &&
			std::is_convertible<Other1, T1>::value &&
			std::is_convertible<Other1, T2>::value>::type* = nullptr>
			constexpr pair(const pair<Other1, Other2>& other) 
			:first(PWL::forward<Other1>(other.first)), second(PWL::forward<Other2>(other.second)) {}


		pair& operator=(const pair&rhs)
		{
			if(this != &rhs)
			{
				this->first = rhs.first;
				this->second = rhs.second;
			}
			return *this;
		}
		pair& operator=(pair&&rhs) noexcept
		{
			if (this != &rhs)
			{
				this->first = PWL::move(rhs.first);
				this->second = PWL::move(rhs.second);
			}
			return *this;
		}

		template<class Other1,class Other2>pair& operator=(const pair<Other1, Other2>& other)
		{
			first = other.first;
			second = other.second;
			return *this;
		}
		template<class Other1, class Other2>pair& operator=(pair<Other1, Other2>&& other)
		{
			this->first = PWL::move(other.first);
			this->second = PWL::move(other.second);
			return *this;
		}
		~pair() = default;
		void swap(pair& other) noexcept
		{
			if(this != other)
			{
				this->first = PWL::swap(other.first);
				this->second = PWL::swap(other.second);
			}
		}
	};

	template<class T1,class T2>bool operator==(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return lhs.first == rhs.first && lhs.second == rhs.second;
	}
	template <class T1, class T2>bool operator<(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return lhs.first < rhs.first || (lhs.first == rhs.first && lhs.second < rhs.second);
	}
	template <class T1, class T2>bool operator>(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return rhs<lhs;
	}
	template <class T1, class T2>bool operator<=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return !(lhs>rhs);
	}
	template <class T1, class T2>bool operator>=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return !(rhs<lhs);
	}

	template<class T1, class T2>bool operator!=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return !(lhs== rhs);
	}
	template <class T1, class T2>
	void swap(pair<T1, T2>& lhs, pair<T1, T2>& rhs) noexcept
	{
		lhs.swap(rhs);
	}
	template<class T1,class T2>pair<T1,T2> make_pair(T1&& first, T2&& second)
	{
		return pair<T1, T2>(PWL::forward<T1>(first), PWL::forward<T2>(second));
	}

}