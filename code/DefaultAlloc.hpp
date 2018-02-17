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
