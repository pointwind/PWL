#pragma once
#include <vector>
#include <algorithm>
template <typename Iter, typename Compare>
void adjust_heap(Iter top, size_t parent, size_t size, Compare cmp)
{
	auto left_child = parent * 2 + 1;
	if(left_child < size)
	{
		auto right_child = left_child + 1;
		if (right_child < size && com(*(top + left_child), *(top + right_child)))
			left_child = right_child;
		if(com(*(top + parent), *(top + left_child)))
		{
			std::iter_swap(top + parent, top + left_child);
			adjust_heap(top, left_child, size, cmp);
		}
	}

}
template<typename Iter,typename compare = std::less<>>
void heap_sort(Iter first,Iter last,compare com = compare())
{
	auto len = last - first;
	for(int i =  len/2 -1;i>= 0;--i)
	{
		adjust_heap(first, first + len - 1, len, com);
	}
	while(len >1)
	{
		std::iter_swap(first, first + len - 1);
		adjust_heap(first, 0, --len, com);
	}
}