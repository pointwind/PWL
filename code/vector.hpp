#pragma once
#include<cstddef>
#include<initializer_list>

#include"memory.h"
#include"Iterator.h"
#include"util.h"
#include"exceptdef.h"
#include"alloctor.h"
namespace PWL
{
	template<class T>class Vector
	{
	public:
		using alloctor_type = PWL::allocator<T>;
		using data_allocator = PWL::allocator<T>;


		using value_type =typename alloctor_type::value_type;
		
		using reference		 = typename  alloctor_type::reference ;
		using pointer		 = typename  alloctor_type::pointer;
		using const_pointer  = typename  alloctor_type::const_pointer;
		using const_reference= typename  alloctor_type::const_reference;
		using difference_type= typename  alloctor_type::difference_type;
		using size_type		 = typename  alloctor_type::size_type;

		using iterator = value_type * ;
		using const_iterator = const value_type*;
		using reverse_iterator = PWL::reverse_iterator<iterator>;
		using const_reverse_iterator = PWL::reverse_iterator<const_iterator>;


		alloctor_type get_alloctor() { return data_allocator(); }

	private:
		iterator begin_;	//目前使用空间的头部
		iterator end_;		//使用空间的尾部	
		iterator cap_;		//目前存储空间的尾部
	public:
		Vector()noexcept
		{
			try_init();
		}
		explicit Vector(size_type n)
		{
			fill_init(n, value_type());
		}
		explicit Vector(size_type n,const value_type& value)
		{
			fill_init(n, value);
		}
		template <class Iter, typename std::enable_if<
			PWL::is_input_iterator<Iter>::value, int>::type = 0>
			Vector(Iter first, Iter last)
		{
			PWL_DEBUG(!(last < first));
			range_init(first, last);
		}
		Vector(const Vector& rhs)
		{
			range_init(rhs.begin_, rhs.end_);
		}

		Vector(Vector&& rhs) noexcept :begin_(rhs.begin_),end_(rhs.end_),cap_(rhs.cap_)
		{
			rhs.begin_ = nullptr;
			rhs.end_   = nullptr;
			rhs.cap_   = nullptr;
		}
		Vector(const std::initializer_list<value_type>& list)
		{
			range_init(list.begin(), list.end());
		}
		Vector& operator=(const Vector& rhs);
		Vector& operator=(Vector&& rhs) noexcept;
		Vector& operator=(const std::initializer_list<value_type>& list)
		{
			Vector tmp(list.begin(), list.end());
			swap(tmp);//////////////////////////////////
			return *this;
		}
		~Vector()
		{
			destroy_and_recover(begin_, end_, cap_ - begin_);
			begin_ = end_ = cap_ = nullptr;
		}


	public:
		iterator               begin()         noexcept
		{
			return begin_;
		}
		const_iterator         begin()   const noexcept
		{
			return begin_;
		}
		iterator               end()           noexcept
		{
			return end_;
		}
		const_iterator         end()     const noexcept
		{
			return end_;
		}
		reverse_iterator       rbegin()        noexcept
		{
			return reverse_iterator(end());
		}
		const_reverse_iterator rbegin()  const noexcept
		{
			return const_reverse_iterator(end());
		}
		reverse_iterator       rend()          noexcept
		{
			return reverse_iterator(begin());
		}
		const_reverse_iterator rend()    const noexcept
		{
			return const_reverse_iterator(begin());
		}

		const_iterator         cbegin()  const noexcept
		{
			return begin();
		}
		const_iterator         cend()    const noexcept
		{
			return end();
		}
		const_reverse_iterator crbegin() const noexcept
		{
			return rbegin();
		}
		const_reverse_iterator crend()   const noexcept
		{
			return rend();
		}
		bool empty() const noexcept
		{
			return begin_ == end_;
		}
		size_type size() const noexcept
		{
			return static_cast<size_type>(end_ - begin_);
		}
		size_type max_size() const noexcept
		{
			return static_cast<size_type>(-1) / sizeof(T);
		}
		size_type capacity() const noexcept
		{
			return static_cast<size_type>(cap_ - begin_);
		}
		void      reserve(size_type n);
		void      shrink_to_fit();


		reference operator[](size_type n)
		{
			PWL_DEBUG(n < size());
			return *(begin_ + n);
		}
		const_reference operator[](size_type n) const
		{
			PWL_DEBUG(n < size());
			return *(begin_ + n);
		}
		reference at(size_type n)
		{
			THROW_OUT_OF_RANGE_IF(!(n < size()), "vector<T>::at() subscript out of range");
			return *this[n];
		}
		const_reference at(size_type n) const
		{
			THROW_OUT_OF_RANGE_IF(!(n < size()), "vector<T>::at() subscript out of range");
			return *this[n];
		}

		reference front()
		{
			PWL_DEBUG(!empty());
			return *begin_;
		}
		const_reference front() const
		{
			PWL_DEBUG(!empty());
			return *begin_;
		}
		reference back()
		{
			PWL_DEBUG(!empty());
			return *(end_ - 1);
		}
		const_reference back() const
		{
			PWL_DEBUG(!empty());
			return *(end_ - 1);
		}

		pointer       data()       noexcept { return begin_; }
		const_pointer data() const noexcept { return begin_; }



		//assign
		void assign(size_type n,const value_type& value)
		{
			fill_assign(n, value);
		}
		template<class Iter,typename std::enable_if<
		PWL::is_input_iterator<Iter>::value,int>::type* = nullptr>void assign(Iter first, Iter last)
		{
			PWL_DEBUG(!(last < first));
			copy_assign(first, last, iterator_category(first));
		}
		void assign(std::initializer_list<value_type> list)
		{
			copy_assign(list.begin(), list.end(), PWL::forward_iterator_tag{});
		}


		//emplace

		template <class... Args>
		iterator emplace(const_iterator pos, Args&& ...args);

		template <class... Args>
		void emplace_back(Args&& ...args);

		//push/pop
		void push_back(const value_type& value);
		void push_back(value_type&& value)
		{
			emplace_back(PWL::move(value));
		}

		void pop_back();


		iterator insert(const_iterator pos, const T& value);
		iterator insert(const_iterator pos, T&& value)
		{
			return emplace(pos, PWL::forward<T>(value));
		}
		iterator insert(const_iterator pos, size_type count, const T& value)
		{
			PWL_DEBUG(pos >= begin() && pos <= end());
			return fill_insert(const_cast<iterator>(pos), count, value);
		}
		template< class InputIt,typename std::enable_if<
		PWL::is_input_iterator<InputIt>::value,int>::type* = nullptr >
		iterator insert(const_iterator pos, InputIt first, InputIt last)
		{
			PWL_DEBUG(pos >= begin() && pos <= end() && !(last < first));
			return copy_insert(const_cast<iterator>(pos), first, last);///////////////////////
		}
		iterator insert(const_iterator pos, std::initializer_list<T> ilist)
		{
			return  copy_insert(const_cast<iterator>(pos), ilist.first(), ilist.last());
		}

		//erase /clear
		iterator erase(const_iterator pos);
		iterator erase(const_iterator first, const_iterator last);
		void clear() noexcept { erase(begin(), end()); }


		//resize/reverse
		void resize(size_type count) { resize(count, value_type()); }
		void resize(size_type count, const value_type& value);
		void swap(Vector& other) noexcept;
		//void reverse() { PWL::reverse(begin(), end()); }



	private:
		// helper functions

		// initialize / destroy
		void      try_init() noexcept;

		void      init_space(size_type size, size_type cap);

		void      fill_init(size_type n, const value_type& value);
		template <class Iter>void range_init(Iter first, Iter last);

		void      destroy_and_recover(iterator first, iterator last, size_type n);

			// calculate the growth size
		size_type get_new_cap(size_type add_size);

			// assign

		void      fill_assign(size_type n, const value_type& value);

		template <class IIter>
		void      copy_assign(IIter first, IIter last, input_iterator_tag);

		template <class FIter>
		void      copy_assign(FIter first, FIter last, forward_iterator_tag);

			// reallocate

		template <class... Args>
		void      reallocate_emplace(iterator pos, Args&& ...args);
		void      reallocate_insert(iterator pos, const value_type& value);

			// insert

		iterator  fill_insert(iterator pos, size_type n, const value_type& value);
		template <class IIter>
		void      copy_insert(iterator pos, IIter first, IIter last);

			// shrink_to_fit

		void      reinsert(size_type size);
	};


