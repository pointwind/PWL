#pragma once
#include<cstddef>
#include"Construct.h"
#include"util.h"
namespace PWL
{
	template<typename T>struct allocator
	{
	public:
		using value_type = T;
		using reference = T & ;
		using pointer = T * ;
		using const_pointer = const T*;
		using const_reference = const T&;
		using size_type = size_t;
		using difference_type = std::ptrdiff_t;
	public:
		static T* Allocate();
		static T* Allocate(size_type n);

		static void Deallocate(T* ptr);
		static void Deallocate(T* ptr, size_type n);

		static void Construct(T* ptr);
		static void Construct(T* ptr, const T& value);
		static void Construct(T* ptr, T&& value);

		template<class... Args>static void Construct(T* ptr, Args&&... args);

		static void Destroy(T* ptr);
		static void Destroy(T* first, T* last);
	};
	template<class T>T* allocator<T>::Allocate()
	{
		return static_cast<T*>(::operator new(sizeof(T)));
	}
	template<class T>T* allocator<T>::Allocate(size_type n)
	{
		if (n == 0)
			return nullptr;
		return static_cast<T*>(::operator new(n * sizeof(T)));
	}

	template<class T>void allocator<T>::Deallocate(T* ptr)
	{
		if (ptr == nullptr)
			return ;
		::operator delete(ptr);
	}
	template<class T>void allocator<T>::Deallocate(T* ptr,size_type )
	{
		if (ptr == nullptr)
			return;
		::operator delete(ptr);
	}

	template<class T>void allocator<T>::Construct(T* ptr)
	{
		PWL::Construct(ptr);
	}
	template<class T>void allocator<T>::Construct(T* ptr, const T& value)
	{
		PWL::Construct(ptr,value);
	}
	template<class T>void allocator<T>::Construct(T* ptr, T&& value)
	{
		PWL::Construct(ptr,PWL::Move(value));
	}

	template<class T>template<class ...Args>void allocator<T>::Construct(T* ptr,Args&&... args)
	{
		return PWL::Construct(ptr, PWL::forward<Args>(args)...);
	}

	template<class T>void allocator<T>::Destroy(T* ptr)
	{
		PWL::Destroy(ptr);
	}
	template<class T>void allocator<T>::Destroy(T* first,T* last)
	{
		PWL::Destroy(first, last);
	}
}