#define VECTOR_HPP
#include<cstddef>
#include<type_traits>
#include<memory>
#include<initializer_list>
namespace PWL {
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
