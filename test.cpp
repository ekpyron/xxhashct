#include "xxh32.hpp"
#include "xxh64.hpp"
#include <cstdlib>
#include <iostream>
#include <array>
#ifdef HAVE_XXHASH
#include <xxhash.h>
#endif

template<uint64_t X>
struct constexpr_test { static const uint64_t value = X; };

constexpr_test<xxh32::hash("test", 4, 0)> constexprTest32;
constexpr_test<xxh64::hash("test", 4, 0)> constexprTest64;

bool checkhash_32 (const void* ptr, int len, uint32_t seed, uint32_t expected)
{
    const uint32_t hash = xxh32::hash (reinterpret_cast<const char*> (ptr), len, seed);
    if (hash != expected) {
        std::cerr << "[xxh32] Test failed: 0x" << std::hex << hash << " != 0x" << std::hex << expected << std::endl;
        return false;
    }
#ifdef HAVE_XXHASH
    uint64_t ref = XXH32(ptr, len, seed);
    if (hash != ref) {
        std::cerr << "Test against reference implementation failed: 0x" << std::hex << hash << " != 0x" << std::hex << ref << std::endl;
        return false;
    }
#endif
    return true;
}

bool checkhash_64 (const void *ptr, int len, uint64_t seed, uint64_t expected)
{
    uint64_t hash = xxh64::hash(reinterpret_cast<const char*> (ptr), len, seed);
    if (hash != expected) {
        std::cerr << "[xxh64] Test failed: 0x" << std::hex << hash << " != 0x" << std::hex << expected << std::endl;
        return false;
    }
#ifdef HAVE_XXHASH
    uint64_t ref = XXH64(ptr, len, seed);
    if (hash != ref) {
        std::cerr << "Test against reference implementation failed: 0x" << std::hex << hash << " != 0x" << std::hex << ref << std::endl;
        return false;
    }
#endif
    return true;
}

int main (int argc, char *argv[])
{
    static std::array<uint8_t, 10100> data;
    static constexpr uint32_t prime = 2654435761U;

    uint32_t byteGen = prime;
    for (auto i = 0; i < data.size (); i++) {
        data[i] = byteGen >> 24;
        byteGen *= byteGen;
    }

    bool result_32 = true;

    result_32 &= checkhash_32 (nullptr, 0, 0, 0x2CC5D05U);
    result_32 &= checkhash_32 (nullptr, 0, prime, 0x36B78AE7U);
    result_32 &= checkhash_32 (data.data (), 1, 0, 0xB85CBEE5U);
    result_32 &= checkhash_32 (data.data (), 1, prime, 0xD5845D64U);
    result_32 &= checkhash_32 (data.data (), 14, 0, 0xE5AA0AB4U);
    result_32 &= checkhash_32 (data.data (), 14, prime, 0x4481951DU);
    result_32 &= checkhash_32 (data.data (), 101, 0, 0x1F1AA412U);
    result_32 &= checkhash_32 (data.data (), 101, prime, 0x498EC8E2U);
    result_32 &= checkhash_32 (data.data (), data.size (), 0, 0x3931B56F);
    result_32 &= checkhash_32 (data.data (), data.size (), prime, 0x44857AA1);

    if (result_32) std::cout << "xxh32 tests passed successfully." << std::endl;

    bool result_64 = true;

    result_64 &= checkhash_64 (nullptr, 0, 0, 0xEF46DB3751D8E999ULL);
    result_64 &= checkhash_64 (nullptr, 0, prime, 0xAC75FDA2929B17EFULL);
    result_64 &= checkhash_64 (data.data (), 1, 0, 0x4FCE394CC88952D8ULL);
    result_64 &= checkhash_64 (data.data (), 1, prime, 0x739840CB819FA723ULL);
    result_64 &= checkhash_64 (data.data (), 14, 0, 0xCFFA8DB881BC3A3DULL);
    result_64 &= checkhash_64 (data.data (), 14, prime, 0x5B9611585EFCC9CBULL);
    result_64 &= checkhash_64 (data.data (), data.size (), 0, 0x7570A206CFC74b78ULL);
    result_64 &= checkhash_64 (data.data (), data.size (), prime, 0x95A9A4036AE9A784ULL);

    if (result_64) std::cout << "xxh64 tests passed successfully." << std::endl;

    if (result_32 && result_64) std::cout << "All tests passed successfully." << std::endl;
    return (result_32 && true) ? EXIT_SUCCESS : EXIT_FAILURE;
}
