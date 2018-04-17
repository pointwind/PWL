//the alorithm is something wrong....
#ifndef PWL_CODE_QUICK_SORT_HPP_
#define PWL_CORE_QUICK_SORT_HPP_
#include <algorithm>
#include <functional>
#include <vector>
#include <iostream>
#include <ctime>
template<typename iter,typename compare>void quick_sort(iter first, iter last,compare com)
{

	auto len = last - first;
	if (len < 2)  return;
	auto mid = partition(first ,last ,[&](auto e) { return com(e, (*first + len) / 2) });
	auto mid2 = ::partition(mid, last, [&](const auto& e) { return !com(*(first+len/2), e); });
	quick_sort(first, mid,com);
	quick_sort(mid2, last,com);
}
template<typename iter,typename compare>auto partition(iter first, iter last,compare com)
{
	while (true)
	{
		while (first != last && com(*first))
		{
			first++;
		}
		if (first == last)
		{
			break;
		}
		while (last != first && !com(last))
		{
			last--;
		}
		if (first == last)
		{
			break;
		}
		std::iter_swap(first, last);
		first++;
	}
	return first;
}
#endif
