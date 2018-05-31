#pragma once
#include<cstddef>
#include"type_traits"
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
		using iterator_category = Category ;
		using value_type = T;
		using difference_type = Distance;
		using pointer = Pointer;
		using reference = Reference;

	};



	template<class Iterator,bool>struct Iterator_traits_impl{};

	template<class Iterator>struct Iterator_traits_impl<Iterator,true>
	{
		using difference_type = typename Iterator::difference_type;
		using value_type = typename Iterator::value_type;
		using pointer = typename Iterator::pointer;
		using reference = typename Iterator::reference;
		using iterator_category = typename Iterator::iterator_category;
	};
	
	template<class Iterator,bool>struct iterator_traits_helper{};		//jusge is this iterator has  iterator
	template<class Iterator>struct iterator_traits_helper<Iterator,true> : public 
																		Iterator_traits_impl<Iterator,std::is_convertible<typename Iterator::iterator_category,input_iterator_tag>::value
																		 ||std::is_convertible<typename Iterator::iterator_category,output_iterator_tag>::value>{};

	template<typename T>struct has_iterator_cat
	{
	private:
		std::pair<char, char> two;
		template<typename U>static two test(...);
		template<typename U>static char test(typename U::iterator_category* = 0);
	public:
		static bool value = sizeof(test<T>(0)) == sizeof(char);

	};

	template <class iterator>
	struct iterator_traits
		: public iterator_traits_helper<iterator,has_iterator_cat<iterator>::value> {};


	template<class T>struct iterator_traits<T*>
	{
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T * ;
		using reference = T & ;
		using iterator_category = random_access_iterator_tag;
	};
	template<class T>struct iterator_traits<const T*>
	{
		using difference_type = std::ptrdiff_t;
		using value_type =  T;
		using pointer = const T * ;
		using reference = const T & ;
		using iterator_category = random_access_iterator_tag;
	};
	//T is the iterator,U is iterator_category
	template<typename T, typename U, bool = has_iterator_cat<T>::value>
	struct has_ierator_cat_of
		: public my_bool_constant<std::is_convertible<typename iterator_traits<T>::iterator_category,U>::value>{};


}