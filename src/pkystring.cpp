#include <stdexcept>
#include <format>

#include "pkystring.h"

namespace pky
{
	/* TODO:
	 *   - add sso
	 *   - improve move semantics
	 *   - implement more std::string methods
	 */
	string::string()
	{
		m_Length = 0;
		m_Data = new char[1];
		m_Data[0] = '\0';
	}

	string::string(const char* ptr)
	{
		m_Length = _strlen(ptr);
		m_Data = new char[m_Length + 1];
		helpercpy(m_Data, ptr, m_Length);
	}

	string::string(const string& other) noexcept
		: m_Length(other.m_Length)
		, m_Data(new char[other.m_Length + 1])
	{
		helpercpy(m_Data, other.m_Data, m_Length);
	}

	string& string::operator=(string&& old) noexcept
	{
		if (this != &old) {
			// set current string
			m_Length = old.m_Length;
			delete[] m_Data;
			m_Data = old.m_Data;

			// invalidate old string
			old.m_Length = 0;

			// may fix some issues, but keep current implementation
			old.m_Data = new char[1];
			old.m_Data[0] = '\0';
		}
		return *this;
	}

	string::~string()
	{
		clear();
	}

	void string::clear()
	{
		if (m_Data != nullptr)
		{
			m_Length = 0;
			delete[] m_Data;
			m_Data = new char[1];
			m_Data[0] = '\0';
		}

		m_Length = 0;
	}
	
	char& string::at(uint32_t index)
	{
		// no need to check negative indexes (index is unsigned)
		if (index >= m_Length)
		{
			// might have to throw for negative numbers
			// because compilers wraps negative numbers
			// leading to misleading errors
			throw std::out_of_range(std::format("Index at {} is out of range", index));
		}

		return m_Data[index];
	}

	uint32_t string::_strlen(const char* ptr)
	{
		if (ptr == nullptr)
			return 0;

		uint32_t len = 0;
		for (; ptr[len] != '\0'; len++);
		return len;
	}

	uint32_t string::GetLength() const { return m_Length; }
	char* string::GetData() const { return m_Data; }

	// operators
	char& string::operator[](uint32_t index)
	{
		return m_Data[index];
	}

	// converts length to byte size, which accounts for null terminator
	// also bypasses the `sub expression may overflow` warning by casting sizeof
	void string::helpercpy(void* dest, const void* src, uint32_t len) const
	{
		char* cdest = (char*)dest;
		const char* csrc = (const char*)src;

		for (uint32_t i = 0; i < len + 1; i++)
		{
			cdest[i] = csrc[i];
		}
	}

	std::ostream& operator<<(std::ostream& stream, const string& mystring)
	{
		stream << mystring.m_Data;
		return stream;
	}
}