#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include <minunit.h>

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

MU_TEST(pky_helpers)
{
    char* buf = new char[13];
    pky::_memcpy(buf, "hello string", 12);
    mu_assert(strncmp(buf, "hello string", 12) == 0, "pky::_memcpy failed: buf should be 12 characters and equal to `hello string'");
}

MU_TEST(pky_string_resize)
{
    pky::string res("res");
    mu_check(res.length() == 3);
    res.resize(5);
    mu_check(res.length() == 5);

    char* check = new char[res.length() + 1];
    for(size_t i = 0; i < res.length(); ++i)
    {
        if(res[i] == '\0')
            check[i] = '0';
        else
            check[i] = res[i];
    }
    check[5] = '\0';

    mu_check(strncmp(check, "res00", 5) == 0);
}

MU_TEST_SUITE(pky_string_suite)
{
    MU_RUN_TEST(pky_string_resize);
    MU_RUN_TEST(pky_helpers);
}

int main()
{
    MU_RUN_SUITE(pky_string_suite);

    // println("sf substr: ", sf.substr(-1, -1));
    pky::ofstream f("texty.txt");
    //f << "hello\n";
    // std::ofstream fa("texty.txt");
	// TODO: replace std::cout with googletests
	std::string test("hello world");
    println("test size: ", test.size());
    test.insert(3, "a");
    println("test: ", test);
    println("test size: ", test.size());
    // println("test substr: ", test.substr(-1, -1));
	//test.append()

    // _strcat
    char newa[7] = "abc";
    const char* a = (char*)"def";
    char* newerer = pky::_strcat(newa, a);
    println(newerer);

    // construct, copy, move
    pky::string res("res");
    res.insert(1, "hi"); // rhies
    println("res: ", res);
    // pky::string first("hello world");
    // first.resize(5);
    // for(size_t i = 0; i < first.length(); ++i)
    // {
    //     if(first[i] == '\0')
    //         std::cout << "0";
    //     else
    //         std::cout << first[i];
    // }
    // println();
    // println("length: ", first.length(), ", buffer: ", first);
    // println("first substr: ", first.substr(0, 3));
    // pky::string second = first;
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
    // second[0] = 'a';

    // std::cout << second.length() << '\n';
    // length() gets the total number of chars, so -1 to get index value
    // std::cout << first[0] << first[first.length() - 1] << '\n';
    // println(first);
    // println(second);
    // first = "new string";
    // println(first.length());
    // println(fourth);

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

     return 0;
}
