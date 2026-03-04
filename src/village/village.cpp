//
// Created by Serhii Pustovit on 30.11.2025.
//

#include "village.hpp"

#include "entities_registry.hpp"
#include "village_config.hpp"
#include "residents/resident_factory.hpp"
#include "tools/random_engine.hpp"

#include <utility>

namespace vsa {
namespace village {

Village::Village(std::vector<std::shared_ptr<Resident>> residents):m_residents(residents)
{
    m_residents_m.reserve(residents.size() / 2);
    m_residents_f.reserve(residents.size() / 2);
    for (const auto& resident : m_residents) {
        if (resident->is_male()) { m_residents_m.emplace_back(resident); }
        else { m_residents_f.emplace_back(resident); }
    }
}

sim::SimulationDataPoint Village::iterate(sim::SimulationDataGlobal& global)
{
    sim::SimulationDataPoint p;
    //
    // Consume (collect requirements)
    // Eat
    // Produce

    for (std::size_t i = 0; i < m_residents.size(); ++i) {
        auto& resident = m_residents.at(i);
        resident->iterate();
    }
    for (auto r : m_residents) {
        if (r->is_dead()) {
            p.m_number_of_deceased++;
            if (r->get_age_years() > global.m_the_eldest_resident_age){ global.m_the_eldest_resident_age = r->get_age_years();}
            if (r->get_age_years() < 18){ global.m_underage_deceased_count++; }
            else if ( r->get_age_years() > 100 ){ global.m_elders_deceased_count++; }
            r->remove_relations();
        }
    }
    std::erase_if(m_residents, [](const std::shared_ptr<Resident>& r) { return r->is_dead(); });
    std::erase_if(m_residents_m, [](const std::shared_ptr<Resident>& r) { return r->is_dead(); });
    std::erase_if(m_residents_f, [](const std::shared_ptr<Resident>& r) { return r->is_dead(); });
    std::erase_if(m_couples, [](const auto& p) { return p.first->is_dead() || p.second->is_dead(); });

    if (tools::RandomEngine::get_instance().get_random_bool(VillageConfig::get_config().population.couple_creation_probability_per_day)) {
        auto m = m_residents_m[tools::RandomEngine::get_instance().get_random_uint(0, m_residents_m.size() - 1)];
        auto f = m_residents_f[tools::RandomEngine::get_instance().get_random_uint(0, m_residents_f.size() - 1)];

        if (!m->has_partner() && !f->has_partner() &&
            m->get_age_years() > VillageConfig::get_config().population.min_couple_creation_age_years &&
            f->get_age_years() > VillageConfig::get_config().population.min_couple_creation_age_years &&
            std::abs(static_cast<std::int64_t>(m->get_age_years() - f->get_age_years())) <= VillageConfig::get_config().population.max_couple_creation_age_diff_years) {
            m->add_partner(f);
            f->add_partner(m);
            m_couples.emplace_back(m, f);
        }
    }

    if (m_couples.size() > 0 && tools::RandomEngine::get_instance().get_random_bool(VillageConfig::get_config().population.child_creation_probability_per_day)) {
        p.m_number_of_born++;
        auto p = m_couples[tools::RandomEngine::get_instance().get_random_uint(0, m_couples.size() - 1)];
        const auto& residents_ids = village::EntitiesRegistry::get_instance().get_residents_ids();
        auto c = ResidentFactory::create_resident(
            residents_ids[tools::RandomEngine::get_instance().get_random_uint(0, residents_ids.size() - 1)],
            tools::RandomEngine::get_instance().get_random_uint(0, 1), 0);
        p.first->add_child(c);
        p.second->add_child(c);
        if (p.first->is_male()) { c->add_father(p.first);  c->add_mother(p.second); }
        else { c->add_mother(p.first);  c->add_father(p.second); }
        m_residents.push_back(c);
        if (c->is_male()) { m_residents_m.emplace_back(c); }
        else { m_residents_f.emplace_back(c); }

        global.m_avg_first_child_age += p.first->get_age_years() + p.second->get_age_years();
        global.m_avg_first_child_age /= 3;

    }

    // Generate statistics data
    p.m_population = m_residents.size();
    p.m_males = m_residents_m.size();
    p.m_females = m_residents_f.size();
    p.m_couples_count = m_couples.size();

    std::size_t residents_with_children_count = 0;
    for (const auto& resident : m_residents) {
        if (resident->get_children_count() > 0) { residents_with_children_count++; }

        p.m_avg_age_years += resident->get_age_years();
        p.m_avg_children_count += resident->get_children_count();
        p.m_count_by_resident[resident->get_id()]++;

    }
    p.m_avg_children_count_unique = p.m_avg_children_count / (residents_with_children_count ? residents_with_children_count : 1);
    p.m_avg_children_count /= m_residents.size() ? m_residents.size() : 1;
    p.m_avg_age_years /= m_residents.size() ? m_residents.size() : 1;

    return p;
}

} // village
} // vsa