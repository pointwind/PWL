#pragma once
#include<cstddef>
#include<cstdlib>
#include<climits>


#include"Construct.h"
#include"algorithm_base.h"
#include"unintialized.h"

//本文件用于高级动态内存管理
namespace PWL
{
	template<class T>constexpr const T* address_of(T& value) noexcept
	{
		return &value;
	}

	// 获取/释放 临时缓冲区
	template<class T>pair<T*, std::ptrdiff_t> get_buffer_helper(std::ptrdiff_t len, T*)
	{
		if (len > static_cast<std::ptrdiff_t>(INT_MAX / sizeof(T)))
			len = INT_MAX / sizeof(T);
		while (len > 0)
		{
			T* temp = static_cast<T*>(malloc(static_cast<size_t>(len) * sizeof(T)));
			if (temp)//成功
				return pair<T*, ptrdiff_t>(temp,len);
			len = len / 2;		
		}
		return pair<T*, ptrdiff_t>(nullptr, 0);
	}
	template <class T>pair<T*, std::ptrdiff_t> get_temporary_buffer(ptrdiff_t len)
	{
		return get_buffer_helper(len, static_cast<T*>(0));
	}
	template<class T>void release_temporary_buffer(T* ptr)
	{
		free(ptr);
	}

	template<class ForwardIter,class T>class temporary_buffer
	{
	private:
		ptrdiff_t origin_len;	//实际大小
		ptrdiff_t len;			//申请大小
		T*        buffer;

	public:
		temporary_buffer(ForwardIter first, ForwardIter last);
		
		~temporary_buffer()
		{
			PWL::Destroy(buffer,buffer+len);
			free(buffer);
		}

	public:
		ptrdiff_t  constexpr size()			  const noexcept{ return len; }
		ptrdiff_t  constexpr requested_size() const noexcept{ return origin_len; }
		T*					 begin()				noexcept{ return buffer; }
		T*					 end()					noexcept{ return buffer + len; }


	private:
		void allocate_buffer();
		void initialize_buffer(const T&,std::true_type);
		void initialize_buffer(const T& value,std::false_type)
		{
			PWL::Uninit_Fill_N(buffer, len, value);
		}

	private:
		temporary_buffer(const temporary_buffer&);
		void operator=(const temporary_buffer&);
	};
	template<class ForwardIter,class T>temporary_buffer<ForwardIter,T>::temporary_buffer(ForwardIter first,ForwardIter last)
	{
		try
		{
			len = PWL::distance(first, last);
			allocate_buffer();
			if(len > 0)
			{
				initialize_buffer(*first, std::is_trivially_default_constructible<T>{});
			}
		}
		catch(...)
		{
			free(buffer);
			buffer = nullptr;
			len = 0;
		}
	}
	template<class ForwardIter, class T>void temporary_buffer<ForwardIter, T>::allocate_buffer()
	{
		origin_len = len;
		if (len > static_cast<ptrdiff_t>(INT_MAX / sizeof(T)))
			len = INT_MAX / sizeof(T);
		while(len > 0)
		{
			buffer = static_cast<T*>(malloc(len * sizeof(T)));
			if (buffer)
				break;
			len /= 2;
		}
	}
}