#include <iostream>
#include <string>
#include <vector>

#include "pky.h"

//#define TRACK_ALLOCATIONS

// should copy successfully and avoids double delete
// https://www.youtube.com/watch?v=BvR1Pgzzr38&t=473s
// ^^ more on copy constructors ^^
//static void PrintString(const pky::string s)
//{
//	std::cout << s << '\n';
//}

template<typename ...Args>
void println(Args && ...args)
{
	(std::cout << ... << args);
	std::cout << '\n';
}

int main()
{
	// TODO: replace std::cout with googletests
	std::string test("hello world");
	//test.api_lookup
	
	pky::string first("hello world");
	pky::string second = first;
	pky::string third = "third but really fourth by line 28";
	pky::string fourth;
	fourth = std::move(third);

	pky::string abc = "abc\0";

	// only changes string `second`
	// indicating correct copy constructor and deep copies
	second[0] = 'a';

	std::cout << first.GetLength() << '\n';
	// GetLength() gets the total number of chars, so -1 to get index value
	std::cout << first[0] << first[first.GetLength() - 1] << '\n';
	println(first);
	println(second);
	first = "new string";
	println(first);
	println(fourth);

	try
	{
		fourth.at(100);
	}
	catch (const std::out_of_range& e)
	{
		std::cout << e.what() << '\n';
	}

	/* expected output:
	 * 11
	 * hd
	 * hello world
	 * aello world
	 * new string
	 * third but really fourth by line 28
	 * Index at 100 is out of range
	*/
}