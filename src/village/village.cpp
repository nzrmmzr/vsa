//
// Created by Serhii Pustovit on 30.11.2025.
//

#include "village.hpp"

namespace vsa {
namespace village {

Village::Village(std::vector<std::shared_ptr<Resident>> residents):m_residents(residents) {}

sim::SimulationDataPoint Village::iterate()
{
    //
    // Consume (collect requirements)
    // Eat
    // Produce

    for (std::size_t i = 0; i < m_residents.size(); ++i) {
        auto& resident = m_residents.at(i);
        resident->iterate();
    }
    std::erase_if(m_residents, [](const std::shared_ptr<Resident>& r) { return r->is_dead(); });


    // Generate statistics data
    sim::SimulationDataPoint p;
    p.m_population = m_residents.size();
    for (const auto& resident : m_residents) {
        if (resident->is_male()) { p.m_males++; }
        else { p.m_females++; }

        p.m_avg_age_years += resident->get_age_years();
        p.m_count_by_resident[resident->get_id()]++;
    }
    if (!m_residents.empty()) {
        p.m_avg_age_years /= m_residents.size();
    } else {
        p.m_avg_age_years = 0;
    }

    return p;
}

} // village
} // vsa