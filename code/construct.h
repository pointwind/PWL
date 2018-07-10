#pragma once
#include<cstddef>
#include"type_traits.h"
#include"Iterator.h"
#include<new>
namespace PWL
{
	template<typename Ty>void construct(Ty* ptr)//placement new,cons the object
	{
		::new ((void*)ptr) Ty();
	}
	template<typename Ty1,typename Ty2>void construct(Ty1* ptr,Ty2& value)
	{
		::new ((void*)ptr) Ty1(value);
	}
	template<typename Ty,class ...Args>void construct(Ty* ptr,Args&&... args)
	{
		::new ((void*)ptr) Ty(std::forward<Args>(args)...);
	}
	template<typename Ty>void destroy_one(Ty*,std::true_type)
	{
		
	}
	template<typename Ty>void destroy_one(Ty* ptr,std::false_type)
	{
		if(ptr != nullptr)
		{
			ptr->~Ty();
		}
	}
	template<class Iter>void destroy_cat(Iter,Iter,std::true_type)
	{
	}
	template<class Iter>void destroy_cat(Iter first, Iter last, std::false_type)
	{
		while (first != last)
		{
			destroy(&*first);
			++first;
		}
	}
	template<typename Ty>void destroy(Ty* ptr)
	{
		destroy_one(ptr, std::is_trivially_destructible<Ty>{});
	}
	template<class Iter>void destroy(Iter first,Iter last)
	{
		destroy_cat(first, last, std::is_trivially_destructible<typename iterator_traits<Iter>::value_type>{});
	}
}