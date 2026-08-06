#include <iostream>

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

TEST_CASE("nullptr")
{
    pky::unique_ptr<double> ptr = nullptr;
    CHECK(ptr == nullptr);
    ptr.reset(new double(5.5));
    CHECK(ptr != nullptr);
    CHECK(*ptr == 5.5);
    (void)ptr.release();
    CHECK(ptr == nullptr);
}

TEST_CASE("operators")
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

TEST_SUITE_BEGIN("pky::vector");

TEST_CASE("instantiating")
{
    pky::vector<int> vec = {1,2,3};
    CHECK(vec.size() == 3);
    CHECK(vec.capacity() == 3);
    for(size_t i{}; i < vec.size(); ++i)
    {
        CHECK(vec[i] == i+1);
    }
    CHECK(sizeof(vec) == (sizeof(int*) + sizeof(size_t) * 2));
    // CHECK(sizeof(vec.))

    pky::vector<int> vec2(vec);
    CHECK(vec2.size() == 3);
    CHECK(vec2.capacity() == 3);
    for(size_t i{}; i < vec2.size(); ++i)
    {
        CHECK(vec2[i] == i+1);
    }

    pky::vector<int> vec3(std::move(vec2));
    CHECK(vec3.size() == 3);
    CHECK(vec3.capacity() == 3);
    for(size_t i{}; i < vec3.size(); ++i)
    {
        CHECK(vec3[i] == i+1);
    }

    pky::vector<int> vec4(5, 1);
    CHECK(vec4.size() == 5);
    CHECK(vec4.capacity() == 5);
    for(size_t i{}; i < vec4.size(); ++i)
    {
        CHECK(vec4[i] == 1);
    }

    pky::vector<int> vec5(10);
}

TEST_CASE("moving")
{
    pky::vector<int> bar{55, 56, 57};
    for(size_t i{}; i < bar.size(); ++i)
    {
        CHECK(bar[i] == i + 55);
    }

    pky::vector<int> foo(3);
    foo = std::move(bar);
    // std::cout << bar[0];
    CHECK(bar.size() == 0);
    CHECK(bar.capacity() == 0);
    // CHECK_FALSE(bar);

    CHECK(foo.size() == 3);
    CHECK(foo.capacity() == 3);
    // CHECK_UNARY(foo);
}

TEST_CASE("member functions")
{
    pky::vector foo = {1,2,3,4,5};
    CHECK(foo.size() == 5);
    CHECK(foo.capacity() == 5);
    foo.push_back(6);
    CHECK(foo.size() == 6);
    CHECK(foo.capacity() == 10);
    for(size_t i{}; i < foo.size(); ++i)
    {
        CHECK(foo[i] == i+1);
    }

    struct Vec2i
    {
        int x, y;
    };

    pky::vector<Vec2i> points{Vec2i(4, 2), Vec2i{44, 32}};
    points.emplace_back(11, 89);
    CHECK(points[0].x == 4);
    CHECK(points[1].x == 44);
    CHECK(points[2].x == 11);
    CHECK(points.size() == 3);
    CHECK(points.capacity() == 4);
    points.pop_back();
    CHECK(points.size() == 2);
    CHECK(points.capacity() == 4);
}

TEST_CASE("reserving")
{
    pky::vector vec = {1,2,3,4,5,6,7};
    vec.reserve(5);
    CHECK(vec.size() == 7);
    CHECK(vec.capacity() == 7);
    vec.reserve(10);
    CHECK(vec.size() == 7);
    CHECK(vec.capacity() == 10);
    CHECK_THROWS_AS(vec.at(9), std::out_of_range);
    for(size_t i{}; i < vec.size(); ++i)
    {
        CHECK(vec[i] == i+1);
    }
}

TEST_CASE("iterator")
{
    pky::vector<int> vec = {1,2,3,4,5,6,7,8,9,10};
    
    for(int n : vec)
    {
        // std::cout << n << '\n';
    }
}

TEST_CASE("swap")
{
    pky::vector<int> vec1 = {1,2};
    pky::vector<int> vec2 = {2,1};
    vec1.swap(vec2);
    CHECK(vec1.capacity() == 2);
    CHECK(vec1.size() == 2);
    CHECK(vec1[0] == 2);
    CHECK(vec1[1] == 1);

    CHECK(vec2.capacity() == 2);
    CHECK(vec2.size() == 2);
    CHECK(vec2[0] == 1);
    CHECK(vec2[1] == 2);
}

// TEST_CASE("shrink_to_fit")
// {
//     pky::vector<int> vec = {1,2,3};
//     vec.reserve(11);
//     CHECK(vec.capacity() == 11);
//     CHECK(vec.capacity() == 3);
// }


TEST_SUITE_END();

int main(int argc, char* argv[])
{
    pky::vector<int> vec1 = {1,2};
    pky::vector<int> vec2 = {2,1};
    vec1.swap(vec2);
    pky::vector<int> vec3(std::move(vec1));
    pky::vector<int> vec4(3);
    vec4 = vec3;
    std::vector<int> v(10);
    pky::vector<int> a(10);
    // try
    // {
    //     std::ignore = a.at(1);
    //     std::cout << a.at(1);
    // }
    // catch(const std::exception& e)
    // {
    //     std::cerr << e.what() << '\n';
    // }
    doctest::Context context;
    context.setOption("order-by", "name");
    context.setOption("no-breaks", true);
    context.applyCommandLine(argc, argv);
    int res = context.run();
}
