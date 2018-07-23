#pragma once
#include"Iterator.h"
#include<new>
namespace PWL
{
	template<typename Ty>void Construct(Ty* ptr)//placement new,cons the object
	{
		::new ((void*)ptr) Ty();
	}
	template<typename Ty1,typename Ty2>void Construct(Ty1* ptr,Ty2& value)
	{
		::new ((void*)ptr) Ty1(value);
	}
	template<typename Ty,class ...Args>void Construct(Ty* ptr,Args&&... args)
	{
		::new ((void*)ptr) Ty(std::forward<Args>(args)...);
	}
	template<typename Ty>void Destroy_One(Ty*,std::true_type)
	{
		
	}
	template<typename Ty>void Destroy_One(Ty* ptr,std::false_type)
	{
		if(ptr != nullptr)
		{
			ptr->~Ty();
		}
	}
	template<class Iter>void Destroy_Cat(Iter,Iter,std::true_type)
	{
	}
	template<class Iter>void Destroy_Cat(Iter first, Iter last, std::false_type)
	{
		while (first != last)
		{
			Destroy(&*first);
			++first;
		}
	}
	template<typename Ty>void Destroy(Ty* ptr)
	{
		Destroy_One(ptr, std::is_trivially_destructible<Ty>{});
	}
	template<class Iter>void Destroy(Iter first,Iter last)
	{
		Destroy_Cat(first, last, std::is_trivially_destructible<typename iterator_traits<Iter>::value_type>{});
	}
}