	template<class T>Vector<T>& Vector<T>::operator=(const Vector& rhs)
	{
		if(this != rhs)
		{
			const auto len = rhs.size();
			if(len > capacity())
			{
				Vector tmp(rhs.begin(), rhs.end());
				swap(tmp);
			}
			else if(size() >= len)
			{
				auto i = PWL::copy(rhs.begin(), rhs.end(), begin());
			}
			else				//   size < len < capacity 
			{
				PWL::copy(rhs.begin(), rhs.begin() + size(), begin());      //copy [begin ,begin + size] to local[begin,end]
				PWL::uninit_copy(rhs.begin() + size(), rhs.end(), end());	//init copt [begin+size,end] to local[end,his end]
				cap_ = end_ = begin_ + len;
			}
		}
		return *this;
	}
	template<class T>
	Vector<T>& Vector<T>::operator=(Vector&& rhs) noexcept
	{
		destroy_and_recover(begin_, end_, cap_ - begin_);
		begin_ = rhs.begin_;
		end_ = rhs.end_;
		cap_ = rhs.cap_;
		rhs.begin_ = nullptr;
		rhs.end_ = nullptr;
		rhs.cap_ = nullptr;
		return *this;
	}

	template<class T>
	void Vector<T>::reserve(size_type n)
	{
		if(capacity() < n)
		{
			THROW_LENGTH_ERROR_IF(n > max_size(),
				"n can not larger than max_size() in vector<T>::reserve(n)");
			const auto old_size = size();
			auto tmp = data_allocator::allocate(n);
			PWL::uninit_move(begin_, end_, tmp);
			data_allocator::deallocate(begin_, cap_ - begin_);
			begin_ = tmp;
			end_ = tmp + old_size;
			cap_ = begin_ + n;
		}
	}

