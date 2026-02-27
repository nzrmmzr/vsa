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
    if (true_probability > 1) { VSA_LOG_INFO("tools", "Incorrect probability (> 0)."); }
    std::vector<double> weights = { 1 - true_probability, true_probability };
    std::discrete_distribution d(weights.begin(), weights.end());
    return d(m_gen);
}

} // tools