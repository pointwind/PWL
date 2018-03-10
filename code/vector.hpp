
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