	template <class T>
	void Vector<T>::shrink_to_fit()
	{
		if (end_ < cap_)
		{
			reinsert(size());
		}
	}



	// 在 pos 位置就地构造元素，避免额外的复制或移动开销
	template <class T>
	template <class ...Args>
	typename Vector<T>::iterator
		Vector<T>::emplace(const_iterator pos, Args&& ...args)
	{
		PWL_DEBUG(pos >= begin() && pos <= end());
		iterator xpos = const_cast<iterator>(pos);
		const size_type n = xpos - begin_;
		if(end_ == xpos && end_ != cap_)// == end
		{
			data_allocator::construct(PWL::address_of(*end_), PWL::forward<Args>(args)...);
			++end_;
		}
		else if(end_ != cap_)//    begin<=end<cap
		{
			auto new_end = end_;
			data_allocator::construct(address_of(*end), *(end_ - 1));
			++new_end;
			PWL::copy_backward(xpos, end_ - 1, end_);
			*xpos = value_type(PWL::forward<Args>(args)...);
		}
		else//end == cap
		{
			reallocate_emplace(xpos, PWL::forward<Args>(args)...);
		}
		return begin() + n;
	}
	//尾部构造
	template <class T>
	template <class ...Args>
	void Vector<T>::emplace_back(Args&& ...args)
	{
		if(end_ < cap_)
		{
			data_allocator::construct(PWL::address_of(*end_), PWL::forward<Args>(args)...);
			++end_;
		}
		else
		{
			reallocate_emplace(end_, PWL::forward<Args>(args)...);
		}
	}

	template<class T>
	void Vector<T>::push_back(const value_type& value)
	{
		if(end_ != cap_)
		{
			data_allocator::construct(PWL::address_of(*end_), value);
			++end_;
		}
		else
		{
			reallocate_insert(end_, value);
		}
	}

	template <class T>
	void Vector<T>::pop_back()
	{
		PWL_DEBUG(!empty());
		data_allocator::destroy(end_ - 1);
		--end_;
	}

	//在pos出插入
	template<class T>typename Vector<T>::iterator
	Vector<T>::insert(const_iterator pos, const T& value)
	{
		PWL_DEBUG(pos >= begin() && pos <= end());
		iterator xpos = const_cast<iterator>(pos);
		const size_type n = pos - begin_;
		if (end_ == xpos && end_ != cap_)// == end
		{
			data_allocator::construct(PWL::address_of(*end_), value);
			++end_;
		}
		else if (end_ != cap_)//    begin<=end<cap
		{
			auto new_end = end_;
			data_allocator::construct(address_of(*end), *(end_ - 1));
			++new_end;
			auto value_copy = value;	//避免因复制改变
			PWL::copy_backward(xpos, end_ - 1, end_);
			*xpos = PWL::move(value_copy);
		}
		else//end == cap
		{
			reallocate_insert(xpos, value);
		}
		return begin() + n;
	}


