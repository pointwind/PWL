#ifndef PWL_CODE_QUICK_SORT_HPP_
#define PWL_CORE_QUICK_SORT_HPP_
#include <algorithm>
#include <functional>
#include <vector>
#include <iostream>
template<typename iter, typename compare>decltype(auto) partition(iter first, iter last, compare com)
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
		--last;
		while (last != first && !com(*last))
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
template<typename iter, typename compare = std::less<>>void quick_sort(iter first, iter last, compare com = compare())
{
	const auto len = last - first;
	if (first >= last)  return;
	auto mid = *(first + len/2);
	auto mid1 = ::partition(first, last, [&](const auto& e) { return com(e, mid); });
	auto mid2 = ::partition(mid1, last, [&](const auto& e) { return !com(mid,e); });
	quick_sort(first, mid1,com);
	quick_sort(mid2, last,com);
}
#endif
