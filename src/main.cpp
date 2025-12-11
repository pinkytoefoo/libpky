#include <iostream>
#include <string>
#include <vector>

#include "pky.h"

#define TRACK_ALLOCATIONS

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
	//test.lookup

	char* a = (char*)"def";
	char* newa = (char*)"abc";
	pky::string first("hello world");
	first._strcat(newa, sizeof(newa) + sizeof(a), a);
	std::cout << newa << '\n';
	
	pky::string second = first;
	pky::string third = "third but really fourth by line 28";
	pky::string fourth;
	fourth = std::move(third);

	pky::string abc = "abc";
	abc.append("def");
	println(abc);
	abc += "ghi";
	println(abc);

	// only changes string `second`
	// indicating correct copy constructor and deep copies
	second[0] = 'a';

	std::cout << second.length() << '\n';
	// GetLength() gets the total number of chars, so -1 to get index value
	std::cout << first[0] << first[first.length() - 1] << '\n';
	println(first);
	println(second);
	first = "new string";
	println(first.length());
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
	 * abc
	 * abcdef
	 * abcdefghi
	 * 11
	 * hd
	 * hello world
	 * aello world
	 * 10
	 * third but really fourth by line 28
	 * Index at 100 is out of range
	*/
}