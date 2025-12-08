#pragma once
#include <cstddef>
#include <cstdint>

namespace vsa::sim {

struct SimulationConfig
{
    std::size_t max_duration_days = 10;

    struct Population
    {
        std::int32_t initial_min_age = 20;
        std::int32_t initial_max_age = 60;

        float percentage_of_women = 50.f;
        float percentage_of_men = 50.f;

        float death_probability_day_increment = 0.003f;

        float couple_creation_probability_per_day = 0.003f;
        float couple_breaking_probability_per_day = 0.0001f;
        float child_creation_probability_per_day = 0.0001f;
    };
    Population population;
};

}
