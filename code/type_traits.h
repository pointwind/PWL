#pragma once
namespace PWL
{
	template<class t,t v>struct my_integral_constant
	{
		static const auto value = v;
	};
	template<bool b>using my_bool_constant = my_integral_constant<bool, b>;
	using my_true_type = my_bool_constant<true>;
	using my_false_type = my_bool_constant<false>;


	template<class lhs, class rhs>struct pair;
	template<class T>struct is_pair : my_false_type{};
	template<class T1,class T2>struct is_pair<pair<T1,T2>> : my_true_type{};
}