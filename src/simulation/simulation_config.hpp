#pragma once

#include <cstddef>
#include <cstdint>
#include <map>
#include <string>

namespace vsa::sim {

struct SimulationConfig
{
    std::size_t m_seed = 123456789;

    std::size_t max_duration_days = 10;

    struct Population
    {
        std::size_t initial_population = 2000;

        std::int32_t initial_min_age = 20;
        std::int32_t initial_max_age = 60;

        std::int32_t max_children_family = 3;

        float percentage_of_women = 50.f;
        float percentage_of_men = 50.f;

        float death_probability_day_increment = 0.003f;

        float couple_creation_probability_per_day = 0.003f;
        float couple_breaking_probability_per_day = 0.0001f;
        float child_creation_probability_per_day = 0.0001f;
    };
    Population population;

    struct Resident
    {
        float initial_percentage = 0.05f;
        float become_probability = 0.05f;
    };
    std::map<std::string, Resident> residents;

};

}