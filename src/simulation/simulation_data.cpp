#include "simulation_data.hpp"



namespace vsa::sim {

SimulationData::SimulationData(std::vector<SimulationDataPoint>&& points)
    :m_points(std::move(points))
{

}
}
