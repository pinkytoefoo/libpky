# libpky
C++ STL-like library with a focus on optimizing tradeoffs between container size, functionality, and speed. The current containers implemented are:\
[`pky::vector`](pkyvector-vs-stdvector)\
[`pky::unique_ptr`](pkyunique_ptr-vs-stdunique_ptr)\
[`pky::string`](pkystring-vs-stdstring)\
[`pky::fstreams`](pkyfstream-vs-stdfstream)

### Sections
[Comparison](#comparison---std-vs-pky)\
[Get started](#installation)\
[Philosophy](#philosophy)

# Comparison - std vs pky
The following table compares the currently implemented `pky` types with their closest standard-library equivalents

| `pky` | Standard library | Purpose |
|---|---|---|
| `pky::vector` | `std::vector` | Dynamic contiguous storage |
| `pky::unique_ptr` | `std::unique_ptr` | Exclusive ownership of dynamically allocated objects |
| `pky::string` | `std::string` | Dynamically allocated null-terminated strings |
| `pky::ifstream` | `std::ifstream` | File input |
| `pky::ofstream` | `std::ofstream` | File output |

## `pky::vector` vs `std::vector`

`pky::vector` is a dynamically sized contiguous container modeled after `std::vector`. The implementation focuses on providing the core functionality while keeping the implementation relatively small and exposing the allocator as part of the design.

| Feature | `pky::vector` | `std::vector` |
|---|---:|---:|
| Contiguous storage | Yes | Yes |
| Dynamic growth | Yes | Yes |
| `push_back()` | Yes | Yes |
| `emplace_back()` | Yes | Yes |
| `pop_back()` | Yes | Yes |
| `clear()` | Yes | Yes |
| `reserve()` | Yes | Yes |
| `shrink_to_fit()` | Yes | Yes |
| `size()` | Yes | Yes |
| `capacity()` | Yes | Yes |
| `data()` | Yes | Yes |
| `operator[]` | Yes | Yes |
| Bounds-checked `at()` | Yes | Yes |
| `front()` / `back()` | Yes | Yes |
| Iterators | Basic, Forward-Iterator only | Full standard iterator support |
| Initializer-list construction | Yes | Yes |
| Copy construction | Yes | Yes |
| Copy assignment | Yes | Yes |
| Move construction | Yes | Yes |
| Move assignment | Yes | Yes |
| Custom allocator | Yes | Yes |
| `resize()` | Not implemented | Yes |
| `insert()` | No | Yes |
| `erase()` | No | Yes |
| `assign()` | No | Yes |
| `swap()` | Yes | Yes |
| Full standard API | No | Yes |

### Memory management

`pky::vector` uses an allocator abstraction to handle allocation, construction, destruction, and deallocation:

```cpp
template<typename T>
struct default_allocator
{
    using value_type = T;
    using pointer = T*;

    [[nodiscard]] pointer allocate(size_t count);
    
    template<typename... Args>
    void construct_at(pointer ptr, Args&&... args);

    void destroy_at(pointer ptr);
    void deallocate(pointer ptr);
};
```

This allows the vector's memory-management implementation to be separated from the container itself and allows a custom allocator to be supplied:

```cpp
pky::vector<T, CustomAllocator>
```

The allocator also uses `[[no_unique_address]]`, allowing an empty allocator to potentially require no additional storage within the vector object.

### Growth strategy

When additional capacity is required, `pky::vector` grows its capacity by approximately 2x:

```cpp
if(size_ >= capacity_)
    reallocate_(capacity_ != 0 ? capacity_ * 2 : 1);
```

This provides amortized constant-time growth for `push_back()` and `emplace_back()`, assuming the element type can be moved or copied appropriately.

`reserve()` can also be used to explicitly allocate additional capacity:

```cpp
pky::vector<int> values;

values.reserve(100);
```

### Object lifetime management

Unlike a simple dynamically allocated array, `pky::vector` separately manages storage and object lifetime.

Elements are constructed using the allocator:

```cpp
allocator_.construct_at(elements_ + size_, ...);
```

and explicitly destroyed when removed or when the vector is destroyed:

```cpp
allocator_.destroy_at(elements_ + size_);
```

During reallocation, existing elements are moved using `std::move_if_noexcept()` where appropriate, to avoid copying wherever possible.

This allows `pky::vector` to support types that are not trivially constructible or destructible.

### Iterator support

`pky::vector` currently provides a basic iterator capable of:

- Incrementing and decrementing.
- Dereferencing.
- Member access through `operator->`.
- Indexing.
- Equality and inequality comparisons.

Example:

```cpp
pky::vector<int> values{1, 2, 3, 4};

for(auto it = values.begin(); it != values.end(); ++it)
{
    std::cout << *it << '\n';
}
```

The current iterator is intentionally smaller than the iterator interface provided by `std::vector`. It does not currently implement the complete set of standard iterator operations and iterator traits.

### Current `pky::vector` tradeoffs

- Provides the core functionality expected from a dynamic array.
- Uses contiguous storage like `std::vector`.
- Supports custom allocators.
- Explicitly manages object construction and destruction.
- Uses geometric growth when capacity is exhausted.
- Supports copy and move semantics.
- Currently has a smaller API than `std::vector`.
- `resize()`, `insert()`, `erase()`, and several other standard operations are not currently implemented.
- The iterator implementation is currently more limited than the standard `std::vector` iterator.

The main difference is therefore **scope rather than container model**: both containers provide dynamically sized contiguous storage, but `pky::vector` currently implements a smaller subset of the functionality offered by `std::vector`.

## `pky::string` vs `std::string`

`pky::string` currently provides a smaller subset of the functionality available in `std::string`.

| Feature | `pky::string` | `std::string` |
|---|---:|---:|
| Dynamic storage | Yes | Yes |
| Null termination | Yes | Yes |
| `operator[]` | Yes | Yes |
| Bounds-checked `at()` | Yes | Yes |
| `append()` | Yes | Yes |
| `operator+=` | Yes | Yes |
| `substr()` | Yes | Yes |
| `insert()` | Yes | Yes |
| `resize()` | Yes | Yes |
| `length()` | Yes | Yes |
| `c_str()` | Yes | Yes |
| Small-string optimization | Planned | Implementation defined |
| Full standard string API | No | Yes |
| Allocator support | No | Yes |
| Iterators | No | Yes |
| Rule-of-5 | Yes | Yes |

The main difference is scope: `pky::string` currently implements a relatively small API intended to cover common string operations,
while `std::string` provides a much more comprehensive and standardized interface.

### Current `pky::string` tradeoffs

- Simple dynamically allocated representation.
- Stores a null terminator explicitly.
- Does not currently implement small-string optimization (SSO).
  - Meaning, all `pky::string`s are dynamically allocated on the heap.
  - However, that also means that the size is less than `std::string`
  - E.g. `std::string` is 32 bytes on my machine, compared to `pky::string` being 16 bytes
    - Implications: std::string will be faster than `pky::string` when it comes to storing smaller strings.
      `pky::string` is better if you know you will be storing strings larger than give or take 15 bytes/chars.
- Does not currently provide the full standard C++ ABI `std::string` interface, with iterators, allocators, and so on.

## `pky::unique_ptr` vs `std::unique_ptr`

`pky::unique_ptr` follows the same basic ownership model as `std::unique_ptr`: an object has a single owning pointer, copying is disabled, and ownership can be transferred through moves.

| Feature | `pky::unique_ptr` | `std::unique_ptr` |
|---|---:|---:|
| Exclusive ownership | Yes | Yes |
| Copyable | No | No |
| Moveable | Yes | Yes |
| Custom deleter | Yes | Yes |
| `release()` | Yes | Yes |
| `reset()` | Yes | Yes |
| `swap()` | Yes | Yes |
| `get()` | Yes | Yes |
| `get_deleter()` | Yes | Yes |
| `operator*` / `operator->` | Yes | Yes |
| `operator bool` | Yes | Yes |
| `make_unique()` | Yes | Yes |
| Array specialization | No | Yes |
| Full standard API | No | Yes |

`pky::unique_ptr` is relatively close to the core ownership semantics of `std::unique_ptr`, while intentionally providing a smaller interface.

The implementation also uses `[[no_unique_address]]` for the deleter member, allowing an empty deleter
to potentially occupy no additional storage, so long as the deleter member has no member variables (state).

## `pky::fstream` vs `std::fstream`

The `pky` file-stream implementation is currently much smaller than the standard C++ stream library.

| Feature | `pky` streams | Standard streams |
|---|---:|---:|
| File opening | Yes | Yes |
| File closing | Yes | Yes |
| File writing | `ofstream` | `ofstream` |
| File reading | `ifstream` | `ifstream` |
| Stream insertion | Limited | Extensive |
| Formatted I/O | No | Yes |
| Stream state/error handling | Limited | Yes |
| Stream buffering/control | Limited | Yes |
| Standard stream interface | No | Yes |

`pky::ofstream` currently provides a lightweight wrapper around C's `FILE*` API. This keeps the implementation small, but also means it does not provide the extensive formatting, state management, buffering controls, and other facilities available through `std::ofstream`.

## Overall design tradeoff

The goal of `libpky` is not to reproduce every feature of the C++ standard library. Instead, it explores what can be achieved by providing smaller, more focused implementations.

The general results in tradeoffs:

| | `pky` | `std` |
|---|---|---|
| API size | Smaller | Larger |
| Aim | Learn C++, and use that less functionality to optimize struct sizing | Fast standard library for all with heavy comptime and runtime optimizations |
| Functionality | More limited | Extensive |
| Implementation complexity | Lower for supported features | Higher |
| Standard compatibility | Limited | Standardized |
| Portability expectations | Project-dependent | Broadly standardized |

I will continue to improve `libpky`, and plan on adding additional benchmarks and measurements, such as **container size, allocation behavior, execution time, runtime performance, generated meta-code**, to compare with the standard library.

# Philosophy
`libpky` is NOT trying to replace the standard library. The standard library is filled with a vast amount of important containers,
helper functions, template specializations, exception-safe moving, and on and on. `libpky` is a way for me to gain a solid foundation
on C++.

With that said, I am trying to mimick most of the standard library as possible, at least in terms of library design and following the standard.
Some things I keep in mind when making `libpky` are
- Exception safety
- Rule-of-5 and 
- Object sizing
- Runtime optimizations
- Simplicity, will still being functional
- Standardization and library design (`[[no_discard]]`, iterators, allocators, using `noexcept` for move constructors / assignment operators, difference between `[]` and `.at()`, etc.)
- Lastly, could I use this in another project without having to worry about its implementation

# Installation
Ensure you have at least CMake 3.10

**Cloning with testing submodules**
```bash
git clone --recursive https://github.com/pinkytoefoo/libpky.git
```

**Building with `ninja`**
```bash
mkdir build && cd build
cmake .. -G Ninja
ninja
```

**Run test suite**
Linux:
```
./pky_test
```

PowerShell / Windows:
```
.\pky_test
```

