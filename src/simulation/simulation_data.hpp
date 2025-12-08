#pragma once

#include <vector>

namespace vsa::sim {

struct SimulationDataPoint
{
    std::size_t m_population;
};

class SimulationData
{
public:
    explicit SimulationData(std::vector<SimulationDataPoint>&& points);

    [[nodiscard]] const std::vector<SimulationDataPoint>& get_points() const { return m_points; }
private:
    std::vector<SimulationDataPoint> m_points;
};

}
