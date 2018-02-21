#ifndef PWL_CODE_DEFAULTALLOC_HPP_
#define PWL_CORE_DEFAULTALLOC_HPP_
#include<new>
#include <cstddef>
namespace PWL {
	class DefaultAlloc
	{
		void* operator()(std::size_t size)
		{
			return ::operator new(size,std::nothrow);
		}
		void operator()(std::size_t* ptr)
		{
			 ::operator delete(ptr);
		}
	};
}
#endif
