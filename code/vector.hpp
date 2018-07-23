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
			Try_Init();
		}
		explicit Vector(size_type n)
		{
			Fill_Init(n, value_type());
		}
		explicit Vector(size_type n,const value_type& value)
		{
			Fill_Init(n, value);
		}
		template <class Iter, typename std::enable_if<
			PWL::is_input_iterator<Iter>::value, int>::type = 0>
			Vector(Iter first, Iter last)
		{
			PWL_DEBUG(!(last < first));
			Range_Init(first, last);
		}
		Vector(const Vector& rhs)
		{
			Range_Init(rhs.begin_, rhs.end_);
		}

		Vector(Vector&& rhs) noexcept :begin_(rhs.begin_),end_(rhs.end_),cap_(rhs.cap_)
		{
			rhs.begin_ = nullptr;
			rhs.end_   = nullptr;
			rhs.cap_   = nullptr;
		}
		Vector(const std::initializer_list<value_type>& list)
		{
			Range_Init(list.begin(), list.end());
		}
		Vector& operator=(const Vector& rhs);
		Vector& operator=(Vector&& rhs) noexcept;
		Vector& operator=(const std::initializer_list<value_type>& list)
		{
			Vector tmp(list.begin(), list.end());
			Swap(tmp);//////////////////////////////////
			return *this;
		}
		~Vector()
		{
			Destroy_And_Recover(begin_, end_, cap_ - begin_);
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
		bool Empty() const noexcept
		{
			return begin_ == end_;
		}
		size_type Size() const noexcept
		{
			return static_cast<size_type>(end_ - begin_);
		}
		size_type Max_Size() const noexcept
		{
			return static_cast<size_type>(-1) / sizeof(T);
		}
		size_type Capacity() const noexcept
		{
			return static_cast<size_type>(cap_ - begin_);
		}
		void      Reserve(size_type n);
		void      Shrink_To_Fit();


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
			THROW_OUT_OF_RANGE_IF(!(n < Size()), "vector<T>::at() subscript out of range");
			return *this[n];
		}
		const_reference at(size_type n) const
		{
			THROW_OUT_OF_RANGE_IF(!(n < Size()), "vector<T>::at() subscript out of range");
			return *this[n];
		}

		reference Front()
		{
			PWL_DEBUG(!Empty());
			return *begin_;
		}
		const_reference Front() const
		{
			PWL_DEBUG(!Empty());
			return *begin_;
		}
		reference back()
		{
			PWL_DEBUG(!Empty());
			return *(end_ - 1);
		}
		const_reference back() const
		{
			PWL_DEBUG(!Empty());
			return *(end_ - 1);
		}

		pointer       Data()       noexcept { return begin_; }
		const_pointer Data() const noexcept { return begin_; }



		//Assign
		void Assign(size_type n,const value_type& value)
		{
			Fill_Assign(n, value);
		}
		template<class Iter,typename std::enable_if<
		PWL::is_input_iterator<Iter>::value,int>::type* = nullptr>void Assign(Iter first, Iter last)
		{
			PWL_DEBUG(!(last < first));
			Copy_Assign(first, last, iterator_category(first));
		}
		void Assign(std::initializer_list<value_type> list)
		{
			Copy_Assign(list.begin(), list.end(), PWL::forward_iterator_tag{});
		}


		//Emplace

		template <class... Args>
		iterator Emplace(const_iterator pos, Args&& ...args);

		template <class... Args>
		void Emplace_back(Args&& ...args);

		//push/pop
		void Push_Back(const value_type& value);
		void Push_Back(value_type&& value)
		{
			Emplace_back(PWL::move(value));
		}

		void Pop_Back();


		iterator Insert(const_iterator pos, const T& value);
		iterator Insert(const_iterator pos, T&& value)
		{
			return Emplace(pos, PWL::forward<T>(value));
		}
		iterator Insert(const_iterator pos, size_type count, const T& value)
		{
			PWL_DEBUG(pos >= begin() && pos <= end());
			return Fill_Insert(const_cast<iterator>(pos), count, value);
		}
		template< class InputIt,typename std::enable_if<
		PWL::is_input_iterator<InputIt>::value,int>::type* = nullptr >
		iterator Insert(const_iterator pos, InputIt first, InputIt last)
		{
			PWL_DEBUG(pos >= begin() && pos <= end() && !(last < first));
			return Copy_Insert(const_cast<iterator>(pos), first, last);///////////////////////
		}
		iterator Insert(const_iterator pos, std::initializer_list<T> ilist)
		{
			return  Copy_Insert(const_cast<iterator>(pos), ilist.first(), ilist.last());
		}

		//Erase /Clear
		iterator Erase(const_iterator pos);
		iterator Erase(const_iterator first, const_iterator last);
		void Clear() noexcept { Erase(begin(), end()); }


		//Resize/reverse
		void Resize(size_type count) { Resize(count, value_type()); }
		void Resize(size_type count, const value_type& value);
		void Swap(Vector& other) noexcept;
		//void reverse() { PWL::reverse(begin(), end()); }



	private:
		// helper functions

		// initialize / Destroy
		void      Try_Init() noexcept;

		void      Init_Space(size_type size, size_type cap);

		void      Fill_Init(size_type n, const value_type& value);
		template <class Iter>
		void Range_Init(Iter first, Iter last);

		void      Destroy_And_Recover(iterator first, iterator last, size_type n);

			// calculate the growth Size
		size_type Get_New_Cap(size_type add_size);

			// Assign

		void      Fill_Assign(size_type n, const value_type& value);

		template <class IIter>
		void      Copy_Assign(IIter first, IIter last, input_iterator_tag);

		template <class FIter>
		void      Copy_Assign(FIter first, FIter last, forward_iterator_tag);

			// reallocate

		template <class... Args>
		void      Reallocate_Emplace(iterator pos, Args&& ...args);
		void      Reallocate_Insert(iterator pos, const value_type& value);

			// Insert

		iterator  Fill_Insert(iterator pos, size_type n, const value_type& value);
		template <class IIter>
		void      Copy_Insert(iterator pos, IIter first, IIter last);

			// Shrink_To_Fit

		void      Reinsert(size_type size);
	};


	template<class T>
	Vector<T>& Vector<T>::operator=(const Vector& rhs)
	{
		if(this != rhs)
		{
			const auto len = rhs.Size();
			if(len > Capacity())
			{
				Vector tmp(rhs.begin(), rhs.end());
				Swap(tmp);
			}
			else if(Size() >= len)
			{
				auto i = PWL::Copy(rhs.begin(), rhs.end(), begin());
			}
			else				//   Size < len < Capacity 
			{
				PWL::Copy(rhs.begin(), rhs.begin() + Size(), begin());      //Copy [begin ,begin + Size] to local[begin,end]
				PWL::Uninit_Copy(rhs.begin() + Size(), rhs.end(), end());	//init copt [begin+Size,end] to local[end,his end]
				cap_ = end_ = begin_ + len;
			}
		}
		return *this;
	}
	template<class T>
	Vector<T>& Vector<T>::operator=(Vector&& rhs) noexcept
	{
		Destroy_And_Recover(begin_, end_, cap_ - begin_);
		begin_ = rhs.begin_;
		end_ = rhs.end_;
		cap_ = rhs.cap_;
		rhs.begin_ = nullptr;
		rhs.end_ = nullptr;
		rhs.cap_ = nullptr;
		return *this;
	}

	template<class T>
	void Vector<T>::Reserve(size_type n)
	{
		if(Capacity() < n)
		{
			THROW_LENGTH_ERROR_IF(n > Max_Size(),
				"n can not larger than Max_Size() in vector<T>::Reserve(n)");
			const auto old_size = Size();
			auto tmp = data_allocator::Allocate(n);
			PWL::Uninit_Move(begin_, end_, tmp);
			data_allocator::Deallocate(begin_, cap_ - begin_);
			begin_ = tmp;
			end_ = tmp + old_size;
			cap_ = begin_ + n;
		}
	}

	template <class T>
	void Vector<T>::Shrink_To_Fit()
	{
		if (end_ < cap_)
		{
			Reinsert(Size());
		}
	}



	// 在 pos 位置就地构造元素，避免额外的复制或移动开销
	template <class T>
	template <class ...Args>
	typename Vector<T>::iterator
		Vector<T>::Emplace(const_iterator pos, Args&& ...args)
	{
		PWL_DEBUG(pos >= begin() && pos <= end());
		iterator xpos = const_cast<iterator>(pos);
		const size_type n = xpos - begin_;
		if(end_ == xpos && end_ != cap_)// == end
		{
			data_allocator::Construct(PWL::address_of(*end_), PWL::forward<Args>(args)...);
			++end_;
		}
		else if(end_ != cap_)//    begin<=end<cap
		{
			auto new_end = end_;
			data_allocator::Construct(address_of(*end), *(end_ - 1));
			++new_end;
			PWL::Copy_Backward(xpos, end_ - 1, end_);
			*xpos = value_type(PWL::forward<Args>(args)...);
		}
		else//end == cap
		{
			Reallocate_Emplace(xpos, PWL::forward<Args>(args)...);
		}
		return begin() + n;
	}
	//尾部构造
	template <class T>
	template <class ...Args>
	void Vector<T>::Emplace_back(Args&& ...args)
	{
		if(end_ < cap_)
		{
			data_allocator::Construct(PWL::address_of(*end_), PWL::forward<Args>(args)...);
			++end_;
		}
		else
		{
			Reallocate_Emplace(end_, PWL::forward<Args>(args)...);
		}
	}

	template<class T>
	void Vector<T>::Push_Back(const value_type& value)
	{
		if(end_ != cap_)
		{
			data_allocator::Construct(PWL::address_of(*end_), value);
			++end_;
		}
		else
		{
			Reallocate_Insert(end_, value);
		}
	}

	template <class T>
	void Vector<T>::Pop_Back()
	{
		PWL_DEBUG(!Empty());
		data_allocator::Destroy(end_ - 1);
		--end_;
	}

	//在pos出插入
	template<class T>typename Vector<T>::iterator
	Vector<T>::Insert(const_iterator pos, const T& value)
	{
		PWL_DEBUG(pos >= begin() && pos <= end());
		iterator xpos = const_cast<iterator>(pos);
		const size_type n = pos - begin_;
		if (end_ == xpos && end_ != cap_)// == end
		{
			data_allocator::Construct(PWL::address_of(*end_), value);
			++end_;
		}
		else if (end_ != cap_)//    begin<=end<cap
		{
			auto new_end = end_;
			data_allocator::Construct(address_of(*end), *(end_ - 1));
			++new_end;
			auto value_copy = value;	//避免因复制改变
			PWL::Copy_Backward(xpos, end_ - 1, end_);
			*xpos = PWL::move(value_copy);
		}
		else//end == cap
		{
			Reallocate_Insert(xpos, value);
		}
		return begin() + n;
	}


	//在pos出插入
	template<class T>typename Vector<T>::iterator
	Vector<T>::Erase(const_iterator pos)
	{
		PWL_DEBUG(pos >= begin() && pos < end());
		iterator xpos = begin_ + (pos - begin());
		PWL::Move(xpos + 1, end_, xpos);//Move back
		data_allocator::Destroy(end_ - 1);
		--end_;
		return xpos;
	}
	template <class T>
	typename Vector<T>::iterator
		Vector<T>::Erase(const_iterator first, const_iterator last)
	{
		MYSTL_DEBUG(first >= begin() && last <= end() && !(last < first));
		const auto n = first - begin();
		iterator r = begin_ + (first - begin());
		data_allocator::Destroy(PWL::Move(r + (last - first), end_, r),end_);
		end_ -= last - first;
		return begin_ + n;
	}

	template <class T>
	void Vector<T>::Resize(size_type count, const value_type& value)
	{
		if(count <Size())
		{
			Erase(begin_ + count, end_);
		}
		else
		{
			Insert(end_, count - Size(), value);
		}
	}

	template <class T>
	void Vector<T>::Swap(Vector<T>& other) noexcept
	{
		if (this != &other)
		{
			PWL::swap(begin_, other.begin_);
			PWL::swap(end_, other.end_);
			PWL::swap(cap_, other.cap_);
		}
	}

	//helper function------------------------

	template<class T>
	void Vector<T>::Try_Init() noexcept
	{
		try
		{
			begin_ = data_allocator::Allocate(16);
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
	void Vector<T>::Init_Space(size_type size, size_type cap)
	{
		try
		{
			begin_ = data_allocator::Allocate(cap);
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
	void Vector<T>::Fill_Init(size_type n, const value_type& value)
	{
		const size_type init_size = PWL::max(static_cast<size_type>(16), n);
		Init_Space(n, init_size);
		PWL::Uninit_Fill_N(begin_, n, value);
	}

	template <class T>
	template <class Iter>
	void Vector<T>::
		Range_Init(Iter first, Iter last)
	{
		const size_type init_size = PWL::max(static_cast<size_type>(last - first),
			static_cast<size_type>(16));
		Init_Space(static_cast<size_type>(last - first), init_size);
		PWL::Uninit_Copy(first, last, begin_);
	}

	template <class T>
	void Vector<T>::
		Destroy_And_Recover(iterator first, iterator last, size_type n)
	{
		data_allocator::Destroy(first, last);
		data_allocator::Deallocate(first, n);
	}

	template <class T>
	typename Vector<T>::size_type
		Vector<T>::
		Get_New_Cap(size_type add_size)
	{
		const auto old_size = Capacity();
		THROW_LENGTH_ERROR_IF(old_size > Max_Size() - add_size,
			"vector<T>'s Size too big");
		if(old_size > Max_Size() - old_size / 2)		//can't expance double,it's too big
		{
			return old_size + add_size > Max_Size() - 16
				? old_size + add_size : old_size + add_size + 16;
		}
		const size_type new_size = old_size == 0
			? PWL::max(add_size, static_cast<size_type>(16))
			: PWL::max(old_size + old_size / 2, old_size + add_size);
		return new_size;
	}

	template <class T>
	void Vector<T>::
		Fill_Assign(size_type n, const value_type& value)
	{
		if(n > Capacity())
		{
			Vector tmp(n, value);
			Swap(tmp);
		}
		else if(n > Size())
		{
			PWL::Fill(begin(), end(), value);
			end_ = PWL::Uninit_Fill_N(end_, n - Size(), value);
		}
		else
		{
			Erase(PWL::Fill_N(begin_, n, value), end_);
		}
	}

	template <class T>
	template <class IIter>
	void Vector<T>::
		Copy_Assign(IIter first, IIter last, input_iterator_tag)
	{
		auto cur = begin_;
		for (; first != last && cur != end_; ++first, ++cur)
		{
			*cur = *first;
		}
		if (first == last)
		{
			Erase(cur, end_);
		}
		else
		{
			Insert(end_, first, last);
		}
	}

	template <class T>
	template <class IIter>
	void Vector<T>::
		Copy_Assign(IIter first, IIter last, forward_iterator_tag)
	{
		const size_type len = PWL::distance(first, last);
		if (len > Capacity())
		{
			Vector tmp(first, last);
			Swap(tmp);
		}
		else if (len <= Size())
		{
			auto new_end = PWL::Copy(first, last, begin_);
			data_allocator::Destroy(new_end, end_);
			end_ = new_end;
		}
		else
		{
			auto mid = first;
			PWL::advance(mid, Size());
			PWL::Copy(first, mid, begin_);
			auto new_end = PWL::Uninit_Copy(mid, last, end_);
			end_ = new_end;
		}
	}

	template <class T>
	template <class ...Args>
	void Vector<T>::
		Reallocate_Emplace(iterator pos, Args&& ...args)
	{
		const auto new_size = Get_New_Cap(1);
		auto new_begin = data_allocator::Allocate(new_size);
		auto new_end = new_begin;
		try
		{
			new_end = PWL::Uninit_Move(begin_, pos, new_begin);
			data_allocator::Construct(PWL::address_of(*new_end), PWL::forward<Args>(args)...);
			++new_end;
			new_end = PWL::Uninit_Move(pos, end_, new_end);
		}
		catch(...)
		{
			data_allocator::Deallocate(new_begin, new_size);
			throw;
		}
		Destroy_And_Recover(begin_, end_, cap_ - begin_);
		begin_ = new_begin;
		end_ = new_end;
		cap_ = new_begin + new_size;
	}

	template <class T>
	void Vector<T>::
		Reallocate_Insert(iterator pos, const value_type& value)
	{
		const auto new_size = Get_New_Cap(1);
		auto new_begin = data_allocator::Allocate(new_size);
		auto new_end = new_begin;
		const value_type& value_copy = value;
		try
		{
			new_end = PWL::Uninit_Move(begin_, pos, new_begin);
			data_allocator::Construct(PWL::address_of(*new_end), value_copy);
			++new_end;
			new_end = PWL::Uninit_Move(pos, end_, new_end);
		}
		catch (...)
		{
			data_allocator::Deallocate(new_begin, new_size);
			throw;
		}
		Destroy_And_Recover(begin_, end_, cap_ - begin_);
		begin_ = new_begin;
		end_ = new_end;
		cap_ = new_begin + new_size;
	}

	template <class T>
	typename Vector<T>::iterator
		Vector<T>::
		Fill_Insert(iterator pos, size_type n, const value_type& value)
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
				PWL::Uninit_Copy(end_ - n, end_, end_);
				end_ += n;
				PWL::Move_Backward(pos, old_end - n, old_end);
				PWL::Uninit_Fill_N(pos, n, value_copy);
			}
			else
			{
				end_ = PWL::Uninit_Fill_N(end_, n - after_elems, value_copy);
				end_ = PWL::Uninit_Move(pos, old_end, end_);
				PWL::Uninit_Fill_N(pos, after_elems, value_copy);
			}
		}
		else
		{
			const auto new_size = Get_New_Cap(n);
			auto new_begin = data_allocator::Allocate(new_size);
			auto new_end = new_begin;
			try
			{
				new_end = PWL::Uninit_Move(begin_, pos, new_begin);
				new_end = PWL::Uninit_Fill_N(new_end, n, value);
				new_end = PWL::Uninit_Move(pos, end_, new_end);
			}
			catch (...)
			{
				Destroy_And_Recover(new_begin, new_end, new_size);
				throw;
			}
			data_allocator::Deallocate(begin_, cap_ - begin_);
			begin_ = new_begin;
			end_ = new_end;
			cap_ = begin_ + new_size;
		}
		return begin_ + xpos;
		
	}

	template<class T>
	template <class IIter>
	void      Vector<T>::Copy_Insert(iterator pos, IIter first, IIter last)
	{
		if (first == last)
		{
			return;
		}
		const auto n = PWL::distance(first, last);
		if((cap_ - end_) > n)
		{
			const size_type after_elems = end_ - pos;
			auto old_end = end_;
			if (after_elems > n)
			{
				PWL::Uninit_Copy(end_ - n, end_, end_);
				end_ += n;
				PWL::Move_Backward(pos, old_end - n, old_end);
				PWL::Uninit_Copy(first,last,pos);
			}
			else
			{
				auto mid = first;
				PWL::advance(mid, after_elems);
				end_ = PWL::Uninit_Copy(mid, last, end_);
				end_ = PWL::Uninit_Move(pos, old_end, end_);
				PWL::Uninit_Copy(first,mid,pos);
			}
		}
		else
		{
			const auto new_size = Get_New_Cap(n);
			auto new_begin = data_allocator::Allocate(new_size);
			auto new_end = new_begin;
			try
			{
				new_end = PWL::Uninit_Move(begin_, pos, new_begin);
				new_end = PWL::Uninit_Copy(first,last,new_end);
				new_end = PWL::Uninit_Move(pos, end_, new_end);
			}
			catch (...)
			{
				Destroy_And_Recover(new_begin, new_end, new_size);
				throw;
			}
			data_allocator::Deallocate(begin_, cap_ - begin_);
			begin_ = new_begin;
			end_ = new_end;
			cap_ = begin_ + new_size;
		}

	}

	template <class T>
	void Vector<T>::Reinsert(size_type size)
	{
		auto new_begin = data_allocator::Allocate(size);
		try
		{
			PWL::Uninit_Move(begin_, end_, new_begin);
		}
		catch (...)
		{
			data_allocator::Deallocate(new_begin, size);
			throw;
		}
		begin_ = new_begin;
		end_ = begin_ + size;
		cap_ = begin_ + size;
	}


	//operate
	template<class T>
	bool operator==(const Vector<T>& lhs, const Vector<T>& rhs)
	{
		return lhs.Size() == rhs.Size() &&
			PWL::Equal(lhs.begin(), lhs.end(), rhs.begin());
	}

	template<class T>
	bool operator<(const Vector<T>& lhs, const Vector<T>& rhs)
	{
		return PWL::Lexicographical_Compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template<class T>
	bool operator!=(const Vector<T>& lhs, const Vector<T>& rhs)
	{
		return !(lhs != rhs);
	}

	template <class T>
	bool operator>
	(const Vector<T>& lhs, const Vector<T>& rhs)
	{
		return rhs < lhs;
	}

	template <class T>
	bool operator<=(const Vector<T>& lhs, const Vector<T>& rhs)
	{
		return !(rhs < lhs);
	}

	template <class T>
	bool operator>=(const Vector<T>& lhs, const Vector<T>& rhs)
	{
		return !(lhs < rhs);
	}

	template<class T>
	void swap(const Vector<T>& lhs, const Vector<T>& rhs)
	{
		lhs.Swap(rhs);
	}
}