#include <stdexcept>
#include <format>
#include <iostream>

#include "pkystring.h"

namespace pky {
	/*
	 * TODO:
	 *   - add sso
	 *   - improve move semantics
	 *   - add std::cout functionality
	 *   - implement more std::string methods
	 */
	string::string()
	{
		m_Length = 0;
		m_Data = nullptr;
	}

	string::string(const char* ptr)
	{
		m_Length = _strlen(ptr);
		m_Data = new char[m_Length + 1];
		helpercpy(m_Data, ptr, m_Length);
	}

	string::string(const string& old)
		: m_Length(old.m_Length)
		, m_Data(new char[old.m_Length + 1])
	{
		helpercpy(m_Data, old.m_Data, m_Length);
	}

	string& string::operator=(string&& other) noexcept
	{
		if (this != &other) {
			delete[] m_Data;
			m_Data = other.m_Data;
			m_Length = other.m_Length;
			other.m_Data = nullptr;
			other.m_Length = 0;
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
			delete[] m_Data;
			m_Data = nullptr;
		}


		m_Length = 0;
	}
	
	char& string::at(uint32_t index)
	{
		if (index < 0 || index >= m_Length)
		{
			throw std::out_of_range(std::format("Index at {} is out of range", index));
		}

		return m_Data[index];
	}

	uint32_t string::_strlen(const char* ptr)
	{
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

		for (uint32_t i = 0; i < ((len + 1) * (uint32_t)sizeof(char)); i++)
		{
			cdest[i] = csrc[i];
		}

		dest = (void*)cdest;
	}
}