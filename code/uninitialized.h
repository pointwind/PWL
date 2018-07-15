#pragma once

// ���ͷ�ļ����ڶ�δ��ʼ���ռ乹��Ԫ��

#include"Iterator.h"
#include"algorithm_base.h"
#include"util.h"
#include"construct.h"
namespace PWL
{
	// uninitialized_copy
	// �� [first, last) �ϵ����ݸ��Ƶ��� result Ϊ��ʼ���Ŀռ䣬���ظ��ƽ�����λ��
	template<class InputIter,class OutputIter>OutputIter unchecked_uninit_copy(InputIter first, InputIter last, OutputIter result,std::true_type)
	{
		return PWL::copy(first, last, result);
	}
	template<class InputIter, class OutputIter>OutputIter unchecked_uninit_copy(InputIter first, InputIter last, OutputIter result, std::false_type)
	{
		auto cur = result;
		try
		{
			for (; first != last; ++first, ++cur)
				PWL::construct(&*cur, *first);
		}
		catch(...)
		{
			for (; result != cur; ++result)
				PWL::destroy(&*result);
		}
		return cur;
	}

	template<class InputIter, class OutputIter>OutputIter uninit_copy(InputIter first, InputIter last, OutputIter result)
	{
		return unchecked_copy(first, last, result,
			std::is_trivially_copy_assignable<typename iterator_traits<OutputIter>::value_type>{});
	}

	// uninitialized_copy_n
	// �� [first, first + n) �ϵ����ݸ��Ƶ��� result Ϊ��ʼ���Ŀռ䣬���ظ��ƽ�����λ��
	template<class InputIter, class OutputIter,class Size>OutputIter unchecked_uninit_copy_n(InputIter first, OutputIter result,Size n, std::true_type)
	{
		return PWL::copy_n(first, result,n).second;
	}
	template<class InputIter, class OutputIter, class Size>OutputIter unchecked_uninit_copy_n(InputIter first,OutputIter result,Size n, std::false_type)
	{
		auto cur = result;
		try
		{
			for (; n > 0; --n, ++first,++cur)
				PWL::construct(&*cur, *first);
		}
		catch (...)
		{
			for (; result != cur; ++result)
				PWL::destroy(&*result);
		}
		return cur;
	}

	template<class InputIter, class OutputIter,class Size>OutputIter uninit_copy_n(InputIter first,OutputIter result,Size n)
	{
		return unchecked_copy_n(first, result,n,
			std::is_trivially_copy_assignable<typename iterator_traits<InputIter>::value_type>{});
	}


	// �� first λ�ÿ�ʼ��� n ��ֵ first
	template <class OutputIter, class Size, class T> OutputIter unchecked_uninit_fill_n(OutputIter first, Size n, const T& value,std::true_type)
	{
		return PWL::fill_n(first, n, value);
	}

	template <class OutputIter, class Size, class T> OutputIter unchecked_uninit_fill_n(OutputIter first, Size n, const T& value, std::false_type)
	{
		auto cur = first;
		try
		{
			for (; n > 0; --n, ++cur)
			{
				PWL::construct(&*cur, value);
			}
		}
		catch (...)
		{
			for (; first != cur; ++first)
				PWL::destroy(&*first);
		}
		return cur;
	}
	template <class OutputIter, class Size, class T>
	OutputIter uninit_fill_n(OutputIter first, Size n, const T& value)
	{
		return unchecked_uninit_fill_n(first, n, value,
			std::is_trivially_copy_assignable<
			typename iterator_traits<OutputIter>::
			value_type>{});
	}

	//fill in (first,last)
	template <class ForwardIter, class T>void uncheched_uninit_fill_cat(ForwardIter first, ForwardIter last, const T& value, std::true_type)
	{
		fill(first, last, value);
	}

	template <class ForwardIter, class T>void unchecked_uninit_fill_cat(ForwardIter first, ForwardIter last, const T& value, std::false_type)
	{
		auto cur = first;
		try
		{
			for (; cur != last; ++cur)
			{
				PWL::construct(&*cur, value);
			}
		}
		catch (...)
		{
			for (; first != cur; ++first)
				PWL::destroy(&*first);
		}
	}

	template <class ForwardIter, class T> void uninit_fill(ForwardIter first, ForwardIter last, const T& value)
	{
		unchecked_uninit_fill_cat(first, last, value,
			std::is_trivially_copy_assignable<
			typename iterator_traits<ForwardIter>::
			value_type>{});
	}



	//move version
	//move [first,last] to [result ,result+ (last-first)]
	template<class InputIter, class OutputIter>OutputIter unchecked_uninit_move_cat(InputIter first, InputIter last, OutputIter result, std::true_type)
	{
		return move(first, last, result);
	}
	template<class InputIter, class OutputIter>OutputIter unchecked_uninit_move_cat(InputIter first, InputIter last, OutputIter result, std::false_type)
	{
		OutputIter cur = result;
		try
		{
			for (; first != last; ++first, ++cur)
			{
				PWL::construct(&*cur, PWL::move(*first));
			}
		}
		catch (...)
		{
			PWL::destroy(result, cur);
		}
		return cur;
	}

	template<class InputIter, class OutputIter>OutputIter uninit_move(InputIter first, InputIter last, OutputIter result)
	{
		return unchecked_uninit_move(first, last, result,
			std::is_trivially_copy_assignable<
			typename iterator_traits<InputIter>::
			value_type>{});
	}

	// uninitialized_move_n
	// ��[first, first + n)�ϵ������ƶ����� result Ϊ��ʼ���Ŀռ䣬�����ƶ�������λ��
	template<class InputIter, class OutputIter, class Size>OutputIter unchecked_uninit_move_n(InputIter first, OutputIter result, Size n, std::true_type)
	{
		return PWL::move(first, first+n,result).second;
	}
	template<class InputIter, class OutputIter, class Size>OutputIter unchecked_uninit_move_n(InputIter first, OutputIter result, Size n, std::false_type)
	{
		auto cur = result;
		try
		{
			for (; n > 0; --n, ++first, ++cur)
				PWL::construct(&*cur, PWL::move(*first));
		}
		catch (...)
		{
			for (; result != cur; ++result)
				PWL::destroy(&*result);
			throw;
		}
		return cur;
	}

	template<class InputIter, class OutputIter, class Size>OutputIter uninit_move_n(InputIter first, OutputIter result, Size n)
	{
		return unchecked_copy_n(first, result, n,
			std::is_trivially_copy_assignable<typename iterator_traits<InputIter>::value_type>{});
	}
}