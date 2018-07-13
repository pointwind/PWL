#pragma once
#include"util.h"
#include"Iterator.h"
namespace PWL
{
	template<class T>constexpr T& max(const T& lhs,const T& rhs)
	{
		return lhs < rhs ? rhs : lhs;
	}
	template<class T>constexpr T& min(const T& lhs,const T& rhs)
	{
		return lhs < rhs ? lhs : rhs;
	}
	template<class T,class compare>constexpr T& max(const T& lhs, const T& rhs,compare com)
	{
		return com(lhs,rhs) ? rhs : lhs;
	}
	template<class T,class compare>constexpr T& min(const T& lhs, const T& rhs,compare com)
	{
		return com(lhs,rhs) ? lhs : rhs;
	}
	template<class Iter1,class Iter2>void iter_swap(Iter1& lhs,Iter2& rhs)
	{
		PWL::swap(*lhs, *rhs);
	}
	//copy [first,last] to [result ,result+ (last-first)]
	template<class InputIter,class OutputIter>OutputIter unchecked_copy_cat(InputIter first,InputIter last,OutputIter result,PWL::input_iterator_tag)
	{
		for(;first != last;++first,++result)
		{
			*result = *first;
		}
		return result;
	}
	template<class InputIter, class OutputIter>OutputIter unchecked_copy_cat(InputIter first, InputIter last, OutputIter result, PWL::random_access_iterator_tag)
	{
		for (auto n = last - first; n > 0; --n, ++first, ++result)
		{
			*result = *first;
		}
		return result;
	}
	template<class InputIter, class OutputIter>OutputIter unchecked_copy(InputIter first, InputIter last, OutputIter result)
	{
		return unchecked_copy_cat(first, last, result, iterator_category(first));
	}

	//partical for the trivally_copy_assignable
	template<class T,class U>typename std::enable_if<
		std::is_same<std::remove_reference<T>,U>::value &&
		std::is_trivially_copy_assignable<U>::value,U*>::type 
	unchecked_copy(T* first,T* last,U* result)
	{
		const auto n = static_cast<size_t>(last - first);
		if (n != 0)
			std::memmove(result, first, n * sizeof(U));
		return result + n;
	}

	template<class InputIter,class OutputIter>OutputIter copy(InputIter first, InputIter last, OutputIter result)
	{
		return unchecked_copy(first, last, result);
	}

	/////////////////////////////////////////!!!!!!!!!!!!!!!!!!!!a little diffrernce
	//copy [first,last] to [result ,result* (last-first)]
	template<class InputIter, class OutputIter>OutputIter unchecked_copy_backward_cat(InputIter first, InputIter last, OutputIter result, PWL::input_iterator_tag)
	{
		for (; first != last; --last, --result)
		{
			*result = *last;
		}
		return result;
	}
	template<class InputIter, class OutputIter>OutputIter unchecked_copy_backward_cat(InputIter first, InputIter last, OutputIter result, PWL::random_access_iterator_tag)
	{
		for (auto n = last - first; n > 0; --n,--last,--result)
		{
			*result = *last;
		}
		return result;
	}
	template<class InputIter, class OutputIter>OutputIter unchecked_copy_backward(InputIter first, InputIter last, OutputIter result)
	{
		return unchecked_copy_cat(first, last, result, iterator_category(first));
	}

	//partical for the trivally_copy_assignable
	template<class T, class U>typename std::enable_if<
		std::is_same<std::remove_reference<T>, U>::value &&
		std::is_trivially_copy_assignable<U>::value, U*>::type
		unchecked_copy_backward(T* first, T* last, U* result)
	{
		const auto n = static_cast<size_t>(last - first);
		if (n != 0)
			std::memmove(result-n, first, n * sizeof(U));
		return result;
	}

	template<class InputIter, class OutputIter>OutputIter copy_ward(InputIter first, InputIter last, OutputIter result)
	{
		return unchecked_copy_backward(first, last, result);
	}

	//copy first,first + n to result,result + n 
	template<class InputIter,class OutputIter>OutputIter unchecked_copy_n_cat(InputIter first,OutputIter result,size_t n)
	{
		for(;n>0;--n,++first,++result)
		{
			*result = *first;
		}
		return result;
	}
}