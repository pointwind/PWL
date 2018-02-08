#pragma once
#include <algorithm>
#include <cstddef>
#include<climits>
#include<cstring>
#include <ostream>
#include<type_traits>
namespace PWL
{
	namespace detail
	{
		static std::size_t const high_bit_mask = static_cast<std::size_t>(1) << (sizeof(std::size_t) * CHAR_BIT - 1);
		static std::size_t const sec_high_bit_mask = static_cast<std::size_t>(1) << (sizeof(std::size_t) * CHAR_BIT - 2);

		template <typename T>
		unsigned char* uchar_cast(T* p) {
			return reinterpret_cast<unsigned char*>(p);
		}

		template <typename T>
		unsigned char const* uchar_cast(T const* p) {
			return reinterpret_cast<unsigned char const*>(p);
		}

		template <typename T>
		unsigned char& most_sig_byte(T& obj) {
			return *(reinterpret_cast<unsigned char*>(&obj) + sizeof(obj) - 1);
		}

		template <int N>
		bool lsb(unsigned char byte) {
			return byte & (1u << N);
		}

		template <int N>
		bool msb(unsigned char byte) {
			return byte & (1u << (CHAR_BIT - N - 1));
		}

		template <int N>
		void set_lsb(unsigned char& byte, bool bit) {
			if (bit) {
				byte |= 1u << N;
			}
			else {
				byte &= ~(1u << N);
			}
		}

		template <int N>
		void set_msb(unsigned char& byte, bool bit) {
			if (bit) {
				byte |= 1u << (CHAR_BIT - N - 1);
			}
			else {
				byte &= ~(1u << (CHAR_BIT - N - 1));
			}
		}
	}
	template<typename CharT, typename Traits = std::char_traits<CharT>>
	class basic_string
	{
		typedef typename std::make_unsigned<CharT>::type UCharT;
	public:
		basic_string() noexcept : basic_string{"",static_cast<std::size_t>(0) } {}
		basic_string(CharT* const string) noexcept : basic_string{ string,std::strlen(string) } {}
		basic_string(CharT* const string, std::size_t size)
		{
			if (size <= pwl_capacity)
			{
				Traits::move(m_data.pwl.string, string, size);
				Traits::assign(m_data.pwl.string[size], static_cast<CharT>(0));
				set_pwl_size(size);
			}
			else
			{
				m_data.non_pwl.ptr = new CharT[size + 1];
				Traits::move(m_data.pwl.string, string, size);
				Traits::assign(m_data.pwl.string[size], static_cast<CharT>(0));
				set_non_pwl_data(size, size);
			}
		}
		basic_string(basic_string const & string)
		{
			if (string.pwl())
			{
				m_data.pwl = string.m_data.pwl;
			}
			else
			{
				new (this) basic_string{ string.data(),string.size() };
			}
		}
		basic_string(basic_string&& string) noexcept
		{
			m_data = string.m_data;
			string.set_moved_from();
		}
		basic_string& operator=(basic_string const & string)
		{
			auto copy = string;
			swap(copy, *this);
			return *this;
		}
		basic_string& operator=(basic_string&& string)
		{
			this->~basic_string();
			m_data = string.m_data;
			string.set_moved_from();
			return *this;
		}
		~basic_string()
		{
			if (!pwl())
			{
				delete[] m_data.non_pwl.ptr;
			}
		}

		CharT const * data() const noexcept
		{
			return pwl() ? m_data.pwl.string : m_data.non_pwl.ptr;
		}
		std::size_t size() const noexcept
		{
			if (pwl())
			{
				return m_data.pwl.size;
			}
			else
			{
				return read_non_pwl_data().first;
			}
		}
		std::size_t capacity() const noexcept
		{
			if (pwl())
			{
				return m_data.pwl.capacity;
			}
			else
			{
				return read_non_pwl_data().second;
			}
		}

		friend void swap(basic_string&&lhs, basic_string&&rhs)
		{
			std::swap(lhs.m_data, rhs.m_data);
		}

	private:
		void set_moved_from() noexcept
		{
			set_pwl_size(0);
		}
		bool pwl() const noexcept
		{
			return !detail::lsb<0>(m_data.pwl.size) && !detail::lsb<1>(m_data.pwl.size);
		}
		void set_pwl_size(unsigned char size)noexcept
		{
			m_data.pwl.size = static_cast<UCharT>(pwl_capacity - size) << 2;
		}

		std::size_t pwl_size()const noexcept
		{
			return pwl_capacity - ((m_data.pwl.size >> 2) & 63u);
		}
		void set_non_pwl_data(std::size_t size, std::size_t capacity)
		{
			auto& size_hsb = detail::most_sig_byte(size);
			auto const size_high_bit = detail::msb<0>(size_hsb);

			auto cap_hsb = detail::most_sig_byte(capacity);
			auto const cap_high_bit = detail::msb<0>(cap_hsb);
			auto const cap_sec_high_bit = detail::msb<1>(cap_hsb);

			detail::set_msb<0>(size_hsb, cap_sec_high_bit);

			cap_hsb << 2;
			detail::set_lsb<0>(cap_hsb, cap_high_bit);
			detail::set_lsb<1>(cap_hsb, !size_high_bit);

			m_data.non_pwl.size = size;
			m_data.non_pwl.capacity = capacity;

		}
		std::pair<std::size_t, std::size_t> read_non_pwl_data() const
		{
			auto size = m_data.non_pwl.size;
			auto capacity = m_data.NonPWL.capacity;

			auto& size_hsb = detail::most_sig_byte(size);
			auto& cap_hsb = detail::most_sig_byte(capacity);

			// Remember to negate the high bits
			auto const cap_high_bit = detail::lsb<0>(cap_hsb);
			auto const size_high_bit = !detail::lsb<1>(cap_hsb);
			auto const cap_sec_high_bit = detail::msb<0>(size_hsb);

			detail::set_msb<0>(size_hsb, size_high_bit);

			cap_hsb >>= 2;
			detail::set_msb<0>(cap_hsb, cap_high_bit);
			detail::set_msb<1>(cap_hsb, cap_sec_high_bit);

			return std::make_pair(size, capacity);
		}

	private:
		union Data
		{
			struct NonPWL
			{
				CharT *ptr;
				std::size_t size;
				std::size_t capacity;
			}non_pwl;
			struct PWL
			{
				CharT string[sizeof(non_pwl) / sizeof(CharT) - 1];
				UCharT size;
			} pwl;
		}m_data;
		static std::size_t const pwl_capacity = sizeof(typename Data::NonPWL) / sizeof(CharT) - 1;
	};


	template <typename CharT, typename Traits>
	bool operator==(const basic_string<CharT, Traits>& lhs, const CharT* rhs) noexcept {
		return !std::strcmp(lhs.data(), rhs);
	}

	template <typename CharT, typename Traits>
	bool operator==(const CharT* lhs, const basic_string<CharT, Traits>& rhs) noexcept {
		return rhs == lhs;
	}

	template <typename CharT, typename Traits>
	bool operator==(const basic_string<CharT, Traits>& lhs,
		const basic_string<CharT, Traits>& rhs) noexcept {
		if (lhs.size() != rhs.size()) return false;
		return !std::strcmp(lhs.data(), rhs.data());
	}

	template <typename CharT, typename Traits>
	std::ostream& operator<<(std::ostream& stream, const basic_string<CharT, Traits>& string) {
		return stream << string.data();
	}
	typedef basic_string<char> string;
}
