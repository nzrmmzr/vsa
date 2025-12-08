#pragma once

#include <simulation/simulation_config.hpp>
#include <simulation/simulation_data.hpp>

namespace vsa::sim {

class Simulation
{
public:
    Simulation(const SimulationConfig& config, const SimulationData& data);

    [[nodiscard]] const SimulationConfig& get_config() const { return m_config; }
    [[nodiscard]] const SimulationData& get_data() const { return m_data; }
private:
    const SimulationConfig m_config;
    const SimulationData m_data;
};

}
