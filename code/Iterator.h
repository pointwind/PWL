#pragma once
#include<cstddef>
#include<type_traits>
namespace PWL
{
	struct input_iterator_tag { };
	struct output_iterator_tag { };
	struct forward_iterator_tag : public input_iterator_tag { };
	struct bidirectional_iterator_tag : public forward_iterator_tag { };
	struct random_access_iterator_tag : public bidirectional_iterator_tag { };
	template<
		class Category,
		class T,
		class Distance = std::ptrdiff_t,
		class Pointer = T * ,
		class Reference = T &
	> struct iterator
	{
		using terator_category = Category ;
		using value_type = T;
		using difference_type = Distance;
		using pointer = Pointer;
		using reference = Reference;

	};


	template<class Iterator,bool>struct Iterator_trait_impl{};

	template<class Iterator>struct Iterator_trait_impl<Iterator,true>
	{
		using difference_type = typename Iterator::difference_type;
		using value_type = typename Iterator::value_type;
		using pointer = typename Iterator::pointer;
		using reference = typename Iterator::reference;
		using iterator_category = typename Iterator::iterator_category;
	};
	template<class Iterator,bool>struct iterator_trait_helper{};
	template<class Iterator>struct iterator_trait_helper<Iterator,true> : public Iterator_trait_impl<Iterator,std::is_convertible<typename Iterator::iterator_category,input_iterator_tag>::value
																											 ||std::is_convertible<typename Iterator::iterator_category,output_iterator_tag>::value>
	{};


	template<class T>struct Iterator_Trait<T*>
	{
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T * ;
		using reference = T&;
		using iterator_category = random_access_iterator_tag;
	};
}