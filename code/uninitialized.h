#pragma once

// ���ͷ�ļ����ڶ�δ��ʼ���ռ乹��Ԫ��

#include"Iterator.h"
#include"algorithm_base.h"
#include"util.h"
#include"Construct.h"
namespace PWL
{
	// uninitialized_copy
	// �� [first, last) �ϵ����ݸ��Ƶ��� result Ϊ��ʼ���Ŀռ䣬���ظ��ƽ�����λ��
	template<class InputIter,class OutputIter>OutputIter Unchecked_Uninit_Copy(InputIter first, InputIter last, OutputIter result,std::true_type)
	{
		return PWL::Copy(first, last, result);
	}
	template<class InputIter, class OutputIter>OutputIter Unchecked_Uninit_Copy(InputIter first, InputIter last, OutputIter result, std::false_type)
	{
		auto cur = result;
		try
		{
			for (; first != last; ++first, ++cur)
				PWL::Construct(&*cur, *first);
		}
		catch(...)
		{
			for (; result != cur; ++result)
				PWL::Destroy(&*result);
		}
		return cur;
	}

	template<class InputIter, class OutputIter>OutputIter Uninit_Copy(InputIter first, InputIter last, OutputIter result)
	{
		return Unchecked_Copy(first, last, result,
			std::is_trivially_copy_assignable<typename iterator_traits<OutputIter>::value_type>{});
	}

	// uninitialized_copy_n
	// �� [first, first + n) �ϵ����ݸ��Ƶ��� result Ϊ��ʼ���Ŀռ䣬���ظ��ƽ�����λ��
	template<class InputIter, class OutputIter,class Size>OutputIter Unchecked_Uninit_Copy_N(InputIter first, OutputIter result,Size n, std::true_type)
	{
		return PWL::Copy_N(first, result,n).second;
	}
	template<class InputIter, class OutputIter, class Size>OutputIter Unchecked_Uninit_Copy_N(InputIter first,OutputIter result,Size n, std::false_type)
	{
		auto cur = result;
		try
		{
			for (; n > 0; --n, ++first,++cur)
				PWL::Construct(&*cur, *first);
		}
		catch (...)
		{
			for (; result != cur; ++result)
				PWL::Destroy(&*result);
		}
		return cur;
	}

	template<class InputIter, class OutputIter,class Size>OutputIter Uninit_Copy_N(InputIter first,OutputIter result,Size n)
	{
		return unchecked_copy_n(first, result,n,
			std::is_trivially_copy_assignable<typename iterator_traits<InputIter>::value_type>{});
	}


	// �� first λ�ÿ�ʼ��� n ��ֵ first
	template <class OutputIter, class Size, class T> OutputIter Unchecked_Uninit_Fill_N(OutputIter first, Size n, const T& value,std::true_type)
	{
		return PWL::Fill_N(first, n, value);
	}

	template <class OutputIter, class Size, class T> OutputIter Unchecked_Uninit_Fill_N(OutputIter first, Size n, const T& value, std::false_type)
	{
		auto cur = first;
		try
		{
			for (; n > 0; --n, ++cur)
			{
				PWL::Construct(&*cur, value);
			}
		}
		catch (...)
		{
			for (; first != cur; ++first)
				PWL::Destroy(&*first);
		}
		return cur;
	}
	template <class OutputIter, class Size, class T>
	OutputIter Uninit_Fill_N(OutputIter first, Size n, const T& value)
	{
		return Unchecked_Uninit_Fill_N(first, n, value,
			std::is_trivially_copy_assignable<
			typename iterator_traits<OutputIter>::
			value_type>{});
	}

	//fill in (first,last)
	template <class ForwardIter, class T>void Uncheched_Uninit_Fill_Cat(ForwardIter first, ForwardIter last, const T& value, std::true_type)
	{
		Fill(first, last, value);
	}

	template <class ForwardIter, class T>void Unchecked_Uninit_Fill_Cat(ForwardIter first, ForwardIter last, const T& value, std::false_type)
	{
		auto cur = first;
		try
		{
			for (; cur != last; ++cur)
			{
				PWL::Construct(&*cur, value);
			}
		}
		catch (...)
		{
			for (; first != cur; ++first)
				PWL::Destroy(&*first);
		}
	}

	template <class ForwardIter, class T> void Uninit_Fill(ForwardIter first, ForwardIter last, const T& value)
	{
		Unchecked_Uninit_Fill_Cat(first, last, value,
			std::is_trivially_copy_assignable<
			typename iterator_traits<ForwardIter>::
			value_type>{});
	}



	//move version
	//move [first,last] to [result ,result+ (last-first)]
	template<class InputIter, class OutputIter>OutputIter Unchecked_Uninit_Move_Cat(InputIter first, InputIter last, OutputIter result, std::true_type)
	{
		return Move(first, last, result);
	}
	template<class InputIter, class OutputIter>OutputIter Unchecked_Uninit_Move_Cat(InputIter first, InputIter last, OutputIter result, std::false_type)
	{
		OutputIter cur = result;
		try
		{
			for (; first != last; ++first, ++cur)
			{
				PWL::Construct(&*cur, PWL::Move(*first));
			}
		}
		catch (...)
		{
			PWL::Destroy(result, cur);
		}
		return cur;
	}

	template<class InputIter, class OutputIter>OutputIter Uninit_Move(InputIter first, InputIter last, OutputIter result)
	{
		return unchecked_uninit_move(first, last, result,
			std::is_trivially_copy_assignable<
			typename iterator_traits<InputIter>::
			value_type>{});
	}

	// uninitialized_move_n
	// ��[first, first + n)�ϵ������ƶ����� result Ϊ��ʼ���Ŀռ䣬�����ƶ�������λ��
	template<class InputIter, class OutputIter, class Size>OutputIter Unchecked_Uninit_Move_N(InputIter first, OutputIter result, Size n, std::true_type)
	{
		return PWL::Move(first, first+n,result).second;
	}
	template<class InputIter, class OutputIter, class Size>OutputIter Unchecked_Uninit_Move_N(InputIter first, OutputIter result, Size n, std::false_type)
	{
		auto cur = result;
		try
		{
			for (; n > 0; --n, ++first, ++cur)
				PWL::Construct(&*cur, PWL::Move(*first));
		}
		catch (...)
		{
			for (; result != cur; ++result)
				PWL::Destroy(&*result);
			throw;
		}
		return cur;
	}

	template<class InputIter, class OutputIter, class Size>OutputIter Uninit_Move_N(InputIter first, OutputIter result, Size n)
	{
		return unchecked_copy_n(first, result, n,
			std::is_trivially_copy_assignable<typename iterator_traits<InputIter>::value_type>{});
	}
}