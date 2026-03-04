#pragma once

#include <vector>
#include <map>
#include <string>

namespace vsa::sim {

struct SimulationDataPoint
{
    std::size_t m_population = 0;
    std::size_t m_males = 0;
    std::size_t m_females = 0;
    std::size_t m_avg_age_years = 0;
    std::size_t m_couples_count = 0;
    float m_avg_children_count = 0;
    float m_avg_children_count_unique = 0;
    std::size_t m_number_of_deceased = 0;
    std::size_t m_number_of_born = 0;
    std::map<std::string, std::size_t> m_count_by_resident;
};

struct SimulationDataGlobal
{
    std::size_t m_avg_first_child_age = 0;
    std::size_t m_underage_deceased_count = 0;
    std::size_t m_elders_deceased_count = 0;
    std::size_t m_the_eldest_resident_age = 0;
};

class SimulationData
{
public:
    explicit SimulationData(std::vector<SimulationDataPoint>&& points, SimulationDataGlobal&& m_global);

    [[nodiscard]] const std::vector<SimulationDataPoint>& get_points() const { return m_points; }
    [[nodiscard]] const SimulationDataGlobal& get_global() const { return m_global; }
private:
    std::vector<SimulationDataPoint> m_points;
    SimulationDataGlobal m_global;
};

}
