#include "simulation.hpp"

namespace vsa::sim {

Simulation::Simulation(const SimulationConfig& config, const SimulationData& data)
  : m_config(config)
  , m_data(data)
{
}

}
