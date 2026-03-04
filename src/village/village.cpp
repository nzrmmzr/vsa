//
// Created by Serhii Pustovit on 30.11.2025.
//

#include "village.hpp"

#include <tracy/Tracy.hpp>

#include <tools/random_engine.hpp>
#include <village/village_config.hpp>
#include <tools/logger.hpp>
#include <village/entities_registry.hpp>

namespace vsa {
namespace village {

Village::Village(std::vector<std::shared_ptr<Resident>> residents):m_residents(residents) {
    m_residents_f.reserve(m_residents.size() / 2);
    m_residents_m.reserve(m_residents.size() / 2);

    for(auto res : m_residents){
        if(res->is_male()){ m_residents_m.push_back(res); }
        else{ m_residents_f.push_back(res); }
    }
}

sim::SimulationDataPoint Village::iterate()
{
    //
    // Consume (collect requirements)
    // Eat
    // Produce

    ZoneScopedN("Village::iterate");

    {
        ZoneScopedN("Residents Update");

        for (std::size_t i = 0; i < m_residents.size(); ++i) {
            auto& resident = m_residents.at(i);
            resident->iterate();
        }

        for(auto res : m_residents){
            if(res->is_dead()){ res->remove_relations(); }
        }

        std::erase_if(m_residents, [](const std::shared_ptr<Resident>& r) { return r->is_dead(); });
        std::erase_if(m_residents_m, [](const std::shared_ptr<Resident>& r) { return r->is_dead(); });
        std::erase_if(m_residents_f, [](const std::shared_ptr<Resident>& r) { return r->is_dead(); });
        std::erase_if(m_couples, [](const auto& r) { return r.first->is_dead() || r.second->is_dead(); });

        if(tools::RandomEngine::get_instance().get_random_bool(VillageConfig::get_config().population.couple_creation_probability_per_day) ){
            auto f = m_residents_f[tools::RandomEngine::get_instance().get_random_uint(0, (m_residents_f.size() - 1))];
            auto m = m_residents_m[tools::RandomEngine::get_instance().get_random_uint(0, (m_residents_m.size() - 1))];

            auto config = VillageConfig::get_config();

            if(!f->has_spouse() && !m->has_spouse() &&
                f->get_age_years() >= config.population.min_marry_age &&
                m->get_age_years() >= config.population.min_marry_age &&
                std::abs(static_cast<std::int64_t>(f->get_age_years() - m->get_age_years())) <= config.population.max_age_gap){
                    VSA_LOG_INFO("village", "Couple created");
                    f->set_spouse(m);
                    m->set_spouse(f);
                    m_couples.push_back({m, f});
            }


            if(m_couples.size() > 0 && tools::RandomEngine::get_instance().get_random_bool(config.population.child_creation_probability_per_day)){
                auto p = m_couples[tools::RandomEngine::get_instance().get_random_uint(0, (m_couples.size() - 1))];
                const auto& residents_ids = village::EntitiesRegistry::get_instance().get_residents_ids();
                //auto c = village::Res
            }
        }
    }

    // Generate statistics data
    sim::SimulationDataPoint p;
    {
        ZoneScopedN("Generate Statistics");

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
    }

    return p;
}

} // village
} // vsa