	//在pos出插入
	template<class T>typename Vector<T>::iterator
	Vector<T>::erase(const_iterator pos)
	{
		PWL_DEBUG(pos >= begin() && pos < end());
		iterator xpos = begin_ + (pos - begin());
		PWL::move(xpos + 1, end_, xpos);//move back
		data_allocator::destroy(end_ - 1);
		--end_;
		return xpos;
	}
	template <class T>
	typename Vector<T>::iterator
		Vector<T>::erase(const_iterator first, const_iterator last)
	{
		MYSTL_DEBUG(first >= begin() && last <= end() && !(last < first));
		const auto n = first - begin();
		iterator r = begin_ + (first - begin());
		data_allocator::destroy(PWL::move(r + (last - first), end_, r),end_);
		end_ -= last - first;
		return begin_ + n;
	}

	template <class T>
	void Vector<T>::resize(size_type count, const value_type& value)
	{
		if(count <size())
		{
			erase(begin_ + count, end_);
		}
		else
		{
			insert(end_, count - size(), value);
		}
	}

	template <class T>
	void Vector<T>::swap(Vector<T>& other) noexcept
	{
		if (this != &other)
		{
			PWL::swap(begin_, other.begin_);
			PWL::swap(end_, other.end_);
			PWL::swap(cap_, other.cap_);
		}
	}

	//helper function------------------------

	template<class T>void Vector<T>::try_init() noexcept
	{
		try
		{
			begin_ = data_allocator::allocate(16);
			end_ = begin_;
			cap_ = begin_ + 16;
		}
		catch(...)
		{
			begin_ = nullptr;
			end_ = nullptr;
			cap_ = nullptr;
		}
	}

	template <class T>
	void Vector<T>::init_space(size_type size, size_type cap)
	{
		try
		{
			begin_ = data_allocator::allocate(cap);
			end_ = begin_ + size;
			cap_ = begin_ + cap;
		}
		catch (...)
		{
			begin_ = nullptr;
			end_ = nullptr;
			cap_ = nullptr;
			throw;
		}
	}
	
	template <class T>
	void Vector<T>::fill_init(size_type n, const value_type& value)
	{
		const size_type init_size = PWL::max(static_cast<size_type>(16), n);
		init_space(n, init_size);
		PWL::uninit_fill_n(begin_, n, value);
	}

	template <class T>
	template <class Iter>
	void Vector<T>::
		range_init(Iter first, Iter last)
	{
		const size_type init_size = PWL::max(static_cast<size_type>(last - first),
			static_cast<size_type>(16));
		init_space(static_cast<size_type>(last - first), init_size);
		PWL::uninit_copy(first, last, begin_);
	}

	template <class T>
	void Vector<T>::
		destroy_and_recover(iterator first, iterator last, size_type n)
	{
		data_allocator::destroy(first, last);
		data_allocator::deallocate(first, n);
	}

	template <class T>
	typename Vector<T>::size_type
		Vector<T>::
		get_new_cap(size_type add_size)
	{
		const auto old_size = capacity();
		THROW_LENGTH_ERROR_IF(old_size > max_size() - add_size,
			"vector<T>'s size too big");
		if(old_size > max_size() - old_size / 2)		//can't expance double,it's too big
		{
			return old_size + add_size > max_size() - 16
				? old_size + add_size : old_size + add_size + 16;
		}
		const size_type new_size = old_size == 0
			? PWL::max(add_size, static_cast<size_type>(16))
			: PWL::max(old_size + old_size / 2, old_size + add_size);
		return new_size;
	}

	template <class T>
	void Vector<T>::
		fill_assign(size_type n, const value_type& value)
	{
		if(n > capacity())
		{
			Vector tmp(n, value);
			swap(tmp);
		}
		else if(n > size())
		{
			PWL::fill(begin(), end(), value);
			end_ = PWL::uninit_fill_n(end_, n - size(), value);
		}
		else
		{
			erase(PWL::fill_n(begin_, n, value), end_);
		}
	}

