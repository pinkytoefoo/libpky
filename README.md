# libpky
C++ STL-like library with a focus on optimizing tradeoffs between container size, functionality, and speed. The current containers implemented are
```cpp
pky::vector
pky::unique_ptr
pky::string
pky::ifstream
pky::ofstream
```
[Comparison](#Comparison---std-vs-pky)
[Get started](#Installation)

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

# Comparison - std vs pky

`libpky` aims to provide STL-like containers and utilities while making different tradeoffs between **container size, functionality, and implementation complexity**. It is not intended to be a drop-in replacement for the C++ standard library.

The following table compares the currently implemented `pky` types with their closest standard-library equivalents:

| `pky` | Standard library | Purpose |
|---|---|---|
| `pky::vector` | `std::vector` | Dynamic contiguous storage |
| `pky::unique_ptr` | `std::unique_ptr` | Exclusive ownership of dynamically allocated objects |
| `pky::string` | `std::string` | Dynamically allocated null-terminated strings |
| `pky::ifstream` | `std::ifstream` | File input |
| `pky::ofstream` | `std::ofstream` | File output |

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

This results in a general tradeoff:

| | `pky` | `std` |
|---|---|---|
| API size | Smaller | Larger |
| Aim | Learn C++, and use that less functionality to optimize struct sizing | Fast standard library for all with heavy comptime and runtime optimizations |
| Functionality | More limited | Extensive |
| Implementation complexity | Lower for supported features | Higher |
| Standard compatibility | Limited | Standardized |
| Portability expectations | Project-dependent | Broadly standardized |

I will continue to improve `libpky`, and plan on adding additional benchmarks and measurements, such as **container size, allocation behavior, execution time, runtime performance, generated meta-code**, to compare with the standard library.

