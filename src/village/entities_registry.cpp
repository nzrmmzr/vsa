#include "entities_registry.hpp"

namespace vsa::village {

EntitiesRegistry& EntitiesRegistry::get_instance()
{
    static EntitiesRegistry instance;
    return instance;
}

EntitiesRegistry::EntitiesRegistry() = default;

void EntitiesRegistry::register_resident(const std::string& id, const std::string& name)
{
    m_residents[id] = {name};
    m_residents_ids.push_back(id);
}

const std::map<std::string, EntitiesRegistry::ResidentEntity>& EntitiesRegistry::get_residents() const
{
    return m_residents;
}

const std::vector<std::string>& EntitiesRegistry::get_residents_ids() const
{
    return m_residents_ids;
}

} // vsa