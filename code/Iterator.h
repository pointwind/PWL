#pragma once
#include<cstddef>
#include<utility>
#include "type_traits.h"

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

	template<typename T>struct has_iterator_category
	{	
	private:
		//std::pair<char, char> two;
		template<typename U>static std::pair<char, char> test(...);
		template<typename U>static char test(typename U::iterator_category* = 0);
	public:
		static const bool value = sizeof(test<T>(0)) == sizeof(char);

	};

	template <class iterator>
	struct iterator_traits
		: public iterator_traits_helper<iterator,has_iterator_category<iterator>::value> {};


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

	//T is the iterator,U is iterator_category_tag
	//get some kind of iterator
	template<typename T, typename U, bool = has_iterator_category<T>::value>
	struct has_iterator_cat_of
		: public my_bool_constant<std::is_convertible<typename iterator_traits<T>::iterator_category,U>::value>{};

	template<typename T, typename U>struct has_iterator_cat_of <T,U,false>: public my_false_type{};

	template<typename iterator>struct is_input_iterator : public has_iterator_cat_of<iterator,input_iterator_tag>{};
	template<typename iterator>struct is_output_iterator : public has_iterator_cat_of<iterator, output_iterator_tag> {};
	template<typename iterator>struct is_forward_iterator : public has_iterator_cat_of<iterator, forward_iterator_tag> {};
	template<typename iterator>struct is_bidirectional_iterator : public has_iterator_cat_of<iterator, bidirectional_iterator_tag> {};
	template<typename iterator>struct is_random_access_iterator : public has_iterator_cat_of<iterator, random_access_iterator_tag> {};

	template<typename iterator>struct is_iterator : public my_bool_constant <is_input_iterator<iterator>::value || is_output_iterator<iterator>::value>;

	//get category 
	template<class iter>typename iterator_traits<iter>::iterator_category iterator_category(const iter&)
	{
		using category = typename iterator_traits<iter>::iterator_category;
		return category();
	}
	//get distance type
	template<class iter>typename iterator_traits<iter>::difference_type* iterator_distance_type(const iter&)
	{
		return static_cast<typename iterator_traits<iter>::difference_type*>(0);
	}

	template<class iter>typename iterator_traits<iter>::value_type* iterator_value_type(const iter&)
	{
		return static_cast<typename iterator_traits<iter>::value_type*>(0);
	}

	//the version o input_iterator_tag
	template<class inputIt>typename iterator_traits<inputIt>::difference_type distance_dispatch(inputIt first, inputIt last,input_iterator_tag)
	{
		typename iterator_traits<inputIt>::difference_type n = 0;
		while(first != last)
		{
			++n;
			++first;
		}
		return n;
	}
	//the version for random_access_iterator_tag
	template<class randomIt>typename iterator_traits<randomIt>::difference_type distance_dispatch(randomIt first, randomIt last,random_access_iterator_tag)
	{
		return last - first;
	}
	template<class iter>typename iterator_traits<iter>::difference_type distance(iter first, iter last)
	{
		return distance_dispatch(first, last, iterator_category(first));
	}

	//move the iterator
	template <class inputIt, class distance>void advance_dispatch(inputIt& i, distance n, input_iterator_tag)
	{
		while (n--)
			++i;
	}
	template <class randomIt, class distance>void advance_dispatch(randomIt& i, distance n, random_access_iterator_tag)
	{
		i += n;
	}
	template <class bidirectionalIt, class distance>void advance_dispatch(bidirectionalIt& i, distance n, bidirectional_iterator_tag)
	{
		if (n >= 0)
		{
			while(n--)
				++i;
		}
		else 
		{
			while(n++)
				++i;
		}
	}
	template <class iterator, class distance>void advance(iterator it,distance n)
	{
		advance_dispatch(it, n,iterator_category(it));
	}



	/////iterator reverse
	template<class iterator>struct reverse_iterator
	{
	private:
		iterator current; // get the forward iterator
	public:
		using difference_type = typename iterator_traits<iterator>::difference_type;
		using value_type = typename  iterator_traits<iterator>::value_type;
		using pointer = typename  iterator_traits<iterator>::pointer;
		using reference = typename  iterator_traits<iterator>::reference;
		using iterator_category = typename iterator_traits<iterator>::iterator_category;
		using iterator_type = iterator;
		using self = reverse_iterator<iterator>;
	public:
		reverse_iterator(){}
		explicit reverse_iterator(iterator_type i) : current(i){}
		reverse_iterator(const self& rhs):current(rhs.current){}
	};


}
