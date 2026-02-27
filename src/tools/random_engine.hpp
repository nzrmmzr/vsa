#pragma once

#include <random>
#include <map>

namespace vsa::tools {

class RandomEngine
{
public:
    static RandomEngine& get_instance();

    void set_seed(std::int64_t seed) { m_gen.seed(seed); }

    std::size_t get_random_uint(std::size_t min, std::size_t max);
    std::int64_t get_random_int(std::int64_t min, std::int64_t max);

    bool get_random_bool(float true_probability = 0.5f);
private:
    std::mt19937 m_gen;
    std::map<std::pair<std::size_t, std::size_t>, std::uniform_int_distribution<std::size_t>> m_uint_dists;
    std::map<std::pair<std::int64_t, std::int64_t>, std::uniform_int_distribution<std::int64_t>> m_int_dists;

};

} // tools