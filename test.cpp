#include "xxh64.hpp"
#include <cstdlib>
#include <iostream>
#include <array>

bool checkhash (const void *ptr, int len, uint64_t seed, uint64_t expected)
{
    uint64_t hash = xxh64::hash (reinterpret_cast<const char*> (ptr), len, seed);
    if (hash != expected) {
        std::cerr << "Test failed: 0x" << std::hex << hash << " != 0x" << std::hex << expected << std::endl;
        return false;
    }
    return true;
}

int main (int argc, char *argv[])
{
    std::array<uint8_t, 101> data;
    static constexpr uint32_t prime = 2654435761U;

    uint32_t byteGen = prime;
    for (auto i = 0; i < data.size (); i++) {
        data[i] = byteGen >> 24;
        byteGen *= byteGen;
    }

    bool result = true;

    result &= checkhash (nullptr, 0, 0, 0xEF46DB3751D8E999ULL);
    result &= checkhash (nullptr, 0, prime, 0xAC75FDA2929B17EFULL);
    result &= checkhash (data.data (), 1, 0, 0x4FCE394CC88952D8ULL);
    result &= checkhash (data.data (), 1, prime, 0x739840CB819FA723ULL);
    result &= checkhash (data.data (), 14, 0, 0xCFFA8DB881BC3A3DULL);
    result &= checkhash (data.data (), 14, prime, 0x5B9611585EFCC9CBULL);
    result &= checkhash (data.data (), data.size (), 0, 0x0EAB543384F878ADULL);
    result &= checkhash (data.data (), data.size (), prime, 0xCAA65939306F1E21ULL);

    if (result) std::cout << "All tests passed successfully." << std::endl;

    return result ? EXIT_SUCCESS : EXIT_FAILURE;
}
