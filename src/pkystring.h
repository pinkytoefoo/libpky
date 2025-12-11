#pragma once

#include <stdint.h>
#include <ostream>

namespace pky
{
	/*
	 * TODO:
	 *   - add sso
	 *   - improve move semantics
	 *   - add std::cout functionality
	 *   - implement more std::string methods
	 */
	class string
	{
	public:
		string();
		string(const char* ptr);
		string(const string& old) noexcept;
		string& operator=(string&& other) noexcept;
		~string();

		void clear();
		char& at(uint32_t index);
		uint32_t _strlen(const char* ptr);

		uint32_t GetLength() const;
		char* GetData() const;

		// operators
		char& operator[](uint32_t index);

		friend std::ostream& operator<<(std::ostream& stream, const string& mystring);
	private:
		void helpercpy(void* dest, const void* src, uint32_t len) const;

		uint32_t m_Length;
		char* m_Data;
	};
}