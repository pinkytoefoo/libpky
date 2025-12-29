#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "pky/pky.h"

//#define TRACK_ALLOCATIONS

// should copy successfully and avoids double delete
// https://www.youtube.com/watch?v=BvR1Pgzzr38&t=473s
// ^^ more on copy constructors ^^
//static void PrintString(const pky::string s)
//{
//  std::cout << s << '\n';
//}

template<typename ...Args>
void println(Args && ...args)
{
    (std::cout << ... << args);
    std::cout << '\n';
}

int main()
{
    char* buf = new char[2];
    pky::_memcpy(buf, "a", 1);
    println(buf);
    std::string sf = "hello world";
    sf.resize(20);
    std::cout << "PRINTING: " << sf[0] << sf[sf.length() - 1] << '\n';

    // println("sf substr: ", sf.substr(-1, -1));
    pky::ofstream f("texty.txt");
    //f << "hello\n";
    // std::ofstream fa("texty.txt");
	// TODO: replace std::cout with googletests
	std::string test("hello world");
    // println("test substr: ", test.substr(-1, -1));
	//test.append()

    // _strcat
    char* newa = (char*)"abc";
    const char* a = (char*)"def";
    char* newerer = pky::_strcat(newa, a);
    println(newerer);

    // construct, copy, move
    pky::string first("hello world");
    first.resize(20);
    println("length: ", first.length(), ", buffer: ", first);
    // println("first substr: ", first.substr(0, 3));
    pky::string second = first;
    pky::string third = "third but really fourth by line 28";
    pky::string fourth;
    fourth = std::move(third); // third is now invalid

    // append, +=
    pky::string abc = "abc";
    abc.append("def");
    println(abc);
    abc += "ghi";
    println(abc);

    // only changes string `second`
    // indicating correct copy constructor and deep copies
    second[0] = 'a';

    std::cout << second.length() << '\n';
    // length() gets the total number of chars, so -1 to get index value
    std::cout << first[0] << first[first.length() - 1] << '\n';
    println(first);
    println(second);
    first = "new string";
    println(first.length());
    println(fourth);

    // bounds checking for .at
    try
    {
        fourth.at(100);
    }
    catch (const std::out_of_range& e)
    {
        std::cout << e.what() << '\n';
    }

    /* expected output:
     * abcdef
     * abc
     * abc
     * 11
     * hd
     * hello world
     * aello world
     * 10
     * third but really fourth by line 28
     * Index at 100 is out of range
     */
}

