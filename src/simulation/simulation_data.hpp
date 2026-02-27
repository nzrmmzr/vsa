#pragma once

#include <map>
#include <string>
#include <vector>

namespace vsa::sim {

struct SimulationDataPoint
{
    std::size_t m_population = 0;
    std::size_t m_males = 0;
    std::size_t m_females = 0;
    std::size_t m_avg_age_years = 0;
    std::map<std::string, std::size_t> m_count_by_resident;
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