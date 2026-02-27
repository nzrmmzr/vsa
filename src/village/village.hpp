#pragma once

#include <memory>
#include <vector>

#include <village/storage.hpp>
#include <village/resident.hpp>

#include <simulation/simulation_data.hpp>

namespace vsa::village {

class Village
{
public:
    Village(std::vector<std::shared_ptr<Resident>> residents);

    sim::SimulationDataPoint iterate();

    std::size_t get_size() const { return m_residents.size(); }
private:
    std::shared_ptr<Storage> m_storage;
    std::vector<std::shared_ptr<Resident>> m_residents;
};

} // village