#include "resident.hpp"

#include "tools/random_engine.hpp"

#include <village/village_config.hpp>

namespace vsa::village {

namespace {
constexpr std::size_t kIncrementingStartDay = 10 * 365;
}

Resident::Resident(bool is_male, std::size_t age_days) : m_is_male(is_male), m_age_days(age_days)
{
    const std::size_t prob_days = m_age_days > kIncrementingStartDay ? m_age_days - kIncrementingStartDay : 0;
    m_death_probability = prob_days * VillageConfig::get_config().population.death_probability_day_increment;
}

bool Resident::iterate()
{
    iterate_impl();

    m_age_days += 1;
    m_death_probability += m_age_days > kIncrementingStartDay ? VillageConfig::get_config().population.death_probability_day_increment : 0;

    m_is_dead = tools::RandomEngine::get_instance().get_random_bool(m_death_probability / 100);

    return m_is_dead;
}

} // vsa