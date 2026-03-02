#include "random_engine.hpp"

#include <tools/logger.hpp>

namespace vsa::tools {

RandomEngine& RandomEngine::get_instance()
{
    static RandomEngine instance;
    return instance;
}

std::size_t RandomEngine::get_random_uint(std::size_t min, std::size_t max)
{
    std::uniform_int_distribution d(min, max);

    return d(m_gen);
}

std::int64_t RandomEngine::get_random_int(std::int64_t min, std::int64_t max)
{
    std::uniform_int_distribution d(min, max);

    return d(m_gen);
}

bool RandomEngine::get_random_bool(float true_probability)
{
    // if (true_probability > 1) { VSA_LOG_INFO("tools", "Incorrect probability (> 0)."); }
    // std::uniform_int_distribution d(0, 1000000000);
    // auto v = d(m_gen);
    // return v / 1000000000.0f <= true_probability;

    std::bernoulli_distribution dist(true_probability);
    return dist(m_gen);
}

} // tools