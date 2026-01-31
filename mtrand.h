
#include <cstdint>
#include <array>

class PhpMtRand {
private:
    static constexpr int N = 624;
    static constexpr int M = 397;

    std::array<uint32_t, N> state{};
    int index = N;

    static constexpr uint32_t MATRIX_A   = 0x9908b0df;
    static constexpr uint32_t UPPER_MASK = 0x80000000;
    static constexpr uint32_t LOWER_MASK = 0x7fffffff;

    void twist()
    {
        for (int i = 0; i < N; ++i) {
            uint32_t y = (state[i] & UPPER_MASK)
                       | (state[(i + 1) % N] & LOWER_MASK);

            uint32_t mag = (y & 1) ? MATRIX_A : 0;
            state[i] = state[(i + M) % N] ^ (y >> 1) ^ mag;
        }
        index = 0;
    }

    uint32_t extract()
    {
        if (index >= N)
            twist();

        uint32_t y = state[index++];

        // Tempering (same as PHP)
        y ^= (y >> 11);
        y ^= (y << 7)  & 0x9d2c5680;
        y ^= (y << 15) & 0xefc60000;
        y ^= (y >> 18);

        // PHP mt_rand() returns **31-bit**
        return y; // & 0x7fffffff;
    }

public:
    // Equivalent to mt_srand($seed)
    void srand(uint32_t seed)
    {
        state[0] = seed & 0xffffffffU;
        for (int i = 1; i < N; ++i) {
            state[i] = (1812433253U *
                       (state[i - 1] ^ (state[i - 1] >> 30)) + i);
        }
        index = N;
    }

    // Equivalent to mt_rand()
    uint32_t rand()
    {
        return extract();
    }

    // Equivalent to mt_rand($min, $max)
    uint32_t rand(uint32_t min, uint32_t max)
    {
        uint32_t r = extract();
        return min + (r % (max - min + 1)); // modulo bias (intentional!)
    }
};

