#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <cstring>
// #include <unique_ptr.h>

// #include <minunit.h>
#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest.h>

#include "pky/pky.h"
#include "pky/vector.h"
#include "pky/unique_ptr.h"

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

TEST_SUITE_BEGIN("pky::string");
TEST_CASE("pky_helpers")
{
    char* buf = new char[13];
    pky::_memcpy(buf, "hello string", 12);
    CHECK_MESSAGE(std::strncmp(buf, "hello string", 12) == 0, "pky::_memcpy failed: buf should be 12 characters and equal to `hello string'");
}

TEST_CASE("pky_string_resize")
{
    pky::string res("res");
    CHECK(res.length() == 3);
    res.resize(5);
    CHECK(res.length() == 5);

    char* check = new char[res.length() + 1];
    for(size_t i = 0; i < res.length(); ++i)
    {
        if(res[i] == '\0')
            check[i] = '0';
        else
            check[i] = res[i];
    }
    check[5] = '\0';
    CHECK(std::strncmp(check, "res00", 5) == 0);
}

TEST_SUITE_END();

TEST_SUITE_BEGIN("pky::unique_ptr");

TEST_CASE("")
{
    pky::unique_ptr<double> ptr = nullptr;
    CHECK(ptr == nullptr);
    ptr.reset(new double(5.5));
    CHECK(ptr != nullptr);
    CHECK(*ptr == 5.5);
    (void)ptr.release();
    CHECK(ptr == nullptr);
}

TEST_CASE("pky::unique_ptr operators")
{
    struct Vec2i
    {
        int x, y;
        void Work() { }
    };

    pky::unique_ptr<Vec2i> point1 = pky::make_unique<Vec2i>(4, 2);
    pky::unique_ptr<Vec2i> point2 = pky::make_unique<Vec2i>(4, 2);
    pky::unique_ptr<Vec2i> point3 = pky::make_unique<Vec2i>(1, 8);

    point1->Work();
    (*point1).Work();
    CHECK(point1->x == 4);
    CHECK(point1->y == 2);
    CHECK(point1 != point2);
    CHECK_UNARY(point3);
    std::ignore = point3.release();
    CHECK_FALSE(point3);
}

TEST_SUITE_END();

int main(int argc, char* argv[])
{
    doctest::Context context;
    context.setOption("order-by", "name");
    context.setOption("no-breaks", true);
    context.applyCommandLine(argc, argv);
    int res = context.run();

    std::unique_ptr<int> ptr2 = nullptr;
    pky::unique_ptr<int> a(new int);
    pky::unique_ptr<int> ptr1 = pky::make_unique<int>(10);

    // pky::vector<int> vec({1, 2, 3});
    // vec.push_back(5);
    // for(size_t i{}; i < vec.size(); ++i)
    // std::cout << vec[i];
    // std::cout << "as + b";
    // MU_RUN_SUITE(pky_string_suite);
    // pky::vector<int> vec({1, 2, 3});
    // vec.push_back(5);
    // for(size_t i{}; i < vec.size(); ++i)
    //     std::cout << vec[i];
    // std::cout << '\n';
    //
    // std::cout << vec.size() << vec.capacity() << '\n';
    // pky::vector vec2{vec};
    //
    // for(size_t i{}; i < vec2.size(); ++i)
    //     std::cout << vec2[i];
}

// int main()
// {
//     MU_RUN_SUITE(pky_string_suite);

//     // println("sf substr: ", sf.substr(-1, -1));
//     pky::ofstream f("texty.txt");
//     //f << "hello\n";
//     // std::ofstream fa("texty.txt");
// 	// TODO: replace std::cout with googletests
// 	std::string test("hello world");
//     println("test size: ", test.size());
//     test.insert(3, "a");
//     println("test: ", test);
//     println("test size: ", test.size());
//     // println("test substr: ", test.substr(-1, -1));
// 	//test.append()

//     // _strcat
//     char newa[7] = "abc";
//     const char* a = (char*)"def";
//     char* newerer = pky::_strcat(newa, a);
//     println(newerer);

//     // construct, copy, move
//     pky::string res("res");
//     res.insert(1, "hi"); // rhies
//     println("res: ", res);
//     // pky::string first("hello world");
//     // first.resize(5);
//     // for(size_t i = 0; i < first.length(); ++i)
//     // {
//     //     if(first[i] == '\0')
//     //         std::cout << "0";
//     //     else
//     //         std::cout << first[i];
//     // }
//     // println();
//     // println("length: ", first.length(), ", buffer: ", first);
//     // println("first substr: ", first.substr(0, 3));
//     // pky::string second = first;
//     pky::string third = "third but really fourth by line 28";
//     pky::string fourth;
//     fourth = std::move(third); // third is now invalid

//     // append, +=
//     pky::string abc = "abc";
//     abc.append("def");
//     println(abc);
//     abc += "ghi";
//     println(abc);

//     // only changes string `second`
//     // indicating correct copy constructor and deep copies
//     // second[0] = 'a';

//     // std::cout << second.length() << '\n';
//     // length() gets the total number of chars, so -1 to get index value
//     // std::cout << first[0] << first[first.length() - 1] << '\n';
//     // println(first);
//     // println(second);
//     // first = "new string";
//     // println(first.length());
//     // println(fourth);

//     // bounds checking for .at
//     try
//     {
//         fourth.at(100);
//     }
//     catch (const std::out_of_range& e)
//     {
//         std::cout << e.what() << '\n';
//     }

//     /* expected output:
//      * abcdef
//      * abc
//      * abc
//      * 11
//      * hd
//      * hello world
//      * aello world
//      * 10
//      * third but really fourth by line 28
//      * Index at 100 is out of range
//      */

//      return 0;
// }
