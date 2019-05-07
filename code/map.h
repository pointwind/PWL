#pragma once
#include "functional.h"
#include"util.h"
namespace PWL
{
	template<class Key, class T, class Compare = Less<Key>>
	class Map
	{
	public:
		using key_type = Key;
		using map_type = T;
		using value_type = PWL::pair<key_type, map_type>;
		using key_compare = Compare;
	public:
	};
}
