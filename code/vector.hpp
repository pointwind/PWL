<<<<<<< HEAD

#define VECTOR_HPP
#include<cstddef>
#include<type_traits>
#include<memory>
#include<initializer_list>
namespace PWL {
	/*template<typename T>
	class Vector
	{
		using value_type = T;
		using size_type = size_t;
		using difference_type = std::ptrdiff_t;
		using reference = value_type & ;
		using const_reference = const value_type &;
		//using interator = RandomAccessIterator;
		//using const_interator = const_RandomAccessIterator;
		//using reverse_iterator = std::reverse_iterator<iterator>;
		//using const_reverse_iterator = std::reverse_iterator<const_interator>;
	public:
		Vector()
		{
			
		}
		~Vector()noexcept
		{
			
		}
		Vector(continer_type&)
		{
			
		}
		Vector(continer_type&&)
		{
			
		}
	public:
		reference At(size_type pos);
		const_reference At(const size_type pos) const;
		Vector operator=(const Vector& other);
		Vector operator=(Vector& other);
		Vector& operator=(Vector&& other) noexcept;
		Vector& operator=(std::initializer_list<T> ilist);
		void Assign(size_type count, const T& value);
		void Assign(std::initializer_list<T> ilist);
		template<typename InputIt>void Assign(InputIt first,InputIt last);*/
	template<typename type>class Vector;
	namespace Internal
	{
		template<typename T>class Vector_Interator
		{
		public:
			using my_type = Vector_Interator<T>;
			using difference_type = std::ptrdiff_t;
			using interator_cagetory = std::random_access_iterator_tag;
			template<typename U>friend class PWL::Vector;

			template<typename U>constexpr auto point_from_this(Vector_Interator<T> it) noexcept ->U*
			{
				return it.current_;
			}

			using current_ = T * ;
		public:
			using container = Vector<T>;
		};
	}
}
=======

#define VECTOR_HPP
#include<cstddef>
#include<type_traits>
#include<memory>
#include<initializer_list>
namespace PWL {
	template<typename T, typename Alloc = std::allocator<T>>
	class Vector
	{
		using value_type = T;
		using alloctor_type = Alloc;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using reference = value_type & ;
		using const_reference = const value_type &;
		using pointer =typename std::allocator_traits<Alloc>::pointer;
		using const_pointer =typename std::allocator_traits<Alloc>::const_pointer;
		//using interator = RandomAccessIterator;
		//using const_interator = const_RandomAccessIterator;
		//using reverse_iterator = std::reverse_iterator<iterator>;
		//using const_reverse_iterator = std::reverse_iterator<const_interator>;
	public:
		Vector();
		~Vector()noexcept;
		Vector(vector&);
		Vector(vector&&);
	public:
		reference At(size_type pos);
		const_reference At(const size_type pos) const;
		Vector operator=(const Vector& other);
		Vector operator=(Vector& other);
		Vector& operator=(Vector&& other) noexcept;
		Vector& operator=(std::initializer_list<T> ilist);
		void Assign(size_type count, const T& value);
		void Assign(std::initializer_list<T> ilist);
		template<typename InputIt>void Assign(InputIt first,InputIt last);

	private:

	};
}
>>>>>>> 2d629cebab94ed7f33711269c46fe3f960ec9bce
