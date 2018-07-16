#pragma once
#include<cstddef>
#include<cstdlib>
#include<climits>

#include"alloctor.h"
#include"construct.h"
#include"algorithm_base.h"
#include"unintialized.h"

//本文件用于高级动态内存管理
namespace PWL
{
	template<class T>constexpr const T* addressof(T& value) noexcept
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
			len / 2;		
		}
		return pair<T*,ptrdiff_t>(nullptr,0)
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
		ptrdiff_t origin_len;
		ptrdiff_t len;
		T*        buffer;

	public:

	};
}