#include <iostream>
#include <string>
#include <exception>
#include <vector>

#include "pky.h"

//#define TRACK_ALLOCATIONS

// should copy successfully and avoids double delete
// https://www.youtube.com/watch?v=BvR1Pgzzr38&t=473s
// ^^ more on copy constructors ^^
static void PrintString(const pky::string s)
{
	std::cout << s.GetData() << '\n';
}

int main()
{
	// TODO: simplify tests with googletests
	std::string test("hello world");
	//test.api_lookup
	
	pky::string first("hello world");
	pky::string second = first;
	pky::string third = "third but really fourth by line 28";
	pky::string fourth;
	fourth = std::move(third);

	// only changes string `second`
	// indicating correct copy constructor and deep copies
	second[0] = 'a';

	std::cout << first.GetLength() << '\n';
	// GetLength() gets the total number of chars, so -1 to get index value
	std::cout << first[0] << first[first.GetLength() - 1] << '\n';
	PrintString(first);
	PrintString(second);
	first = "new string";
	PrintString(first);
	PrintString(fourth);
	std::cout << second._strlen(nullptr);

	/* expected output:
	 * 11
	 * hd
	 * hello world
	 * aello world
	 * new string
	 * third but really fourth by line 28
	*/
}