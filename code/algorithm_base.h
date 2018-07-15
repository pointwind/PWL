#pragma once
#include"util.h"
#include"Iterator.h"
namespace PWL
{
#ifdef max
#pragma message("#undefing marco max")
#undef max
#endif // max

#ifdef min
#pragma message("#undefing marco min")
#undef min
#endif // min
	template<class T>constexpr const T& max(const T& lhs,const T& rhs)
	{
		return lhs < rhs ? rhs : lhs;
	}
	template<class T>constexpr const T& min(const T& lhs,const T& rhs)
	{
		return lhs < rhs ? lhs : rhs;
	}
	template<class T,class compare>constexpr const T& max(const T& lhs, const T& rhs,compare com)
	{
		return com(lhs,rhs) ? rhs : lhs;
	}
	template<class T,class compare>constexpr const T& min(const T& lhs, const T& rhs,compare com)
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
	//copy [first,last] to [result ,result- (last-first)]
	template<class InputIter, class OutputIter>OutputIter unchecked_copy_backward_cat(InputIter first, InputIter last, OutputIter result, PWL::bidirectional_iterator_tag)
	{
		while (first != last)
			*--result = *--last;
		return result;
	}
	template<class InputIter, class OutputIter>OutputIter unchecked_copy_backward_cat(InputIter first, InputIter last, OutputIter result, PWL::random_access_iterator_tag)
	{
		for (auto n = last - first; n > 0; --n)
			*--result = *--last;
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

	template<class InputIter, class OutputIter>OutputIter copy_backward(InputIter first, InputIter last, OutputIter result)
	{
		return unchecked_copy_backward(first, last, result);
	}




	// copy_if
	// 把[first, last)内满足一元操作 unary_pred 的元素拷贝到以 result 为起始的位置上
	template <class InputIter, class OutputIter, class UnaryPredicate>OutputIter copy_if(InputIter first, InputIter last, OutputIter result, UnaryPredicate unary_pred)
	{
		for (; first != last; ++first)
		{
			if (unary_pred(*first))
				*result++ = *first;
		}
		return result;
	}



	//copy (first,first + n )to (result,result + n )and    return a pair which point to container 's tail
	template<class InputIter,class OutputIter,class Size>PWL::pair<InputIter,OutputIter> unchecked_copy_n_cat(InputIter first,OutputIter result,Size n,PWL::input_iterator_tag)
	{
		for(;n>0;--n,++first,++result)
		{
			*result = *first;
		}
		return PWL::pair<InputIter, OutputIter>(first,result);
	}
	template<class RandomIter, class OutputIter, class Size>PWL::pair<RandomIter, OutputIter> unchecked_copy_n_cat(RandomIter first, OutputIter result, Size n, PWL::random_access_iterator_tag)
	{
		auto last = first + n;
		return PWL::pair<RandomIter, OutputIter>(first,PWL::copy(first,last,result));
	}

	template<class RandomIter, class OutputIter, class Size>PWL::pair<RandomIter, OutputIter> copy_n(RandomIter first, OutputIter result, Size n)
	{
		return PWL::unchecked_copy_n_cat(first,result,n,PWL::iterator_category(first));
	}

	//move version
	//move [first,last] to [result ,result+ (last-first)]
	template<class InputIter, class OutputIter>OutputIter unchecked_move_cat(InputIter first, InputIter last, OutputIter result, PWL::input_iterator_tag)
	{
		for (; first != last; ++first, ++result)
		{
			*result =PWL::move( *first);
		}
		return result;
	}
	template<class InputIter, class OutputIter>OutputIter unchecked_move_cat(InputIter first, InputIter last, OutputIter result, PWL::random_access_iterator_tag)
	{
		for (auto n = last - first; n > 0; --n, ++first, ++result)
		{
			*result = PWL::move(*first);
		}
		return result;
	}
	template<class InputIter, class OutputIter>OutputIter unchecked_move(InputIter first, InputIter last, OutputIter result)
	{
		return unchecked_move_cat(first, last, result, iterator_category(first));
	}

	//partical for the trivally_move_assignable
	template<class T, class U>typename std::enable_if<
		std::is_same<std::remove_reference<T>, U>::value &&
		std::is_trivially_copy_assignable<U>::value, U*>::type
		unchecked_move(T* first, T* last, U* result)
	{
		const auto n = static_cast<size_t>(last - first);
		if (n != 0)
			std::memmove(result, first, n * sizeof(U));
		return result + n;
	}

	template<class InputIter, class OutputIter>OutputIter move(InputIter first, InputIter last, OutputIter result)
	{
		return unchecked_move(first, last, result);
	}



	//move version
	//copy [first,last] to [result ,result- (last-first)]
	template<class InputIter, class OutputIter>OutputIter unchecked_move_backward_cat(InputIter first, InputIter last, OutputIter result, PWL::bidirectional_iterator_tag)
	{
		while (first != last)
			*--result = *--last;
		return result;
	}
	template<class InputIter, class OutputIter>OutputIter unchecked_move_backward_cat(InputIter first, InputIter last, OutputIter result, PWL::random_access_iterator_tag)
	{
		for (auto n = last - first; n > 0; --n)
			*--result = *--last;
		return result;
	}
	template<class InputIter, class OutputIter>OutputIter unchecked_move_backward(InputIter first, InputIter last, OutputIter result)
	{
		return unchecked_move_cat(first, last, result, iterator_category(first));
	}

	//partical for the trivally_copy_assignable
	template<class T, class U>typename std::enable_if<
		std::is_same<std::remove_reference<T>, U>::value &&
		std::is_trivially_copy_assignable<U>::value, U*>::type
		unchecked_move_backward(T* first, T* last, U* result)
	{
		const auto n = static_cast<size_t>(last - first);
		if (n != 0)
			std::memmove(result - n, first, n * sizeof(U));
		return result;
	}

	template<class InputIter, class OutputIter>OutputIter move_backward(InputIter first, InputIter last, OutputIter result)
	{
		return unchecked_move_backward(first, last, result);
	}

	// equal
	// 比较第一序列在 [first, last)区间上的元素值是否和第二序列相等
	template <class InputIter1, class InputIter2> bool equal(InputIter1 first1, InputIter1 last1, InputIter2 first2)
	{
		for (; first1 != last1; ++first1, ++first2)
		{
			if (*first1 != *first2)
				return false;
		}
		return true;
	}

	template <class InputIter1, class InputIter2, class Compared> bool equal(InputIter1 first1, InputIter1 last1, InputIter2 first2, Compared com)
	{
		for (; first1 != last1; ++first1, ++first2)
		{
			if (!com(*first1, *first2))
				return false;
		}
		return true;
	}

	// 从 first 位置开始填充 n 个值 first
	template <class OutputIter, class Size, class T> OutputIter unchecked_fill_n(OutputIter first, Size n, const T& value)
	{
		for (; n > 0; --n, ++first)
		{
			*first = value;
		}
		return first;
	}

	//partical for one-byte
	template<class T,class Size,class U>typename std::enable_if<
		std::_Is_integral<T>::value && sizeof(T) == 1 &&
		!std::is_same<T,bool>::value && 
		std::is_integral<U>::value && sizeof(U) == 1,T*>::type unchecked_fill_n(T* first,Size n,U value)
	{
		if(n > 0)
		{
			std::memset(first, static_cast<unsigned char>(value),static_cast<size_t>(n));
		}
		return first + n;
	}

	template <class OutputIter, class Size, class T>
	OutputIter fill_n(OutputIter first, Size n, const T& value)
	{
		return unchecked_fill_n(first,n,value);
	}

	//fill in (first,last)
	template <class ForwardIter, class T>void fill_cat(ForwardIter first, ForwardIter last, const T& value,PWL::forward_iterator_tag)
	{
		for (; first != last; ++first)
		{
			*first = value;
		}
	}

	template <class ForwardIter, class T>void fill_cat(ForwardIter first, ForwardIter last, const T& value, PWL::random_access_iterator_tag)
	{
		fill_n(first, last - first, value);
	}

	template <class ForwardIter, class T> void fill(ForwardIter first, ForwardIter last, const T& value)
	{
		fill_cat(first, last, value, iterator_category(first));
	}



	// lexicographical_compare
	// 以字典序排列对两个序列进行比较，当在某个位置发现第一组不相等元素时，有下列几种情况：
	// (1)如果第一序列的元素较小，返回 true ，否则返回 false
	// (2)如果到达 last1 而尚未到达 last2 返回 true
	// (3)如果到达 last2 而尚未到达 last1 返回 false
	// (4)如果同时到达 last1 和 last2 返回 false
	template <class InputIter1, class InputIter2>bool lexicographical_compare(InputIter1 first1, InputIter1 last1,
		InputIter2 first2, InputIter2 last2)
	{
		for (; first1 != last1 && first2 != last2; ++first1, ++first2)
		{
			if (*first1 < *first2)
				return true;
			if (*first2 < *first1)
				return false;
		}
		return first1 == last1 && first2 != last2;
	}

	// 重载版本使用函数对象 comp 代替比较操作
	template <class InputIter1, class InputIter2, class Compred>bool lexicographical_compare(InputIter1 first1, InputIter1 last1,
		InputIter2 first2, InputIter2 last2, Compred comp)
	{
		for (; first1 != last1 && first2 != last2; ++first1, ++first2)
		{
			if (comp(*first1, *first2))
				return true;
			if (comp(*first2, *first1))
				return false;
		}
		return first1 == last1 && first2 != last2;
	}

	// 针对 const unsigned char* 的特化版本
	inline bool lexicographical_compare(const unsigned char* first1,
		const unsigned char* last1,
		const unsigned char* first2,
		const unsigned char* last2)
	{
		const auto len1 = last1 - first1;
		const auto len2 = last2 - first2;
		// 先比较相同长度的部分
		const auto result = std::memcmp(first1, first2, PWL::min(len1, len2));
		// 若相等，长度较长的比较大
		return result != 0 ? result < 0 : len1 < len2;
	}

	// mismatch
	// 平行比较两个序列，找到第一处失配的元素，返回一对迭代器，分别指向两个序列中失配的元素
	template <class InputIter1, class InputIter2>
	PWL::pair<InputIter1, InputIter2>
		mismatch(InputIter1 first1, InputIter1 last1, InputIter2 first2)
	{
		while (first1 != last1 && *first1 == *first2)
		{
			++first1;
			++first2;
		}
		return PWL::pair<InputIter1, InputIter2>(first1, first2);
	}

	template <class InputIter1, class InputIter2, class Compre>
	PWL::pair<InputIter1, InputIter2>
		mismatch(InputIter1 first1, InputIter1 last1, InputIter2 first2, Compre com)
	{
		while (first1 != last1 && comp(*first1, *first2))
		{
			++first1;
			++first2;
		}
		return PWL::pair<InputIter1, InputIter2>(first1, first2);
	}
}