	template <class T>
	template <class IIter>
	void Vector<T>::
		copy_assign(IIter first, IIter last, input_iterator_tag)
	{
		auto cur = begin_;
		for (; first != last && cur != end_; ++first, ++cur)
		{
			*cur = *first;
		}
		if (first == last)
		{
			erase(cur, end_);
		}
		else
		{
			insert(end_, first, last);
		}
	}

	template <class T>
	template <class IIter>
	void Vector<T>::
		copy_assign(IIter first, IIter last, forward_iterator_tag)
	{
		const size_type len = PWL::distance(first, last);
		if (len > capacity())
		{
			Vector tmp(first, last);
			swap(tmp);
		}
		else if (len <= size())
		{
			auto new_end = PWL::copy(first, last, begin_);
			data_allocator::destroy(new_end, end_);
			end_ = new_end;
		}
		else
		{
			auto mid = first;
			PWL::advance(mid, size());
			PWL::copy(first, mid, begin_);
			auto new_end = PWL::uninit_copy(mid, last, end_);
			end_ = new_end;
		}
	}

	template <class T>
	template <class ...Args>
	void Vector<T>::
		reallocate_emplace(iterator pos, Args&& ...args)
	{
		const auto new_size = get_new_cap(1);
		auto new_begin = data_allocator::allocate(new_size);
		auto new_end = new_begin;
		try
		{
			new_end = PWL::uninit_move(begin_, pos, new_begin);
			data_allocator::construct(PWL::address_of(*new_end), PWL::forward<Args>(args)...);
			++new_end;
			new_end = PWL::uninit_move(pos, end_, new_end);
		}
		catch(...)
		{
			data_allocator::deallocate(new_begin, new_size);
			throw;
		}
		destroy_and_recover(begin_, end_, cap_ - begin_);
		begin_ = new_begin;
		end_ = new_end;
		cap_ = new_begin + new_size;
	}

	template <class T>
	void Vector<T>::
		reallocate_insert(iterator pos, const value_type& value)
	{
		const auto new_size = get_new_cap(1);
		auto new_begin = data_allocator::allocate(new_size);
		auto new_end = new_begin;
		const value_type& value_copy = value;
		try
		{
			new_end = PWL::uninit_move(begin_, pos, new_begin);
			data_allocator::construct(PWL::address_of(*new_end), value_copy);
			++new_end;
			new_end = PWL::uninit_move(pos, end_, new_end);
		}
		catch (...)
		{
			data_allocator::deallocate(new_begin, new_size);
			throw;
		}
		destroy_and_recover(begin_, end_, cap_ - begin_);
		begin_ = new_begin;
		end_ = new_end;
		cap_ = new_begin + new_size;
	}

	template <class T>
	typename Vector<T>::iterator
		Vector<T>::
		fill_insert(iterator pos, size_type n, const value_type& value)
	{
		if (n == 0)
			return pos;
		const auto xpos = pos - begin_;
		const value_type value_copy = value;
		if(static_cast<size_type>(cap_ - end_) >= n)  //enough
		{
			const size_type after_elems = end_ - pos;
			auto old_end = end_;
			if(after_elems > n)
			{
				PWL::uninit_copy(end_ - n, end_, end_);
				end_ += n;
				PWL::move_backward(pos, old_end - n, old_end);
				PWL::uninit_fill_n(pos, n, value_copy);
			}
			else
			{
				end_ = PWL::uninit_fill_n(end_, n - after_elems, value_copy);
				end_ = PWL::uninit_move(pos, old_end, end_);
				PWL::uninit_fill_n(pos, after_elems, value_copy);
			}
		}
		else
		{
			const auto new_size = get_new_cap(n);
			auto new_begin = data_allocator::allocate(new_size);
			auto new_end = new_begin;
			try
			{
				new_end = PWL::uninit_move(begin_, pos, new_begin);
				new_end = PWL::uninit_fill_n(new_end, n, value);
				new_end = PWL::uninit_move(pos, end_, new_end);
			}
			catch (...)
			{
				destroy_and_recover(new_begin, new_end, new_size);
				throw;
			}
			data_allocator::deallocate(begin_, cap_ - begin_);
			begin_ = new_begin;
			end_ = new_end;
			cap_ = begin_ + new_size;
		}
		return begin_ + xpos;
		
	}
}