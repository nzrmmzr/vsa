#pragma once

#include <simulation/simulation_config.hpp>

namespace vsa::village {

class VillageConfig
{
public:
    static void set_config(const sim::SimulationConfig& config) { m_sim_config = config; }
    static const sim::SimulationConfig& get_config() { return m_sim_config; }

private:
    static sim::SimulationConfig m_sim_config;
};

}